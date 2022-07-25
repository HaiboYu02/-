#include <stdio.h>
#include "string.h"
#include <time.h> 
#include "openssl/evp.h"
#include "math.h"
#include "sm3.h"

#define Birthday_Attack       //���չ���
//#define Length_Extension_Attack         //������չ����

#ifdef Birthday_Attack   
int main(void)
{
	//���ݸ�����Ϣ������SM3ժҪֵ
	unsigned char hash_value2[64];
	unsigned char hash_value[64];
	unsigned int i, hash_len;
	char sample2[] = { 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
										 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
										 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
										 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
										 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
										 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
										 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
										 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64 };
	unsigned int sample2_len = sizeof(sample2);
	sm3_hash(sample2, sample2_len, hash_value, &hash_len);
	printf("raw data:\n");
	for (i = 0; i < sample2_len; i++)
	{
		printf("0x%x  ", sample2[i]);
	}
	printf("\n");
	printf("hash length: %d bytes.\n", hash_len);
	printf("hash value:\n");
	for (i = 0; i < hash_len; i++)
	{
		printf("0x%x  ", hash_value[i]);
	}
	printf("\n");

	//������Ϣ��Ѱ��һ����ײ
	char sample[10];
	srand((unsigned)time(NULL));
	int a = rand() * rand();
	for (int i = a; i < a + 0xfffff; i++) {
		int_to_hex(i, sample);
		unsigned int sample_len = strlen((char*)sample);
		sm3_hash(sample, sample_len, hash_value2, &hash_len);
		//ժҪֵ��ǰ16bit��ͬ������һ����ײ
		if (hash_value2[0] == hash_value[0] && hash_value2[1] == hash_value[1])
		{
			printf("�ҵ�һ��16bitǰ׺��ײ,������%d��\n", i - a);
			printf("raw data: %x\n", i);
			printf("hash length: %d bytes.\n", hash_len);
			printf("hash value:\n");
			for (int j = 0; j < hash_len; j++)
			{
				printf("0x%x  ", hash_value2[j]);
			}
			printf("\n\n");
			break;
		}
	}
	return 0;
}
#endif 

#ifdef Length_Extension_Attack
int main()
{
	unsigned char Hash[32] = { 0 };
	unsigned char Hash2[32] = { 0 };
	unsigned char Hash3[32] = { 0 };
	unsigned int* hash_len = NULL;
	unsigned char str[] = { 'a','b','c',0 };
	printf("��Ϣm1�����ݣ�%s\n\n", str);
	//66c7f0f4 62eeedd9 d1f2d46b dc10e4e2 4167c487 5cf2f7a2 297da02b 8f4ba8e0
	sm3(str, strlen((char*)str), Hash, hash_len);
	printf("��Ϣm1��ժҪֵ��\n");
	for (int i = 0; i < 32; i++)
	{
		printf("0x%x  ", Hash[i]);
	}
	printf("\n\n");
	//Ԥ����hash(_m1||m2),����str2=_m1||m2
	printf("����sm3����Ϣ���������Ϣm1���õ�_m1����β�����������Ϣm2\n"); 
	unsigned char m2[] = { 'd','e','f',0 };
	unsigned char str2[100] = { 0 };
	str2[0] = 'a';
	str2[1] = 'b';
	str2[2] = 'c';
	str2[3] = 0x80;     //���1����1���Ͷ����0��
	str2[63] = 0x18;       //���64bit�ı��ش�����m1�ĳ��ȣ�24bit
	str2[64] = 'd';
	str2[65] = 'e';
	str2[66] = 'f';
	sm3(str2, 67, Hash2, hash_len);
	printf("��Ϣm2��ժҪֵ��\n");
	for (int i = 0; i < 32; i++)
	{
		printf("0x%x  ", Hash2[i]);
	}
	printf("\n\n");

	//������չ��������֪��ԭʼ����m1����֪���������β��������m2�������ܳ���NSrcLen
	printf("��ʼ���Գ�����չ������\n");
	printf("��֪��Ϣ��\n");
	printf("ԭ��Ϣ�ı�������%d\n", strlen((char*)str) * 8);
	printf("ԭ��Ϣ��ժҪֵ��");
	for (int i = 0; i < 32; i++)
	{
		printf("0x%x  ", Hash[i]);
	}
	printf("\n");
	printf("ԭ��Ϣβ����ӵ��ַ�����%s\n\n", m2);
	unsigned int tempHash[8] = { 0 };     //������һ��ѹ�������Ĺ�ϣֵ
	unsigned int temp = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 4 * i + 3; j >= 4 * i; j--) {
			temp = Hash[j];
			temp = temp * pow(256, (4 * i + 3 - j));
			tempHash[i] += temp;
		}
	}
	//��m2������Ϣ���
	int nSrcLen = strlen((char*)m2);
	int NSrcLen = strlen((char*)m2) + 64;
	unsigned int nGroupNum = (nSrcLen + 1 + 8 + 64) / 64;
	unsigned char* ucpMsgBuf = (unsigned char*)malloc(nGroupNum * 64);
	memset(ucpMsgBuf, 0, nGroupNum * 64);
	memcpy(ucpMsgBuf, m2, nSrcLen);
	ucpMsgBuf[nSrcLen] = 0x80;
	int i = 0;
	for (i = 0; i < 8; i++)
	{
		ucpMsgBuf[nGroupNum * 64 - i - 1] = ((unsigned long long)(NSrcLen * 8) >> (i * 8)) & 0xFF;
	}
	//ִ��ѹ����������������ֱ�����һ��ѹ�������Ĺ�ϣֵ��Ҫ��ӵ�β������Ϣ�����ֵ
	//for (int i = 0; i < 64; i++) {
	//	printf("%x", ucpMsgBuf[i]);
	//}
	_CF(ucpMsgBuf, tempHash);     //ִ����Ϻ��������Ĺ�ϣֵ���ԭ����tempHash����
	free(ucpMsgBuf);
	for (i = 0; i < 8; i++)
	{
		Hash3[i * 4 + 0] = (unsigned char)((tempHash[i] >> 24) & 0xFF);
		Hash3[i * 4 + 1] = (unsigned char)((tempHash[i] >> 16) & 0xFF);
		Hash3[i * 4 + 2] = (unsigned char)((tempHash[i] >> 8) & 0xFF);
		Hash3[i * 4 + 3] = (unsigned char)((tempHash[i] >> 0) & 0xFF);
	}

	printf("��������֪��Ϣ���в����õ�ժҪֵ��");
	for (int i = 0; i < 32; i++)
	{
		printf("0x%x  ", Hash3[i]);
	}
	printf("\n");
	for (int i = 0; i < 32; i++) {
		if (Hash2[i] == Hash3[i]) {
			if (i != 31)
				continue;
			else
				printf("α��ɹ����õ���һ��ͨ����֤����Ϣ��ժҪ��\n\n");
		}
		else
			printf("α��ʧ�ܣ�");
	}
}
#endif

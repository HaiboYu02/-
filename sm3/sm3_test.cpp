#include <stdio.h>
#include "string.h"
#include <time.h> 
#include "openssl/evp.h"
#include "math.h"
#include "sm3.h"

#define Birthday_Attack       //生日攻击
//#define Length_Extension_Attack         //长度扩展攻击

#ifdef Birthday_Attack   
int main(void)
{
	//根据给定消息，计算SM3摘要值
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

	//构造消息，寻找一个碰撞
	char sample[10];
	srand((unsigned)time(NULL));
	int a = rand() * rand();
	for (int i = a; i < a + 0xfffff; i++) {
		int_to_hex(i, sample);
		unsigned int sample_len = strlen((char*)sample);
		sm3_hash(sample, sample_len, hash_value2, &hash_len);
		//摘要值的前16bit相同就算是一次碰撞
		if (hash_value2[0] == hash_value[0] && hash_value2[1] == hash_value[1])
		{
			printf("找到一个16bit前缀碰撞,共穷搜%d次\n", i - a);
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
	printf("消息m1的内容：%s\n\n", str);
	//66c7f0f4 62eeedd9 d1f2d46b dc10e4e2 4167c487 5cf2f7a2 297da02b 8f4ba8e0
	sm3(str, strlen((char*)str), Hash, hash_len);
	printf("消息m1的摘要值：\n");
	for (int i = 0; i < 32; i++)
	{
		printf("0x%x  ", Hash[i]);
	}
	printf("\n\n");
	//预计算hash(_m1||m2),其中str2=_m1||m2
	printf("根据sm3的消息填充规则对消息m1填充得到_m1，在尾部填充任意消息m2\n"); 
	unsigned char m2[] = { 'd','e','f',0 };
	unsigned char str2[100] = { 0 };
	str2[0] = 'a';
	str2[1] = 'b';
	str2[2] = 'c';
	str2[3] = 0x80;     //填充1个“1”和多个“0”
	str2[63] = 0x18;       //填充64bit的比特串代表m1的长度：24bit
	str2[64] = 'd';
	str2[65] = 'e';
	str2[66] = 'f';
	sm3(str2, 67, Hash2, hash_len);
	printf("消息m2的摘要值：\n");
	for (int i = 0; i < 32; i++)
	{
		printf("0x%x  ", Hash2[i]);
	}
	printf("\n\n");

	//长度扩展攻击，不知道原始数据m1，但知道后添加在尾部的数据m2及数据总长度NSrcLen
	printf("开始尝试长度扩展攻击：\n");
	printf("已知信息：\n");
	printf("原消息的比特数：%d\n", strlen((char*)str) * 8);
	printf("原消息的摘要值：");
	for (int i = 0; i < 32; i++)
	{
		printf("0x%x  ", Hash[i]);
	}
	printf("\n");
	printf("原消息尾部添加的字符串：%s\n\n", m2);
	unsigned int tempHash[8] = { 0 };     //经历完一轮压缩函数的哈希值
	unsigned int temp = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 4 * i + 3; j >= 4 * i; j--) {
			temp = Hash[j];
			temp = temp * pow(256, (4 * i + 3 - j));
			tempHash[i] += temp;
		}
	}
	//对m2进行消息填充
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
	//执行压缩函数，两个输入分别是上一轮压缩函数的哈希值和要添加到尾部的消息的填充值
	//for (int i = 0; i < 64; i++) {
	//	printf("%x", ucpMsgBuf[i]);
	//}
	_CF(ucpMsgBuf, tempHash);     //执行完毕后，输出结果的哈希值替代原来的tempHash参数
	free(ucpMsgBuf);
	for (i = 0; i < 8; i++)
	{
		Hash3[i * 4 + 0] = (unsigned char)((tempHash[i] >> 24) & 0xFF);
		Hash3[i * 4 + 1] = (unsigned char)((tempHash[i] >> 16) & 0xFF);
		Hash3[i * 4 + 2] = (unsigned char)((tempHash[i] >> 8) & 0xFF);
		Hash3[i * 4 + 3] = (unsigned char)((tempHash[i] >> 0) & 0xFF);
	}

	printf("对上面已知信息进行操作得到摘要值：");
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
				printf("伪造成功！得到另一对通过验证的消息和摘要对\n\n");
		}
		else
			printf("伪造失败！");
	}
}
#endif

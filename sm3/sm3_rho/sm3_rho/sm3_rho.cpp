#include "openssl/evp.h"
#include <stdlib.h>
#include "string.h"
#include <vector>
#include <iostream>

using namespace std;

int sm3_hash(char* message, size_t len, unsigned char* hash, unsigned int* hash_len)
{
	EVP_MD_CTX* md_ctx;
	const EVP_MD* md;

	md = EVP_sm3();
	md_ctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(md_ctx, md, NULL);
	EVP_DigestUpdate(md_ctx, message, len);
	EVP_DigestFinal_ex(md_ctx, hash, hash_len);
	EVP_MD_CTX_free(md_ctx);
	return 0;
}

void int_to_hex(int a, char ch[10]) {
	char mid_char;
	int temp = 0, i = 0;
	while (a / 16 > 0)
	{
		temp = a % 16;
		if (temp > 9)
			ch[i] = temp + 87;
		else
			ch[i] = temp + 48;
		i++;
		a = a / 16;

		if (a < 16)
		{
			if(a > 9)
				ch[i] = a + 87;
			else
				ch[i] = a + 48;
			ch[i + 1] = 0;
			break;
		}
	}
	temp = strlen(ch);
	for (i = 0; i < strlen(ch) / 2; i++)
	{
		mid_char = ch[i];
		ch[i] = ch[temp - i - 1];
		ch[temp - i - 1] = mid_char;
	}
}

int main() {
	char hash_value[9] = "12345678";
	unsigned int len = sizeof(hash_value);
	unsigned char hash_v[64];
	unsigned int hash_len;
	char tmp[3] = { 0 };
	char *str;
	std::vector<string> demo = { hash_value };

	for (int i = 0; i < 100000; i++) {
		sm3_hash(hash_value, len, hash_v, &hash_len);
		int_to_hex(int(hash_v[0]), tmp);
		strcpy_s(hash_value, tmp);
		for (int i = 1; i < hash_len / 8; i++)
		{
			int_to_hex(int(hash_v[i]), tmp);
			strcat_s(hash_value, tmp);
		}
		vector<string>::iterator result = find(demo.begin(), demo.end(), hash_value); 

		if (result != demo.end())
		{
			int index = distance(demo.begin(), result);
			cout << "找到一个碰撞" << demo[index - 1] << " and " << demo[0] << endl;
			cout << "哈希值为" << hash_value << endl;
			break;
		}
		else
			demo.insert(demo.begin(), hash_value);
	}
}

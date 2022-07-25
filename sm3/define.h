#pragma once
#ifndef _SM3_H_
#define _SM3_H_

//调用openssl库实现
extern int sm3_hash(char* message, size_t len, unsigned char* hash, unsigned int* hash_len);
//纯c语言实现版本
extern unsigned int sm3(unsigned char* ucpSrcData, unsigned int nSrcLen, unsigned char* ucpHash, unsigned int* nHashLen);
extern void int_to_hex(int a, char ch[10]);
extern unsigned int _CF(unsigned char* ucpSrcMsg, unsigned int nHash[8]);
#endif
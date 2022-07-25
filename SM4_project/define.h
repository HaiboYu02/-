#include <stdint.h>

void dump_buf_32(uint32_t* buf, uint32_t len);
void dump_buf_8(uint8_t* buf, uint32_t len);
uint32_t move(uint32_t data, int length);
uint32_t t(uint32_t input);
void t_1(uint32_t* input, uint32_t* rk);
void f(uint32_t* input, uint32_t rk, uint32_t* output);
void SM4(uint32_t* key, uint32_t* plain);

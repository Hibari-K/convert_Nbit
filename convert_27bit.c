#include<stdio.h>

#define BITSIZE 512
#define N (BITSIZE/32)+4 // size of array "data"
#define M (BITSIZE/27)+4 // size of array "result"
#define DIGITS (BITSIZE/54) // 54 = (27 * 4) / 2 used in convert_27bit

void convert_27bit(unsigned int* data, unsigned int* result, int digits){

    __asm__ volatile("sub $20, %rsp;");

    __asm__ volatile(

	// initialize
	"movq $0x7ffffff, %rax;"
	"movq %rax, %xmm1;"
	"movdqa %xmm1, %xmm2;"
	"psllq $27, %xmm2;"
	"movq $0x1ffffffc0, %rax;"
	"movq %rax, %xmm3;"
	"pslldq $6, %xmm3;"
	"movq $0xffffffe, %rax;"
	"movq %rax, %xmm4;"
	"pslldq $10, %xmm4;"
	"movq $0x7ffffff0, %rax;"	
	"movq %rax, %xmm5;"
	"movdqa %xmm5, %xmm6;"
	"psllq $27, %xmm6;"
	"movq $0x1ffffffc, %rax;"
	"movq %rax, %xmm7;"
	"pslldq $7, %xmm7;"
	"movq $0xffffffe0, %rax;"
	"movq %rax, %xmm8;"
	"pslldq $10, %xmm8;"

	// load data, result
	"movq -8(%rbp), %rbx;"
	"movq -16(%rbp), %rdx;"
	"xor %rdi, %rdi;"
	"xor %rsi, %rsi;"

	/*
	 in inner,
	 xmm9 : tmp
	 xmm10: result
	 xmm11: tmp
	 */
    "inner:"
	"movdqu (%rbx, %rdi), %xmm0;"
	// [0]
	"movdqa %xmm0, %xmm10;"
	"pand %xmm1, %xmm10;"
	// [1]
	"movdqa %xmm0, %xmm9;"
	"pand %xmm2, %xmm9;"
	"psllq $5, %xmm9;"
	"pxor %xmm9, %xmm10;"
	// [2]
	"movdqa %xmm0, %xmm9;"
	"pand %xmm3, %xmm9;"
	"movq %xmm9, %xmm11;" //lower,  10-bit in 27-bit
	"psrlq $54, %xmm11;"
	"psrldq $8, %xmm9;" //upper, 17-bit in 27-bit
	"psllq $10, %xmm9;"
	"pxor %xmm11, %xmm9;"
	"pslldq $8, %xmm9;"
	"pxor %xmm9, %xmm10;"
	//[3]
	"movdqa %xmm0, %xmm9;"
	"pand %xmm4, %xmm9;"
	"psllq $15, %xmm9;"
	"pxor %xmm9, %xmm10;"

	"movdqa %xmm10, (%rdx, %rsi, 8);"
	"add $13, %rdi;"
	"add $2, %rsi;"
	"cmp -20(%rbp), %esi;"
	"jg _ret;"

	"movdqu (%rbx, %rdi), %xmm0;"
	// [0]
	"movdqa %xmm0, %xmm10;"
	"pand %xmm5, %xmm10;"
	"psrlq $4, %xmm10;"
        // [1]
	"movdqa %xmm0, %xmm9;"
	"pand %xmm6, %xmm9;"
	"psllq $1, %xmm9;"
	"pxor %xmm9, %xmm10;"
	// [2]
	"movdqa %xmm0, %xmm9;"
	"pand %xmm7, %xmm9;"
	"movq %xmm9, %xmm11;" //lower,  6-bit in 27-bit
	"psrlq $58, %xmm11;"
	"psrldq $8, %xmm9;" //upper, 21-bit in 27-bit
	"psllq $6, %xmm9;"
	"pxor %xmm11, %xmm9;"
	"pslldq $8, %xmm9;"
	"pxor %xmm9, %xmm10;"
	//[3]
	"movdqa %xmm0, %xmm9;"
	"pand %xmm8, %xmm9;"
	"psllq $11, %xmm9;"
	"pxor %xmm9, %xmm10;"

	"movdqa %xmm10, (%rdx, %rsi, 8);"
	"add $14, %rdi;"
	"add $2, %rsi;"
	"cmp -20(%rbp), %esi;"
	"jle inner;"

	);

    __asm__ volatile(
    "_ret:"
        "add $20, %rsp;"
	);
}

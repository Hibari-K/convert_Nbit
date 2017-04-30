#include<stdio.h>
#include<stdlib.h>

#define BITSIZE 512 * 1024 * 1024
#define DIGITS 2 * (BITSIZE/116) // 116 = 29 * 4

void convert_29bit(int* data, int* result, int digits){

    __asm__ volatile("sub $20, %rsp;");

    __asm__ volatile(

	// initialize
	"movq $0x1fffffff, %rax;"
	"movq %rax, %xmm1;"
	"movdqa %xmm1, %xmm2;"
	"psllq $29, %xmm2;"
	"movq $0x7ffffffc, %rax;"
	"movq %rax, %xmm3;"
	"pslldq $7, %xmm3;"
	"movq $0xfffffff80, %rax;"
	"movq %rax, %xmm4;"
	"pslldq $10, %xmm4;"
	"movq $0x1fffffff0, %rax;"
	"movq %rax, %xmm5;"
	"movq $0x3ffffffe, %rax;"
	"movq %rax, %xmm6;"
	"pslldq $4, %xmm6;"
	"movq $0x7ffffffc0, %rax;"
	"movq %rax, %xmm7;"
	"pslldq $7, %xmm7;"
	"movq $0xfffffff8, %rax;"
	"movq %rax, %xmm8;"
	"pslldq $11, %xmm8;"

	// load data, result
	"movq -8(%rbp), %rbx;"
	"movq -16(%rbp), %rdx;"
	"xor %rdi, %rdi;"
	"xor %rsi, %rsi;"

	/*
	 in inner,
	 xmm9 : tmp
	 xmm10: result
	 */
    "inner:"
	"movdqu (%rbx, %rdi), %xmm0;"
	// [0]
	"movdqa %xmm0, %xmm10;"
	"pand %xmm1, %xmm10;"
	// [1]
	"movdqa %xmm0, %xmm9;"
	"pand %xmm2, %xmm9;"
	"psllq $3, %xmm9;"
	"pxor %xmm9, %xmm10;"
	// [2]
	"movdqa %xmm0, %xmm9;"
	"pand %xmm3, %xmm9;"
	"movq %xmm9, %xmm11;" //lower,  6-bit in 29-bit
	"psrlq $58, %xmm11;"
	"psrldq $8, %xmm9;" //upper, 23-bit in 29-bit
	"psllq $6, %xmm9;"
	"pxor %xmm11, %xmm9;"
	"pslldq $8, %xmm9;"
	"pxor %xmm9, %xmm10;"
	//[3]
	"movdqa %xmm0, %xmm9;"
	"pand %xmm4, %xmm9;"
	"psllq $9, %xmm9;"
	"pxor %xmm9, %xmm10;"

	"movdqa %xmm10, (%rdx, %rsi, 8);"
	"add $14, %rdi;"
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
	"psrlq $1, %xmm9;"
	"pxor %xmm9, %xmm10;"
	// [2]
	"movdqa %xmm0, %xmm9;"
	"pand %xmm7, %xmm9;"
	"movq %xmm9, %xmm11;" //lower,  2-bit in 29-bit
	"psrlq $62, %xmm11;"
	"psrldq $8, %xmm9;" //upper, 27-bit in 29-bit
	"psllq $2, %xmm9;"
	"pxor %xmm11, %xmm9;"
	"pslldq $8, %xmm9;"
	"pxor %xmm9, %xmm10;"
	//[3]
	"movdqa %xmm0, %xmm9;"
	"pand %xmm8, %xmm9;"
	"psllq $5, %xmm9;"
	"pxor %xmm9, %xmm10;"

	"movdqa %xmm10, (%rdx, %rsi, 8);"
	"add $15, %rdi;"
	"add $2, %rsi;"
	"cmp -20(%rbp), %esi;"
	"jle inner;"

	);

    
    __asm__ volatile(
    "_ret:"
        "add $20, %rsp;"
	);
}

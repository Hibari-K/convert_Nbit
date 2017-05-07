#include<stdio.h>
#include<stdlib.h>

#define BITSIZE 512 * 1024 * 1024
#define DIGITS 2*(BITSIZE/112) // 112 = 28*4
#define N BITSIZE/32
#define M BITSIZE/28


void convert_28bit(unsigned int* data, unsigned int* result, int digits){

    __asm__ volatile("sub $20, %rsp;");

    __asm__ volatile(

	// initialize
	"movq $0xfffffff, %rax;"
	"movq %rax, %xmm1;"
	"movdqa %xmm1, %xmm2;"
	"psllq $28, %xmm2;"
	"movq %xmm1, %xmm3;"
	"pslldq $7, %xmm3;"
	"movq %xmm2, %xmm4;"
	"pslldq $7, %xmm4;"

	// load data, result
	"movq -8(%rbp), %rbx;"
	"movq -16(%rbp), %rdx;"
	"xor %rdi, %rdi;"
	"xor %rsi, %rsi;"

	/*
	 in split,
	 xmm9 : tmp
	 xmm10: result
	 */
    "split:"
	"movdqu (%rbx, %rdi), %xmm0;"
	// [0]
	"movdqa %xmm0, %xmm10;"
	"pand %xmm1, %xmm10;"
	// [1]
	"movdqa %xmm0, %xmm9;"
	"pand %xmm2, %xmm9;"
	"psllq $4, %xmm9;"
	"pxor %xmm9, %xmm10;"
	// [2]
	"movdqa %xmm0, %xmm9;"
	"pand %xmm3, %xmm9;"
	"pslldq $1, %xmm9;"
	"pxor %xmm9, %xmm10;"
	//[3]
	"movdqa %xmm0, %xmm9;"
	"pand %xmm4, %xmm9;"
	"psllq $12, %xmm9;"
	"pxor %xmm9, %xmm10;"

	"movdqa %xmm10, (%rdx, %rsi, 8);"
	"add $14, %rdi;"
	"add $2, %rsi;"
	"cmp -20(%rbp), %esi;"
	"jle split;"

	);
    
    __asm__ volatile("add $20, %rsp;");
}

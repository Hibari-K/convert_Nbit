#include<stdio.h>

#define BITSIZE 128
#define N BITSIZE/32 //dataの配列数
#define M BITSIZE/29 //resultの配列数を決める指標
#define R BITSIZE - 58 * (BITSIZE / 58) //余りのビット数


void convert_29bit(int* data, int* result){

    // seek
    long* tmp = (long*)data;

    int i;
    long shift_data = tmp[0];

    //M-1, M番目は余り格納用なのでM-2まで
    //i < M-2 としてしまうとMが偶数の時に配列に書ききれなくなる
    for(i=0; i<M-(1+(M&1)); i+=2){

	shift_data = tmp[0] >> (i & 7);

	result[i] = shift_data & 0x1fffffff;
	result[i+1] = (shift_data & 0x3ffffffe0000000) >> 29;

	tmp = (long*)((char*)tmp + 8 - ((i&7) != 6));
    }

    shift_data = tmp[0] >> (i & 7);

    /*
      余りの処理
      M == odd  : 余りが29bitより大きい
      M == even : 余りが29bit以下
    */
    
    const int r = (M&1) ? -(R-64) : -(R-29);

    if(M & 1){
	result[i] = shift_data & 0x1fffffff;
	result[i+1] = (shift_data & (((0xffffffffffffffff << r) >> (r + 29)) << 29)) >> 29;
    }
    else{
	result[i] = shift_data & (0x1fffffff >> r);
    }
}



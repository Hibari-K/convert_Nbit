#include<stdio.h>

#define BITSIZE 512
#define N BITSIZE/32 //dataの配列数
#define M BITSIZE/30 //resultの配列数を決める指標
#define R BITSIZE - 60 * (BITSIZE / 60) //余りのビット数

void convert_30bit(unsigned int* data,unsigned int* result){

    // seek
    long* tmp = (long*)data;

    int i;
    long shift_data = tmp[0];

    //M-1, M番目は余り格納用なのでM-2まで
    //i < M-2 としてしまうとMが偶数の時に配列に書ききれなくなる
    for(i=0; i<M-(1+(M&1)); i+=2){

	shift_data = tmp[0] >> ((i & 3) << 1);

	result[i] = shift_data & 0x3fffffff;
	result[i+1] = (shift_data & 0xfffffffc0000000) >> 30;

	tmp = (long*)((char*)tmp + 7 + ((i&3) == 2));
    }

    shift_data = tmp[0] >> ((i & 3) << 1);

    /*
      余りの処理
      M == odd  : 余りが30bitより大きい
      M == even : 余りが30bit以下
    */
    
    const int r = (M&1) ? -(R-64) : -(R-30);

    if(M & 1){
	result[i] = shift_data & 0x3fffffff;
	result[i+1] = (shift_data & (((0xffffffffffffffff << r) >> (r + 30)) << 30)) >> 30;
    }
    else{
	result[i] = shift_data & (0x3fffffff >> r);
    }
}

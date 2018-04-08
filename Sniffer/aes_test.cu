

#include<stdio.h>
#include "aeslib.h"


#define CHECK(call)                                                            \
{                                                                              \
    const cudaError_t error = call;                                            \
    if (error != cudaSuccess)                                                  \
    {                                                                          \
        printf("Error: %s:%d, ", __FILE__, __LINE__);                 \
        printf("code: %d, reason: %s\n", error,                       \
                cudaGetErrorString(error));                                    \
        exit(1);                                                               \
    }                                                                          \
}


__device__ aes256_context ctx;

__device__ unsigned char key[32];



__device__ void initialize(){

	for( unsigned char i = 0; i < 32; i++ ){
		key[i] = i ;
	}

}

__device__ void encrypt( unsigned char* ptr ){
	printf( "Before encry. %s\n", ptr );
	aes256_init(&ctx, key);
	aes256_encrypt_ecb(&ctx, ptr);
	printf( "Encryped on gpu %s\n", ptr );
	aes256_init(&ctx, key);
	aes256_decrypt_ecb(&ctx, ptr);
	printf( "dEncryped on gpu %s\n", ptr );
	 aes256_done(&ctx);

}


__global__ void copy( unsigned char* ptr ){
	initialize();
	encrypt( ptr );
}


int main(){

	unsigned char* array = (unsigned char* ) malloc( 7 );

	array[0] = 'u';
	array[1] = 'm';
	array[2] = 'i';
	array[3] = 't';
	array[4] = 'a';
	array[5] = 'y';
	array[6] = '\0';

	unsigned char* cc = ( unsigned char* )malloc( sizeof(array) );
	unsigned char* dev ;
    CHECK( cudaMalloc( (void**)&dev, 7 ) );

	CHECK(cudaMemcpy( dev, array, 7, cudaMemcpyHostToDevice ));
	copy<<<1,1>>>(dev);
	CHECK(cudaMemcpy( cc ,dev, 7, cudaMemcpyDeviceToHost ));
	 printf("Cuda status: %s\n", cudaGetErrorString( cudaGetLastError() ) );



	cudaDeviceReset();

	printf( "\nEncryped on gpu %s\n", cc );
	return 0;
}

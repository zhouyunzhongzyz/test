/*
 * mp_pool.c
 *
 *  Created on: Jul 5, 2019
 *      Author: zyz
 */
#include "mem_pool.h"

#define MEMBLOCK_FREE 0
#define MEMBLOCK_BUSY 1
#define MAX_POOL      10

mp_pool* mp_init(uint16_t poolSize, uint16_t bufSize) {
	mp_pool *pool = (mp_pool*) malloc(sizeof(mp_pool));

	if (pool == NULL) {
		goto error;
	}
	memset(pool, 0, sizeof(mp_pool));

	pool->poolSize = poolSize;
	pool->bufSize = bufSize;

	pool->_memUsages = (uint8_t*) malloc(sizeof(uint8_t) * poolSize);
	if (pool->_memUsages == NULL) {
		goto memUsageError;
	}

	memset(pool->_memUsages, MEMBLOCK_FREE, sizeof(uint8_t) * poolSize);

	pool->memBlock = (uint8_t**) malloc(sizeof(uint8_t*) * poolSize);
	memset(pool->memBlock,0,sizeof(uint8_t*) * poolSize);

	if (pool->memBlock == NULL) {
		free(pool->memBlock);
		goto memUsageError;
	}
	for (int i = 0; i <  poolSize; i++) {
         pool->memBlock[i] = (uint8_t*)malloc(bufSize);
         memset(pool->memBlock[i],0,sizeof(bufSize));
	}

	return pool;

	memUsageError:
	   free(pool->_memUsages);

	error:
	   free(pool);

	return NULL;
}

void* mp_alloc(mp_pool *pool) {
	if (pool == NULL)
		return NULL;

	for (int i = 0; i < pool->poolSize; i++) {
		if (pool->_memUsages[i] == MEMBLOCK_FREE) {
			pool->_memUsages[i] = MEMBLOCK_BUSY;
			return pool->memBlock[i];
		}
	}

	return NULL;
}

int mp_free(mp_pool *pool, void *ptr) {

	if (pool == NULL || ptr == NULL)
		return -1;

	for (int i = 0; i < pool->poolSize; ++i) {

		if (pool->_memUsages[i] == MEMBLOCK_BUSY) {

			if (pool->memBlock[i] == ptr) {
				pool->_memUsages[i] = MEMBLOCK_FREE;
				return 0;
			}
		}
	}

	return -1;
}

void mp_deInit(mp_pool *pool) {

	if (pool == NULL)
		return;


	for(int i = 0; i < pool->poolSize; i++) {

			if (pool->memBlock[i])
				free(pool->memBlock[i]);
		}

     if(pool->memBlock)
    	  free(pool->memBlock);

     if(pool->_memUsages)
    	 free(pool->_memUsages);

	free(pool);
	return;
}

//#define TEST_MEM

#ifdef  TEST_MEM

void test_mp(){


	mp_pool* pool = mp_init(MAX_POOL,64);
	uint8_t* mems[MAX_POOL] = {NULL};

	for(int i = 0; i < MAX_POOL; ++i){

		void* data = mp_alloc(pool);
		if(data == NULL){
			printf("memory pool alloc fail\n");
			return;
		}
		mems[i] = data;
	}


	for(int i = 0 ; i < MAX_POOL; i++){
		if(mp_free(pool,mems[i]) == -1){
			printf("memory pool free fail \n");
			return;
		}
	}


	for(int i = 0; i < MAX_POOL; ++i){

		if(pool->_memUsages[i] != MEMBLOCK_FREE){
			printf("memory pool test fail \n");
			return;
		}
	}


   printf("memory pool test success \n");


   mp_deInit(pool);





}
#endif













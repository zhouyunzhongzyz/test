/*
 * mem_pool.h
 *
 *  Created on: Jul 5, 2019
 *      Author: zyz
 */

#ifndef MEM_POOL_H_
#define MEM_POOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef  struct mp_pool_t{
	uint16_t  poolSize;
	uint16_t  bufSize;
	uint8_t** memBlock;
	uint8_t*  _memUsages;
}mp_pool;



mp_pool*  mp_init(uint16_t poolSize,uint16_t bufSize);


void*     mp_alloc(mp_pool* pool);

int       mp_free(mp_pool*  pool,void* ptr);

void      mp_deInit(mp_pool* pool);


void      test_mp();












#endif /* MEM_POOL_H_ */

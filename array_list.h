/*
 * array_list.h
 *
 *  Created on: Jul 5, 2019
 *      Author: zyz
 */

#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct array_list_t;

typedef struct array_list_t{

	struct array_list_t* prev;
	struct array_list_t* next;

    void* data;

    void* _mempool;


}array_list;



array_list* create_array_list(uint16_t capacity);

int array_list_push(array_list* list, void* data);
int  array_get_size(array_list* list);

void array_list_remove(array_list* list,void* data);


void array_list_remove_head(array_list* list);


void array_list_destory(array_list* list);


void dump_array(array_list* list);


void tets_array();



#endif /* ARRAY_LIST_H_ */

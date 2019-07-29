/*
 * array_list.c
 *
 *  Created on: Jul 5, 2019
 *      Author: zyz
 */


#include "array_list.h"
#include "mem_pool.h"
#define  MAX_ARRAY_CAPACITY 10


array_list* create_array_list(uint16_t capacity){
	array_list*  list = NULL;
    if(capacity <= 0)
    	capacity =  MAX_ARRAY_CAPACITY;

    list = (array_list*)malloc(sizeof(array_list));

    memset(list,0,sizeof(array_list));

    list->_mempool = mp_init(capacity,sizeof(array_list));

	return list;
}



int  array_get_size(array_list* list){
	int count = 0;
    if(list == NULL)
    	 return 0;
	array_list* node = list->next;
	while(node != NULL){
		count++;
		node = node->next;
	}
	return count;
}

int array_list_push(array_list* list, void* data){
   if(list == NULL || data == NULL)
      return -1;
    array_list* node = (array_list*)mp_alloc((mp_pool*)list->_mempool);
    if(node == NULL)
       return -1;
    node->data = data;

    while(list->next != NULL)
        list = list->next;

    node->prev = list;
    node->next = NULL;
    node->_mempool = list->_mempool;
    list->next = node;
    return 0;

}


void array_list_remove(array_list* list,void* data){

	if(list == NULL)
		return;
	array_list* node = list->next;
	while(node){

		if(node->data == data){
           node->prev->next = node->next;
           if(node->next){
        	   node->next->prev = node->prev;
           }
           mp_free((mp_pool*)list->_mempool,node);

		}
		node = node->next;
	}

}

void array_list_remove_head(array_list* list){

	if(list == NULL)
		return;

     array_list* node = list->next;

     if(node == NULL)
    	 return;

     list->next = node->next;
     if(node->next)
    	 node->next->prev = list;
      mp_free((mp_pool*)list->_mempool,node);



}


void array_list_destory(array_list* list){

	if(list == NULL)
			return;

	     array_list* node = list->next;


	     while(node){
	    	 mp_free((mp_pool*)list->_mempool,node);
	    	 node = node->next;
	     }

	    mp_deInit((mp_pool*)list->_mempool);
	    free(list);
}




void dump_array(array_list* list){

	if(list == NULL)
		return;

    printf("*****dump array *********\n");

    array_list *node = list->next;

    while(node){
    	  uint8_t* tmp = (uint8_t*)node->data;
    	  for(int i = 0; i < strlen(tmp);i++)
    	  printf("push data is %d \n",tmp[i]);

        printf("*******get data end******\n");
    	node = node->next;
    }
	//for(int i = 0; i < sizeof(list->data); i++)
		//printf("push  %d------\n",*((int*)list->data));
}

void tets_array(){

	array_list* list = create_array_list(MAX_ARRAY_CAPACITY);


	 uint8_t data[] = {1,2,34,99,0};
	 uint8_t data_end[] = {6,7,8,8,8,9,0};
	  array_list_push(list, (void*)data);
	//array_list_push(list, bufEnd);
	array_list_push(list, (void*)data_end);





    int size = array_get_size(list);

    dump_array(list);
    printf("*********array size is %d ************\n",size);
    array_list_remove_head(list);
    size = array_get_size(list);
    dump_array(list);
    printf("*********array size is %d ************\n",size);

    array_list_destory(list);

}

















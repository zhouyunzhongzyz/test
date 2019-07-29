/*
 * list_test.h
 *
 *  Created on: Jul 29, 2019
 *      Author: zyz
 */

#ifndef LIST_TEST_H_
#define LIST_TEST_H_

#include "list.h"


typedef struct person {
	int age;
	int weight;
	struct list_head list;


}person_t;



void test_list(){
	person_t *person;
	struct list_head *pos;
	person_t person_head;


	LIST_HEAD(plist);

	for(int i = 0; i < 5; i++){
		person = (person_t *)malloc(sizeof(person_t));
		person->age = i+2;
		person->weight = i+4;
		list_add_tail(&(person->list),&plist);
	}

	printf("\n");
	printf("======== print list after delete a node which age is 15 =======\n");

	list_for_each(pos,&plist){
		person = list_entry(pos,struct person,list);
        printf("person  age %d  weight %d \n",person->age,person->weight);
	}

	list_for_each(pos, &plist){
			person = list_entry(pos,struct person,list);
			printf("------\n");
			list_del_entry(pos);
			free(person);
	}

	list_for_each(pos,&plist){
			person = list_entry(pos,struct person,list);
	        printf("person2  age %d  weight %d \n",person->age,person->weight);
		}
}







#endif /* LIST_TEST_H_ */

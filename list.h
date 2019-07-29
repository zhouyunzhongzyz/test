/*
 * list.h
 *
 *  Created on: Jul 29, 2019
 *      Author: zyz
 */

#ifndef LIST_H_
#define LIST_H_

#include <stddef.h>
#include <stdbool.h>

#define prefetch(x)


#define container_of(ptr, type, member) ({\
       const typeof( ((type *)0)->member ) *__mptr = (ptr);\
          (type*)((char *)__mptr - offsetof(type,member));})


struct list_head{

	struct list_head *prev;
	struct list_head *next;

};

#define LIST_HEAD_INIT(name) { &(name), &(name)}

#undef LIST_HEAD
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list){

	list->next = list->prev = list;
}

static inline bool list_empty(const struct list_head* head){
	return (head->next == head);
}

static inline bool list_is_first(const struct list_head* head){
	return (head->prev == head);
}

static inline bool list_is_last(const struct list_head* head){
	return (head->next == head);
}

static inline void _list_del(struct list_head *prev,struct list_head *next){
	next->prev = prev;
	prev->next = next;
}

static inline void list_del_entry(struct list_head *entry){
	_list_del(entry->prev,entry->next);
}


static inline void _list_add(struct list_head  *new,
		                        struct list_head *prev,
							   struct list_head  *next){
	next->prev = new;
	prev->next = new;
	new->prev = prev;
	new->next = next;

}


static inline void list_del_init(struct list_head* entry){
	list_del_entry(entry);
	INIT_LIST_HEAD(entry);
}


#define list_entry(ptr, type, member) \
       container_of(ptr, type, member)

#define list_first_entry(ptr,type,field) list_entry((ptr)->next,type,field)
#define list_last_entry(ptr,type,field)  list_entry((ptr)->prev,type,field)


#define list_for_each(p,head)     \
	for(p = ((head)->next); p != (head); p = p->next)

#define list_for_each_safe(p,n,head) \
	for(p = (head)->next,n = p->next;p != (head);p = n,n = p->next)

#define list_for_each_entry(p,h,field) \
	for(p = list_first_entry(h,typeof(*p),field); &p->field != (h); \
	   p = list_entry(p->field.next,typeof(*p) ,field);



static inline void list_add(struct list_head *new,struct list_head *head){
	_list_add(new,head,head->next);
}

static inline void list_add_tail(struct list_head *new,struct list_head* head){
	_list_add(new,head->prev,head);
}


static inline void list_move(struct list_head* list,struct list_head* head){
	list_del_entry(list);
	list_add(list,head);
}

static inline void list_move_tail(struct list_head* list,struct list_head* head){
	list_del_entry(list);
	list_add_tail(list,head);
}




#endif /* LIST_H_ */

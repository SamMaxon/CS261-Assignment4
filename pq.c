/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name:
 * Email:
 */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */

struct pq {
	struct dynarray* p;
	struct dynarray* v;  
};


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */

struct pq* pq_create() {
	struct pq* pq = malloc(sizeof(struct pq)); 
	assert(pq); 

	pq->p = dynarray_create();
	pq->v = dynarray_create();  

	return pq; 
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	
	dynarray_free(pq->p);
	dynarray_free(pq->v);
	free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
	if(dynarray_size(pq->p) == 0)
		return 1; 
	return 0; 
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
int parent(int i) { return ((i-1)/2); }
int right(int i) { return ((2*i)+2); }
int left(int i) { return ((2*i)+1); }

void swap(struct dynarray* da, int curr, int parent){
 
	void* parent_point = dynarray_get(da, parent); 
	void* curr_point = dynarray_get(da, curr); 
	void* temp = parent_point; 

	dynarray_set(da, parent, curr_point); 
	dynarray_set(da, curr, temp); 

}

void pq_insert(struct pq* pq, void* value, int priority) {
	int i; 
	int* priority_temp = (int*)malloc(sizeof(int));
	*priority_temp = priority; 
	if(pq_isempty(pq)){
		dynarray_insert(pq->p, -1, priority_temp); 
		dynarray_insert(pq->v, -1, value); 
	}else{
		dynarray_insert(pq->p, -1, priority_temp); 
		dynarray_insert(pq->v, -1, value); 
		
		i = (dynarray_size(pq->p) - 1);
		
		while(i != 0 && *(int*)(dynarray_get(pq->p, parent(i))) > *(int*)(dynarray_get(pq->p, i))){
			swap(pq->p, i, parent(i)); 
			swap(pq->v, i, parent(i)); 
			i = parent(i);
		}		
	}
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
	assert(pq);
	int* val = dynarray_get(pq->v, 0); 
	return val; 
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
	assert(pq); 
	int* p = dynarray_get(pq->p, 0); 
	return *p; 
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */

void perc(struct pq* pq, int idx){

	int l = left(idx); 
	int r = right(idx);
	int small = idx;  
	if(l < dynarray_size(pq->p) && *(int*)dynarray_get(pq->p, l) < *(int*)dynarray_get(pq->p, idx))
		small = l; 
	if(r < dynarray_size(pq->p) && *(int*)dynarray_get(pq->p, r) < *(int*)dynarray_get(pq->p, small))
		small = r; 
	if(small != idx){
		swap(pq->p, idx, small);
		swap(pq->v, idx, small);  
		perc(pq, small);
	}
}

void* pq_remove_first(struct pq* pq) {
	int* temp = dynarray_get(pq->p, 0);
	int* temp_return = dynarray_get(pq->v, 0);  
	
	free(dynarray_get(pq->p, 0)); 
		
	dynarray_set(pq->p, 0, dynarray_get(pq->p, -1)); 
	dynarray_set(pq->v, 0, dynarray_get(pq->v, -1)); 
	
	dynarray_remove(pq->p, -1); 
	dynarray_remove(pq->v, -1); 

	perc(pq, 0); 

	return temp_return; 

}

//************************************************************************************************************************
//
//	CS261- Assignment 2 - dynamicArray.c
//
//	Name:			Tucker Dane Walker
//	Date:			07 July 2017
//	Description:	Dynamic Array implementation.
//
//***********************************************************************************************************************/

#include <assert.h>
#include <stdlib.h>
#include "dynArray.h"

struct DynArr
{
	TYPE *data;		/* pointer to the data array		*/
	int size;		/* number of elements in the array	*/
	int capacity;	/* capacity of the array			*/
};


/* ************************************************************************
	Dynamic Array Functions
************************************************************************ */

/* Initialize (including allocation of data array) dynamic array.

	param: 	v - pointer to the dynamic array
	param:	capacity - capacity of the dynamic array
	pre:	v is not null
	post:	internal data array can hold capacity elements
	post:	v->data is not null
*/
void initDynArr(DynArr *v, int capacity)
{
	assert(capacity > 0);									/* assert that the structure can contain elements							*/
	assert(v!= 0);											/* assert that v is not a NULL pointer										*/
	v->data = (TYPE *) malloc(sizeof(TYPE) * capacity);		/* create an array that can contain capacity TYPEs and set equal to data	*/
	assert(v->data != 0);									/* confirm that data is not NULL; that it is allocated						*/
	v->size = 0;											/* initialize the size of the array to 0 elements							*/
	v->capacity = capacity;									/* initialize the capacity of the array to capacity elements				*/
}

/* Allocate and initialize dynamic array.

	param:	cap 	desired capacity for the dyn array
	pre:	none
	post:	none
	ret:	a non-null pointer to a dynArr of cap capacity
			and 0 elements in it.		
*/
DynArr* newDynArr(int cap)
{
	assert(cap > 0);										/* ensure the capacity passed for desired new array is greater than zero	*/
	DynArr *r = (DynArr *)malloc(sizeof( DynArr));			/* allocate memory for a DynArr struct										*/
	assert(r != 0);											/* confirm that r was allocated												*/
	initDynArr(r,cap);										/* initialize the carray to capacity elements								*/
	return r;												/* return a pointer, r, to the DynArr struct								*/
}

/* Deallocate data array in dynamic array. 

	param: 	v		pointer to the dynamic array
	pre:	none
	post:	d.data points to null
	post:	size and capacity are 0
	post:	the memory used by v->data is freed
*/
void freeDynArr(DynArr *v)
{
	if(v->data != 0)
	{
		free(v->data);										/* free the space on the heap */
		v->data = 0;   										/* make it point to null */
	}
	v->size = 0;
	v->capacity = 0;
}

/* Deallocate data array and the dynamic array. 

	param: 	v		pointer to the dynamic array
	pre:	none
	post:	the memory used by v->data is freed
	post:	the memory used by d is freed
*/
void deleteDynArr(DynArr *v)
{
	freeDynArr(v);
	free(v);
}

/* Resizes the underlying array to be the size cap 

	param: 	v		pointer to the dynamic array
	param:	cap		the new desired capacity
	pre:	v is not null
	post:	v has capacity newCap
*/
void _dynArrSetCapacity(DynArr *v, int newCap)
{	
	/* FIXME: You will write this function */
	assert(newCap > 0);
	assert(v != 0);
		
	DynArr *newArr = newDynArr(newCap);		/* initialize a new Dynamic Array with capacity newCap */

	/* fill the new array with values from the old array */
	for (int i = 0; i < v->size; i++)
	{
		addDynArr(newArr, v->data[i]);
	}
	/*newArr->size = v->size;
	newArr->data = v->data;*/

	/* free old array */
	freeDynArr(v);

	/* point v to new array */
	*v = *newArr;

	free(newArr);
}

/* Get the size of the dynamic array

	param: 	v		pointer to the dynamic array
	pre:	v is not null
	post:	none
	ret:	the size of the dynamic array
*/
int sizeDynArr(DynArr *v)
{
	return v->size;
}

/* 	Adds an element to the end of the dynamic array

	param: 	v		pointer to the dynamic array
	param:	val		the value to add to the end of the dynamic array
	pre:	the dynArry is not null
	post:	size increases by 1
	post:	if reached capacity, capacity is doubled
	post:	val is in the last utilized position in the array
*/
void addDynArr(DynArr *v, TYPE val)
{
	if(!LT(v->size, v->capacity))				/* if the array is full, double its capacity */
	{
		_dynArrSetCapacity(v, v->capacity * 2);
	}

	v->data[v->size] = val;						/* add val to the next position in the array */
	v->size++;									/* increment size to reflect the additional data element */
}

/*	Get an element from the dynamic array from a specified position
	
	param: 	v		pointer to the dynamic array
	param:	pos		integer index to get the element from
	pre:	v is not null
	pre:	v is not empty
	pre:	pos < size of the dyn array and >= 0
	post:	no changes to the dyn Array
	ret:	value stored at index pos
*/
TYPE getDynArr(DynArr *v, int pos)
{

	assert(v != 0);						/* ensure v is not null */
	assert(v->size > 0);				/* ensure v is not empty */
	assert(pos < v->size + 1);			/* ensure position is valid */

	return (v->data[pos]);				/* return the value at the requested position */ 
}

/*	Put an item into the dynamic array at the specified location,
	overwriting the element that was there

	param: 	v		pointer to the dynamic array
	param:	pos		the index to put the value into
	param:	val		the value to insert 
	pre:	v is not null
	pre:	v is not empty
	pre:	pos >= 0 and pos < size of the array
	post:	index pos contains new value, val
*/
void putDynArr(DynArr *v, int pos, TYPE val)
{
	assert(v != 0);				/* ensure v is not null */
	assert(v->size > 0);		/* ensure v is not empty */
	assert(pos >= 0);			/* ensure pos is non-negative (within the left bound of the array) */
	assert(pos < v->size);		/* ensure pos is within the right bound of the array */

	v->data[pos] = val;			/* replace value at position with the new value */
}

/*	Swap two specified elements in the dynamic array

	param: 	v		pointer to the dynamic array
	param:	i,j		the elements to be swapped
	pre:	v is not null
	pre:	v is not empty
	pre:	i, j >= 0 and i,j < size of the dynamic array
	post:	index i now holds the value at j and index j now holds the value at i
*/
void swapDynArr(DynArr *v, int i, int  j)
{
	assert(v != 0);				/* ensure v is not null */
	assert(v->size > 0);		/* ensure v is not empty */
	assert(i >= 0);				/* ensure i is non-negative (within the left bound of the array) */
	assert(i < v->size);		/* ensure i is within the right bound of the array */
	assert(j >= 0);				/* ensure j is non-negative (within the left bound of the array) */
	assert(j < v->size);		/* ensure j is within the right bound of the array */

	/* swap elements using temp */
	TYPE temp = v->data[i];
	v->data[i] = v->data[j];
	v->data[j] = temp;
}

/*	Remove the element at the specified location from the array,
	shifts other elements back one to fill the gap

	param: 	v		pointer to the dynamic array
	param:	idx		location of element to remove
	pre:	v is not null
	pre:	v is not empty
	pre:	idx < size and idx >= 0
	post:	the element at idx is removed
	post:	the elements past idx are moved back one
*/
void removeAtDynArr(DynArr *v, int idx)
{
	assert(v != 0);				/* ensure v is not null */
	assert(v->size > 0);		/* ensure v is not empty */
	assert(idx >= 0);			/* ensure idx is non-negative (within the left bound of the array) */
	assert(idx < v->size);		/* ensure idx is within the right bound of the array */

	for (int i = idx; i < v->size - 1; i++)
	{
		v->data[i] = v->data[i + 1];
	}

	v->size--;
	v->data[v->size] = NULL;
}


/* ************************************************************************
	Stack Interface Functions
************************************************************************ */

/*	Returns boolean (encoded in an int) demonstrating whether or not the 
	dynamic array stack has an item on it.

	param:	v		pointer to the dynamic array
	pre:	the dynArr is not null
	post:	none
	ret:	1 if empty, otherwise 0
*/
int isEmptyDynArr(DynArr *v)
{
	assert(v != 0);			/* ensure v is not null */
	
	if (v->size == 0)
		return 1;
	else
		return 0;
}

/* 	Push an element onto the top of the stack

	param:	v		pointer to the dynamic array
	param:	val		the value to push onto the stack
	pre:	v is not null
	post:	size increases by 1
			if reached capacity, capacity is doubled
			val is on the top of the stack
*/
void pushDynArr(DynArr *v, TYPE val)
{
	assert(v != 0);				/* ensure v is not null */
	addDynArr(v, val);
}

/*	Returns the element at the top of the stack 

	param:	v		pointer to the dynamic array
	pre:	v is not null
	pre:	v is not empty
	post:	no changes to the stack
*/
TYPE topDynArr(DynArr *v)
{
	assert(v != 0);					/* ensure v is not null */
	assert(v->size != 0);			/* ensure v is not empty */

	return getDynArr(v, v->size-1);	/* return the value on the top of the stack */
}

/* Removes the element on top of the stack 

	param:	v		pointer to the dynamic array
	pre:	v is not null
	pre:	v is not empty
	post:	size is decremented by 1
			the top has been removed
*/
void popDynArr(DynArr *v)
{
	assert(v != 0);				/* ensure v is not null */
	assert(v->size != 0);		/* ensure v is not empty */

	v->data[v->size-1] = NULL;	/* replace the top element with a NULL value */
	v->size--;					/* reduce the size of the array */
}

/* ************************************************************************
	Bag Interface Functions
************************************************************************ */

/*	Returns boolean (encoded as an int) demonstrating whether or not
	the specified value is in the collection
	true = 1
	false = 0

	param:	v		pointer to the dynamic array
	param:	val		the value to look for in the bag
	pre:	v is not null
	pre:	v is not empty
	post:	no changes to the bag
*/
int containsDynArr(DynArr *v, TYPE val)
{
	assert(v != 0);							/* ensure v is not null */
	assert(v->size != 0);					/* ensure v is not empty */

	for (int i = 0; i < v->size; i++)		/* run through the array until the value is found */
	{
		if (v->data[i] == val)
		{
			return 1;						/* if it is found, return 1 for true: the value is in the array */
		}
	}
	return 0;								/* if it is not found, return 0: the value is not in the array */
}

/*	Removes the first occurrence of the specified value from the collection
	if it occurs

	param:	v		pointer to the dynamic array
	param:	val		the value to remove from the array
	pre:	v is not null
	pre:	v is not empty
	post:	val has been removed
	post:	size of the bag is reduced by 1
*/
void removeDynArr(DynArr *v, TYPE val)
{
	assert(v != 0);							/* ensure v is not null */
	assert(v->size != 0);					/* ensure v is not empty */

	if (containsDynArr(v, val))
	{
		int i = 0;
		while (i < v->size)					/* run through the array until the value is found */
		{
			if (v->data[i] == val)
			{
				removeAtDynArr(v, i);		/* when it is found, remove it */
				i = v->size;				/* skip to the end of the array; only the first ocurrance is to be removed. */
			}
			i++;
		}
	}
}

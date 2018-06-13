//Written by indexohan Eliasson <indexohane@cs.umu.se>.
//May be used in the course Datastrukturer och Algoritmer (C) at Umeå University.
//Usage exept those listed above requires permission by the author.
#include <stdlib.h>

#include <stdio.h>
#include "table.h"
#include "array_1d.h"


#define ARRAYSIZE 40000	//Needs to be the same as TABLESIZE in tabletest.c


typedef struct table {
	array_1d *entries;
	compare_function *key_cmp_func;
	free_function key_free_func;
	free_function value_free_func;
    int nextIndexAvailable;
} table;

typedef struct table_entry {
	void *key;
	void *value;
} table_entry;



/* Creates a table.
 *  compare_function - Pointer to a function that is called for comparing
 *                     two keys. The function should return <0 if the left
 *                     parameter is smaller than the right parameter, 0 if
 *                     the parameters are equal, and >0 if the left
 *                     parameter is larger than the right item.
 * Returns: A pointer to the table. NULL if creation of the table failed.
 * Simplified asymptotic complexity analysis : O(1)
 * */
table *table_empty(compare_function *key_cmp_func,
		   free_function key_free_func,
		   free_function value_free_func)
{
	//Creating pointer to table.
	table *t = calloc(sizeof (table),1);
	//Creating array
	t->entries = array_1d_create(free,0,ARRAYSIZE);
	// Store the key compare function and key/value free functions.
	t->key_cmp_func = key_cmp_func;
	t->key_free_func = key_free_func;
	t->value_free_func = value_free_func;
	t->nextIndexAvailable = 0;
	return t;
}


/**
 * table_is_empty() - Check if a table is empty.
 * @table: Table to check.
 *
 * Return: True if table contains no key/value pairs, false otherwise.
 * Simplified asymptotic complexity analysis : O(1)
 *
 */
bool table_is_empty(const table *t)
{
	//Pointer to the table.
	table *tablePointer = (table*)t;
	//Simple lookup if nextIndexAvailable equals 0.
	if(tablePointer->nextIndexAvailable == 0){
		return true;
	}
	return false;
}

/**
 * table_insert() - Add a key/value pair to a table.
 * @table: Table to manipulate.
 * @key: A pointer to the key value.
 * @value: A pointer to the value value.
 *
 * Insert the key/value pair into the table. No test is performed to
 * check if key is a duplicate. table_lookup() will return the latest
 * added value for a duplicate key. table_remove() will remove all
 * duplicates for a given key.
 *
 * Returns: Nothing.
 * Simplified asymptotic complexity analysis : O(n)
 */
void table_insert(table *t, void *key, void *value)
{
	//Pointer to the table.
	table *tablePointer = (table*)t;
	//Used as working pointer
	table_entry *checkElement;
	//Used to insert element.
	table_entry *insertElement = malloc(sizeof(table_entry));

	//Values who will be inserted.
	insertElement->key = key;
	insertElement->value = value;

	//Exist to make sure we won't go to the wrong places.
	bool inserted = false;

	int index = 0;
	/*
	 * There are three different situations we insert values.
	 * 1. Table is empty, just insert in array[0] slot.
	 * 2. Table key exist and we want to overwrite, traverse through the array until we find the
	 * right key and overwrite it.
	 * 3. If key does not exist and array is not empty. Insert in next available slot.
	 */

	//If table is empty
	if(table_is_empty(tablePointer) == true){
		//Insert first slot of the array.
		array_1d_set_value(tablePointer->entries,insertElement,tablePointer->nextIndexAvailable);
		checkElement = array_1d_inspect_value(tablePointer->entries, index);
		inserted = true;					//Used to exit loop and function.
		tablePointer->nextIndexAvailable++;	//Update inserted slots of array.
	}

	//Traverse through all values to see if key exist, then replace if it does.
	while(array_1d_has_value(tablePointer->entries,index) && inserted == false){
		//Look at current slot of array
		checkElement = array_1d_inspect_value(tablePointer->entries, index);
		if(tablePointer->key_cmp_func(checkElement->key, key) == 0){	//See if key match
			//Remove whatever is there to avoid memory-leak.
			if (t->key_free_func != NULL) {
				t->key_free_func(checkElement->key);
			}
			if (t->value_free_func != NULL) {
				t->value_free_func(checkElement->value);
			}

			array_1d_set_value(tablePointer->entries,insertElement,index);
			checkElement = array_1d_inspect_value(tablePointer->entries, index);
			inserted = true;		//Used to exit loop and function.
		}
		index++;	//Check next index of array.
	}
	//If Key do not exist put it to next available slot.
	if(inserted == false){
		array_1d_set_value(tablePointer->entries,insertElement,tablePointer->nextIndexAvailable);
		checkElement = array_1d_inspect_value(tablePointer->entries, tablePointer->nextIndexAvailable);
		inserted = true;					//Used to exit loop and function.
		tablePointer->nextIndexAvailable++;	//Update nextIndexAvailable in the array.
	}
}

/**
 * table_lookup() - Look up a given key in a table.
 * @table: Table to inspect.
 * @key: Key to look up.
 *
 * Return: The value corresponding to a given key, or NULL if the key
 * is not found in the table. If the table contains duplicate keys,
 * the value that was latest inserted will be returned.
 * Simplified asymptotic complexity analysis : O(n)
 */
void *table_lookup(const table *t, const void *key)
{
	//Pointer used to check element.
	table_entry *checkElement;
	//Pointer for table.
	table *tablePointer = (table*)t;
	int index = array_1d_low(t->entries);
	//Traverse through all values to see if key exist, then replace if it does.
	while(array_1d_has_value(tablePointer->entries,index)){
		//Look at current slot of array
		checkElement = array_1d_inspect_value(tablePointer->entries, index);
		if(tablePointer->key_cmp_func(checkElement->key, key) == 0){	//If key found, return value.
			return checkElement->value;
		}
		index++;	//Point at the next slow.
	}
	return NULL;
}

/**
 * table_remove() - Remove a key/value pair in the table.
 * @table: Table to manipulate.
 * @key: Key for which to remove pair.
 *
 * Any matching duplicates will be removed. Will call any free
 * functions set for keys/values. Does nothing if key is not found in
 * the table.
 *
 * Returns: Nothing.
 * Simplified asymptotic complexity analysis : O(n)
 */
void table_remove(table *t, const void *key)
{
	//Pointer to check element.
	table_entry *checkElement;
	//Pointer used for moving element.
	table_entry *moveElement = malloc(sizeof(table_entry));
	//Pointer to table.
	table *tablePointer = (table*)t;
	int index = array_1d_low(t->entries);
	int removedIndex;
	bool removed = false;
	//Traverse through all values to see if key exist, then remove if it does.
	while(array_1d_has_value(tablePointer->entries,index) && removed == false){
		checkElement = array_1d_inspect_value(tablePointer->entries, index);	//Load current slot
		if(tablePointer->key_cmp_func(checkElement->key, key) == 0){	//If key found, remove it.
			if (t->key_free_func != NULL) {
				t->key_free_func(checkElement->key);
			}
			if (t->value_free_func != NULL) {
				t->value_free_func(checkElement->value);
			}
			array_1d_set_value(tablePointer->entries,NULL,index);
			removedIndex = index;
			tablePointer->nextIndexAvailable--;	//Update nextIndexAvailable
			removed = true;
		}
		index++;	//Check next index.
	}
	//Move last element of array to removed index to make sure there are no "holes" in the array.
	//Don't if removed element was the last element in the array.
	if(removed && tablePointer->nextIndexAvailable != removedIndex){
		checkElement = array_1d_inspect_value(tablePointer->entries, tablePointer->nextIndexAvailable);
		//Copy the element at the end of the array to the index where the removed element used to be.
		moveElement->key = checkElement->key;
		moveElement->value = checkElement->value;
		array_1d_set_value(tablePointer->entries,moveElement,removedIndex);
		//Remove element at the end of the array.
		array_1d_set_value(tablePointer->entries,NULL,tablePointer->nextIndexAvailable);
	}
	else{
		free(moveElement);	//Free if not used
	}
}

/*
 * table_kill() - Destroy a table.
 * @table: Table to destroy.
 *
 * Return all dynamic memory used by the table and its elements. If a
 * free_func was registered for keys and/or values at table creation,
 * it is called each element to free any user-allocated memory
 * occupied by the element values.
 *
 * Returns: Nothing.
 * Simplified asymptotic complexity analysis : O(n)
 */
void table_kill(table *t)
{
	int index = array_1d_low(t->entries);
	//Pointer to table.
	table *tablePointer = (table*)t;
	//Pointer to element to check.
	table_entry *checkElement;
	//Traverse through array. Remove all elements.
	while(array_1d_has_value(tablePointer->entries,index) && index < tablePointer->nextIndexAvailable+1){
		checkElement = array_1d_inspect_value(tablePointer->entries, index);	//Load current slot
			if (t->key_free_func != NULL) {
				t->key_free_func(checkElement->key);
			}
			if (t->value_free_func != NULL) {
				t->value_free_func(checkElement->value);
			}
		array_1d_set_value(tablePointer->entries,NULL,index);
		index++;
	}
	array_1d_kill(t->entries);
	free(t);
}
/*
 * Used for printing table, useful while debugging.
 */
void table_print(const table *t)
{
	table *tablePointer = (table*)t;
	table_entry *checkElement;

	int index = array_1d_low(t->entries);
	while(array_1d_has_value(tablePointer->entries,index)){
		checkElement = array_1d_inspect_value(tablePointer->entries, index);	//Load current slot
		printf("key->%s value->%s\n",(char*)checkElement->key, (char*)checkElement->value);
		index++;
	}
	printf("\n");
}

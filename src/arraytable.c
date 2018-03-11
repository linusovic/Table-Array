//Written by indexohan Eliasson <indexohane@cs.umu.se>.
//May be used in the course Datastrukturer och Algoritmer (C) at Umeå University.
//Usage exept those listed above requires permission by the author.
#include <stdlib.h>

#include <stdio.h>
#include "table.h"
#include "array_1d.h"


#define ARRAYSIZE 10


typedef struct table {
	array_1d *entries;
	compare_function *key_cmp_func;
	free_function key_free_func;
	free_function value_free_func;
    int freeIndex;
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
 * Returns: A pointer to the table. NULL if creation of the table failed. */
table *table_empty(compare_function *key_cmp_func,
		   free_function key_free_func,
		   free_function value_free_func)
{
	table *t = calloc(sizeof (table),1);
	t->entries = array_1d_create(free,0,ARRAYSIZE);
	// Store the key compare function and key/value free functions.
	t->key_cmp_func = key_cmp_func;
	t->key_free_func = key_free_func;
	t->value_free_func = value_free_func;
	t->freeIndex = 0;
	return t;
}


/**
 * table_is_empty() - Check if a table is empty.
 * @table: Table to check.
 *
 * Return: True if table contains no key/value pairs, false otherwise.
 */
bool table_is_empty(const table *t)
{
	table *tablePointer = (table*)t;
	if(tablePointer->freeIndex == 0){
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
 */
void table_insert(table *t, void *key, void *value)
{
	table *tablePointer = (table*)t;
	//Value who will changed through the array
	table_entry *checkElement = malloc(sizeof(table_entry));

	table_entry *insertElement = malloc(sizeof(table_entry));

	//Values who will be inserted.
	insertElement->key = key;
	insertElement->value = value;



	bool inserted = false;
	int index = 0;	//Used as index.
	//If table is empty indexust pick the first slot in it.
	if(table_is_empty(tablePointer) == true){
		//insert first slot of the array.
		array_1d_set_value(tablePointer->entries,insertElement,tablePointer->freeIndex);
		inserted = true;					//Used to exit loop and function.
		tablePointer->freeIndex++;	//Update inserted slots of array.
	}

	//Traverse through all values to see if key exist, then replace if it does.
	while(array_1d_has_value(tablePointer->entries,index) && inserted == false){
		//Look att current slot of array
		checkElement = array_1d_inspect_value(tablePointer->entries, index);
		if(tablePointer->key_cmp_func(checkElement->key, key) == 0){	//See if key match
			array_1d_set_value(tablePointer->entries,insertElement,index);
			tablePointer->freeIndex++;	//Update inserted slots of array.
			inserted = true;		//Used to exit loop and function.
		}
		index++;	//Check next index of array.
	}
	//If Key do not exist put it to next available slot.


	if(inserted == false){
		array_1d_set_value(tablePointer->entries,insertElement,tablePointer->freeIndex);
		tablePointer->freeIndex++;
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
 */
void *table_lookup(const table *t, const void *key)
{
	table_entry *checkElement = malloc(sizeof(table_entry));
	table *tablePointer = (table*)t;
	int index = array_1d_low(t->entries);
	//Traverse through all values to see if key exist, then replace if it does.
	bool found = false;
	while(array_1d_has_value(tablePointer->entries,index)){
		//Look att current slot of array
		checkElement = array_1d_inspect_value(tablePointer->entries, index);
		//		checkElement->key == key //Implementera key-compare.
		if(tablePointer->key_cmp_func(checkElement->key, key) == 0){	//Implementera key-compare.
		//	printf("looked up element %s %s\n", checkElement->key, checkElement->value);
			return checkElement->value;
		}
		printf("update index\n");
		index++;
	}
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
 */
void table_remove(table *t, const void *key)
{
	table_entry *checkElement = malloc(sizeof(table_entry));
	table *tablePointer = (table*)t;
	int index = array_1d_low(t->entries);
	//Traverse through all values to see if key exist, then replace if it does.
	bool found = false;
	while(array_1d_has_value(tablePointer->entries,index)){
		//Look att current slot of array
		checkElement = array_1d_inspect_value(tablePointer->entries, index);
		//		checkElement->key == key //Implementera key-compare.
		if(tablePointer->key_cmp_func(checkElement->key, key) == 0){	//Implementera key-compare.
			//Remove element in array

		}
		printf("update index\n");
		index++;
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
 */
void table_kill(table *t)
{

}

void table_print(const table *t, inspect_callback_pair print_func)
{

}

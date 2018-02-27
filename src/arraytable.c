//Written by Johan Eliasson <johane@cs.umu.se>.
//May be used in the course Datastrukturer och Algoritmer (C) at Umeå University.
//Usage exept those listed above requires permission by the author.
#include <stdlib.h>

#include <stdio.h>
#include "table.h"
#include "array_1d.h"


#define ARRAYSIZE 10


typedef struct ArrayTable {
	array_1d *entries;
	compare_function *key_cmp_func;
	free_function key_free_func;
	free_function value_free_func;
    int HowBigIsTheArray;

} ArrayTable;



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
	ArrayTable *t = calloc(sizeof (ArrayTable),1);
	t->entries = array_1d_create(free,0,ARRAYSIZE);
	// Store the key compare function and key/value free functions.
	t->key_cmp_func = key_cmp_func;
	t->key_free_func = key_free_func;
	t->value_free_func = value_free_func;
	t->HowBigIsTheArray = 0;
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
	ArrayTable *t1 = (ArrayTable*)t;
	printf("t1->HowBigIsTheArray = %d\n", t1->HowBigIsTheArray);
	if(t1->HowBigIsTheArray == 0){
		printf("True\n");
		return true;
	}
	printf("false\n");
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
	ArrayTable *t1 = (ArrayTable*)t;
	int j = 0;

	//If table is empty just pick the first slot in it.
	if(table_is_empty(t1) == true){
		//insert first slot of the array.
		array_1d_set_value(t1,value,array_1d_low(t1));
	}

	//Traverse through all values to see if key exist, then replace if it does.
	while(array_1d_has_value(t1->entries,j)){

				//Set value
				//break
			j++;
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

#include <stdio.h>
#include <stdlib.h>

#include "array_1d.h"

/*
 * Implementation of a generic 1D array for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *	    Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *	    Lars Karlsson (larsk@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se).
 *
 * Version information:
 *   2018-01-28: v1.0, first public version.
 */

// ===========INTERNAL DATA TYPES============

struct array_1d {
	int low; // Low index limit.
	int high; // High index limit.
	int array_size; // Number of array elements.
	void **values; // Pointer to where the actual values are stored.
	free_function free_func;
};

// ===========INTERNAL FUNCTION IMPLEMENTATIONS============

/**
 * array_1d_create() - Create an array without values.
 * @free_func: A pointer to a function (or NULL) to be called to
 *	       de-allocate memory on remove/kill.
 * @lo: low index limit.
 * @hi: high index limit.
 *
 * The index limits are inclusive, i.e. all indices i such that low <=
 * i <= high are defined.
 *
 * Returns: A pointer to the new array, or NULL if not enough memory
 * was available.
 */
array_1d *array_1d_create(free_function free_func, int lo, int hi)
{
	// Allocate array structure.
	array_1d *a=calloc(1, sizeof(*a));
	// Store index limit.
	a->low=lo;
	a->high=hi;

	// Number of elements.
	a->array_size = hi-lo+1;

	// Store free function.
	a->free_func=free_func;

	a->values=calloc(a->array_size, sizeof(void *));

	// Check whether the allocation succeeded.
	if (a->values == NULL) {
		free(a);
		a=NULL;
	}
	return a;
}

/**
 * array_1d_low() - Return the low index limit for the array.
 * @a: array to inspect.
 *
 * Returns: The low index limit.
 */
int array_1d_low(const array_1d *a)
{
	return a->low;
}

/**
 * array_1d_high() - Return the high index limit for the array.
 * @a: array to inspect.
 *
 * Returns: The high index limit.
 */
int array_1d_high(const array_1d *a)
{
	return a->high;
}

/**
 * array_1d_inspect_value() - Inspect a value at a given array position.
 * @a: array to inspect.
 * @i: index of position to inspect.
 *
 * Returns: The element value at the specified position. The result is
 *	    undefined if no value are stored at that position.
 */
void *array_1d_inspect_value(const array_1d *a, int i)
{
	int offset=i-array_1d_low(a);
	// Return the value.
	return a->values[offset];
}

/**
 * array_1d_has_value() - Check if a value is set at a given array position.
 * @a: array to inspect.
 * @i: index of position to inspect.
 *
 * Returns: True if a value is set at the specified position, otherwise false.
 */
bool array_1d_has_value(const array_1d *a, int i)
{
	int offset=i-array_1d_low(a);
	// Return true if the value is not NULL.
	return a->values[offset] != NULL;
}

/**
 * array_1d_set_value() - Set a value at a given array position.
 * @a: array to modify.
 * @v: value to set element to, or NULL to clear value.
 * @i: index of position to modify.
 *
 * If the old element value is non-NULL, calls free_func if it was
 * specified at array creation.
 *
 * Returns: Nothing.
 */
void array_1d_set_value(array_1d *a, void *v, int i)
{
	int offset=i-array_1d_low(a);
	// Call free_func if specified and old element value was non-NULL.
	if (a->free_func != NULL && a->values[offset] != NULL) {
		a->free_func( a->values[offset] );
	}
	// Set value.
	a->values[offset]=v;
}

/**
 * array_1d_kill() - Return memory allocated by array.
 * @a: array to kill.
 *
 * Iterates over all elements. If free_func was specified at array
 * creation, calls it for every non-NULL element value.
 *
 * Returns: Nothing.
 */
void array_1d_kill(array_1d *a)
{
	if (a->free_func) {
		// Return user-allocated memory for each non-NULL element.
		for (int i=0; i<a->array_size; i++) {
			if (a->values[i] != NULL) {
				a->free_func(a->values[i]);
			}
		}
	}
	// Free actual storage.
	free(a->values);
	// Free array structure.
	free(a);
}

/**
 * array_1d_print() - Iterate over the array element and print their values.
 * @a: Array to inspect.
 * @print_func: Function called for each non-NULL element.
 *
 * Iterates over each position in the array. Calls print_func for each
 * non-NULL value.
 *
 * Returns: Nothing.
 */
void array_1d_print(const array_1d *a, inspect_callback print_func)
{
	printf("[ ");
	for (int i=array_1d_low(a); i<=array_1d_high(a); i++) {
		if (array_1d_has_value(a,i)) {
			printf("[");
			print_func(array_1d_inspect_value(a,i));
			printf("]");
		} else {
			printf(" []");
		}
		if (i<array_1d_high(a)) {
			printf(", ");
		}
	}
	printf(" ]\n");
}

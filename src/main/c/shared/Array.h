#ifndef ARRAY_H
#define ARRAY_H

// AbstractSyntaxTree.h has to be included because clang is stupid when dealing with forward declarations...
#include "../../c/frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "ArrayElement.h"
#include <stddef.h>
#include <stdint.h>

#pragma clang diagnostic ignored "-Wtypedef-redefinition"
typedef struct ArrayCDT* Array;

typedef void (*FreeEleFn)(ArrayElement ele);
/**
 * @typedef `ToStringEleFn`
 *
 * @param `ele` The element to be converted to a string.
 * @return A heap-allocated `char*` representing the element.
 *         The caller is responsible for freeing the allocated memory.
 */
typedef char* (*ToStringEleFn)(ArrayElement ele);

/**
 * @param `dest` Destination array, all elements from `src` will be pushed to `dest`.
 * @param `src` Source array. Will be freed after call to prevent double free errors on the elements.
 */
void Array_concat(Array dest, Array src);
void Array_free(Array array);
void Array_freeLogger();
ArrayElement Array_get(Array array, int64_t idx);
size_t Array_getLen(Array array);
void Array_initializeLogger();
Array Array_new(size_t initialCapacity, FreeEleFn freeEleFn, ToStringEleFn toStringEleFn);
void Array_pop(Array array);
void Array_printInfo(Array array);
void Array_push(Array array, ArrayElement ele);
/**
 * @param `array` The array to be converted to string.
 * @return Heap-allocated `char*` representing `array`.
 */
char* Array_toString(Array array);

#endif

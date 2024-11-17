#ifndef SET_H
#define SET_H

// AbstractSyntaxTree.h has to be included because clang is stupid when dealing with forward declarations...
#include "../../c/frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "SetElement.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#pragma clang diagnostic ignored "-Wtypedef-redefinition"
typedef struct SetCDT* Set;

typedef struct SetIteratorCDT* SetIterator;

typedef SetElement (*Set_CloneEleFn)(SetElement ele);
typedef bool (*Set_EqualsEleFn)(SetElement ele1, SetElement ele2);
typedef void (*Set_FreeEleFn)(SetElement ele);
typedef uint32_t (*Set_HashEleFn)(SetElement ele);
/**
 * @typedef `ToStringEleFn`
 *
 * @param `ele` The element to be converted to a string.
 * @return A heap-allocated `char*` representing the element.
 *         The caller is responsible for freeing the allocated memory.
 */
typedef char* (*Set_ToStringEleFn)(SetElement ele);

/**
 * Add `ele` to `set` if not already present. If already present and `freeEleFn`
 * was set on initialization, then `ele` will be freed by this function.
 *
 * @return `true` if element was inserted, `false` if it was already present.
 */
bool Set_add(Set set, SetElement ele);
Set Set_clone(Set set);
/**
 * @return pointer to element if found, `NULL` otherwise.
 */
SetElement* Set_find(Set set, SetElement ele);
void Set_free(Set set);
void Set_freeLogger();
void Set_freeNotElements(Set set);
bool Set_has(Set set, SetElement ele);
void Set_initializeLogger();
/**
 * Removes and frees all elements from `left` that aren't on present in `right`.
 * `right` remains unchanged.
 */
void Set_intersection(Set left, Set right);
bool Set_isEmpty(Set set);
Set Set_new(
  Set_HashEleFn hashEleFn, Set_EqualsEleFn equalsEleFn, Set_CloneEleFn cloneEleFn, Set_FreeEleFn freeEleFn,
  Set_ToStringEleFn toStringEleFn
);
void Set_printInfo(Set set);
/**
 * If `ele` is present in `set`, then remove `ele` from `set` and free node containing `ele`.
 *
 * @return `true` if element was removed, `false` if it was not present to begin with.
 */
bool Set_remove(Set set, SetElement ele);
/**
 * Any elements present in `subtrahend` will be removed and freed from `minuend`.
 */
void Set_subtraction(Set minuend, Set subtrahend);
char* Set_toString(Set set);
/**
 * @param `dest` Destination set. All elements from `src` will be pushed to `dest`.
 * @param `src` Source set. Will be freed after call to prevent double free errors on the elements.
 */
void Set_union(Set dest, Set src);

void SetIterator_free(SetIterator iter);
bool SetIterator_hasNext(SetIterator iter);
SetIterator SetIterator_new(Set set);
SetElement* SetIterator_next(SetIterator iter);

#endif

//This header file is modified from the Spring 2008 version of this assignment.
//Lots of modifications because the assignment was for a cmap, not hashmap.

#ifndef _hashmap_
#define _hashmap_
#include <stdio.h>
/* File: cmap.h
 * ------------------
 * Defines the interface for the cmap.
 */
/**
 * Type: HashMapFreeFunction
 * -------------------------
 * Class of functions designed to dispose of and/or clean up
 * any resources embedded within the element at the specified
 * address. Typically, such resources are dynamically allocated
 * memory, open file pointers, and data structures requiring that
 * some specific cleanup routine be called.
 */
typedef void (*HashMapFreeFunction)(void *elemAddr);


/**
 * Type: map
 * -------------
 * The concrete representation of the map.
 * In spite of all of the fields being publicly accessible, the
 * client is absolutely required to initialize, dispose of, and
 * otherwise interact with all map instances via the suite
 * of the six map-related functions described below.
 */
typedef struct {
// implementation-specific…for you to decide
	void **buckets; //use void** to define linked list blobs
	size_t nbuckets;
	size_t valsz;
	HashMapFreeFunction clean;
} map;


/**
 * Function: cmap_new
 * ---------------------
 * Initializes the identified cmap to be empty. It is assumed that
 * the specified cmap is either raw memory or one previously initialized
 * but later destroyed (using HastSetDispose.)
 *
 * The elemSize parameter specifies the number of bytes that a single element of the
 * table should take up. For example, if you want to store elements of type
 * Binky, you would pass sizeof(Binky) as this parameter. An assert is
 * raised if this size is less than or equal to 0.
 *
 * The numBuckets parameter specifies the number of buckets that the elements
 * will be partitioned into. Once a cmap is created, this number does
 * not change. The numBuckets parameter must be in sync with the behavior of
 * the hashfn, which must return a hash code between 0 and numBuckets - 1.
 * An assert is raised if numBuckets is less than or equal to 0.
 *
 * The freefn is the function that will be called on an element that is
 * about to be overwritten (by a new entry in cmap_put) or on each element
 * in the table when the entire table is being freed (using cmap_dispose). This
 * function is your chance to do any deallocation/cleanup required,
 * (such as freeing any pointers contained in the element). The client can pass
 * NULL for the freefn if the elements don't require any handling.
 *
 *
 * An assert is raised unless all of the following conditions are met:
 * - elemSize is greater than 0.
 * - numBuckets is greater than 0.
 * - hashfn is non-NULL
 */
void cmap_new(map *h, int elemSize, int numBuckets,
	HashMapFreeFunction freefn);
/**
 * Function: cmap_dispose
 * ------------------------
 * Disposes of any resources acquired during the lifetime of the
 * cmap. It does not dispose of client elements properly unless the
 * HashMapFreeFunction specified at construction time does the right
 * thing. cmap_dispose will apply this cleanup routine to all
 * of the client elements stored within.
 *
 * Once cmap_dispose has been called, the cmap is rendered
 * useless. The diposed of cmap should not be passed to any
 * other cmap routines (unless for some reason you want to reuse
 * the same cmap variable and re-initialize is by passing it to
 * cmap_new... not recommended.)
 */
void cmap_dispose(map *h);

/**
 * Function: cmap_put
 * ----------------------
 * Inserts the specified element into the specified
 * cmap. If the specified element matches an
 * element previously inserted, then the
 * old value is replaced by this new element.
 *
 * An assert is raised if the specified address is NULL, or
 * if the embedded hash function somehow computes a hash code
 * for the element that is out of the [0, numBuckets) range.
 */
void cmap_put(map *h, const char *key, const void *valAddr);
/**
 * Function: cmap_get
 * -----------------------
 * Examines the specified cmap to see if anything matches
 * the item residing at the specified elemAddr. If a match
 * is found, then the address of the stored item is returned.
 * If no match is found, then NULL is returned as a sentinel.
 *
 * An assert is raised if the specified address is NULL, or
 * if the embedded hash function somehow computes a hash code
 * for the element that is out of the [0, numBuckets) range.
 */
void *cmap_get(const map *h, const char *key);
/**
 * Function: cmap_remove
 * --------------------
 * removes the key:value pair associated with the given key
 */
void cmap_remove(map *h, const char *key);

/**
 * Function: cmap_first
 * --------------------
 * Returns an arbitrary key from the hashmap
 * which is intended as the first element in iteration
 * of the hashmap
 */
char *cmap_first(map *h);

/**
 * Function: cmap_next
 * --------------------
 * Given the previous key, returns the next key in the hashmap
 * (in the context of iteration). Returns NULL if the previous key
 * was the last one.
 */
char *cmap_next(map *h, const char *prev);
#endif
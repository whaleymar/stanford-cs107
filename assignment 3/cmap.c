#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmap.h"

//string hashing algorithm from http://www.cse.yorku.ca/~oz/hash.html
static unsigned long hash(const char *str, const unsigned int mod) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % mod;
}

// static void *_cmap_get(const map *h, int ix)

static void **get_bucket(const map *h, int ix) {
	//helper for getting address of buckets[ix]
	return (void **)((char *)h->buckets + ix*sizeof(char *));
}

static char *keyptr(void **blob) {
	//gives starting address of key within blob
	return (char *)blob+sizeof(char*);
}

static void *valptr(void **blob) {
	//gives starting address of value within blob

	//find key
	char *ptr = keyptr(blob);
	//find string terminator
	ptr = strchr(ptr, '\0');
	//make sure it was found
	assert(ptr!=NULL);

	return ptr+1;
}

static void set_val(void **blob, const void *valAddr, size_t valsz) {
	void *vptr = valptr(blob);
	memcpy(vptr, valAddr, valsz);
}

static void **new_blob(const char *key, size_t valsz) {
	//allocate memory to store:
	//	1) pointer to next element (new blob is always last element so this is set to NULL)
	//	2) char buffer for key string
	//	3) space for value (will be set by another helper fxn)
	// printf("new blob\n");

	void **blob = malloc(sizeof(char *) + (strlen(key)+1) + valsz);

	//set first 8 bytes to null pointer
	*blob = NULL;

	//after pointer, add key string
	strcpy(keyptr(blob), key);

	return blob;
}

static void **next_blob(void **ptr) {
	return *ptr;
}

void cmap_new(map *h, int elemSize, int numBuckets,
	HashMapFreeFunction freefn) {

 	assert(elemSize>0 && numBuckets>0);

	h->buckets = malloc(sizeof(char *) * numBuckets);
	h->nbuckets = numBuckets;
	h->valsz = elemSize;
	h->clean = freefn;

	// init all bucket pointers to point to NULL (pretty sure this isn't guaranteed)
	for (int i=0; i<numBuckets; i++) {
		*get_bucket(h, i) = NULL; 
	}
}

//TODO: call h->clean on values
void cmap_dispose(map *h) {
	void **ptr;
	void **tmp;
	for (int i=0; i<h->nbuckets; i++) {
		ptr = *get_bucket(h, i);
		do {
			if (ptr==NULL) break;
			tmp = next_blob(ptr); 
			free(ptr);
			ptr = tmp;
		} while (1);
	}
	free(h->buckets);
}

void *cmap_get(const map *h, const char *key) {
	int ix = hash(key, h->nbuckets);
	void **ptr = *get_bucket(h, ix);
	
	if (ptr==NULL) return NULL;
	//go through linkedlist looking for key, return value
	else {
		// go through linkedlist looking for key,
		do
		 {
		 	if (strcmp(key, keyptr(ptr))==0) {
		 		return valptr(ptr);
		 	}
		 	ptr = next_blob(ptr);
		 	if (ptr==NULL) break;
		 } while (1);
	}
	return NULL;
}

void cmap_put(map *h, const char *key, const void *valAddr) {
	int ix = hash(key, h->nbuckets);
	void **ptr = get_bucket(h, ix);
	void **ptrog = ptr;

	if (*ptr==NULL) {
		//bucket is empty, create new blob and point bucket to it
		*ptr = new_blob(key, h->valsz);
		ptr = next_blob(ptr);
	}
	else {
		// go through linkedlist looking for key,
		char found=0;
		ptr = next_blob(ptr);
		do
		 {
		 	if (strcmp(key, keyptr(ptr))==0) {
		 		found=1;
		 		break;
		 	}
		 	if (*ptr==NULL) break;
		 	ptr = next_blob(ptr);
		 } while (1);
		 if (!found) {
		 	*ptr = new_blob(key, h->valsz);
		 	ptr = next_blob(ptr);
		 }
	}

	set_val(ptr, valAddr, h->valsz);
}

//TODO; cmap_remove, cmap_first/cmap_next (for client iteration)
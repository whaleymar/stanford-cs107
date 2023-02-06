#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmap.h"

//helper functions:
static unsigned long hash(const char *str, const unsigned int mod);
static void **get_bucket(const map *h, int ix);
static char *keyptr(void **blob);
static void *valptr(void **blob);
static void set_val(void **blob, const void *valAddr, size_t valsz);
static void **new_blob(const char *key, size_t valsz);
static void **next_blob(void **ptr);

//client functions:
void cmap_new(map *h, int elemSize, int numBuckets, HashMapFreeFunction freefn) {

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
	
	if (ptr!=NULL) {
		// go through linkedlist looking for key, return value
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
	char found=0;

	if (*ptr!=NULL) {
		// go through linkedlist looking for key,
		ptr = next_blob(ptr);
		do {
		 	if (strcmp(key, keyptr(ptr))==0) {
		 		found=1;
		 		break;
		 	}
		 	if (*ptr==NULL) break;
		 	ptr = next_blob(ptr);
		} while (1);
	}

	if (!found) {
		*ptr = new_blob(key, h->valsz);
		ptr = next_blob(ptr);
	}

	set_val(ptr, valAddr, h->valsz);
}

//TODO; cmap_remove, cmap_first/cmap_next (for client iteration)
void cmap_remove(map *h, const char *key) {


}

////////////////////////////////
//helper function definitions://
////////////////////////////////

//string hashing algorithm from http://www.cse.yorku.ca/~oz/hash.html
static unsigned long hash(const char *str, const unsigned int mod) {
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c;
    return hash % mod;
}

//helper for getting address of buckets[ix]
static void **get_bucket(const map *h, int ix) {
	return (void **)((char *)h->buckets + ix*sizeof(char *));
}

//gives starting address of key within block of memory
static char *keyptr(void **blob) {
	return (char *)blob+sizeof(char*);
}

//gives starting address of value within blob
static void *valptr(void **blob) {
	char *ptr = keyptr(blob); //find key
	ptr = strchr(ptr, '\0'); //find string terminator
	assert(ptr!=NULL); //make sure it was found

	return ptr+1;
}

// sets value part of key:value pair within mem block
static void set_val(void **blob, const void *valAddr, size_t valsz) {
	void *vptr = valptr(blob);
	memcpy(vptr, valAddr, valsz);
}

//allocate a block (or "blob" :^) ) of memory to store:
	//	1) pointer to next element (this is set to NULL because a new blob will always be last in the linkedlist)
	//	2) char buffer for key string
	//	3) space for value (will be set by another helper fxn)
static void **new_blob(const char *key, size_t valsz) {
	void **blob = malloc(sizeof(char *) + (strlen(key)+1) + valsz);
	*blob = NULL; //set first 8 bytes to null pointer
	strcpy(keyptr(blob), key);//after pointer, add key string
	return blob;
}

//given a blob, gets next one in linked list
static void **next_blob(void **ptr) {
	return *ptr;
}
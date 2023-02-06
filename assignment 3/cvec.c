#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cvec.h"

static void SizeUp(vector *v){
	//From the assignment description:
	//"Whenever the current capacity is filled, the CVector doubles the capacity to allow for future growth."

	void* elemsNew = realloc(v->elems, v->nalloc*2*v->elemsz);
	assert(elemsNew!=NULL);

	v->elems = elemsNew;
	v->nalloc*=2;
}

//adding a private version without assertions (public version below)
//so I can use in insert and append without errors
static void *_VectorNth(const vector *v, int position) {
	return (char*)(v->elems) + position * v->elemsz;
}

static void addElem(vector *v, int position, const void *elemAddr) {
	//helper fxn for adding element to vector

	memcpy(_VectorNth(v, position), elemAddr, v->elemsz);

	v->nelems++;
	if (v->nelems == v->nalloc) SizeUp(v); // allocate more space if we hit our limit
}

void VectorCreate(vector *v, int elemSize,
	VectorFreeFunction freefn, int initialAllocation) {

	assert(elemSize>0 && initialAllocation>=0);

	//allocate underlying array
	v->elems = malloc(elemSize * initialAllocation);

	//store vector attributes
	v->nelems=0;
	v->nalloc=initialAllocation;
	v->elemsz = elemSize;
	v->clean = freefn;
}

void VectorDispose(vector *v) {
	//keeping this one simple
	//we run the given VectorFreeFunction from the vector constructor on all elements
	//which should free any dynamically allocated memory within v->elems
	//then we just free v->elems
	if (v-> != NULL) {
		for (int i=0; i<VectorLength(v); i++) {
		v->clean(VectorNth(v,i));
		}
	}
	free(v->elems);
}

int VectorLength(const vector *v) {
	return v->nelems;
}

void *VectorNth(const vector *v, int position) {
	assert(position>=0 && position<VectorLength(v));

	return (char*)(v->elems) + position * v->elemsz; // I still think casting to char* for pointer arithmetic is dumb but what do I know
}

void VectorInsert(vector *v, const void *elemAddr, int position) {
	//Inserts an element at specified position, moving everything after position to the right
	//O(n) time complexity). Specifically, O(v->nelems - position)

	assert(position>=0 && position<VectorLength(v));

	//offset everything right of position
	memmove(VectorNth(v,position+1), VectorNth(v,position), v->elemsz * (VectorLength(v)-position));

	//vector[position] can now be filled
	addElem(v, position, elemAddr);
}

void VectorAppend(vector *v, const void *elemAddr) {

	addElem(v, VectorLength(v), elemAddr);
}

void VectorReplace(vector *v, const void *elemAddr, int position) {

	assert(position>=0 && position<VectorLength(v));

	void* posAddr = VectorNth(v,position);

	//free element at position
	v->clean(posAddr);

	//write new element to position
	memcpy(posAddr, elemAddr, v->elemsz);
}

void VectorDelete(vector *v, int position) {

	assert(position>=0 && position<VectorLength(v));

	void* posAddr = VectorNth(v,position);
	v->clean(posAddr);

	//want to write (len-pos-1)*elemsz bytes
	//easiest case to think of is if we delete the first element
	//say len=5, then we want to shift 4 elements, len-pos-1 = 5-0-1 = 4
	memmove(posAddr, VectorNth(v,position+1), v->elemsz*(VectorLength(v)-position-1));

	v->nelems--;
}

int VectorSearch(const vector *v, const void *key,
	VectorCompareFunction searchfn, int startIndex,
	bool isSorted) {

	assert(startIndex>=0 && startIndex <= VectorLength(v)); // using <= required by assignment
	assert(searchfn!=NULL);
	assert(key!=NULL);

	if (!isSorted) {
		for (int i=startIndex; i<VectorLength(v); i++) {
			if (searchfn(VectorNth(v, i), key)==0) return i;
		}
	}
	else {
		void* ptr = bsearch(key, VectorNth(v, startIndex), VectorLength(v)-startIndex, v->elemsz, searchfn);
		if (ptr!=NULL) {
			return (int)(ptr - v->elems)/v->elemsz;
		}
	}

	return -1;
}

void VectorSort(vector *v, VectorCompareFunction comparefn) {
	
	assert(comparefn!=NULL);

	qsort(v->elems, VectorLength(v), v->elemsz, comparefn);
}

void VectorMap(vector *v, VectorMapFunction mapfn, void *auxData) {

	assert(mapfn!=NULL);
	void *elemAddr;

	for (int i=0; i<VectorLength(v); i++) {
		elemAddr = VectorNth(v, i);
		mapfn(elemAddr, auxData);
	}
}
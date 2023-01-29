//not sure what all the expected data types are for these functions
//I'll assume int, char, and char * (not sure last one is possible) to keep things simple

//won't do has_duplicates because it sounds super easy

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int cmpint(const void *elem1, const void *elem2) {
	int a,b;
	a = *(int *)elem1; b = *(int *)elem2;
	if (a>b) return 1;
	if (a<b) return -1;
	return 0;
}

void setint(void *loc, const void *newvalue) {
	int *arrloc = (int *)loc;
	*arrloc = *(int *)newvalue;
	return;
}

int cmpchar(const void *elem1, const void *elem2) {
	char a,b;
	a = *(char *)elem1; b = *(char *)elem2;
	return b-a;
}

void setchar(void *loc, const void *newvalue) {
	*(char *)loc = *(char *)newvalue;
	return;
}

int cmpstr(const void *elem1, const void *elem2) {
	char *a,*b;
	a = (char *)elem1; b = (char *)elem2;
	return strcmp(a,b);
}

void setstr(void *loc, const void *newvalue) {
	strcpy((char *)loc, (char *)newvalue);
	return;
}

// substitute:
// takes an array, and replaces all occurences of oldvalue with newvalue
void substitute(void *arr, void *oldvalue, void *newvalue, size_t arrsz, size_t elemsz,
	int (*cmpfn)(const void *, const void *), void (*setfn)(void *, const void *)) {
	for (int i=0; i<arrsz; i++) {
		void *elem = (char *)arr + elemsz*i;
		// printf("%d\n", cmpfn(elem, oldvalue));
		if (cmpfn(elem, oldvalue)==0) setfn(elem, newvalue);
	}
}

int main() {
	int i;


	int arrint[] = {0,1,2,3,4,5,6,7,8,9};
	int size1 = sizeof(arrint)/sizeof(arrint[0]);
	printf("arrint has %d elements\n", size1);
	int oldvalue1 = 9;
	int newvalue1 = 9000;
	substitute(arrint, &oldvalue1, &newvalue1, size1, sizeof(arrint[0]), cmpint, setint);
	for (i=0; i<size1; i++) printf("arrint[%d]: %d\n", i, arrint[i]);


	char arrchar[] = {'a','b','d','e','z','a','a','$'};
	int size2 = sizeof(arrchar)/sizeof(arrchar[0]);
	printf("arrchar has %d elements\n", size2);
	char oldvalue2 = 'a';
	char newvalue2 = '&';
	substitute(arrchar, &oldvalue2, &newvalue2, size2, sizeof(arrchar[0]), cmpchar, setchar);
	for (i=0; i<size2;i++) printf("arrchar[%d]: %c\n", i, arrchar[i]);

	// this doesn't work
	// I don't think I can use the same substitute function because one that operates on strings
	// would need to take void ** as a pointer to the array
	char *arrstring[] = {"element", "D.","Luffy", ":-)"}; 
	int size3 = sizeof(arrstring)/sizeof(arrstring[0]);
	printf("arrstring has %d elements\n", size3);
	char *oldvalue3 = "element";
	char *newvalue3 = "Monkey";
	substitute(arrstring, oldvalue3, newvalue3, size3, sizeof(arrstring[0]), cmpstr, setstr);
	for (i=0; i<size3;i++) printf("arrstring[%d]: %s\n", i, arrstring[i]);
	// printf("arrstring's elements are size %d\n", (int)sizeof(arrstring[0]));



	// for (int i=0; i<4; i++) {
	// 	printf("%s\n", arrstring[i]);
	// }

	return 0;
}
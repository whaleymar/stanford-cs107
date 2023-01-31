#include <ctype.h> // isalpha
#include <stdio.h> //printf, file io
#include <stdlib.h> //malloc, free
#include <string.h> 
// #include "./c-vector/vec.h"
// #include "./uthash/src/uthash.h"

#define maxlen 50
#define maxwords 5000000

int main() {
	// wikitext dataset: C:\Users\cwhal\coding\random_notebooks\datasets\wikitext.txt

	FILE* f;
	char buf[maxlen];
	char c;
	int i,start,numwords;

	f = fopen("/mnt/c/Users/cwhal/coding/random_notebooks/datasets/wikitext.txt", "r");
	// f = fopen("./test.txt", "r");
	if (f == NULL) {
		printf("Couldn't open file\n");
		return 0;
	}

	i=0; start=0; numwords=0;
	char** words = malloc(sizeof(char*)*maxwords);
	do {
		// using fgetc:
		c = fgetc(f);
		if (c==EOF) {
			//add final word
			if (i-start>0) {
				words[numwords] = malloc(sizeof(char)*(i-start)+1); // create string to store new word
				strncpy(words[numwords], buf, (i-start)); // copy string from buffer to heap
				words[numwords][i-start] = '\0';
				numwords++;
			} 
			break;
		}

		if (!isalpha(c) && (i-start)>0){
			// add word to array
			words[numwords] = malloc(sizeof(char)*(i-start)+1); // create string to store new word
			strncpy(words[numwords], buf, (i-start)); // copy string from buffer to heap
			words[numwords][i-start] = '\0';
			numwords++;
			start = i+1; // move start
		}
		else if (isalpha(c)) {
			if (i-start > maxlen) start = i+1; //reset string, will ignore words > 50 chars 
			else buf[i-start]=c; // extend word
		}
		else start=i+1; //i-start==1; move start to the right so word doesn't start with whitespace

		if (numwords==maxwords) break;
		i++;
	} while (1);

	// print each word, then free memory from heap:
	for (i=0; i<numwords; i++) {
		// printf("%s\n", words[i]);
		free(words[i]);
	}

	free(words);

	// strncpy(buf, f, 15);

	fclose(f);
	return 0;
}
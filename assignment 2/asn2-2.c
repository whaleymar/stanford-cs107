#include <ctype.h> // isalpha
#include <stdio.h> //printf, file io
#include <stdlib.h> //malloc, free
#include <string.h> 
// #include "./c-vector/vec.h"
// #include "./uthash/src/uthash.h"

struct corpus {
	char** words;
	int len;
};

#define maxlen 50
#define maxwords 5000000
typedef struct corpus corpus;

corpus* read_corpus() {
	FILE* f;
	char buf[maxlen];
	char c;
	int i,start;
	corpus* cor = malloc(sizeof(corpus));

	f = fopen("/mnt/c/Users/cwhal/coding/random_notebooks/datasets/wikitext.txt", "r");
	if (f == NULL) {
		printf("Couldn't open file\n");
		return cor;
	}

	i=0; start=0;
	cor->len = 0;
	cor->words = malloc(sizeof(char*)*maxwords);
	do {
		// using fgetc:
		c = fgetc(f);
		if (c==EOF) {
			//add final word
			if (i-start>0) {
				cor->words[cor->len] = malloc(sizeof(char)*(i-start)+1); // create string to store new word
				strncpy(cor->words[cor->len], buf, (i-start)); // copy string from buffer to heap
				cor->words[cor->len][i-start] = '\0';
				cor->len++;
			} 
			break;
		}

		if (!isalpha(c) && (i-start)>0){
			// add word to array
			cor->words[cor->len] = malloc(sizeof(char)*(i-start)+1); // create string to store new word
			strncpy(cor->words[cor->len], buf, (i-start)); // copy string from buffer to heap
			cor->words[cor->len][i-start] = '\0';
			cor->len++;
			start = i+1; // move start
		}
		else if (isalpha(c)) {
			if (i-start > maxlen) start = i+1; //reset string, will ignore words > 50 chars 
			else buf[i-start]=c; // extend word
		}
		else start=i+1; //i-start==1; move start to the right so word doesn't start with whitespace

		if (cor->len==maxwords) break;
		i++;
	} while (1);

	fclose(f);
	return cor;
}

int main() {
	int i;
	corpus* cor = read_corpus();


	// free memory from heap:
	for (i=0; i<cor->len; i++) {
		// printf("%s\n", words[i]);
		free(cor->words[i]);
	}

	free(cor);
	return 0;
}
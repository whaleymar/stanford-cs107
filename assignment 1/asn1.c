#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//2 implementations
///1) stdin
///2) fragments from a text file
////fragments are within {curly braces}, arbitrary whitespace can be between them
////fragment file has <=5000 fragments, each fragment is <=1000 characters

// ^ not doing this because a lot of it was given as part of starter code (which I don't have)
// Things I could've improved to get more points if I was actually taking this class:
/// written a 4th function which executes shared code btwn align_overlap and merge_overlap, then called it from those respective fxns


void optim_overlap(char **frags, int len);
int align_overlap(char *seq1, char *seq2); //len(seq2) <= len(seq1)
void merge_overlap(char *seq1, char *seq2, char **result);

int main(int argc, char *argv[]) {
	if (argc<2) return 0;
	//loop through sequences, get max sequence length, then place in a heap array
	int i;
	int totalseqs = argc-1;

	char **frags = malloc(totalseqs * sizeof(char *)); // allocate array of totalseqs pointers
	//now we allocate a string of appropriate length for each sequence
	//and populate the array
	for (i=0; i<totalseqs; i++) {
		frags[i] = malloc((strlen(argv[i+1])+1)*sizeof(char)); //+1 to account for string terminator \0
		strcpy(frags[i], argv[i+1]);
	}

	optim_overlap(frags, totalseqs);
	printf("%s\n", frags[0]);

	//free each element and array
	for (i=0; i<totalseqs; i++){
		free(frags[i]);
	}
	free(frags);

	return 0;
}


int align_overlap(char *seq1, char *seq2) {
	//implementation note: strlen(seq1) >= strlen(seq2)
	//returns length of overlap between seq1 and seq2
	int i;

	// first check if seq2 is contained in seq1, if so, overlap is length of seq2
	if (strstr(seq1, seq2) != NULL) return strlen(seq2);

	// loop through characters in seq1, aligning the current character with the start of seq2, see if it's a match
	for (i=1; i<strlen(seq1); i++) {
		if (strncmp(seq1+i, seq2, strlen(seq1)-i)==0) return strlen(seq1)-i;
	}

	// if no overlap was found, try the reverse
	for (i=1; i<strlen(seq2); i++) {
		if (strncmp(seq2+i, seq1, strlen(seq2)-i)==0) return strlen(seq2)-i;
	}

	return 0;
}

void merge_overlap(char *seq1, char *seq2, char **result) {
	//implementation note: strlen(seq1) >= strlen(seq2)	
	//merges two sequences

	int i;

	// first check if seq2 is contained in seq1, if so, return seq1
	if (strstr(seq1, seq2) != NULL) {
		strcpy(*result, seq1);
		return;
	}

	// loop through characters in seq1, aligning the current character with the start of seq2, see if it's a match
	for (i=1; i<strlen(seq1); i++) {
		if (strncmp(seq1+i, seq2, strlen(seq1)-i)==0) {
			strncpy(*result, seq1, i);
			strcat(*result, seq2);
			return;
		}
	}

	// if no overlap was found, try the reverse
	for (i=1; i<strlen(seq2); i++) {
		if (strncmp(seq2+i, seq1, strlen(seq2)-i)==0) {
			strncpy(*result, seq2, i);
			strcat(*result, seq1);
			return;
		}
	}

	// if, for whatever reason, there is no overlap between the two strings, concat the strings in either order
	strcat(seq1, seq2);
	strcpy(*result, seq1);
	return;
}

void optim_overlap(char** frags, int len) {
	int maxlap;
	int overlap;
	int ix1, ix2, temp;
	int loops = len-1;

	// compute align_overlap on all sequence combinations
	for (int k=0; k<loops; k++) {
		//outer loop decreases sequence count by 1 each time
		// loops len-1 times until we are left with 1 string

		maxlap=0;
		for (int i=0; i<len; i++) {
			for (int j=0; j<len; j++) {
				if (i<j) {
					//compute overlap of i and j
					if (strlen(frags[i]) >= strlen(frags[j])) {
						overlap = align_overlap(frags[i], frags[j]);
						//check if this is the new max overlap
						if (overlap>maxlap) {
							maxlap=overlap;
							ix1=i; ix2=j;
						}
					}
					else {
						overlap = align_overlap(frags[j], frags[i]);
						//check if this is the new max overlap
						if (overlap>maxlap) {
							maxlap=overlap;
							ix1=j; ix2=i;
						}
					}
				}
			}
		}

		//merge strings
		char *merged = malloc(sizeof(char)*(strlen(frags[ix1])+1+strlen(frags[ix2])+1));
		merge_overlap(frags[ix1], frags[ix2], &merged);

		//final step: 
		///first, reassign ix1 and ix2 such that ix1<ix2
		///overwrite frags[ix1] with merged
		///overwrite frags[ix2] with frags[len-1] so there are no empty spaces in frags
		if (ix1>ix2){
			temp=ix1;
			ix1=ix2;
			ix2=temp;
		}
		frags[ix1] = realloc(frags[ix1], (strlen(merged)+1)*sizeof(char));
		strcpy(frags[ix1], merged);
		free(merged);

		if (ix2!=len-1){
			frags[ix2] = realloc(frags[ix2], (strlen(frags[len-1])+1)*sizeof(char));
			strcpy(frags[ix2], frags[len-1]);
		}

		len--; // frags has 1 less element
	}
	return;
}
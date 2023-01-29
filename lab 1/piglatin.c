#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int vowix(char *word){
	int result=-1;
	for (int i=0;i<strlen(word);i++){
		if (word[i]=='a' || word[i]=='e' || word[i]=='o' || word[i]=='i' || word[i]=='u') {
			result = i;
			break;
		}
	}
	return result;
}

void pig(char *word, int i, char *result);

int main(int argc,char *argv[]) {
	int i = vowix(argv[1]);
	char *result = malloc(sizeof(char[50]));
	pig(argv[1], i, result);
	printf("%s\n",result);
	free(result);
	return 0;
}

void pig(char *word, int i, char *result) {
	if (i==0) {
		strcat(word, "way");
		strcpy(result, word); return;
	}
	if (i<1) {
		strcpy(result, word); 
		return;	
	}
	memset(result, '\0', sizeof(result));
	strcpy(result, word+i);
	strncat(result, word, i);
	strcat(result, "ay");
	return;
}

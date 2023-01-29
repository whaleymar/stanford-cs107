#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./c-vector/vec.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//part 1 of assignment 2
//things I am not going to implement: tracking directories to avoid looping 
//due to symlinks (it doesn't add much to what I'd learn from this part)


void searchdir(char *, char *);

int main() {
	char * dirname = ".";
	char *search = "asn";
	searchdir(dirname, search);

	return 0;
}

void recurse(void **visited, const char *dirname, const char *search) {
	struct dirent* dir;
	DIR *dr = opendir(dirname);

	if (dr==NULL) printf("Directory not found");

	while ((dir = readdir(dr)) != NULL) {
		//skip cur and parent dir
		if (strcmp(dir->d_name, ".")!=0 && strcmp(dir->d_name, "..")!=0) {
			char path[1024];

			//create new path for future navigation
			snprintf(path, sizeof(path), "%s/%s", dirname, dir->d_name);

			//recurse if next file is a directory
			if (dir->d_type == DT_DIR) recurse(visited, path, search);
			else {
				//See if search term is in filename
				if (strncmp(search, dir->d_name, strlen(search))==0) {
					//dynamically allocate string to store in vec
					//cannot store dir->d_name, because it gets 
					//deallocated when I call closedir()
					char *newstr = malloc(strlen(dir->d_name)+1);
					strcpy(newstr, dir->d_name);
					vector_add((char ***)visited, newstr);
				}
			}			//todo: separate dir tracker
		}
	}
	closedir(dr);
	
	return;
}


void searchdir(char * dirname, char *search) {
	printf("Starting Location: %s\nSearch Term: %s\n\n", dirname, search);

	void *vec = vector_create();

	recurse(&vec, dirname, search);

	int len = vector_size(vec);
	for (int i=0; i<len; i++) {
		printf("%s\n", *(char **)(vec+sizeof(char **)*i));
	}


	for (int i=0; i<len; i++) {
		free(*(char **)(vec+sizeof(char **)*i));
	}
	vector_free(vec);
	
	return;
}
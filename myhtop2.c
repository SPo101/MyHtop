//Better version of myhtop.c here we use scandir which allows us not to use extra files for work of our application
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/dir.h>
#include <dirent.h>

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"
#define BACK_COLOR_RED "\x1b[41m"
#define BACK_COLOR_MAGENTA "\x1b[45m"
#define BACK_COLOR_CYAN "\x1b[46m"

#define COLUMNS 3 //count of columns which will be displayed
#define TEXT_OFFS 35 //width of each column
/*
struct dirent {
    ino_t          d_ino;       // inode number 
    off_t          d_off;       // offset to the next dirent 
    unsigned short d_reclen;    // length of this record 
    unsigned char  d_type;      // type of file; not supported by all file system types 
    char           d_name[256]; // filename 
};
*/
void printname(char *str, int off){//string with name and offset
	size_t len=0;
	while(*(str+6+len) != '\n'){
		len++;
	}
	if(off<len){
		printf("\n\n" COLOR_RESET BACK_COLOR_RED"Sorry, wrong TEXT_OFFS, make it bigger" COLOR_RESET "\n");
		exit(-1);
	}
	for(size_t i=0; i<(off-len); i++)
		printf(" ");
	for(size_t i=0; i<len; i++)
		printf("%c", *(str+6+i));
	printf(COLOR_RESET);
}

int main(){
	printf("\t\t\t\t\t\t" COLOR_RESET BACK_COLOR_RED "List of running processes" COLOR_RESET "\n\n");
	struct dirent **listproc;//list of pid's
	ssize_t filecnt = scandir("/proc", &listproc, NULL, alphasort);//count of files in dirrectory
	if(filecnt < 0){
		printf("\n\n" COLOR_RESET BACK_COLOR_RED"Sorry, have problems" COLOR_RESET "\n");
		exit(-1);
	}

	size_t spes = 1;//special counter for output
	ssize_t i = -1;//counter to read listproc not reverse(not necessary)
	while(filecnt > ++i){
		size_t lenofproc = strlen(listproc[i]->d_name);
		char *procid = (char*) malloc(lenofproc+13);//path to proc (/proc/<pid>/status) 
		memcpy(procid, "/proc/", 6);
		memcpy(procid+6, listproc[i]->d_name, lenofproc);
		memcpy(procid+6+lenofproc, "/status", 7);

		ssize_t openfiled = open(procid, O_RDWR);
		if(openfiled==-1){
			procid=NULL;
			free(procid);
			continue;
		}

		char *nameproc = (char*) malloc(50);//name of process
		read(openfiled, nameproc, 50);

		if((spes%(COLUMNS*2))<=COLUMNS && (spes%(COLUMNS*2)) != 0){
			printf(COLOR_CYAN);
			printname(nameproc, TEXT_OFFS);
		}
		if((spes%(COLUMNS*2))>COLUMNS || (spes%(COLUMNS*2)) == 0){
			printf(COLOR_GREEN);
			printname(nameproc, TEXT_OFFS);
		}
		if((spes%COLUMNS)==0)
			printf("\n");


		nameproc=NULL;
		procid=NULL;
		free(procid);
		free(nameproc);
		close(openfiled);
		spes++;
	}
	free(listproc);
}

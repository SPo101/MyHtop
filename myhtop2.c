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
/*
struct dirent {
    ino_t          d_ino;       // inode number 
    off_t          d_off;       // offset to the next dirent 
    unsigned short d_reclen;    // length of this record 
    unsigned char  d_type;      // type of file; not supported by all file system types 
    char           d_name[256]; // filename 
};
*/
int main(){
	printf("\t\t\t\t\t\t" BACK_COLOR_RED "List of running processes" COLOR_RESET "\n\n");
	struct dirent **listproc;//list of pid's
	ssize_t filecnt = scandir("/proc", &listproc, NULL, alphasort);//count of files in dirrectory
	if(filecnt < 0){
		printf("Sorry, have problems\n");
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
		size_t namelen=0;//len of process's name
		read(openfiled, nameproc, 50);
		while(*(nameproc+namelen) != '\n')
			namelen++;
		char *NAME = (char*) malloc(namelen-6);
		memcpy(NAME, nameproc+6, namelen-6);

		if((spes%6)<=3 && (spes%6) != 0)
			printf( COLOR_CYAN "%35s\t" COLOR_RESET , NAME );
		if((spes%6)>3 || (spes%6) == 0)
			printf( COLOR_GREEN "%35s\t" COLOR_RESET , NAME );
		if((spes%3)==0)
			printf("\n");



		nameproc=NULL;
		NAME=NULL;
		procid=NULL;
		free(procid);
		free(nameproc);
		free(NAME);
		close(openfiled);
		spes++;
	}


	free(listproc);
}

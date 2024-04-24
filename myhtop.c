#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

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


int main(){
	printf("\t\t\t\t\t\t" BACK_COLOR_RED "List of running processes" COLOR_RESET "\n\n");
	system("ls /proc > dataformyhtop");
	ssize_t fd = open("dataformyhtop", O_RDONLY);
	size_t lenfile = lseek(fd, 0L, SEEK_END);	
	char *listofprocesses =  malloc(sizeof(char)*lenfile);
	lseek(fd, 0L, SEEK_SET);
	read(fd, listofprocesses, lenfile);
	
	
	char pcom1[] = "/proc/";//6//1st piese of path /proc/<pid>/status
	char pcom3[] = "/status";//7//2nd piese
	size_t lenofproc=0;//len of each process name
	int spes = 1;//special counter just for better output


	for(size_t iter=0; iter<lenfile;iter++){
		if(*(listofprocesses+iter) == '\n'){
			char *procid = (char*) malloc(12+lenofproc);//path to process (pid)(/proc/<pid>/status)
			memcpy(procid, &pcom1[0], 6);
			memcpy(procid+6, listofprocesses+iter-lenofproc+1, lenofproc-1);
			memcpy(procid+5+lenofproc, &pcom3[0], 7);
		
			ssize_t openfiled = open(procid, O_RDWR);
			if(openfiled==-1){
				procid=NULL;
				free(procid);
				lenofproc=0;
				continue;
			}

			char *nameproc = (char*) malloc(50);//name of process
			size_t namelen=0;//len of process's name
			read(openfiled, nameproc, 50);
			while(*nameproc != '\n'){
				namelen++;
				nameproc++;
			}
			nameproc-=namelen;
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
			lenofproc=0;
			spes++;
		}
		lenofproc++;	
	}
	listofprocesses=NULL;
	free(listofprocesses);
	close(fd);
}

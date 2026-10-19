#include<copy.h>
int process_wait(void){
	pid_t zpid;

	while((zpid=wait(NULL))>0);
	printf("clear all child\n");
	}

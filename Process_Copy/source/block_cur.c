#include<copy.h>
int block_cur(const char*srcfile,int pronum){
	
	int srcfd;
	srcfd=open(srcfile,O_RDONLY);
	int filesize=lseek(srcfd,0,SEEK_END);
	close(srcfd);
	if((filesize%pronum==0)){
		
		return (filesize/pronum);
		}
	else{
		
		return filesize/pronum+1;
		}
	
	}

#include<copy.h>
int main(int argc,char** argv){
	
	const char* srcfile	= argv[1];
	const char* desfile = argv[2];
	int offset=atoi(argv[3]);
	int blocksize=atoi(argv[4]);
	printf("pid: %d  offset: %d  blocksize; %d\n",getpid(),offset,blocksize);
	int srcfd=open(srcfile,O_RDONLY);
	int desfd=open(desfile,O_WRONLY | O_CREAT,0664);
	int len;
	char buffer[blocksize];
	bzero(buffer,sizeof(buffer));
	lseek(srcfd,offset,SEEK_SET);
	lseek(desfd,offset,SEEK_SET);
	len=read(srcfd,buffer,sizeof(buffer));
	write(desfd,buffer,len);
	close(srcfd);
	close(desfd);
	}

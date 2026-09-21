#include<sys/mman.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<regex.h>
int main(){
	int fd = open("url.txt",O_RDWR);
	int fsize=lseek(fd,0,SEEK_END);
	char * mmap_ptr = NULL;
	mmap_ptr = mmap(NULL,fsize,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);
	close(fd);
	//完成文件映射
	char* mark=mmap_ptr;
	regex_t finder;
	char* regstr="<a *\\(class=\"[^\"]\\+\\?\" *\\)*\\(href=\"[^\"]\\+\\?\" *\\)\\+\\(target=\"[^\"]\\+\\?\" *\\)*\\(title=\"[^\"]\\+\\?\" *\\)*>[^<]\\+</a>";
	regcomp(&finder,regstr,0);
	int regnum=5;
	regmatch_t match[regnum];
	char name[1024];
	while(regexec(&finder,mmap_ptr,regnum,match,0)==0){
			
			bzero(name,sizeof(name));
			snprintf(name,match[0].rm_eo-match[0].rm_so+1,"%s",mmap_ptr+match[0].rm_so);
			printf("%s\n",name);
			mmap_ptr+= match[0].rm_eo;
		}
	
	regfree(&finder);
    munmap(mark,fsize);	
	return 0;
}

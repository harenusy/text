#include<copy.h>
int process_create(const char* srcfile,const char*desfile,int pronum ,int blocksize){
	pid_t pid;
	int i;
	for(i=0;i<pronum;i++){
		
		pid=fork();
		if(pid==0)break;

		}
	if(pid==0){

	int offset=i*blocksize;
	char str_offset[10];
	char str_blocksize[10];
	bzero(str_offset,10);//先刷新
	bzero(str_blocksize,10);
	sprintf(str_offset,"%d",offset);//将内容格式化输出到字符数组内
	sprintf(str_blocksize,"%d",blocksize);
	execl("/home/colin/20251018/Process_Copy/Mod/copy","copy",srcfile,desfile,str_offset,str_blocksize,NULL);

		}
	}

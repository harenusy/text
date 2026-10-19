#include<copy.h>
int main(int argc , char** argv){
	
	//校验参数
	int pramtest = check_pram(argc,argv[1],atoi(argv[3]));//参数数量，源文件名，进程个数
	if(pramtest==-1){
		printf("参数数量错误\n");
		return -1;
		}
	if(pramtest==-2){
		
		printf("源文件不存在\n");
		return -1;
		}
	if(pramtest==-3){
		
		printf("进程数量错误\n");
		return -1;
		}
	
	 const char* srcfile = argv[1];
	 const char* desfile = argv[2];
	 int pronum = atoi(argv[3]);


	int srcfd , destfd;
	srcfd=open(srcfile,O_RDONLY);
	destfd=open(desfile,O_WRONLY|O_CREAT,0664);//打开目标文件，不存在就创建
	int blocksize=block_cur(srcfile,pronum);
	process_create(srcfile,desfile,pronum,blocksize);
	process_wait();
	close(srcfd);
	close(destfd);
	}

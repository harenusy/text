#include<copy.h>
int check_pram(int argc,const char* srcfile,int pronum){
	
	if(argc<3){
		
		return -1;

		}	
	if((access(srcfile,F_OK)!=0)){
		
		return -2;
		
		}
	
	if(pronum>100||pronum<1){
		
		return -3;

		}
	return 1;
	
	}

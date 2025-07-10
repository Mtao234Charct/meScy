#include <windows.h>
#include <string>
#include <iostream>
#define _WINDOWS_

int main(){
	
	
	int ssl_server = system("start meScy");
	
	
	if(ssl_server = 801) {
		std::cerr<<"meScy.exe returned 801 exit status"<<std::endl;
		std::cerr<<"Initiation termination."<<std::endl;
		std::cerr<<"For more information, please run meScy only to check its error message."<<std::endl;
	}else if(ssl_server != 0){
		std::cerr<<"FATAL ERROR: Can't start meScy-ssl-mode.EXIT CODE::"<<ssl_server<<std::endl;
		
	}else{
		std::cout<<"Start meScy-ssl-mode success!"<<std::endl;
		
	}
	
	int http_only  = system("start meScy-http-only");
	
	if(http_only = 801) {
		std::cerr<<"meScy-http-only.exe returned 801 exit status"<<std::endl;
		std::cerr<<"Initiation termination."<<std::endl;
		std::cerr<<"For more information, please run meScy only to check its error message."<<std::endl;
	}else if(http_only != 0){
		std::cerr<<"FATAL ERROR: Can't start meScy-http-only.EXIT CODE::"<<ssl_server<<std::endl;
		
	}else{
		std::cout<<"Start meScy-http-only success!"<<std::endl;
		
	}
	
	return 0;
}

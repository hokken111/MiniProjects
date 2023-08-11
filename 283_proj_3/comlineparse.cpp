#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include "comlineparse.h"
#include "complete_binary_tree.h"
#include "myheap.h"


comlineparse::comlineparse(int argc, char *argv[]){
	if(argc == 1){
		inputlocation = "";
	}else {
		inputlocation = argv[1];
	}

}
istream * comlineparse::returnstream(){

	if(inputlocation.empty()){
		input = &cin;		
	} else{
		filestream.open(inputlocation.c_str());
		if(!filestream.is_open()){
			fprintf(stderr, "Can't open the file - fatal error");
			exit(1);
		}
		input = &filestream;
	}
	return input;
}
comlineparse::~comlineparse(){}

#include <iostream>
#include <istream>
#include <fstream>
#include <ext/hash_map>
#include <utility>
#include <vector>
#include "comlineparse.h"
#include "complete_binary_tree.h"
#include "myheap.h"
#include "commandexec.h"

using namespace std;

int main(int argc, char *argv[]){
	comlineparse commandline(argc, argv);
	istream *stream = commandline.returnstream();
	
	commandexec allcommands;
	bool exit = 0;
	char newcommand[200];
	while(!exit){
		(*stream).getline(newcommand, 200);
		exit = allcommands.executecommand(newcommand);
	
	}
}

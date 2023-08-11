#include <string>
#include <iostream>
#include <ext/hash_map>
#include <utility>
#include <vector>
#include "complete_binary_tree.h"
#include "myheap.h"
#include "commandexec.h"

using namespace std;

commandexec::commandexec(){
	outstandingprints = 0;
}
commandexec::~commandexec(){
}
bool commandexec::executecommand(char incoming[]){
	
	if(incoming[0] == '$'){
		if(!strcmp(incoming, "$ quit")){
			while(outstandingprints>0){
				cout << "No results found.\n";
				outstandingprints--;
			}
			return 1;
		}else if(!strcmp(incoming, "$ print")){
			outstandingprints ++;
			outstandingprints += theheap.print();
		}else if(!strcmp(incoming, "$ clear")){
			while(outstandingprints>0){
				cout << "No results found.\n";
				outstandingprints--;
			}
			theheap.clear();
		}else if(!strcmp(incoming, "$ save")){
			theheap.save();
		}else if(!strcmp(incoming, "$ revert")){
			theheap.revert();
		}else if(!strcmp(incoming, "$ history")){
			theheap.printhistory();
		}else{
			cout << "Error: Invalid command" << strchr(incoming, ' ') << ".\n";
		}
		
	} else {
		int test = atoi(strchr(incoming, ' '));
		point = strtok(incoming, " ");
		if(test <=0){
			cerr << "Fatal error: Invalid relevance for file "
				<< point << ".\n";	
			exit(1);
		}
		mapit = map.find(point);
		if((mapit == map.end())){
			map.insert(make_pair(point, test));
			theheap.addtoheap(point, test);
		} else {
			cerr << "Fatal error: Indexer process found file " 
				<< point << " twice.\n";
			exit(1);
		}
		

	}
	if(outstandingprints >0){
		while(theheap.print() != 0 and outstandingprints >0){
			outstandingprints--;
		}
	}
	return 0;
}

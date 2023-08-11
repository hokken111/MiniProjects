#include <vector>
#include "complete_binary_tree.h"
#include "myheap.h"

using namespace std;

myheap::myheap(){
	numhistoricaldoc = 0;
	lastsaveat = 0;
	historicaldocsize = 10;
	historicaldoc.resize(10);
}

myheap::~myheap(){
	clear();
}

bool myheap::checkNeedHeapify(CBTNODE child, CBTNODE parent){
	datapoint *parentp, *childp;
	if(!bintree.valid(parent) or !bintree.valid(child)){
		return 0;
	}
	parentp = bintree.element(parent);
	childp = bintree.element(child);
	if(childp->second >= parentp->second ){
		if(childp->second == parentp->second){
			if(strcmp((parentp->first).c_str(), (childp->first).c_str()) < 0){
				return 0;
			} 
		}
		bintree.swap(child, parent);
		CBTNODE placeholder = (bintree.element(child))->third;
		childp->third = parentp->third;
		parentp->third = placeholder;
		return 1;
	}
	return 0;
}
void myheap::heapifyDown(){
	if(!bintree.valid(bintree.root())){
		return;
	}
	CBTNODE headnode = bintree.root();
	CBTNODE leftnode, rightnode;
	datapoint *leftp, *rightp;
	while(headnode!=CBTNODE_DNE){
		leftnode = bintree.leftChild(headnode);
		if(bintree.valid(leftnode) and leftnode != CBTNODE_DNE){
			rightnode = bintree.rightChild(headnode);
			if(bintree.valid(rightnode) and rightnode != CBTNODE_DNE){
				leftp = bintree.element(leftnode);
				rightp = bintree.element(rightnode);
				if(leftp->second > rightp->second){
					checkNeedHeapify(leftnode, headnode); 
					headnode = leftnode;
				}else if(leftp->second < rightp->second){
					checkNeedHeapify(rightnode, headnode);
					headnode = rightnode;
				}else if(leftp->second = rightp->second){
				    if(strcmp((leftp->first).c_str(), 
				    		(rightp->first).c_str()) < 0){
						checkNeedHeapify(leftnode, headnode);
						headnode = leftnode;
					}else{
						checkNeedHeapify(rightnode, headnode);
						headnode = rightnode;
					}
				}
			}else{
				checkNeedHeapify(leftnode, headnode);
				return;
			}
		}else{
			return;
		}
	}
}

void myheap::clear(){
	while(!bintree.empty()){	
		datapoint *pointer;
		pointer = bintree.removeLast();
		delete pointer;
	}
	while(!dumpster.empty()){	
		datapoint *pointer;
		pointer = dumpster.back();
		dumpster.pop_back();
		delete pointer;
	}
	numhistoricaldoc = 0;
	return;
}

void myheap::printhistory(){
	if(numhistoricaldoc>lastsaveat){
		string temp;
		for(int i = 0; i<numhistoricaldoc; i++){
			historicaldoc[i]->printint = historicaldoc[i]->added;
		}
		for(int i = 0; i<numhistoricaldoc; i++){
			if(historicaldoc[i]->printint == 2 
				or historicaldoc[i]->printint == 1 ){
				temp = "Added file ";
				historicaldoc[i]->printint--;
			} else if(historicaldoc[i]->printint==0){
				temp = "Removed file ";
			}
			if(i>=lastsaveat){
				cout << temp << historicaldoc[i]->first 
					<< " " << historicaldoc[i]->second << endl;
			}
		}	
	}
	return;
}

int myheap::print(){
	if(!bintree.empty()){
		CBTNODE targetNode = bintree.root(), lastNode = bintree.lastLeaf();
		if(!bintree.valid(targetNode) or !bintree.valid(lastNode)){
			return 0;
		}
		cout << (bintree.element(targetNode))->first << " " 
			<< (bintree.element(targetNode))->second << endl;
		(bintree.element(targetNode))->added = 1;
		bintree.element(targetNode)->third = -1;
		if(numhistoricaldoc+1 == historicaldocsize){
			historicaldocsize = historicaldocsize*2;
			historicaldoc.resize(historicaldocsize);
		}
		historicaldoc[numhistoricaldoc] = bintree.element(targetNode);
		numhistoricaldoc ++;
		dumpster.push_back(bintree.element(targetNode));
		if(targetNode != lastNode){
			bintree.element(lastNode)->third = 0;
			bintree.swap(targetNode, lastNode);
		}
		bintree.removeLast();
		heapifyDown();
		return -1;
	}else{
		return 0;
	}
}

void myheap::addtoheap(string addstring, int relevance){
	datapoint *newpoint = new datapoint;
	newpoint->first = addstring;
	newpoint->second = relevance;
	newpoint->third = bintree.lastLeaf()+1;
	newpoint->added = 2;
	bintree.append(newpoint);
	CBTNODE lastadded, parent;
	lastadded = bintree.lastLeaf();
	if(numhistoricaldoc+1 == historicaldocsize){
			historicaldocsize = historicaldocsize*2;
			historicaldoc.resize(historicaldocsize);
	}
	historicaldoc[numhistoricaldoc] = newpoint;
	numhistoricaldoc ++;
	if(bintree.parent(lastadded) != CBTNODE_DNE){
		do{
			parent = bintree.parent(lastadded);
			checkNeedHeapify(lastadded, parent);
			lastadded = parent;
		}while(parent != bintree.root());
	}
}
void myheap::adddatapointtoheap(datapoint *newpoint){
	newpoint->third = bintree.lastLeaf()+1;
	bintree.append(newpoint);
	CBTNODE lastadded, parent;
	lastadded = bintree.lastLeaf();
	if(numhistoricaldoc+1 == historicaldocsize){
			historicaldocsize = historicaldocsize*2;
			historicaldoc.resize(historicaldocsize);
	}
	if(bintree.parent(lastadded) != CBTNODE_DNE){
		do{
			parent = bintree.parent(lastadded);
			checkNeedHeapify(lastadded, parent);
			lastadded = parent;
		}while(parent != bintree.root());
	}
}


void myheap::save(){
	lastsaveat = numhistoricaldoc;
	return;
}
void myheap::revert(){
	for(int i = 0; i<numhistoricaldoc; i++){
			historicaldoc[i]->printint = historicaldoc[i]->added;
	}
	for(int i = numhistoricaldoc-1; i>=lastsaveat; i--){
		if(historicaldoc[i]->printint == 2){
				CBTNODE target = historicaldoc[i]->third;
				dumpster.push_back(historicaldoc[i]);
				bintree.swap(target, bintree.lastLeaf());
				if(!bintree.empty()){
					bintree.removeLast();
				}
		} else if(historicaldoc[i]->printint==1){
				dumpster.pop_back();
				adddatapointtoheap(historicaldoc[i]);
				historicaldoc[i]->printint ++;
		}
	}
	numhistoricaldoc = lastsaveat;
}

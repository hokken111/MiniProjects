#include <vector>
#include <iostream>
#include <cstdlib>
#include "mybitvec.h"

using namespace std;

mybitvec::mybitvec(int size, string *number){
	int eight = 8, poweroftwo = 128;
	rootnum = *number;
	globalsize = size/eight;
	propertysize = size;
	if(size%eight != 0){
		globalsize ++;		
	}
	myvec.resize(globalsize+1);
	for(int i = 0; i<globalsize; i++){
		unsigned char temp = 0;
		for(int j = 0; j < 8; j++){
			if((j+8*i) >= size){
				break;
			}
			if((*number).at(j+8*i)=='1'){
				(temp) += poweroftwo;
			}
			poweroftwo = poweroftwo/2;
		}
		myvec.at(i) = temp;
		poweroftwo = 128;
	}
}


double mybitvec::returnrelevance(std::string *newstring){
	int poweroftwo = 128, difference = 0;
	double totaldifference = 0;
	for(int i = 0; i<globalsize; i++){
		unsigned char temp;
		temp = 0;
		for(int j = 0; j < 8; j++){
			if((j+8*i) >= propertysize){
				break;
			}
			if((*newstring).at(j+8*i)!=rootnum.at(j+8*i)){
				(temp) += poweroftwo;
			}
			poweroftwo = poweroftwo/2;
		}
		difference = abs((int)(temp));
		if(difference != 0){
			totaldifference += (256*(globalsize-i-1) + abs(difference));
		}
		poweroftwo = 128;
	}
	return totaldifference;
}

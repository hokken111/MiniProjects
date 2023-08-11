#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "mybitvec.h"
#include "mymap.h"
#include "mergeSortContainer.cpp"

using namespace std;

mymap::mymap(double initsize, string name){
	rootname = name;
	mapvec.resize(0);
}

void mymap::addtomap(double relevance, string myname, double xcoord, double ycoord){
	server tempserver;
	tempserver.relevance = relevance;
	tempserver.name = myname;
	tempserver.xcoord = xcoord;	
	tempserver.ycoord = ycoord; 
	mapvec.push_back(tempserver);
}

bool myCompare(server &a, server &b){
	string aname = a.name;
	string bname = b.name;
	if(a.relevance > b.relevance){
		return false;
	}else if(a.relevance < b.relevance){
		return true;
	}else if(a.relevance == b.relevance){
		double adistance, bdistance;
		adistance = sqrt(pow(a.xcoord, 2) + pow(a.ycoord, 2));
		bdistance = sqrt(pow(b.xcoord, 2) + pow(b.ycoord, 2));
		if(adistance < bdistance){
			return false;
		}else if(adistance > bdistance){
			return true;
		}else if(adistance == bdistance){
			if((strcmp((a.name).c_str(), (b.name).c_str())) > 0){
				//a comes first
				return false;
			}else{
				return true;
			}
		}
	}		
}

void mymap::sort(){
	mergeSort(mapvec.begin(), mapvec.end(), myCompare);
}

void mymap::printOut(){
	cout << rootname << endl;
	string tempname;
	for(int i = 0; i<mapvec.size(); i++){
		tempname = mapvec[i].name;
		if(!tempname.empty()){
			cout << (mapvec[i]).name << endl;
		}
		tempname.clear();
	}

}

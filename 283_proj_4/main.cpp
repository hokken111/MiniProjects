#include <iostream>
#include <vector>
#include "mybitvec.h"
#include "mymap.h"

using namespace std;

int main(){
	double locations, bitdigits, xcoord, ycoord, numservs, rootxcoord, rootycoord;
	string binary, name;
	cin >> locations >> bitdigits;
	cin >> xcoord >> ycoord; 
	cin >> numservs;
	cin >> binary >> name;
	rootxcoord = xcoord;
	rootycoord = ycoord;
	mybitvec bvec((int)bitdigits, &binary);
	mymap mapofall(locations, name);
	numservs --;
	for(int i = 0; i<numservs; i++){
		cin >> binary >> name;
		mapofall.addtomap(bvec.returnrelevance(&binary), name, (xcoord-rootxcoord), (ycoord-rootycoord));
	}	
	for(int i = 1; i<locations; i++){
		cin >> xcoord >> ycoord;
		cin >> numservs;
		for(int k = 0; k<numservs; k++){
			cin >> binary >> name;
			mapofall.addtomap(bvec.returnrelevance(&binary), name, (xcoord-rootxcoord), (ycoord-rootycoord));		
		}
	}
	mapofall.sort();	
	mapofall.printOut();
	
	return 0;
}

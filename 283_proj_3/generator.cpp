#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(){
	cout << "Generating!" << endl;
	ofstream os;
	os.open("Test1.txt");
	string a, b, c, d, e;
	a = b = c = d = e = "abcdefghijklmnopqrstuvwxyz";
	for(int i = 0; i < 26; i++){
		for(int j = 0; j < 26; j++){
			for(int k = 0; k < 26; k++){
				for(int n = 0; n<26; n++){
					os << a[i] << b[j] << c[k] << d[n] << ".txt " << i+1 << endl;
				}
			}
			os << "$ history" << endl;
			os << "$ save" << endl;
		}
		os << "$ revert" << endl;
		os << "$ clear" << endl;
	}
	os << "$ quit" << endl;
	os.close();

}

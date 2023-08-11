using namespace std;
#ifndef _COMLINEPARSE_H_
#define _COMLINEPARSE_H_

class comlineparse{
	public:
		//Constructor - pass in command line args - constructor parses
		comlineparse(int, char *[]);
		
		//Destructor
		~comlineparse();
		
		//Return the stream (either cin or filestream) of desired input
		istream * returnstream();

	private:
		istream* input;
		string inputlocation;
		ifstream filestream;
};







#endif

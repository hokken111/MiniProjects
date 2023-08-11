using namespace std;

#ifndef _COMMANDEXEC_H_
#define _COMMANDEXEC_H_

using namespace std;
namespace __gnu_cxx{
  template<> struct hash<string>{
    size_t operator()(const string& x) const{
      return hash<const char*>()(x.c_str());
    }
  };
}

	
class commandexec{
	public:
		//Initializes an empty command executor
		commandexec();
		
		//Destructor
		~commandexec();
		
		//Pass in either file or command and this will execute
		bool executecommand(char []);
		
	private:
		char *point;
		myheap theheap;
		__gnu_cxx::hash_map<string, int> map; 
		__gnu_cxx::hash_map<string, int>::iterator mapit;
		int outstandingprints;


};
#endif

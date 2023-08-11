//
// Add header description for this file, and your group here
//


#ifndef BUFMAP_H
#define BUFMAP_H
#include <vector>
#include "db.h"

using namespace std; 


// Class to keep track of pages in the buffer pool
class BufMap 
{ 
public:   

  // Insert an entry mapping (file,pageNo) to frameNo.
  // Return BUFMAPERROR if the entry already exists, OK otherwise.
  const Status insert(File* file, const int pageNo, const unsigned int frameNo); 

  // Find the frameNo corresponding to (file,pageNo), and return by reference.
  // Return BUFMAPNOTFOUND if the entry is not found, OK otherwise.
  const Status lookup(File* file, const int pageNo, unsigned int & frameNo) const; 

  // Remove the entry corresponding to (file,pageNo).
  // Return BUFMAPNOTFOUND if entry is not found, OK otherwise.
  const Status remove(File* file, const int pageNo); 
  
private: 
	vector<File*> files;
	vector<int> pageNos;
	
	// You may add additional private variables and data
	// structures as you see fit!
  
};

#endif

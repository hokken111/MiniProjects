
#include "bufMap.h"
#include "buf.h"

//---------------------------------------------------------------
// insert entry mapping (file,pageNo) to frameNo;
// returns OK if OK, BUFMAPERROR if an error occurred
//---------------------------------------------------------------

const Status BufMap::insert(File* file, 
				const int pageNo, 
				const unsigned int frameNo) 
{
	// Your solution goes here
	int vectorsize = files.size();
	int framenum = frameNo;
	if(vectorsize <= framenum){
		files.resize(framenum+1);
		pageNos.resize(framenum+1);
	}
	if(files[frameNo] != NULL){
		//cout << "Part 1 " << endl;
		return BUFMAPERROR;
	}
	files[frameNo] = file;
	pageNos[frameNo] = pageNo;
	//cout << "Put file pointer " << file << " and pageNo " << pageNo << " in frame no " << frameNo << endl;
	return OK;
}


//-------------------------------------------------------------------        
// Check if (file,pageNo) is currently in the buffer pool.  If so, 
// return corresponding frameNo. else return BUFMAPNOTFOUND
//-------------------------------------------------------------------

const Status BufMap::lookup(File* file, 
				const int pageNo,
				unsigned int & frameNo) const
{
	// Your solution goes here
	int vectorsize = files.size();
	bool found = false;
	//cout << "Trying to find file pointer " << file << " by comparing with " << files[0] << endl;
	//cout << " and pageNo " << pageNo << " with " << pageNos[0] << endl;
	for(int i = 0; i<vectorsize; i++){
		if(files[i] == file && pageNos[i] == pageNo){
			frameNo = i;
			found = true;
			break;
		}
	}
	if(!found){
		//cout<< "Huh?"<<endl;
		return BUFMAPNOTFOUND;
	}
	return OK;
}


//-------------------------------------------------------------------
// delete entry (file,pageNo) from mapping. Return OK if page was
// found.  Else if the entry is not found, return BUFMAPNOTFOUND
//-------------------------------------------------------------------

const Status BufMap::remove(File* file, const int pageNo) 
{
	// Your solution goes here
	int frameNo = -1;
	int vectorsize = files.size();
	bool found = false;
	//cout << "Trying to find file pointer " << file << " with Pageno " << pageNo << " vectorsize = " << vectorsize<< endl;
	for(int i = 0; i<vectorsize; i++){
		if(files[i] == file && pageNos[i] == pageNo){
			frameNo = i;
			found = true;
			break;
		}
	}
	if(!found){
		return BUFMAPNOTFOUND;
	}else{
		//cout << "Found it at frameno " << frameNo << endl;
		files[frameNo] = NULL;
		pageNos[frameNo] = -1;
		return OK;
	}
	
}

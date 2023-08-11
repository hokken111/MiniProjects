#include <memory.h>
#include "buf.h"
#include "db.h"

//----------------------------------------
// Constructor of the class BufMgr
//----------------------------------------

BufMgr::BufMgr(const unsigned int bufs)
{
	// Your solution goes here
	clockHand = 0;
	numBufs = bufs;
	bufPool = new Page[bufs];
   bufTable = new BufDesc[bufs];
}


BufMgr::~BufMgr() 
{
	int temp = numBufs;
   for(int i = 0; i < temp; i++){
       if(bufTable[i].dirty == true){
        		Status isgood = (bufTable[i].file)->writePage(bufTable[i].pageNo, &bufPool[i]);
        		if(isgood == OK){
        			bufStats.diskwrites ++;
        		}
       }
	}
	delete[] bufPool;
	delete[] bufTable;
}


const Status BufMgr::allocBuf(unsigned int & frame) 
{ 
	// Your solution goes here
	// Phorum says we should remove the entry from BufMap too. I don't really know how to do this.
	if(numBufs == 0){
		frame = clockHand;
		return OK;
	}
	advanceClock();
	unsigned int temp = clockHand;
	//cout << "Clockhand = " << clockHand << endl;	
   	do{
       	if(bufTable[clockHand].valid == false){
       		//cout << "1" << endl;
				bufTable[clockHand].Clear();
				frame = clockHand;
				return OK;
			}
			else if(bufTable[clockHand].valid == true){
				
				if(bufTable[clockHand].loved == true){
					//cout << "2" << endl;
					bufTable[clockHand].loved = false;
					advanceClock();
				}
				else if(bufTable[clockHand].loved == false){
				//cout << " a " << endl;
					if(bufTable[clockHand].pinCnt > 0){
						//cout << "3 " << endl;
						advanceClock();
					}
					else if(bufTable[clockHand].pinCnt <= 0){
						//cout << "4? " << endl;						
						if(bufTable[clockHand].dirty == true){
							//cout << "is this ever called" << endl;
							Status tempstatus = bufTable[clockHand].file->writePage(bufTable[clockHand].pageNo, &bufPool[clockHand]);//I think this is how you flush the page to disk.
							if(tempstatus == UNIXERR){
								return UNIXERR;
							}else if(tempstatus == OK){
									bufStats.diskwrites ++;
							}
							//flush page to disk? UNIXERR if fail;
						}
						bufMap.remove(bufTable[clockHand].file, bufTable[clockHand].pageNo);
						//if(tempst == OK)  cout << "all good here" << endl;
						bufTable[clockHand].Clear();
						
						frame = clockHand;
						return OK;
					}
				}
			}
			//cout << "Clockhand = " << clockHand << endl;	
		}while(clockHand != temp);

   	return BUFFEREXCEEDED;   
}

	
const Status BufMgr::readPage(File* file, const int PageNo, Page*& page)
{
	// Your solution goes here
	bufStats.accesses ++;
	unsigned int frameno;
   	Status stats = bufMap.lookup(file, PageNo, frameno);
   	if(stats == OK){
				bufTable[frameno].pinCnt++;
      		page = &(bufPool[frameno]);
       		return OK;
   	}else{
       		Status tempstatus2 = allocBuf(frameno);
				if(tempstatus2 == BUFFEREXCEEDED) return BUFFEREXCEEDED;//I think this is how to add the BUFFEREXCEEDED error.
      			Page *pgpointer = &bufPool[frameno];
      	 		Status tempstatus = file->readPage(PageNo, pgpointer); //probably need to add error catch!
      	 		//cout << "Page pointer = " << pgpointer << endl;
      	 		if(tempstatus == UNIXERR){    
      	     		return UNIXERR;
      	 		} else if (tempstatus == OK){
      	 			bufStats.diskreads ++;
      	     		bufTable[frameno].Set(file, PageNo);
      	     		if(bufMap.insert(file, PageNo, frameno) == BUFMAPERROR){
       					return BUFMAPERROR;
   					}

						//page = &bufPool[frameno];
      	     		page = pgpointer;
      	     		//cout << "Buf pool test = " << bufPool[frameno].data << endl;
      	     		return OK;
                }        
   		}
   		return OK;
   	
   //BUFFEREXCEEDED error = ?
   // do we need to write to the bufmap here? 
   // I don't think so...
}


const Status BufMgr::unPinPage(File* file, const int PageNo, const bool dirty, const bool love) 
{
	// Your solution goes here
	unsigned int frameno;
   	Status stats = bufMap.lookup(file, PageNo, frameno);
   	if(stats == OK){
       		if(bufTable[frameno].pinCnt == 0){
           		return PAGENOTPINNED;
       		}
       		bufTable[frameno].pinCnt--;
       		if(dirty == true){
           		bufTable[frameno].dirty = dirty;
       		}
       		if(love == true){
           		bufTable[frameno].loved = love;
       		}  
   	}
	else if(stats == BUFMAPNOTFOUND){
		return BUFMAPNOTFOUND;
   	}
   	return OK;
   //Need to add BADBUFFER check
}


const Status BufMgr::flushFile(File* file) 
{
	// Your solution goes here
	int temp = numBufs;
   for(int i = 0; i < temp; i++){
       		if(bufTable[i].pinCnt>0){
           		return PAGEPINNED;
       		}
       		if(bufTable[i].dirty == true){
           		Status isgood = (bufTable[i].file)->writePage(bufTable[i].pageNo, &bufPool[i]);
           		if(isgood == OK){
           				bufStats.diskwrites ++;
           		}
       		}
	}

	return OK;
}


const Status BufMgr::disposePage(File* file, const int pageNo)
{
	// Your solution goes here
		if(pageNo == 0){
        	return BADPAGENO;
   	}
   	unsigned int frameno;
   	Status stats = bufMap.lookup(file, pageNo, frameno);
   	if(stats == OK){
       		if(bufTable[frameno].pinCnt>0){
           		return PAGEPINNED;
       		}
       		bufMap.remove(file, pageNo);
       		bufTable[frameno].Clear();
   	}
   	Status status = file->disposePage(pageNo);
   	if(status == OK){
	   	return OK;
	   	bufStats.diskwrites++;
	   }else if(status == UNIXERR){
	   	return UNIXERR;
	   }
	   return OK;
}


const Status BufMgr::allocPage(File* file, int& pageNo, Page*& page) 
{
	// Your solution goes here
		bufStats.accesses ++;
	unsigned int frameno;
   	if(allocBuf(frameno) == BUFFEREXCEEDED){
       		return BUFFEREXCEEDED;
   	}

	//cout << frameno << " " << bufTable[frameno].pinCnt << " " << bufTable[frameno].pageNo << endl;
		Status isgood = file->allocatePage(pageNo);
   	if(isgood == UNIXERR){
	//cout << "THERE!" << endl;
       		return UNIXERR;
   	}else if(isgood == OK){
   		bufStats.diskwrites ++;
   	}
   	bufTable[frameno].Set(file, pageNo);
	//cout << "WHERE!" << endl;
   	if(bufMap.insert(file, pageNo, frameno) == BUFMAPERROR){
       		return BUFMAPERROR;
   	}

	page = &bufPool[frameno];//Do I need to dereference this or something else? 
		//cout << " PAGE "  << page << endl;
   	return OK;
}



// return the number of pages in the buffer pool that are not pinned
int BufMgr::numUnpinnedPages()
{
  int numFree = 0;
  BufDesc* tmpbuf;

  for (unsigned i=0; i<numBufs; i++) 
  {
    tmpbuf = &(bufTable[i]);
    if (tmpbuf->pinCnt == 0) numFree++;
  }

  return numFree;
}

void BufMgr::printSelf(void) {

  BufDesc* tmpbuf;
  
  cout << endl << "Print buffer...\n";
  for (unsigned i=0; i<numBufs; i++) {
    tmpbuf = &(bufTable[i]);
    cout << i << "\t" << (char*)(&bufPool[i]) 
      << "\tpinCnt: " << tmpbuf->pinCnt;
    
    if (tmpbuf->valid == true)
      cout << "\tvalid\n";
    cout << endl;
  };
}

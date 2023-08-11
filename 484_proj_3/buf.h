//
// Add header description for this file, and your group here
//

#ifndef BUF_H
#define BUF_H

#include "db.h"
#include "bufMap.h"

using namespace std; 

// Uncomment for debug output
//#define DEBUGBUF

// Forward declaration of BufMgr class
class BufMgr; 

// BufDesc class maintains bookkeeping information
// about individual buffer pool frames
class BufDesc {
    friend class BufMgr;
private:
  File* file;   // pointer to file object
  int   pageNo; // page within file
  int   pinCnt; // number of times this page has been pinned
  bool dirty;   // true if dirty;  false otherwise
  bool valid;   // true if page is valid
  bool loved; // true if someone has recently set a love "hint" for this page


  // Initialize buffer frame for a new requester
  void Clear() {  
	pinCnt = 0;
	file = NULL;
	pageNo = -1;
	dirty = false;
	loved = false;
	valid = false;
  };

  // Setup buffer frame for a particular page
  void Set(File* filePtr, int pageNum) { 
      file = filePtr;
      pageNo = pageNum;
      pinCnt = 1;
      dirty = false;
      loved = false;
      valid = true;
  }

  // Constructor
  BufDesc() {
      Clear();
  }
};

// Maintains usage statistics about buffer pool
//
struct BufStats
{
  unsigned accesses;    // Total number of accesses to buffer pool
  unsigned diskreads;   // Number of pages read from disk 
  unsigned diskwrites;  // Number of pages written back to disk

  void clear() { accesses = diskreads = diskwrites = 0; }
      
  BufStats() { clear(); }
};

inline ostream & operator << (ostream & os, const BufStats & stats)
{
  os << "\nNumber of accesses    = " << stats.accesses
     << "\nNumber of disk reads  = " << stats.diskreads
     << "\nNumber of disk writes = " << stats.diskwrites << endl << endl;

  return os;
}


class BufMgr 
{
private:
  unsigned int clockHand; // Clock hand for page replacement algorithm
  BufMap bufMap; // Data structure allows us to quickly locate a page
  BufDesc *bufTable; // Vector of information describing each buffer frame
  unsigned int numBufs;  // Number of pages in the buffer pool
  BufStats bufStats; // Statistics about buffer pool usage
  
  // Allocate a free frame; return the number of the frame by reference
  const Status allocBuf(unsigned int & frame);  
  
  // Advance the clock hand
  void advanceClock() {
      clockHand = (clockHand + 1) % numBufs;
  }

public:
  // Buffer pool comprised of an array of in-memory Page objects
  Page *bufPool;

  BufMgr(const unsigned int bufs);
  ~BufMgr();

  // Read page identified by (file, pageNo) pair
  // Pass back the in-memory representation of the page by reference
  const Status readPage(File* file, const int PageNo, Page*& page);
  
  // Unpin a page
  // Call to unpin must include two pieces of information:
  // (1) Is the page dirty? and 
  // (2) Is the page "loved" (likely to be needed again soon)?
  const Status unPinPage(File* file, const int PageNo, const bool dirty, const bool love);
  
  // Allocate a new empty page
  const Status allocPage(File* file, int& PageNo, Page*& page); 
  
  // Write all of the dirty pages in the file back to disk
  const Status flushFile(File* file); 
  
  // Dispose of the specified page
  const Status disposePage(File* file, const int PageNo);

  void printSelf(void); // print the contents of the buffer pool
  int numUnpinnedPages(); // get the number of unpinned pages in the buffer pool

  const BufStats & getBufStats() const // Get buffer pool usage
    {
      return bufStats;
    }

  void clearBufStats() 
    {
      bufStats.clear();
    }
};

#endif

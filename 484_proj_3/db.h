/**************************************
 * DO NOT IN ANY WAY MODIFY THIS FILE *
 **************************************/

#ifndef DB_H
#define DB_H

#include <sys/types.h>
#include <string>
#include <functional>
#include <map>
#include <iostream>
#include "error.h"

using namespace std;

// Uncomment the following to see debugging output
//
//#define DEBUGIO
//#define DEBUGFREE

// size of a data page in bytes (constant)
const int PAGESIZE = 1024;             

// Page class definition
// Page provides an in-memory (C++) data structure for storing data pages
class Page {
 public:
  char data[PAGESIZE]; // bytes contain the contents of the page
};

// Forward declaration of DB class
class DB; 

// The File class behaves as an in-memory "handle" for open files
// Whenever you open a file, even if the file already exists, you
// need to create a new File object to handle interaction with the file.
// 
class File {
  friend class DB;

 public:
  // Allocate a new page in the file
  // The page number of the new page is passed back by reference
  const Status allocatePage(int& pageNo);   
  
  // Destroy an existing page
  const Status disposePage(const int pageNo);
  
  // Read page pageNo from file
  // Returns a pointer to a Page object
  // (The Page object is an in-memory copy of the page from disk.)
  const Status readPage(const int pageNo, Page* pagePtr) const;
  
  // Write page pageNo to file
  // Takes pageNo and a pointer to the Page object as input
  const Status writePage(const int pageNo, const Page* pagePtr);
  
  // Returns the page number of the first page by reference
  const Status getFirstPage(int& pageNo) const;

  // Overloaded operators
  bool operator == (const File & other) const
    {
      return fileName == other.fileName;
    }

  bool operator < (const File & other) const
    {
      return fileName < other.fileName;
    }

 private: 

  // Constructor and destructor
  File(const string & fname);         
  ~File();                  

  static const Status create(const string & fileName);
  static const Status destroy(const string & fileName);

  const Status open();
  const Status close();

  // Check if the page is on the free list
  const Status onFreeList(const int pageNo, bool& onFL) const; 

  // internal file read
  const Status intread(const int pageNo, Page* pagePtr) const;     
  // internal file write   
  const Status intwrite(const int pageNo, const Page* pagePtr);       

#ifdef DEBUGFREE
  void listFree();                      // list free pages
#endif

  string fileName;                    // The name of the file
  int openCnt;                        // # times file has been opened
  int unixFile;                       // unix file stream for file
};


// Forward declaration of BufMgr
class BufMgr;
extern BufMgr* bufMgr;


// The DB class maintains all of the database's files
// 
class DB {
 public:
  DB(); // initialize open file table
  ~DB(); // clean up any remaining open files

  // Create a new file
  // Takes file name as input
  const Status createFile(const string & fileName) const;  
  
  // Destroy the specified file
  const Status destroyFile(const string & fileName) const; 
  
  // Open a file
  // Takes the file name as input, and returns a pointer to the File object by reference
  const Status openFile(const string & fileName, File* & file); 
  
  // Close a file
  const Status closeFile(File* file);

 private:
  typedef map<string, File* > OpenFileMap;
  OpenFileMap   openFiles;    // list of open files
};


// structure of DB (header) page

typedef struct {
  int nextFree;                         // page # of next page on free list
  int firstPage;                        // page # of first page in file
  int numPages;                         // total # of pages in file
} DBPage;

#endif

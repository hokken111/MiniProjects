/**************************************
 * DO NOT IN ANY WAY MODIFY THIS FILE *
 **************************************/

#include <memory.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <iostream>
#include "db.h"
#include "buf.h"


#define DBP(p)      (*(DBPage*)&p)


// Construct a File object which can operate on Unix files.

File::File(const string & fname)
{
  fileName = fname;
  openCnt = 0;
  unixFile = -1;
}

// Deallocate a file object
File::~File()
{
  if (openCnt == 0)
    return;

  // This means that file must be closed down if open
  // and buffer pages flushed.
  // To ensure that all this happens, must push down the openCnt to 1.
  openCnt = 1;

  Status status = close();
  if (status != OK)
    {
      Error error;
      error.print(status);
    }
}

const Status File::create(const string & fileName)
{
  int file;
  if ((file = ::open(fileName.c_str(), O_CREAT | O_EXCL | O_WRONLY, 0666)) < 0)
    {
      if (errno == EEXIST)
	return FILEEXISTS;
      else
	return UNIXERR;
    }

  // An empty file contains just a DB header page.

  Page header;
  memset(&header, 0, sizeof header);
  DBP(header).nextFree = -1;
  DBP(header).firstPage = -1;
  DBP(header).numPages = 1;
  if (write(file, (char*)&header, sizeof header) != sizeof header)
    return UNIXERR;

  if (::close(file) < 0)
    return UNIXERR;

  return OK;
}

const Status File::destroy(const string & fileName)
{
  if (unlink(fileName.c_str()) < 0)
    return UNIXERR;

  return OK;
}

const Status File::open()
{
  // Open file -- it will be closed in closeFile().

  if (openCnt == 0)
    {
      if ((unixFile = ::open(fileName.c_str(), O_RDWR)) < 0)
	return UNIXERR;

      // Store file info in open files table.

      openCnt = 1;
    }
  else
    openCnt++;

  return OK;
}

const Status File::close()
{
  if (openCnt <= 0)
    return FILENOTOPEN;

  openCnt--;

  // File actually closed only when open count goes to zero.

  if (openCnt == 0) {

  Status status;
  if (bufMgr)
    if ((status = bufMgr->flushFile(this)) != OK)
    {
        openCnt++;
       return status;
    }
    if (::close(unixFile) < 0)
    {
        openCnt++;
        return UNIXERR;
    }
  }

  return OK;
}


// Allocate a page either from a free list (list of pages which
// were previously disposed of), or extend file if no free pages
// are available.

const Status File::allocatePage(int& pageNo)
{
  Page header;
  Status status;

  if ((status = intread(0, &header)) != OK)
    return status;

  // If free list has pages on it, take one from there
  // and adjust free list accordingly.

  if (DBP(header).nextFree != -1) {     // free list exists?

    // Return first page on free list to the caller,
    // adjust free list accordingly.

    pageNo = DBP(header).nextFree;
    Page firstFree;
    if ((status = intread(pageNo, &firstFree)) != OK)
      return status;
    DBP(header).nextFree = DBP(firstFree).nextFree;

  } else {                              // no free list, have to extend file

    // Extend file -- the current number of pages will be
    // the page number of the page to be returned.

    pageNo = DBP(header).numPages;
    Page newPage;
    memset(&newPage, 0, sizeof newPage);
    if ((status = intwrite(pageNo, &newPage)) != OK)
      return status;

    DBP(header).numPages++;

    if (DBP(header).firstPage == -1)    // first user page in file?
      DBP(header).firstPage = pageNo;
  }

  if ((status = intwrite(0, &header)) != OK)
    return status;
  
#ifdef DEBUGFREE
  listFree();
#endif

  return OK;
}


// Check if the page is on the free list. Requires traversing the link list
// More efficient data structures are possible!
const Status File::onFreeList(const int pageNo, bool& onFL) const
{
  int pgNo;
  Page header;
  Status status;

  onFL = false;
  if ((status = intread(0, &header)) != OK)
    return status;

  // Check the free list pages
  if (DBP(header).nextFree != -1)  // Free list exists
  {
     while ((pgNo = DBP(header).nextFree) != -1)  
     {
        if (pageNo == pgNo)
        {
           onFL = true;
           return OK;
        }

        // Read the next page;
        if ((status = intread(pgNo, &header)) != OK) 
           return status;
     }
  }

  return OK;
}


// Deallocate a page from file. The page will be put on a free
// list and returned back to the caller upon a subsequent
// allocPage() call.
// ALERT: No error returned if disposePage called on the same page twice!
const Status File::disposePage(int pageNo)
{
  if (pageNo < 1)
    return BADPAGENO;

  Page header;
  Status status;

  if ((status = intread(0, &header)) != OK)
    return status;

  // The first user-allocated page in the file cannot be
  // disposed of. The File layer has no knowledge of what
  // is the next page in the file and hence would not be
  // able to adjust the firstPage field in file header.

  if (DBP(header).firstPage == pageNo || pageNo >= DBP(header).numPages)
    return BADPAGENO;

  // Deallocate page by attaching it to the free list.

  Page away;
  if ((status = intread(pageNo, &away)) != OK)
    return status;
  memset(&away, 0, sizeof away);
  DBP(away).nextFree = DBP(header).nextFree;
  DBP(header).nextFree = pageNo;

  if ((status = intwrite(pageNo, &away)) != OK)
    return status;
  if ((status = intwrite(0, &header)) != OK)
    return status;

#ifdef DEBUGFREE
  listFree();
#endif

  return OK;
}


// Read a page from file and store page contents at the page address
// provided by the caller.

const Status File::intread(int pageNo, Page* pagePtr) const
{
  if (lseek(unixFile, pageNo * sizeof(Page), SEEK_SET) == -1)
    return UNIXERR;

  int nbytes = read(unixFile, (char*)pagePtr, sizeof(Page));

#ifdef DEBUGIO
  cerr << "%%  File " << (int)this << ": read bytes ";
  cerr << pageNo * sizeof(Page) << ":+" << nbytes << endl;
  cerr << "%%  ";
  for(int i = 0; i < 10; i++)
    cerr << *((int*)pagePtr + i) << " ";
  cerr << endl;
#endif

  if (nbytes != sizeof(Page))
    return UNIXERR;

  return OK;
}


// Write a page to file. Page data is at the page address
// provided by the caller.

const Status File::intwrite(const int pageNo, const Page* pagePtr)
{
  if (lseek(unixFile, pageNo * sizeof(Page), SEEK_SET) == -1)
    return UNIXERR;

  int nbytes = write(unixFile, (char*)pagePtr, sizeof(Page));

#ifdef DEBUGIO
  cerr << "%%  File " << (int)this << ": wrote bytes ";
  cerr << pageNo * sizeof(Page) << ":+" << nbytes << endl;
  cerr << "%%  ";
  for(int i = 0; i < 10; i++)
    cerr << *((int*)pagePtr + i) << " ";
  cerr << endl;
#endif

  if (nbytes != sizeof(Page))
    return UNIXERR;

  return OK;
}


// Read a page from file, check parameters for validity.

const Status File::readPage(int pageNo, Page* pagePtr) const
{
  if (!pagePtr)
    return BADPAGEPTR;
  if (pageNo < 1)
    return BADPAGENO;

  bool onFL;
  Status status;
  if ((status = onFreeList(pageNo, onFL)) != OK) 
     return status;

  if (onFL) 
     return BADPAGENO; // The page is on the free list.
  else 
     return intread(pageNo, pagePtr);
 
}


// Write a page to file, check parameters for validity.

const Status File::writePage(const int pageNo, const Page *pagePtr)
{
  if (!pagePtr)
    return BADPAGEPTR;
  if (pageNo < 1)
    return BADPAGENO;

  return intwrite(pageNo, pagePtr);
}


// Return the number of the first page in file. It is stored
// on the file's header page (field firstPage).

const Status File::getFirstPage(int& pageNo) const
{
  Page header;
  Status status;

  if ((status = intread(0, &header)) != OK)
    return status;

  pageNo = DBP(header).firstPage;

  return OK;
}


#ifdef DEBUGFREE

// Print out the page numbers on the free list. For debugging only.

void File::listFree()
{
  cerr << "%%  File " << (int)this << " free pages:";
  int pageNo = 0;
  for(int i = 0; i < 10; i++) {
    Page page;
    if (intread(pageNo, &page) != OK)
      break;
    pageNo = DBP(page).nextFree;
    cerr << " " << pageNo;
    if (pageNo == -1)
      break;
  }
  cerr << endl;
}
#endif


// Construct a DB object which keeps track of creating, opening, and
// closing files.

DB::DB()
{
  // Check that DB header page data fits on a regular data page.

  if (sizeof(DBPage) >= sizeof(Page)) {
    cerr << "sizeof(DBPage) cannot exceed sizeof(Page): "
         << sizeof(DBPage) << " " << sizeof(Page) << endl;
    exit(1);
  }
}


// Destroy DB object. Make sure all files that were left open will
// be closed properly.

DB::~DB()
{
  // Go through the open files map and destroy each file object. That will
  // also close the file and flush its pages.
  OpenFileMap::iterator iter;
  for (iter = openFiles.begin(); iter != openFiles.end(); iter++)
    delete iter->second;
}


  
// Create a database file.

const Status DB::createFile(const string &fileName) const
{
  if (fileName.empty())
    return BADFILE;

  // First check if the file has already been opened
  if (openFiles.find(fileName) != openFiles.end())
    return FILEEXISTS;

  // Do the actual work
  return File::create(fileName);
}


// Delete a database file.

const Status DB::destroyFile(const string & fileName) const
{
  if (fileName.empty())
    return BADFILE;

  // Make sure file is not open currently.
  if (openFiles.find(fileName) != openFiles.end())
    return FILEOPEN;
  
  // Do the actual work
  return File::destroy(fileName);
}


// Open a database file. If file already open, increment open count,
// otherwise find a vacant slot in the open files table and store
// file info there.

const Status DB::openFile(const string & fileName, File*& file)
{
  Status status;

  if (fileName.empty())
    return BADFILE;

  // Check if file already open. 
  OpenFileMap::iterator iter = openFiles.find(fileName);

  if (iter != openFiles.end())
    {
      // If it is, call open again on the file object
      // to increment it's open count.
      status = iter->second->open();

      file = iter->second;
    }
  else
    {
      // Otherwise create a new file object and open it

      file = new File(fileName);
      status = file->open();

      if (status != OK)
      {
        delete file;
	return status;
      }

      // Insert into the mapping table
      openFiles.insert(OpenFileMap::value_type(fileName, file));
    }

  return status;
}


// Close a database file. Get file info from open files table,
// call Unix close() only if open count now goes to zero.

const Status DB::closeFile(File* file)
{
  if (!file)
    return BADFILEPTR;

  Status status;

  // Close the file
  if ((status = file->close())  != OK) return status;

  // If there are no remaining references to the file, then we should delete
  // the file object and remove it from the openFilesMap

  if (file->openCnt == 0)
    {
      if (openFiles.erase(file->fileName) != 1)
	return BADFILEPTR;

	  // ** 3/08: Please note that the following is commented out to prevent
	  //          invalid File* objects from being left in the BufMap object.
	  //          This is a bug, and this fix will result in a small memory leak.
	  
      //delete file;
    }

  return OK;
}

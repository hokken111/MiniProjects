#ifndef ERROR_H
#define ERROR_H

// error/status codes defined here

// add error codes under appropriate heading
// -- remember to add corresponding message to
//    Error.print() also!

enum Status {

// no error

       OK = 0, NOTUSED1 = -999,

// File and DB errors

       BADFILEPTR, BADFILE, FILEOPEN, FILENOTOPEN,
       UNIXERR, BADPAGEPTR, BADPAGENO, FILEEXISTS,

// BufMgr and HashTable errors

       BUFMAPERROR, BUFMAPNOTFOUND, BUFFEREXCEEDED, PAGENOTPINNED,
       BADBUFFER, PAGEPINNED,

// Relation errors

       BADTID, BADRECPTR, BADSCANPARM, BADSCANID,
       SCANTABFULL, RELEOF,

// do not touch filler -- add codes before it

       NOTUSED2
};


// definition of Error class

class Error {
 public:
   static void print(Status status);
};

#endif


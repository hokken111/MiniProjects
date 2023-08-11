#include <iostream>
#include <stdio.h>
#include "error.h"

using namespace std;

void Error::print(Status status)
{
  cerr << "Error: ";
  switch(status) {

    // no error

    case OK:           cerr << "no error"; break;

    // File and DB errors

    case BADFILEPTR:   cerr << "bad file pointer"; break;
    case BADFILE:      cerr << "bad filename"; break;
    case FILEOPEN:     cerr << "file open"; break;
    case FILENOTOPEN:  cerr << "file not open"; break;
    case UNIXERR:      perror("Unix error"); break;
    case BADPAGEPTR:   cerr << "bad page pointer"; break;
    case BADPAGENO:    cerr << "bad page number"; break;
    case FILEEXISTS:   cerr << "file exists already"; break;

    // BufMgr and BufMap errors

    case BUFMAPERROR: cerr << "buffer map error"; break;
    case BUFMAPNOTFOUND: cerr << "entry not found"; break;
    case BUFFEREXCEEDED: cerr << "buffer pool full"; break;
    case PAGENOTPINNED: cerr << "page not pinned"; break;
    case BADBUFFER: cerr << "buffer pool corrupted"; break;
    case PAGEPINNED: cerr << "page still pinned"; break;

    // Relation errors

    case BADTID:       cerr << "bad tuple id"; break;
    case BADRECPTR:    cerr << "bad record pointer"; break;
    case BADSCANPARM:  cerr << "bad scan parameter"; break;
    case BADSCANID:    cerr << "bad scan id"; break;
    case SCANTABFULL:  cerr << "scan table full"; break;
    case RELEOF:       cerr << "end of relation"; break;

    default:           cerr << "undefined error status: " << status;
  }
  cerr << endl;
}


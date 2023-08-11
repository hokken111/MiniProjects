#include <stdio.h>
#include <iostream>
#include "buf.h"

// This file contains some rudimentary tests to help you test
// your buffer manager.
//
// Please note that when grading, we will use additional tests not included in this file!
//


#define CALL(c)    { Status s; \
                     if ((s = c) != OK) { \
		       cerr << "At " << __FILE__ << ":" << __LINE__ << endl << "  "; \
                       Error::print(s); \
                       exit(1); \
                     } \
                   }

#define ASSERT(c)  { if (!(c)) { \
		       cerr << "At " << __FILE__ << ":" << __LINE__ << endl << "  "; \
                       cerr << "This condition should hold: " #c << endl; \
                       exit(1); \
		     } \
                   }

#define FAIL(c)  { Status s; \
                   if ((s = c) == OK) { \
		     cerr << "At " << __FILE__ << ":" << __LINE__ << endl << "  "; \
                     cerr << "This call should fail: " #c << endl; \
                     exit(1); \
		     } \
                   else \
                   { \
                     cout << "Failing as expected while executing: " #c << endl; \
                   } \
		     }

BufMgr*     bufMgr;
BufMgr*     bufMgr2;
int main()
{
    DB          db;
    File*	file1;
    File*	file2;
    File* 	file3;
    File*   file4;
    int		i;
    const int   num = 50;
    int         j[num*2];    

    // create buffer manager 
    bufMgr = new BufMgr(num);

	// create some files
	
    (void)db.destroyFile("test.1");
    (void)db.destroyFile("test.2");
    (void)db.destroyFile("test.3");
    (void)db.destroyFile("test.4");

    CALL(db.createFile("test.1"));
    ASSERT(db.createFile("test.1") == FILEEXISTS);
    CALL(db.createFile("test.2"));
	ASSERT(db.createFile("test.2") == FILEEXISTS);
    CALL(db.createFile("test.3"));
	ASSERT(db.createFile("test.3") == FILEEXISTS);
    CALL(db.createFile("test.4"));
	ASSERT(db.createFile("test.4") == FILEEXISTS);

    CALL(db.openFile("test.1", file1));
    CALL(db.openFile("test.2", file2));
    CALL(db.openFile("test.3", file3));
    CALL(db.openFile("test.4", file4));


    // test buffer manager

    Page* page;
    Page* page2;
    Page* page3;
    Page cmp;
    int pageno, pageno2, pageno3;

	int num2 = 2*num;
	cout << "Allocating " << num2 << " pages in a file... " << endl;

    for (i = 0; i < num2; i++) {
      CALL(bufMgr->allocPage(file1, j[i], page));
      sprintf((char*)page, "test.1 Page %d %7.1f", j[i], (float)j[i]);
      CALL(bufMgr->unPinPage(file1, j[i], true, false));
    }
	
	cout << "BUFSTATS:" << bufMgr->getBufStats() << endl;
    bufMgr->clearBufStats();


    cout << "Reading pages back..." << endl;
    for (i = 0; i < num2; i++) {
      CALL(bufMgr->readPage(file1, j[i], page));
      sprintf((char*)&cmp, "test.1 Page %d %7.1f", j[i], (float)j[i]);
      ASSERT(memcmp(page, &cmp, strlen((char*)&cmp)) == 0);
      CALL(bufMgr->unPinPage(file1, j[i], false, false)); 
    }
	
    cout << "BUFSTATS:" << bufMgr->getBufStats() << endl;
    bufMgr->clearBufStats();
	
	
	
    cout << "Writing and reading back multiple files..." << endl;
    for (i = 0; i < num/3; i++) {
      CALL(bufMgr->allocPage(file2, pageno2, page2));
      sprintf((char*)page2, "test.2 Page %d %7.1f", pageno2, (float)pageno2);
      CALL(bufMgr->allocPage(file3, pageno3, page3));
      sprintf((char*)page3, "test.3 Page %d %7.1f", pageno3, (float)pageno3);

      if ( i == 0)
	{
	  cout << "BUFSTATS:" << bufMgr->getBufStats() << endl;
	}

      pageno = j[random() % num];
      CALL(bufMgr->readPage(file1, pageno, page));
      sprintf((char*)&cmp, "test.1 Page %d %7.1f", pageno, (float)pageno);
      ASSERT(memcmp(page, &cmp, strlen((char*)&cmp)) == 0);
      cout << (char*)page << endl;

      CALL(bufMgr->readPage(file2, pageno2, page2));
      sprintf((char*)&cmp, "test.2 Page %d %7.1f", pageno2, (float)pageno2);
      ASSERT(memcmp(page2, &cmp, strlen((char*)&cmp)) == 0);
      CALL(bufMgr->readPage(file3, pageno3, page3));
      sprintf((char*)&cmp, "test.3 Page %d %7.1f", pageno3, (float)pageno3);
      ASSERT(memcmp(page3, &cmp, strlen((char*)&cmp)) == 0);

      CALL(bufMgr->unPinPage(file1, pageno, true, false));
    }

    cout << "BUFSTATS:" << bufMgr->getBufStats() << endl;
    bufMgr->clearBufStats();

    for (i = 0; i < num/3; i++) {
      CALL(bufMgr->unPinPage(file2, i+1, true, false));
      CALL(bufMgr->unPinPage(file2, i+1, true, false));
      CALL(bufMgr->unPinPage(file3, i+1, true, false));
      CALL(bufMgr->unPinPage(file3, i+1, true, false));
    }

    cout << "BUFSTATS:" << bufMgr->getBufStats() << endl;
    bufMgr->clearBufStats();

#ifdef DEBUGBUF
    bufMgr->printSelf();
#endif 

    cout << "\nReading \"test.1\"...\n";
    for (i = 1; i < num/3; i++) {
      CALL(bufMgr->readPage(file1, i, page2));
      sprintf((char*)&cmp, "test.1 Page %d %7.1f", i, (float)i);
      ASSERT(memcmp(page2, &cmp, strlen((char*)&cmp)) == 0);
      CALL(bufMgr->unPinPage(file1, i, false, false));
    }

    cout << "BUFSTATS:" << bufMgr->getBufStats() << endl;
    bufMgr->clearBufStats();

    cout << "\nReading \"test.2\"...\n";
    for (i = 1; i < num/3; i++) {
      CALL(bufMgr->readPage(file2, i, page2));
      sprintf((char*)&cmp, "test.2 Page %d %7.1f", i, (float)i);
      ASSERT(memcmp(page2, &cmp, strlen((char*)&cmp)) == 0);
      cout << (char*)page2 << endl;
      CALL(bufMgr->unPinPage(file2, i, false, false));
    }

    cout << "BUFSTATS:" << bufMgr->getBufStats() << endl;
    bufMgr->clearBufStats();

    cout << "\nReading \"test.3\"...\n";
    for (i = 1; i < num/3; i++) {
      CALL(bufMgr->readPage(file3, i, page3));
      sprintf((char*)&cmp, "test.3 Page %d %7.1f", i, (float)i);
      ASSERT(memcmp(page3, &cmp, strlen((char*)&cmp)) == 0);
      cout << (char*)page3 << endl;
      CALL(bufMgr->unPinPage(file3, i, false, false));
    }

    cout << "BUFSTATS:" << bufMgr->getBufStats() << endl;
    bufMgr->clearBufStats();



    cout << "\nTesting error conditions...\n\n";

    Status status;
    FAIL(status = bufMgr->readPage(file4, 1, page));
    Error::print(status);
    CALL(bufMgr->allocPage(file4, i, page));
    CALL(bufMgr->unPinPage(file4, i, true, false));
    FAIL(status = bufMgr->unPinPage(file4, i, false, false));
    Error::print(status);

    for (i = 0; i < num; i++) {
      CALL(bufMgr->allocPage(file4, j[i], page));
      sprintf((char*)page, "test.4 Page %d %7.1f", j[i], (float)j[i]);
    }
    int tmp;
    FAIL(status = bufMgr->allocPage(file4, tmp, page));
    Error::print(status);

#ifdef DEBUGBUF
    bufMgr->printSelf();
#endif 

    for (i = 0; i < num; i++)
      CALL(bufMgr->unPinPage(file4, i+2, true, false));
    
    cout << "\nReading \"test.1\"...\n";
    for (i = 1; i < num; i++) {
      CALL(bufMgr->readPage(file1, i, page));
      sprintf((char*)&cmp, "test.1 Page %d %7.1f", i, (float)i);
      ASSERT(memcmp(page, &cmp, strlen((char*)&cmp)) == 0);
      
      if ( i < num/2 )
      {
             sprintf((char*)page, "test.1 Page %d %7.1f", i, (float)i);
             CALL(bufMgr->unPinPage(file1, i, true, false)); // make the page dirty
             CALL(bufMgr->readPage(file1, i, page));  // make the page pinned 
      }
      cout << (char*)page << endl;
    }
    
    FAIL(status = bufMgr->flushFile(file1));
    Error::print(status);

    for (i = 1; i < num; i++) 
      CALL(bufMgr->unPinPage(file1, i, true, false));

    CALL(bufMgr->flushFile(file1));
		
	 cout << "\nTesting disposePage \n";
	 int temp = 4;
	 CALL(bufMgr->allocPage(file1, temp, page));
    CALL(bufMgr->unPinPage(file1, temp, true, false));
	 CALL(bufMgr->disposePage(file1, temp));
		
		
	 cout << "\nSmall buffer test\n";
	 bufMgr2 = new BufMgr(1);
	 num2 = 50;
	cout << "Allocating " << num2 << " pages in a file... " << endl;

    for (i = 0; i < num2; i++) {
      CALL(bufMgr2->allocPage(file1, j[i], page));
      sprintf((char*)page, "test.1 Page %d %7.1f", j[i], (float)j[i]);
      CALL(bufMgr2->unPinPage(file1, j[i], true, false));
    }
	 	cout << "BUFSTATS:" << bufMgr2->getBufStats() << endl;
    bufMgr->clearBufStats();


    cout << "Reading pages back..." << endl;
    for (i = 0; i < num2; i++) {
      CALL(bufMgr2->readPage(file1, j[i], page));
      sprintf((char*)&cmp, "test.1 Page %d %7.1f", j[i], (float)j[i]);
      ASSERT(memcmp(page, &cmp, strlen((char*)&cmp)) == 0);
      CALL(bufMgr2->unPinPage(file1, j[i], false, false)); 
    }
	
    cout << "BUFSTATS:" << bufMgr2->getBufStats() << endl;
    bufMgr->clearBufStats();
    CALL(bufMgr->flushFile(file1));
    
    cout << "\nStress Test \n";
	num2 = 10000;
	cout << "Allocating " << num2 << " pages in a file... " << endl;
	int k[10000];  
    for (i = 0; i < num2; i++) {
      CALL(bufMgr->allocPage(file1, k[i], page));
      sprintf((char*)page, "test.1 Page %d %7.1f", k[i], (float)k[i]);
      CALL(bufMgr->unPinPage(file1, k[i], true, false));
    }
	
	cout << "BUFSTATS:" << bufMgr->getBufStats() << endl;
    bufMgr->clearBufStats();


    cout << "Reading pages back..." << endl;
    for (i = 0; i < num2; i++) {
      CALL(bufMgr->readPage(file1, k[i], page));
      sprintf((char*)&cmp, "test.1 Page %d %7.1f", k[i], (float)k[i]);
      ASSERT(memcmp(page, &cmp, strlen((char*)&cmp)) == 0);
      CALL(bufMgr->unPinPage(file1, k[i], false, false)); 
    }
	
    cout << "BUFSTATS:" << bufMgr->getBufStats() << endl;
    bufMgr->clearBufStats();
    

    CALL(db.closeFile(file1));
    CALL(db.closeFile(file2));
    CALL(db.closeFile(file3));
    CALL(db.closeFile(file4));

    CALL(db.destroyFile("test.1"));
    CALL(db.destroyFile("test.2"));
    CALL(db.destroyFile("test.3"));
    CALL(db.destroyFile("test.4"));

    delete bufMgr;

    cout << endl << "Passed all tests." << endl;

    return (1);
}

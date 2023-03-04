#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"

// test name
char *testName;

/* test output files */
#define TESTPF "test_pagefile.bin"

/* prototypes for test functions */
static void testCreateOpenClose(void);
static void testSinglePageContent(void);

/* main function running all tests */
int
main (void)
{
  testName = "";
  
  initStorageManager();

  testCreateOpenClose();
  testSinglePageContent();

  return 0;
}


/* check a return code. If it is not RC_OK then output a message, error description, and exit */
/* Try to create, open, and close a page file */
void
testCreateOpenClose(void)
{
  SM_FileHandle fh;

  testName = "test create open and close methods";

  TEST_CHECK(createPageFile (TESTPF));
  
  TEST_CHECK(openPageFile (TESTPF, &fh));
  ASSERT_TRUE(strcmp(fh.fileName, TESTPF) == 0, "filename correct");
  ASSERT_TRUE((fh.totalNumPages == 1), "expect 1 page in new file");
  ASSERT_TRUE((fh.curPagePos == 0), "freshly opened file's page position should be 0");

  TEST_CHECK(closePageFile (&fh));
  TEST_CHECK(destroyPageFile (TESTPF));

  // after destruction trying to open the file should cause an error
  ASSERT_TRUE((openPageFile(TESTPF, &fh) != RC_OK), "opening non-existing file should return an error.");

  TEST_DONE();
}

/* Try to create, open, and close a page file */
void
testSinglePageContent(void)
{
  SM_FileHandle fh;
  SM_PageHandle ph;
  int i;

  testName = "test single page content";

  // allocate memory for a page
  ph = (SM_PageHandle) malloc(PAGE_SIZE);

  // create a new page file
  TEST_CHECK(createPageFile (TESTPF));
  TEST_CHECK(openPageFile (TESTPF, &fh));
  printf("created and opened file\n");
  
  // read first page into handle
  TEST_CHECK(readFirstBlock (&fh, ph));
  // the page should be empty (zero bytes)
  for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == 0), "expected zero byte in first page of freshly initialized page");
  printf("first block was empty\n");
    
  // change ph to be a string and write that one to disk
  for (i=0; i < PAGE_SIZE; i++)
    ph[i] = (i % 10) + '0';
  TEST_CHECK(writeBlock (0, &fh, ph));
  printf("writing first block\n");

  // read back the page containing the string and check that it is correct
  TEST_CHECK(readFirstBlock (&fh, ph));
  for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
  printf("reading first block\n");


  // Additionally added test cases

  int oldPageCount = fh.totalNumPages;

  // Add New Page
  TEST_CHECK(appendEmptyBlock(&fh));
  ASSERT_TRUE(fh.totalNumPages == oldPageCount + 1, "Page added. Page count incremented by 1. New page count 2.");

  oldPageCount = fh.totalNumPages;

  // Change Page Count (New Count > Old Count)
  TEST_CHECK(ensureCapacity(5, &fh));
  ASSERT_TRUE(fh.totalNumPages > oldPageCount, "Pages added. Page count increased to 5.");

  oldPageCount = fh.totalNumPages;

  // Change Page Count (New Count <= Old Count)
  TEST_CHECK(ensureCapacity(3, &fh));
  ASSERT_TRUE(fh.totalNumPages == oldPageCount, "No pages added. Page count remained the same (5).");

  // Read last page into handle
  TEST_CHECK(readLastBlock (&fh, ph));
  // The page should be empty (zero bytes)
  for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == 0), "expected zero byte in last page of freshly initialized page");
  printf("first block was empty\n");
    
  // Change ph to be a string and write that one to disk
  for (i=0; i < PAGE_SIZE; i++)
    ph[i] = (i % 10) + '0';
  TEST_CHECK(writeCurrentBlock (&fh, ph));
  printf("writing current (last) block\n");

  // read back the page containing the string and check that it is correct
  TEST_CHECK(readLastBlock (&fh, ph));
  for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
  printf("reading last block\n");

  // Read given page into handle
  TEST_CHECK(readBlock (2, &fh, ph));
  // The page should be empty (zero bytes)
  for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == 0), "expected zero byte in page 2 of freshly initialized page");
  printf("3rd block was empty\n");

  // Read current page into handle
  TEST_CHECK(readCurrentBlock (&fh, ph));
  // The page should be empty (zero bytes)
  for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == 0), "expected zero byte in current page (2) of freshly initialized page");
  printf("Current block (3rd) was empty\n");

  // Read previous page into handle
  TEST_CHECK(readPreviousBlock (&fh, ph));
  // The page should be empty (zero bytes)
  for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == 0), "expected zero byte in previous page (1) of freshly initialized page");
  printf("Previous block (2nd) was empty\n");

  // Read next page into handle
  TEST_CHECK(readNextBlock (&fh, ph));
  // The page should be empty (zero bytes)
  for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == 0), "expected zero byte in next page (2) of freshly initialized page");
  printf("Next block (3rd) was empty\n");

  // destroy new page file
  TEST_CHECK(closePageFile (&fh));
  TEST_CHECK(destroyPageFile (TESTPF));  

  // free page memory
  free(ph);
  
  TEST_DONE();
}

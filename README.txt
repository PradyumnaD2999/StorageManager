Project Name - Storage Manager

Name: Pradyumna Deshpande
Email: deshpradyumna@gmail.com

A) File Structure:

    	dberror.c
    	dberror.h
    	storage_mgr.c
    	storage_mgr.h
    	test_assign1_1.c
    	test_helper.h
	README.txt
	makefile    


B) Steps to run and test:

1) Open terminal and navigate to the project root directory "StorageManager".
2) To verify that we are in the right directory, use ls to list the files. This will also show you the file structure.
3) To remove outdated object files, type "make clean".
4) To build and compile all project files, type "make".
5) To run the test cases type "make run".


C) Implementation:

a) File Functions:

The file-related operations, such as initializing, creating, opening, closing, and deleting files.

1) initStorageManager():
- The storage manager is initialized using this function. In this method, the file stream object is referenced as NULL.

2) createPageFile():
- New file with specified file name is created with 1 page and '\0' bytes of data.
- In the event that the file could not be created, we return RC_FILE_NOT_FOUND; otherwise, we return RC_OK.

3) openPageFile():
- We open the file for reading and writing.
- We initialize the fileName, mgmtInfo, curPagePos, totalPageNum fields in struct FileHandle.
* If the file doesn't exist, we return RC_FILE_NOT_FOUND; otherwise, we return RC_OK.

4) closePageFile():
- We close the file.
- We return RC_OK if closed successfully, RC_FILE_HANDLE_NOT_INIT is FileHandle is NULL, and RC_FILE_NOT_FOUND if file is already closed or doesn't exist.

5) destroyPageFile():
- We delete the file from memory.
- We return RC_OK if deleted, or RC_FILE_NOT_FOUND if file is already deleted / doesn't exist or if file is not closed.


b) Read Functions:

The read related functions are used to read blocks of data from the page file into the disk (memory).

1) readBlock():
- We determine the validity of the page number. There should be more pages than zero and fewer pages overall.
- Verify whether the page file pointer is present.
- We use fseek() to go to the specified position using the valid file pointer.
- If fseek() succeeds, we use the C function fwrite() to write the data to the proper position and save it in the memPage parameter that was supplied.
- In FileHandle, we save the currently read page to currPagePos.

2) getBlockPos():
- This function retrieves the current page position from FileHandle's curPagePos and returns it.

3) readFirstBlock():
- Use 0 as the pageNum argument when calling the readBlock() method.

4) readPreviousBlock()
- Using the pageNum argument, we invoke the readBlock() function (current page position - 1)

5) readCurrentBlock():
- We use the pageNum argument to invoke the readBlock() function (current page position)

6) readNextBlock():
- By passing (current page position + 1) as the pageNum argument when calling the readBlock() function.

7) readLastBlock():
- The pageNum argument is used when calling the readBlock() function (total number of pages - 1)


c) Write Functions:

Blocks of data are written to the page file using the write functions from the disk (memory).

1) writeBlock():
- We determine the validity of the page number. There should be more pages than zero and fewer pages overall.
- Verify whether the page file pointer is present.
- We use fseek() to go to the specified position using the valid file pointer.
- If fseek() succeeds, we use the C function fwrite() to write the data to the proper position and save it in the memPage parameter that was supplied.
- In FileHandle, we save the currently read page to currPagePos.

2) writeCurrentBlock():
- We call the writeBlock() procedure with the parameter pageNum set to the current page position.

3) appendEmptyBlock() :
- We build an empty block with size = PAGE_SIZE. Since we just added a new page, write the empty block data and increase the total number of pages by 1.
- The cursor (pointer) of the file stream is moved to the last page.

4) ensureCapacity():
- Using the appendEmptyBlock() function, keep adding new empty pages till totalNumPages in FileHandle are less than given numberOfPages.
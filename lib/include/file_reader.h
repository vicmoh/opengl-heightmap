/**********************************************************
 * @author Vicky Mohammad
 * Copyright 2017, Vicky Mohammad, All rights reserved.
 * https://github.com/vicmoh/DynamicStringAPI
 **********************************************************/

#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  unsigned int length;
  char** lines;
} FileReader;

/**
 * Constructor to read file from file name and path.
 * @param fileName, including path.
 * @return the instance of the object.
 */
FileReader* new_FileReader(char* fileName);

/**
 * Dispose the instance @FileReader memory.
 * @param this @FileReader object.
 */
void free_FileReader(FileReader* this);

/**
 * Get number of lines in the file.
 * @param this @FileReader object.
 * @return the total lines in file.
 */
int FileReader_getLength(FileReader* this);

/**
 * Get the string at index of line.
 * @param this @FileReader object.
 * @return an allocated string of the line.
 */
char* FileReader_getLineAt(FileReader* this, int index);

/**
 * Function to get the whole file string.
 * @param this @FileReader object.
 * @return whole file string.
 */
char* FileReader_toString(FileReader* this);

/**
 * Function tp run test if it is working.
 */
void FileReader_runTest();

#endif

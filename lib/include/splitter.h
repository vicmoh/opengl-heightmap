/**********************************************************
 * @author Vicky Mohammad
 * Copyright 2017, Vicky Mohammad, All rights reserved.
 * https://github.com/vicmoh/DynamicStringAPI
 **********************************************************/

#ifndef SPLITTER_H
#define SPLITTER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char** list;
  unsigned int length;
  void* __private__;
} Splitter;

/**
 * Create new object to split a string.
 * @param toBeSplit of the string.
 * @param delimiter of the split.
 * @return this allocated object.
 */
Splitter* new_Splitter(char* toBeSplit, const char* delimiter);

/**
 * Free the this object.
 * @param this object including the attribute.
 */
void free_Splitter(Splitter* this);

/**
 * Get the string at certain position
 * of the splitted strings.
 * @param this object.
 * @param position of the string that has been split.
 * Position starts at zero like an array.
 * @return Allocated string.
 */
char* Splitter_getSplitAt(Splitter* this, int position);

/**
 * Get the multple splits string from the split possition.
 * @param this object.
 * @param startPosition of split included. Index start at zero.
 * @param endPosition of split includeed. Index start at zero.
 */
char* Splitter_getMultipleSplits(Splitter* this, unsigned int startPosition,
                                 unsigned int endPosition);

/**
 * Search the splitted string by index or count.
 * @param this object.
 * @param search string that has been split.
 * @param trueIfIndexSearch_falseIfCounting type of search.
 * @return index of the search of the count found.
 */
int Splitter_search(Splitter* this, const char* search,
                    bool trueIfIndexSearch_falseIfCounting);

#endif

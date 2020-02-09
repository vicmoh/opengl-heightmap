/**********************************************************
 * @author Vicky Mohammad
 * Copyright 2017, Vicky Mohammad, All rights reserved.
 * https://github.com/vicmoh/DynamicStringAPI
 **********************************************************/

#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Param preprocessor.
#define ARGS_SEQUENCE(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, \
                      _14, _15, _16, _17, _18, _19, _20, N, ...)              \
  N
#define ARGS(...)                                                              \
  ARGS_SEQUENCE(__VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, \
                7, 6, 5, 4, 3, 2, 1)

// ASCII range values.
#define GET_ARRAY_SIZE(array) (sizeof(array) / sizeof(*array))
#define IS_UPPERCASE_RANGE(var) (var >= 65 && var <= 90)
#define IS_LOWERCASE_RANGE(var) (var >= 97 && var <= 122)
#define IS_ALPHABET_RANGE(var) \
  ((var >= 97 && var <= 122) || (var >= 65 && var <= 90))
#define IS_NUMBER_RANGE(var) (var >= 48 && var <= 57)
#define IS_VISIBLE_RANGE(var) (var >= 33 && var <= 126)

// Function wrapper to use the variadic arguments.
#define numberToString(...) __numberToString(ARGS(__VA_ARGS__), __VA_ARGS__)

/**
 * Convert number to allocated string.
 * @param firstArg is the long double of the number to be converted.
 * @param secondArg is the int of the decimal places.
 * @return allocated string, must be free.
 */
char* __numberToString(int numOfArgs, ...);

/**
 * Check if the string is in number format.
 * @param toBeChecked.
 * @return true if it is in the format.
 */
bool isStringNumberFormat(const char* toBeChecked);

/**
 * Convert string to lower case.
 * @param toBeConverted.
 * @return allocated string. Return NULL if failed.
 */
char* stringtoLowerCase(const char* toBeConverted);

/**
 * Convert string to upper case.
 * @param toBeConverted.
 * @return allocated string. Return NULL if failed.
 */
char* stringToUpperCase(const char* toBeConverted);

/**
 * Trim the white space of the string.
 * @param toBeTrimmed.
 * @return allocated string. Return NULL if failed.
 */
char* trimString(const char* toBeTrimmed);

/**
 * Slice the string to get certain substring.
 * @param toBeSliced.
 * @param startingIndex.
 * @param endIndexToKeep.
 * @return allocated string. Return NULL if failed.
 */
char* sliceString(const char* toBeSliced, unsigned int startIndexToKeep,
                  unsigned int endIndexToKeep);

/**
 * Get the char of a string.
 * @param theString.
 * @param indexOfChar.
 * @return allocated string of the char. Return NULL if failed.
 */
char* getCharAtIndexAsString(const char* theString, int indexOfChar);

#endif

/**********************************************************
 * @author Vicky Mohammad
 * Copyright 2017, Vicky Mohammad, All rights reserved.
 * https://github.com/vicmoh/DynamicStringAPI
 **********************************************************/

#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defining some primitive and non-primitive value.
#define String char*
#define var void*
#define null NULL

/// Helper functionality.
#define $(...) __newString(__VA_ARGS__, NULL)
#define dispose(...) __multipleFree(__VA_ARGS__, NULL)
#define loop(index, start, end) for (int index = start; index <= end; index++)
#define free(val)             \
  if (val != NULL) free(val); \
  val = NULL

// Credit for lambda https://blog.noctua-software.com/c-lambda.html
#define LAMBDA(varfunction) ({ varfunction function; })

// Class preprocessor
#define OBJECT void* _this
#define THIS(object) object* this = _this
#define CONSTRUCTOR(object, param, code) \
  object* new_##object param {           \
    object* this = new (object);         \
    code return this;                    \
  }
#define CLASS(object, instance, constructor, function) \
  typedef struct {                                     \
    instance;                                          \
  } object;                                            \
  function constructor

// Ignore the function below. These are here
// so that you can use dispose() and $() function.
char* __newString(char* val, ...);
void __multipleFree(void* val, ...);

#endif

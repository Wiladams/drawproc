#pragma once
/*
* Copyright (c) 2003 Koninklijke Philips Electronics
*
* Memory allocation functions.
*
* Contents of this file are not for general export
*/

#ifndef MWSYSTEM_H_INCLUDED 
#define MWSYSTEM_H_INCLUDED

/*
* This file defines the memory allocation functions for Microwindows.
* It is useful when porting to platforms where Microwindows needs to
* use a non-standard memory allocator.
*
* This file defines macros or functions with the following prototypes:
*
* void * GdMalloc(size_t new_size);
* void * GdCalloc(size_t num, size_t size);
* void * GdRealloc(void * block, size_t old_size, size_t new_size);
* void * GdFree(void * block);
* char * GdStrDup(const char * src);
*
* The default implementation is to use the standard malloc()/free().
*
* Note the extra "old_size" parameter to GdRealloc().
*/

/* for definitions of calloc, malloc, realloc, and free */
#include <stdlib.h>

#define GdMalloc(size)                  malloc((size))
#define GdCalloc(num,size)              calloc((num),(size))
#define GdRealloc(addr,oldsize,newsize) realloc((addr),(newsize))
#define GdFree(addr)                    free((addr))
#define GdStrDup(string)                strdup((string))


#endif

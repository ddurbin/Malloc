
#ifndef malloc_malloc_h
#define malloc_malloc_h

#define malloc( x ) my_malloc( x, __FILE__ , __LINE__ )
#define free( x ) my_free( x, __FILE__ , __LINE__ )

#include	"malloc.h"
#include	<unistd.h>
#include	<stdio.h>
#include	<string.h>
#include	<errno.h>

struct MemEntry{
    unsigned int size;
    struct MemEntry *prev;
    struct MemEntry *succ;
    unsigned char isfree;
};

int recogPat = 0xFFFF;  //pattern inserted infront of MemEntry struct

/* Function called upon the creation of a new MemEntry struct and inserts
 * the recognition pattern infront of the struct
 */
void *insertPattern(void *p);

#endif

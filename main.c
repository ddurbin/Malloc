
#include "malloc.h"

//Function allocates memory of specified size on the heap.
void *my_malloc(unsigned int size, char* file, int lineNo){
    
    static struct MemEntry *        root = 0;
    static struct MemEntry *        last = 0;
    struct MemEntry *                p;
    struct MemEntry *                succ;
    
    p = root;
    while ( p != 0 ){
        if (p->size < size){
            p = p->succ;                // too small
        }
        else if (!p->isfree){
            p = p->succ;                // in use
        }
        else if (p->size < (size + sizeof(struct MemEntry))){
            p->isfree = 0;                // too small to chop up
            return (char *)p + sizeof(struct MemEntry);
        }
        else{
            
            printf("Placing in existing location\n");
            succ = (struct MemEntry *)((char *)p + sizeof(struct MemEntry) + size);
            succ->prev = p;
            succ->succ = p->succ;
            if(p->succ != 0){
                p->succ->prev = succ;
            }
            p->succ = succ;
            succ->size = p->size - sizeof(struct MemEntry) - size;
            succ->isfree = 1;
            p->size = size;
            p->isfree = 0;
            last = (p == last) ? succ : last;
            return (char *)p + sizeof(struct MemEntry);
        }
    }
    
    if ((p = (struct MemEntry *)sbrk( sizeof(struct MemEntry) + size )) == (void *)-1){
        return 0;
    }
    else if ( last == 0 ){        // first block created
        printf( "Making first chunk size %d\n", size );
        p = insertPattern(p);   //insert recognition pattern
        p->prev = 0;
        p->succ = 0;
        p->size = size;
        p->isfree = 0;
        root = last = p;
        return (char *)p + sizeof(struct MemEntry);
    }
    else{                // other blocks appended
        printf( "Making another chunk size %d\n", size );
        p = insertPattern(p);   //insert recognition pattern
        p->prev = last;
        p->succ = last->succ;
        p->size = size;
        p->isfree = 0;
        last->succ = p;
        last = p;
        return (char *)p + sizeof(struct MemEntry);
    }
    return 0;
}


//Function frees the memory of an allocated pointer or gives and error if no memory was allocated or pointer was already freed
void my_free(void * p, char* file, int lineNo){
    struct MemEntry *                ptr;
    struct MemEntry *                pred;
    struct MemEntry *                succ;
    
    if(p == NULL){
	
	printf("ERROR: in FILE: %s, LINE: %d: pointer was never allocated.\n", file, lineNo);
	return;
    }
    ptr = (struct MemEntry *)((char *)p - sizeof(struct MemEntry));
    
    void *temp = (char*)ptr - sizeof(int); //get address of recognition pattern
    int *num = (int*)temp; //store int at address
    if(*num != 0xFFFF){ //not a valid pointer to allocated memory
        printf("ERROR in FILE: %s, LINE: %d: pointer was never allocated.\n", file, lineNo);
        return;
    }
    
    if (ptr->isfree == 1){ //pointer has already been freed
        printf("ERROR in FILE: %s, LINE: %d: that pointer has already been freed.\n", file, lineNo);
        return;
    }
    if ((pred = ptr->prev) != 0 && pred->isfree){ // merge with predecessor
        pred->size += sizeof(struct MemEntry) + ptr->size;
        pred->succ = ptr->succ;
        ptr->isfree=1;
        if(ptr->succ != 0)
            ptr->succ->prev = pred;
        
        printf( "Freeing block %p merging with predecessor new size is %d.\n", p, pred->size );
    }
    else{
        printf( "Freeing block %p.\n", p );
        ptr->isfree = 1;
        pred = ptr;
    }
    if ((succ = ptr->succ) != 0 && succ->isfree){
        pred->size += sizeof(struct MemEntry) + succ->size;        // merge with successor
        pred->succ = succ->succ;
        pred->isfree = 1;
        if(succ->succ != 0){
            succ->succ->prev=pred;
        }
        printf( "Freeing block %p merging with successor new size is %d.\n", p, pred->size );
    }
}

//Funtion takes a address and places a recognition pattern (integer), then returns the address directly after the pattern.
void *insertPattern(void *p){

    int *x = (int*)p;
    *x = recogPat; //insert recognition pattern
    p += sizeof(int);
    return p;
}

int main(int argc, char **argv){
	void *ptr1  = malloc(150);
	void *ptr2 = malloc(150);
	void *ptr3 = malloc(150);
	free(ptr1);
	free(ptr3);
	char *string = "Hello";
	free(string);
    free(ptr2);
	return 0;
 
}



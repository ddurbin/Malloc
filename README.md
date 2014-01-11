Malloc
======



 The implementation uses the heap with the function call sbrk(). The free function take in a size and two hidden parameters file and lineNo for user error information. The function will allocate memory space and track it via a struct MemEntry. It first starts by finding a place in memory, if there exists one, it will return the address to the memory location, otherwise the function will increase the heap by size of struct MemEntry and needed size of allocated memory and will create a new struct MemEntry followed by returning an address to the needed memory amount.!
! The free function takes a void pointer to the address of the allocated memory that is being freed. The function then ensures that the address is a valid pointer to allocated memory and sets the isfree flag to true. It then checks the previous and next struct to determine if they are free and if they are it will combine the memory spaces.!
! The method for error checking to ensure that the passed address is to a valid pointer is implemented by a function called insertPattern(). It takes an address to the heap and then places an integer which is set to 0xFFFF in front of each struct MemEntry. So, when calling the free() function we can manipulate a temporary pointer and move it back the size of a struct MemEntry and size of an int, allowing us to compare the value at that address, if it matches then we know it is a valid address to allocated memory, otherwise it is an error.!
!

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <cstring>      //standard string operations
#include <iostream>    //standard iostream operations
#include <climits>      //numeric limits
#include <cmath>        //mathematical functions
#include <fstream>     //file input and output
#include <cctype>       //character classification
#include <ctime>        //date and time function
#include <cstdlib>      //standard libray
#include <cstdio>       //standard I/O libray
enum Status_code {SUCCESS, FAIL, UNDER_FLOW, OVER_FLOW,RANGE_ERROR, DUPLICATE_ERROR,
	NOT_PRESENT, ENTRY_INSERTED, ENTRY_FOUND};
//enum bool{false,true}; //used to turbo C++ 3.0

//#include "book.h"       //add utility functions in the book
//#include "compare.h"    //overload compare operation

#ifndef DefaultListSize
#define DefaultListSize 1000
#endif  

#endif


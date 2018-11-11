/* Ryan Kadri, Austin Su, Nyambi Acho, Utkarsha Mavani*/
#ifndef MATHFS_HEADER
	#define MATHFS_HEADER

	#define FUSE_USE_VERSION 30
	#define NUM_OPS 7
	
	/* File "type" macro for distinguishing different files*/
	#define INVALID -1
	#define PATH_DIR_HARD 0
	#define PATH_FILE_HARD 1
	#define PATH_DIR_SOFT 2
	#define PATH_FILE_SOFT 3

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <time.h>
	#include <fuse.h>
	#include <math.h> /* Use this for tricky (num ^ decimal) calculations */
	
	/* Directory names for each directory */
	static const char* paths[] = {"/add", "/sub", "/mul",
					"/div", "/exp", "/fib", "/factor"};

	/* Documentation for each operation (in corresponding order) */
	static const char* docs[] = {
		"Adds two numbers\n\tA path of the form /add/a/b evaluates to a+b\n",
		"Subtracts two numbers\n\tA path of the form /sub/a/b evaluates to a-b\n",
		"Multiplies two numbers\n\tA path of the form /mul/a/b evaluates to a*b\n",
		"Divides two numbers\n\tA path of the form div/a/b evaluates to a/b\n",
		"Raises a number to an exponent\n\tA path of the form exp/a/b evaluates to a^b\n\tCan be used with decimal exponents\n",
		"Calculates up to the nth fibonacci number.\n\tFibonacci numbers are separated by newlines\n\tInput must be a positive integer\n",
		"Breaks a number into its prime factors\n\tThe input number must be an integer\n"
	};
	
	/* The number of arguments for each operation */
	static const int args[] = {2,2,2,2,2,1,1};
	
	/* A function to parse any subdirectory of a given operation directory*/
	char* (*funcs[NUM_OPS]) (const char*);
	
	/* The number of operations that exist in the FS*/
	static int numOps = NUM_OPS;

	/* This structure ties together the different components of an operation 
	 * for organization */
	struct operation {
		char* path;
		char* doc;
		int numArgs;
		char* (*parse)(const char*);
	};
	
	/* Use a global array of operation structures to keep organized */
	struct operation myOperations[NUM_OPS];

	/* Loads corresponding values from above arrays into operations in
	 * the array. Also matches functions to the parse pointer fields */
	int loadOperations();

	/* Checks if the prefix parameter matches the first characters of input
	 * if it matches perfectly, return 0, if it does not match, return -1
	 * otherwise return the length of prefix*/
	int prefixMatch(const char* input, const char* prefix);
	
	/* Return 	PATH_DIR_HARD if path is a hardcoded directory
	 * 			PATH_FILE_HARD if path is a hardcoded file
	 * 			PATH_DIR_SOFT if path is a dynamically accessed dir (add/1/ for instance)
	 * 			PATH_FILE_SOFT if path is a dynamically accessed file (add/1/2 ie)*/
	int getFileType(const char* path);
	
	/* Returns an operation based on path or NULL if path is invalid.*/
	struct operation* getOp(const char* path);
	
	/* All of these functions take input as a string and parse it with parseOperation
	 * They then output a string containing a numerical answer or error message*/
	char* opAdd(const char* input);
	char* opSub(const char* input);
	char* opMul(const char* input);
	char* opDiv(const char* input);
	char* opFactor(const char* input);	
	char* factorThis(long input);/* Helper function for opFactor*/
	char* opExp(const char* input);	
	char* opFib(const char* input);	
		char* fibThis(long input); /* Helper function for opFib*/
	
	/* Parses the information after the main directory. Extracts number input and stores
	 * it in inInts or inDoubles. Returns the number of inputs. If numbers should
	 * be treated as doubles, doubleOrInt can be set to 1*/
	int parseOperation(const char* input, long* inInts, double* inDoubles, int* doubleOrInt);

#endif

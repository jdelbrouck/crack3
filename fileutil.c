#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileutil.h"

// DIRECTIONS
// Choose whether you are doing the 2D array or
// the array of arrays.
// For the 2D array,
//    implement loadFile2D, substringSearch2D, and free2D.
// For the array of arrays, 
//    implement loadFileAA, substringSearchAA, and freeAA.

// Global constants
const int CAP_START = 20;
const float CAP_INC = 1.5;
const int BUFFER_LEN = 1000;

//Trim a trailing newline char if found
void trim(char str[])
{
    int len = strlen(str);
    if (str[len-1] == '\n')
    {
        str[len-1] = '\0';
    }
}

// Load the text file from the given filename.
// Return a pointer to the array of strings.
// Sets the value of size to be the number of valid
// entries in the array (not the total array length).
char ** loadFileAA(char *filename, int *size)
{
	FILE *in = fopen(filename, "r");
	if (!in)
	{
	    perror("Can't open file");
	    exit(1);
	}
	
	// TODO
	// Allocate memory for an array of strings (arr).
	int cap = CAP_START;
	char ** arr = malloc(cap * sizeof(char *));
	// The size should be the number of entries in the array.
	*size = 0;
	char buffer[BUFFER_LEN];
	
	// Read the file line by line.
	while (fgets(buffer, BUFFER_LEN, in) != NULL)
	{
    	//   Trim newline.
		trim(buffer);
		//   Expand array if necessary (realloc).
		if (*size == cap)
		{
			cap = (int)(cap * CAP_INC); // I had to look up how to cast this way in C, but I'm familiar with this general technique from CSCI 13
			arr = realloc(arr, cap * sizeof(char *));
		}
		//   Allocate memory for the string (str).
		//   Copy each line into the string (use strcpy).
		//   Attach the string to the large array (assignment =).
		arr[*size] = malloc((strlen(buffer)+1) * sizeof(char)); //the order of the above instructions didnt make sense to me so I first tried these two lines
		strcpy(arr[*size], buffer);
		
		(*size)++; //increment size, ready for the next string. Had to look up syntax for incrementing the value at a dereferenced pointer in this way.
	}
	
    // Close the file.
	fclose(in);
	
	// Return pointer to the array of strings.
	return arr;
}

char (*loadFile2D(char *filename, int *size))[COLS]
{
	FILE *in = fopen(filename, "r");
	if (!in)
	{
	    perror("Can't open file");
	    exit(1);
	}
	
	// TODO
	// Allocate memory for an 2D array, using COLS as the width.
	// Read the file line by line into a buffer.
    //   Trim newline.
	//   Expand array if necessary (realloc).
	//   Copy each line from the buffer into the array (use strcpy).
    // Close the file.
	
	// The size should be the number of entries in the array.
	*size = 0;
	
	// Return pointer to the array.
	return NULL;
}

//Full String Search for Level 11
char * fullStringSearchAA(char *target, char ** arr, int size)
{
	int cur = 0;
	while (cur < size) //iterate through the array of strings
	{
		if (strcmp(target, lines[cur])) //compare the target tp the string at index[cur]
		{
			return lines[cur]; //return the first match
		}
		cur++;
	}
	return NULL; //return null if the loop completes without a match
}

// Search the array for the target string.
// Return the found string or NULL if not found.
char * substringSearchAA(char *target, char **lines, int size)
{
	int cur = 0;
	while (cur < size) //iterate through the array of strings
	{
		if (strstr(lines[cur], target)) //compare the string at index cur to the target
		{
			return lines[cur]; //return the first match
		}
		cur++;
	}
	return NULL; //return null if the loop completes without a match
}

char * substringSearch2D(char *target, char (*lines)[COLS], int size)
{
    
    return NULL;
}

// Free the memory used by the array
void freeAA(char ** arr, int size)
{
	int cur = 0;
	while (cur < size) //iterate through
	{
		free(arr[cur]); //free each string
		cur++;
	}
	free(arr); //free the array of strings
}

void free2D(char (*arr)[COLS])
{

}
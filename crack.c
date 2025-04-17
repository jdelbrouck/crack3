#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

#if __has_include("fileutil.h")
#include "fileutil.h"
#endif

#define PASS_LEN 50     // Maximum length any password will be.
#define HASH_LEN 33     // Length of hash plus one for null.

//Function Prototypes
int asciiCompare(const void *a, const void *b);
int strsearch(const void *key, const void *elem);
char ** passVariants(char *password, int *size);
void appendChar(char *str, char c);
void appendToArray(char *str, char c, char **arr, int *size);
void removeLastChar(char *str, int strLen);


//QSort Comparison Function
int asciiCompare(const void *a, const void *b)
{
    char ** aa = (char **)a;
    char ** bb = (char **)b;
    
    return strcmp(*aa, *bb);
}

//Binary Search Comparison Function
int strsearch(const void *key, const void *elem)
{
    char *a = (char *)key;
    char **b = (char **)elem; 
    
    return strcmp(a, *b);
}

//Appends a Single Character to a String
void appendChar(char *str, char c)
{
    char single[2]; //convert the single character into a string to work with string library functions
    single[0] = c;
    single[1] = '\0';

    strcat(str, single); //append
}

//Removes the last character of a string
void removeLastChar(char *str, int strLen)
{
    str[strLen-1] = '\0';
}

int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("Usage: %s hash_file dictionary_file\n", argv[0]);
        exit(1);
    }

    // TODO: Read the hashes file into an array.
    //   Use either a 2D array or an array of arrays.
    //   Use the loadFile function from fileutil.c
    //   Uncomment the appropriate statement.
    int size;
    //char (*hashes)[HASH_LEN] = loadFile(argv[1], &size);
    char **hashes = loadFileAA(argv[1], &size);
    
    // CHALLENGE1: Sort the hashes using qsort.
    qsort(hashes, size, sizeof(char *), asciiCompare);

    // TODO
    // Open the password file for reading.
    FILE *in = fopen(argv[2], "r");
    if (!in)
    {
        fprintf(stderr, "Can't open %s for reading\n", argv[2]);
        exit(1);
    }

    // TODO
    // For each password, hash it, then use the array search
    // function from fileutil.h to find the hash.
    // If you find it, display the password and the hash.
    // Keep track of how many hashes were found.
    // CHALLENGE1: Use binary search instead of linear search.
    char password[1000];
    int cracks = 0; //counter for successful cracks
    
    //First attempt at assignment; code works with challenge 1 criteria but doesn't meet challenge 2
    while (fgets(password, 1000, in) != NULL)
    {
        trim(password); //trim the password
        char *hash = md5(password, strlen(password)); //hash the password
        if (bsearch(hash, hashes, size, sizeof(char *), strsearch)) //if binary search returns true
        {
            printf("%s %s\n", hash, password); //print hash and password
            cracks++;
        }
        free(hash);//free the memory
    }

    //Attempt at Challenge 2, worked at it for a while but ran out of time
    //Probably just made the password variant function too complex
    /*
    while (fgets(password, 1000, in) != NULL)
    {
        trim(password); //trim the password
        
        int listSize = 0;
        char **pList = passVariants(password, &listSize); //challenge 2 variant generating function

        int pIndex = 0;
        while (pList[pIndex])
        {
            char *hash = md5(pList[pIndex], strlen(pList[pIndex])); //hash the password
            if (bsearch(hash, hashes, size, sizeof(char *), strsearch)) //if binary search returns true
            {
                printf("%s %s\n", hash, pList[pIndex]); //print hash and password
                cracks++;
            }
            free(hash);//free the memory for the hash
            pIndex++;
        }

        freeAA(pList, listSize); //free the memory of the variant list for this password
    }*/

    // TODO
    // When done with the file:
    //   Close the file
    fclose(in);
    //   Display the number of hashes found.
    printf("Hashes Cracked: %d\n", cracks);
    //   Free up memory.
    freeAA(hashes, size);
}

//Couldn't get working
/*
//Function to Generate an array of likely password variations
//Should catch and variations with 1 or 2 common trailing characters, chosen from digits and common special characters
//Intended to crack common passwords modified to meet complexity requirements
//Could clean this up quite a bit by using larger sub-functions to handle some of the repeated code, but I've already spent a while on this
//Sorry if it is hard to read
char ** passVariants(char *password, int *size)
{
    char **passList = malloc(3000 * sizeof(char *));

    passList[0] = malloc(100 * sizeof(char *));
    strcpy(passList[0], password);
    *size += 1;
    
    char curVar[1000]; //String to store current variant
    strcpy(curVar, password); //copy password to current variant

    //Variants with at least a trailing digit
    for (int i = 48; i < 58; i++) //iterate through ascii values of first trailing digit
    {
        appendToArray(curVar, i, passList, size); //append character i and add this variant to the list

        //iterate through ascii values of a second trailing digit
        for(int j = 48; j < 58; j++)
        {
            appendToArray(curVar, j, passList, size); //append character j and add this variant to the list
            removeLastChar(curVar, strlen(curVar)); //remove character j from the current variant
        }
        
        //iterate through ascii values of some common special characters
        for(int j = 33; j < 48; j++)
        {
            appendToArray(curVar, j, passList, size); //append character j and add this variant to the list
            removeLastChar(curVar, strlen(curVar)); //remove character j from the current variant
        }

        //iterate through ascii values of more common special characters
        for(int j = 58; j < 65; j++)
        {
            appendToArray(curVar, j, passList, size); //append character j and add this variant to the list
            removeLastChar(curVar, strlen(curVar)); //remove character j from the current variant
        }

        removeLastChar(curVar, strlen(curVar)); //remove character i from the current variant
    }

    //Variants with at least a trailing special character (group 1)
    for (int i = 33; i < 48; i++) //iterate through ascii values of first trailing digit
    {
        appendToArray(curVar, i, passList, size); //append character i and add this variant to the list

        //iterate through ascii values of a trailing digit
        for(int j = 48; j < 58; j++)
        {
            appendToArray(curVar, j, passList, size); //append character j and add this variant to the list
            removeLastChar(curVar, strlen(curVar)); //remove character j from the current variant
        }
        
        //iterate through ascii values for a second common special character (group 1)
        for(int j = 33; j < 48; j++)
        {
            appendToArray(curVar, j, passList, size); //append character j and add this variant to the list
            removeLastChar(curVar, strlen(curVar)); //remove character j from the current variant
        }

        //iterate through ascii values of more common special characters
        for(int j = 58; j < 65; j++)
        {
            appendToArray(curVar, j, passList, size); //append character j and add this variant to the list
            removeLastChar(curVar, strlen(curVar)); //remove character j from the current variant
        }

        removeLastChar(curVar, strlen(curVar)); //remove character i from the current variant
    }

    //Variants with at least a trailing special character (group 2)
    for (int i = 58; i < 65; i++) //iterate through ascii values of first trailing digit
    {
        appendToArray(curVar, i, passList, size); //append character i and add this variant to the list

        //iterate through ascii values of a second trailing digit
        for(int j = 48; j < 58; j++)
        {
            appendToArray(curVar, j, passList, size); //append character j and add this variant to the list
            removeLastChar(curVar, strlen(curVar)); //remove character j from the current variant
        }
        
        //iterate through ascii values of some common special characters
        for(int j = 33; j < 48; j++)
        {
            appendToArray(curVar, j, passList, size); //append character j and add this variant to the list
            removeLastChar(curVar, strlen(curVar)); //remove character j from the current variant
        }

        //iterate through ascii values of more common special characters
        for(int j = 58; j < 65; j++)
        {
            appendToArray(curVar, j, passList, size); //append character j and add this variant to the list
            removeLastChar(curVar, strlen(curVar)); //remove character j from the current variant
        }

        removeLastChar(curVar, strlen(curVar)); //remove character i from the current variant
    }
    
    return passList; //returns the address of the list generated
}

//Appends a Character to a String, adds that string to an array, increments size of the array
void appendToArray(char *str, char c, char **arr, int *size)
{
    appendChar(str, c);
    arr[*size] = malloc(100 * sizeof(char *));
    strcpy(arr[*size], str);
    *size += 1;
}*/
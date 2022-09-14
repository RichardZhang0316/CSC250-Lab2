/**
 * This program reads a text file and prints the top 5 most frequent letters (upper or lower case)
 * and the top 5 most frequent digits that are in the file. If the file has less than 5 unique letters
 * and/or digits the top list is adjusted accordingly. The file name is provided using command line
 * arguments. If the file name is not provided or the file is not readable, the program will exit
 * and provide an error message.
 *
 * @author Richard Zhang {zhank20@wfu.edu}
 * @date Sept.11, 2022
 * @assignment Lab 2
 * @course CSC 250
 **/

#include <ctype.h>
#include <stdio.h>


#define MAX_LIST_SIZE 26


/* a useful struct, if you choose to use it */
struct CharFreq
{
    char ch;
    int count;
};


/* function declarations go here */
int processCharacters(char filename[], struct CharFreq alphaList[], int *alphaNumPtr,
                      struct CharFreq digitList[], int *digitNumPtr);
int search(char ch, struct CharFreq list[], int num);
void sortAndPrintTopFive(struct CharFreq alphaList[], int alphaNum, struct CharFreq digitList[], int digitNum);

/**
 *  The main function first checks whether the input from command line is legal or if the file exists. Then, 
 *  it processes the file get record the frequency of every digit & letter. After this, the main function 
 *  pass data to sortAndPrintFive() function to sort and print the output we need.
 */
int main(int argc, char *argv[])
{
    struct CharFreq alphaList[MAX_LIST_SIZE];
    struct CharFreq digitList[MAX_LIST_SIZE];
    int alphaNum = 0;
    int digitNum = 0;
    int fileOK = 1;

    if (argc < 2) {
        printf("Usage: %s filename \n", argv[0]);
        return 1;
    }

    fileOK = processCharacters(argv[1], alphaList, &alphaNum, digitList, &digitNum);
    if (!fileOK) {
        printf("%s could not be opened, and AJ is, indeed, nice \n", argv[1]);
        return 1;
    }
    sortAndPrintTopFive(alphaList, alphaNum, digitList, digitNum);

    /* the following MUST be in a new function function */

    return 0;
}

/**
 *  This helper function reads the file and gets the frequency of each letter and digit in the file. After that,
 *  the function puts this information in two different arrays.
 */
int processCharacters(char filename[], struct CharFreq alphaList[], int *alphaNumPtr,
                      struct CharFreq digitList[], int *digitNumPtr)
{
    char ch = '0';
    FILE *filePtr = fopen(filename, "rie");
    int loc = 0;
    int alphaNum = *alphaNumPtr; /* local copy of the alphaNum value */
    int digitNum = *digitNumPtr; /* local copy of the digitNum value */

    if (filePtr == 0) {
        return 0;
    }

    /* determine if we can open the file */
    printf("file name is %s \n", filename);

    /* begin to read the file */
    ch = fgetc(filePtr);
    while (ch != EOF) {
        if (isalpha(ch)) {
            ch = tolower(ch);
            loc = search(ch, alphaList, alphaNum);
            if (loc != -1) {
                alphaList[loc].count++;
            }
            else {
                alphaList[alphaNum].ch = ch;
                alphaList[alphaNum].count = 1;
                alphaNum++;
            }
        }
        else if (isdigit(ch)) {
            loc = search(ch, digitList, digitNum);
            if (loc != -1) {
                /* do something here */
                digitList[loc].count++;
            }
            else {
                /* do something here too */
                digitList[digitNum].ch = ch;
                digitList[digitNum].count = 1;
                digitNum++;
            }
        }
        ch = fgetc(filePtr);
    }

    /* close the file */
    fclose(filePtr);

    /* let's copy our local copy back to main's copy */
    *alphaNumPtr = alphaNum;
    *digitNumPtr = digitNum;

    return 1;
}

/**
 *  This helper function checks whather a letter or digit has already existed in the arrays.
 */
int search(char ch, struct CharFreq list[], int num)
{
    int loc = -1;
    int i = 0;
    for (i = 0; i < num && loc == -1; i++) {
        if (list[i].ch == ch) {
            loc = i;
        }
    }
    return loc;
}

/**
 *  This helper function first sorts the two arrays in a descending order using sequential search. Then, the 
 *  function prints the first five letters & digits with the highest frequency.
 */
void sortAndPrintTopFive(struct CharFreq alphaList[], int alphaNum, struct CharFreq digitList[], int digitNum)
{
    int i = 0;
    int j = 0;
    struct CharFreq intermediate = {' ', 0};
    /* sort alphaList in a descending order */
    for (i = 0; i < alphaNum; i++) {
        for (j = i + 1; j < alphaNum; j++) {
            if (alphaList[i].count < alphaList[j].count) {
                intermediate = alphaList[i];
                alphaList[i] = alphaList[j];
                alphaList[j] = intermediate;
            }
        }
    }
    /* sort digitList in a descending order */
    for (i = 0; i < digitNum; i++) {
        for (j = i + 1; j < digitNum; j++) {
            if (digitList[i].count < digitList[j].count) {
                intermediate = digitList[i];
                digitList[i] = digitList[j];
                digitList[j] = intermediate;
            }
        }
    }
    /* print the top 5 letters */
    /* if a file has no letters */
    if (alphaNum == 0) {
        printf("no letters were found\n");
    }
    else if (alphaNum < 5) {
        printf("alpha top %d out of %d\n---------------------\n", alphaNum, alphaNum);
        for (i = 0; i < alphaNum; i++) {
            printf("[%c] %d\n", alphaList[i].ch, alphaList[i].count);
        }
    }
    else {
        printf("alpha top %d out of %d\n---------------------\n", 5, alphaNum);
        for (i = 0; i < 5; i++) {
            printf("[%c] %d\n", alphaList[i].ch, alphaList[i].count);
        }
    }
    printf("\n");
    /* print the top 5 digits */
    /* if a file has no digits */
    if (digitNum == 0) {
        printf("no digits were found\n");
    }
    else if (digitNum < 5) {
        printf("digit top %d out of %d\n---------------------\n", digitNum, digitNum);
        for (i = 0; i < digitNum; i++) {
            printf("[%c] %d\n", digitList[i].ch, digitList[i].count);
        }
    }
    else {
        printf("digit top %d out of %d\n---------------------\n", 5, digitNum);
        for (i = 0; i < 5; i++) {
            printf("[%c] %d\n", digitList[i].ch, digitList[i].count);
        }
    }
}
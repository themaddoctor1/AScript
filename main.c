#include "main.h"
#include "srccompile.h"

#include <errno.h>
#include <string.h>

char* getCodeBlock(FILE* file) {
    return stringUpTo(file, '}', '{', '}');
}

char* getNextLine(FILE* file) {
    return stringUpTo(file, ';', '{', '}');
}

char* stringUpTo(FILE* file, char c, char up, char down) {

    int i = 0, j = 0;
    char* line = (char*) malloc(sizeof(char)); //Creates empty string.
    char* swp;
    char nextChar;
    
    int level = 0;

    *line = fgetc(file);

    if(*line == EOF) return line;
    else while(*line == ' ') *line = fgetc(file);
    
    /**
     * Iterates until the end of the file or until the terminator
     * has been reached, provided that closure has been reached.
     * A compilable program will have closure on every line.
     */
    while(line[i] != EOF && (line[i] != c || level)) {
        i++;
        //Read char
        nextChar = fgetc(file);

        if(nextChar == up)
            level++;
        if(nextChar == down)
            level--;

        //Create new memory slot, and put line into it
        swp = (char*) malloc(i+1 * sizeof(char));
        j = 0;
        while(j < i) {
            swp[j] = line[j];
            j++;
        }
        swp[i] = nextChar;
        
        //Reallocate memory and do freeing. 
        if(line) free(line);
        line = swp;
    }

    line[i] = '\0'; //Terminates the string.
    
    return line;

}

int main(int argc, char* argv[]) {
    
    //If no file is provided, immediately exit w/ an error message.
    if(argc < 2) {
        printf("Error during compilation: No input file specified.\n");
        return EINVAL;
    }
    
    //The name of the file
    char* FILENAME = argv[1];
    char* fileext = FILENAME;
    while(*fileext && *fileext != '.')
        fileext = &fileext[1];
    
    if(strcmp(fileext, ".scr")) {
        printf("Error during compilation: File extension must be .scr\n");
        return EINVAL;
    }

    //The source file
    FILE* sourceFile = fopen(FILENAME, "r");
    
    //Checks to see if the file exists, and returns an error
    //if the file does not exist.
    if(sourceFile) {
        printf("Compiling %s...\n", FILENAME);
    } else {
        printf("Error during compilation: Input file not found.\n");
        return ENOENT;
    }

    //Create a swap file for writing
    printf("Opening swapspace0...\n");
    FILE* swapFile0 = fopen(".swapspace0.dta", "w+");
    
    //Moves the chars to the new file, without the newlines.
    printf("Morphing source into swapspace0...\n");
    char* nextLine;
    while(*(nextLine = stringUpTo(sourceFile, '\n', '\0', '\0')) != EOF) {
        //Gets the length of the String.
        int i = -1;
        while(nextLine[++i]);
        
        //printf("%s\n", nextLine);
        fwrite(nextLine, 1, i, swapFile0);

        //Needs to be freed
        free(nextLine);
    }

    
    printf("Closing source and swapspace0...\n");
    //At this point, .swapspace0.swp contains 
    //the source without newlines.
    fclose(sourceFile); //Source file is no longer needed
    fclose(swapFile0);
    
    printf("Opening swapspace0 for reading...\n");

    //Reopen the swap file, only this time for reading.
    swapFile0 = fopen(".swapspace0.dta", "r");

    printf("Opening asm file for writing...\n");

    //Create new stack frame file and execution file.
    //Hold stack frame data and execution instructions.
    FILE* execdata = fopen("program.asm", "w");
    
    /*
     * Iterates through each line one by one. The use of parseSegment()
     * allows for lines containing code segments to be broken down.
     */
    int i = 0;
    nextLine = getNextLine(swapFile0);
    while(*nextLine != '\0' && *nextLine != EOF) {
        if(i++ > 10) break;

        //Parse the current line.
        parseSegment(execdata, nextLine);
        
        //Gets the next line
        nextLine = getNextLine(swapFile0);
    }
    
    printf("Finalizing...\n");

    fclose(swapFile0);

    //Final cleanup
    remove(".swapspace0.dta");

    fclose(execdata);

}


#ifndef FUNCTIONS_C
#define FUNCTIONS_C

#include "functions.h"

inline const char*  getOption(const char * name, const char * defaut,
								int argc, char ** argv)
{
    const char *result = 0;

    int k = 0;
    // compare all of the arguments to the name until its found
    while (k < argc && strcmp(argv[k],name)) ++k;
    
    // returning the default if name wasn't found
    // but if k = argc return the last argv element, or just argv of k
    if(k++ == argc)
        result = defaut;
    else if(k == argc)
        result = argv[--k];
    else result = argv[k];

    return result;
}

char getFileData(char pattern,char *buf, FILE *f)
{
    char c;
    int start = ftell(f);

    // counting number of chars from start point to the searching pattern
    while(c = fgetc(f), c != pattern && c!= '\n' && c != EOF);
    
    // save the pattern position in file
    int end = ftell(f);
    // if its end of file, the difference between start and end will be 0
    // so 1 is added to get the last char of the file
    if(c == EOF) end += 1;
    // set the file pointer on start position
    fseek(f, start, SEEK_SET);
    // get all chars to buf from start to end
    fgets(buf, end-start, f);
    // set the file pointer on found pattern
    fseek(f, end, SEEK_SET);

    // return last checked char of file
    return c;
}

#endif
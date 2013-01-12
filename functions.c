#ifndef FUNCTIONS_C
#define FUNCTIONS_C

#include "functions.h"

inline const char*  getOption(const char * name, const char * defaut,
								int argc, char ** argv) {
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

char getFileData(char pattern,char *buf, FILE *f) {
    char c;
    int start = ftell(f);

    // counting number of chars from start point to the searching pattern
    while(c = fgetc(f), c != pattern && c != '\t' && c!= '\n' && c != EOF);
    
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

int isType(char* buf){
    return (!strcmp(buf, "int") || !strcmp(buf, "char")
                || !strcmp(buf, "short") || !strcmp(buf, "float")
                || !strcmp(buf, "double") || !strcmp(buf, "long")
                || !strcmp(buf, "signed") || !strcmp(buf, "unsigned")
                || !strcmp(buf, "void"));
}

int isVariable(char* buf, FILE* f_i){

    char c;
    int start = ftell(f_i);
    while(c = fgetc(f_i), c != ';'){
       if(c == '{') break; 
    }

    fseek(f_i, start, SEEK_SET);
    if(c != '{') return true;
    else {
        fseek(f_i, start - strlen(buf) -1, SEEK_SET);
        return false;
    }
}

struct variables* getVaribles(char* buf, struct variables* var_head, 
    FILE* f_i){

    struct variables* vars_global_head = var_head;
    char *tmp = malloc(84*sizeof(char));
    if(tmp == NULL) {
        free(tmp);
        return NULL;
    }

    char c = getFileData(',', tmp, f_i);
    do{
        fseek(f_i, ftell(f_i)-1, SEEK_SET);
        c = fgetc(f_i); fgetc(f_i);

        while(tmp[strlen(tmp)-1] == ','
           || tmp[strlen(tmp)-1] == ' '
           || tmp[strlen(tmp)-1] == ';'
           || tmp[strlen(tmp)-1] == '\n')
            tmp[strlen(tmp)-1] = 0;

        struct variables *element = malloc(sizeof(struct variables));
        if(element == NULL) {
            free(tmp);
            free(element);
            return NULL;
        }
        element->typ = malloc(strlen(buf)*sizeof(char));
        strcpy(element->typ, buf);
        element->name = malloc(strlen(tmp)*sizeof(char));
        strcpy(element->name, tmp);
        element->next = vars_global_head;
        vars_global_head = element;

        if(c == ',') getFileData(' ', tmp, f_i);
        else fseek(f_i, ftell(f_i)-1, SEEK_SET);
    }while(c == ',');

    free(tmp);
    return vars_global_head;
}

struct constants* getConstants(char* buf, struct constants* const_head,
    FILE* f_i){

    char c, j;
    char *tmp = malloc(84*sizeof(char));
    if(tmp == NULL) {
        free(tmp);
        return NULL;
    }

    getFileData(' ', tmp, f_i);

    //------------------------------------------------------------------
    struct constants *element = malloc(sizeof(struct constants));
    element->name = malloc(strlen(tmp)*sizeof(char));
    strcpy(element->name, tmp);
    element->typ = NULL;
    element->next = const_head;
    const_head = element;
    //------------------------------------------------------------------

    j = fgetc(f_i);
    while(c = fgetc(f_i), true){
        if(j != '/' && c == '\n') break;
        j = c;
    }

    return const_head;
}
#endif
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

struct constants {
    char* name;
    char* typ;
    struct constants *next;
};

struct variables {
    char* name;
    char* typ;
    struct variables *next;
};

struct types {
    char* name;
    struct variables* v;
    struct types* next;
};

struct functions {
    struct constants* c;
    struct variables* v;
    struct types* t;
    struct functions* next;
};

struct procedures {
    struct constants* c;
    struct variables* v;
    struct types* t;
    struct procedures* next;
};

inline const char*  getOption(const char * name, const char * defaut,
                                int argc, char ** argv);
char getFileData(char patern, char *buf, FILE *f);
int isType(char* buf);
int isVariable(char* buf, FILE* f_i);
struct variables* getVaribles(char* buf, struct variables* var_head, 
    FILE* f_i);
#endif

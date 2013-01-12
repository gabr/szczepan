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
    struct types* next;
};

struct functions {
    char* name;
    struct constants* c;
    struct variables* v;
    struct types* t;
    struct functions* next;
};


inline const char*  getOption(const char * name, const char * defaut,
                                int argc, char ** argv);
char getFileData(char patern, char *buf, FILE *f);
int isType(char* buf);
int isVariable(char* buf, FILE* f_i);
struct variables* getVaribles(char* buf, struct variables* var_head, 
    FILE* f_i);
struct constants* getConstants(char* buf, struct constants* const_head,
    FILE* f_i);
struct types* getTypes(char* buf, struct types* types_global_head,
    FILE* f_i);
struct functions* getFunction(char* buf,
    struct functions* functions_global_head, FILE* f_i);
void freeMemory(char* buf_a, char* buf_b, FILE* f_a, FILE* f_b,
    struct constants* s_c, struct variables* s_v,
    struct types* s_t, struct functions* s_f,
    struct functions* s_p);

#endif

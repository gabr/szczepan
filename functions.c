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

    free(tmp);
    return const_head;
}

struct types* getTypes(char* buf, struct types* types_global_head,
    FILE* f_i){

    char c, j;
    char *tmp = malloc(104*sizeof(char));
    if(tmp == NULL) {
        free(tmp);
        return NULL;
    }

    getFileData(' ', tmp, f_i);

    //------------------------------------------------------------------
    struct types *element = malloc(sizeof(struct types));
    element->name = malloc(strlen(tmp)*sizeof(char));
    strcpy(element->name, tmp);
    element->next = types_global_head;
    types_global_head = element;
    //------------------------------------------------------------------

    j = fgetc(f_i);
    while(c = fgetc(f_i), true){
        if(j == '}' && c == ';') break;
        j = c;
    }

    free(tmp);
    return types_global_head;
}

struct functions* getFunction(char* buf,
    struct functions* functions_global_head, FILE* f_i){

    char c;
    char *tmp = malloc(104*sizeof(char));
    if(tmp == NULL) {
        free(tmp);
        return NULL;
    }
    getFileData('{', buf, f_i);

    while(buf[strlen(buf)-1] != ')')
        buf[strlen(tmp-1)] = 0;

    while(fgetc(f_i) != ')')
        fseek(f_i, ftell(f_i)-2, SEEK_SET);

    int func_start = ftell(f_i);
    int i = 0;
    do{
        while(c = fgetc(f_i), c != '}' && c != '{');
        if(c == '{') ++i;
        if(c == '}') --i;
    }while(i != 0);
    int func_end = ftell(f_i);
    fseek(f_i, func_start, SEEK_SET);

    struct functions *func_tmp = malloc(sizeof(struct functions));
    func_tmp->name = malloc(strlen(buf)*sizeof(char));
    strcpy(func_tmp->name, buf);
    func_tmp->t = NULL;
    func_tmp->c = NULL;

    while(getFileData(' ', buf, f_i), ftell(f_i) < func_end){
        if(!strcmp(buf, "const")){
            getFileData(' ', buf, f_i);
            if(isType(buf)){
                func_tmp->c = (struct constatns*) getVaribles(buf,
                    (struct variables*) func_tmp->c, f_i);
                if(func_tmp->c == NULL){
                    free(tmp);
                    return NULL;
                }
            }

        }
        else if(!strcmp(buf, "struct")){
            func_tmp->t = getTypes(buf, func_tmp->t, f_i);
            if(func_tmp->t == NULL){
                free(tmp);
                return NULL;
            } 
        }
        else if(isType(buf)){
            func_tmp->v = getVaribles(buf, func_tmp->v, f_i);
        }
    }

    func_tmp->next = functions_global_head;
    functions_global_head = func_tmp;


    free(tmp);
    return functions_global_head;
}

void freeMemory(char* buf_a, char* buf_b, FILE* f_a, FILE* f_b,
    struct constants* s_c, struct variables* s_v,
    struct types* s_t, struct functions* s_f,
    struct functions* s_p){

    struct constants* tmp_c;
    struct variables* tmp_v;
    struct types* tmp_t;
    struct functions* tmp_f;


    if(buf_a != NULL) free(buf_a);
    if(buf_b != NULL) free(buf_b);
    if(f_a != NULL) fclose(f_a);
    if(f_b != NULL) fclose(f_b);

    if(s_c != NULL){
        while(s_c != NULL){
            tmp_c = s_c;
            s_c = s_c->next;
            free(tmp_c);
        }
    }

    if(s_v != NULL){
        while(s_v != NULL){
            tmp_v = s_v;
            s_v = s_v->next;
            free(tmp_v);
        }
    }

    if(s_t != NULL){
        while(s_t != NULL){
            tmp_t = s_t;
            s_t = s_t->next;
            free(tmp_t);
        }
    }

}


#endif
#include "functions.h"

int main(int argc, char const *argv[]) {
    const char *help_options = "\n\tDocumentation\n \toptions: \n\t-u optional:\n\t-o <output file> // by default output.txt \n\n";


    if(argc < 2) {
        printf("%s", help_options);
        return 1;
    }                    

    const char * _i = getOption("-i",(char*)0, argc, argv);
    const char * _o = getOption("-o","output.txt", argc, argv);

    if ((char*)0 == _i || ('-' == _i[0] || '-' == _o[0])) {
        printf("%s", help_options);
        return 1;
    }

    FILE *f_i;
    f_i = fopen(_i, "r");
    if(f_i == NULL) {
        printf("! error: file %s not found!\n", _i);
        return 1;
    }

    char *buf = malloc(255*sizeof(char));
    char *tmp = malloc(255*sizeof(char));
    if(buf == NULL || tmp == NULL) {
        printf("! error: not enough memory!\n");
        freeMemory(buf, tmp, f_i, NULL, NULL, NULL, NULL, NULL, NULL);
        return 1;
    }


    struct constants *consts_global_head = NULL;
    struct types *types_global_head = NULL;
    struct variables *vars_global_head = NULL;
    struct functions *functions_global_head = NULL;
    struct functions *procedures_global_head = NULL;

    char c;
    while(c = getFileData(' ', buf, f_i), c != EOF){
        if(!strcmp(buf, "#define")){ 
            consts_global_head = getConstants(buf, consts_global_head, f_i);
            if(consts_global_head == NULL){
                printf("! error: not enough memory!\n");
                freeMemory(buf, tmp, f_i, NULL, consts_global_head,
                    vars_global_head, types_global_head,
                    functions_global_head, procedures_global_head);
                return 1;
            }
        }
        else if (!strcmp(buf, "struct")) {
            types_global_head = getTypes(buf, types_global_head, f_i);
            if(types_global_head == NULL){
                printf("! error: not enough memory!\n");
                freeMemory(buf, tmp, f_i, NULL, consts_global_head,
                    vars_global_head, types_global_head,
                    functions_global_head, procedures_global_head);
                return 1;
            }
         }
        else if(isType(buf)){ 
            if(isVariable(buf, f_i)){
                vars_global_head = getVaribles(buf, vars_global_head, f_i);
                if(vars_global_head == NULL){
                printf("! error: not enough memory!\n");
                freeMemory(buf, tmp, f_i, NULL, consts_global_head,
                    vars_global_head, types_global_head,
                    functions_global_head, procedures_global_head);
                    return 1;
                }
            } else {
                functions_global_head = getFunction(buf,
                    functions_global_head, f_i);
                if(functions_global_head == NULL){
                    printf("! error: not enough memory!\n");
                    freeMemory(buf, tmp, f_i, NULL, consts_global_head,
                        vars_global_head, types_global_head,
                        functions_global_head, procedures_global_head);
                    return 1;
                }
                
            }
        }
    }

    FILE *f_o;
    f_o = fopen(_o, "w");

    while(consts_global_head != NULL) {
        printf("stała: %s\n", consts_global_head->name);
        consts_global_head = consts_global_head->next;
    }

    while(types_global_head != NULL) {
        printf("typ: %s\n", types_global_head->name);
        types_global_head = types_global_head->next;

    }

    printf("ZMIENNE:\n");
    while(vars_global_head != NULL) {
        printf("typ: %s ", vars_global_head->typ);
        printf("nazwa: %s", vars_global_head->name);
        vars_global_head = vars_global_head->next;
        printf("\n");
    }

    printf("FUNKCJE:\n");
    while(functions_global_head != NULL) {
        printf("%s\n", functions_global_head->name);

        if(functions_global_head->c == NULL)
            printf("\tlack of constats\n");
        else {
            printf("\tconstants:");
            while(functions_global_head->c != NULL){
                printf("\n\t%s: %s\n", functions_global_head->c->name, 
                    functions_global_head->c->typ);
                functions_global_head->c = functions_global_head->c->next;
            }
        }

        if(functions_global_head->v == NULL)
            printf("\tlack of variables\n");
        else {
            printf("\tvariables:");
            while(functions_global_head->v != NULL){
                printf("\n\t%s: %s\n", functions_global_head->v->name, 
                    functions_global_head->v->typ);
                functions_global_head->v = functions_global_head->v->next;
            }
        }

        if(functions_global_head->t == NULL)
            printf("\ttypes are not defined");
        else {
            printf("\tconstants:");
            while(functions_global_head->t != NULL){
                functions_global_head->t = functions_global_head->t->next;
            }
        }

        functions_global_head = functions_global_head->next;
        printf("\n");
    }

    freeMemory(buf, tmp, f_i, f_o, consts_global_head,
                    vars_global_head, types_global_head,
                    functions_global_head, procedures_global_head);;
    return 0;
}

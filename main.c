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
        free(buf);
        free(tmp);
        fclose(f_i);
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
                free(buf);
                free(tmp);
                fclose(f_i);
                return 1;
            }
        }
        else if (!strcmp(buf, "struct")) {
            getFileData(' ', tmp, f_i);

            //------------------------------------------------------------------
            struct types *element = malloc(sizeof(struct types));
            element->name = malloc(strlen(tmp)*sizeof(char));
            strcpy(element->name, tmp);
            element->v = NULL;
            element->next = types_global_head;
            types_global_head = element;
            //------------------------------------------------------------------

            char j = fgetc(f_i);
            while(c = fgetc(f_i), true){
                if(j == '}' && c == ';') break;
                j = c;
            }
        }
        else if(isType(buf)){ 
            if(isVariable(buf, f_i)){
                vars_global_head = getVaribles(buf, vars_global_head, f_i);
                if(vars_global_head == NULL){
                    printf("! error: not enough memory!\n");
                    free(buf);
                    free(tmp);
                    fclose(f_i);
                    return 1;
                }
            } else {

                getFileData('{', buf, f_i);

                while(buf[strlen(buf)-1] != ')')
                    buf[strlen(tmp-1)] = 0;

                while(fgetc(f_i) != ')')
                    fseek(f_i, ftell(f_i)-2, SEEK_SET);

                int func_start = ftell(f_i);
                while(fgetc(f_i) != '}');
                int func_end = ftell(f_i);
                fseek(f_i, func_start, SEEK_SET);

                struct functions *func_tmp = malloc(sizeof(struct functions));
                func_tmp->name = malloc(strlen(buf)*sizeof(char));
                strcpy(func_tmp->name, buf);

                while(getFileData(' ', buf, f_i), ftell(f_i) < func_end){
                    if(isType(buf)){
                        func_tmp->v = getVaribles(buf, func_tmp->v, f_i);
                    }
                }

                func_tmp->c = NULL;
                func_tmp->t = NULL;
                func_tmp->next = functions_global_head;
                functions_global_head = func_tmp;
            }
        }
    }

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
            printf("\tlack of types");
        else {
            printf("\tconstants:");
            while(functions_global_head->t != NULL){
                functions_global_head->t = functions_global_head->t->next;
            }
        }

        functions_global_head = functions_global_head->next;
        printf("\n");
    }

    free(buf);
    free(tmp);
    fclose(f_i);
    return 0;
}

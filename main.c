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
    char c;
    while(c = getFileData(' ', buf, f_i), c != EOF){
        if(!strcmp(buf, "#define")){ 
            getFileData(' ', tmp, f_i);

            //------------------------------------------------------------------
            struct constants *element = malloc(sizeof(struct constants));
            element->name = malloc(strlen(tmp)*sizeof(char));
            strcpy(element->name, tmp);
            element->typ = NULL;
            element->next = consts_global_head;
            consts_global_head = element;
            //------------------------------------------------------------------

            char j = fgetc(f_i);
            while(c = fgetc(f_i), true){
                if(j != '/' && c == '\n') break;
                j = c;
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
                printf("\nFUNKCJA:\n");
                while(c = fgetc(f_i), c != '}'){
                    fseek(f_i, ftell(f_i)-1, SEEK_SET);
                    getFileData('{', buf, f_i);
                    while(buf[strlen(buf)-1] != ')')
                        buf[strlen(tmp-1)] = 0;
                    fseek(f_i, ftell(f_i)-1, SEEK_SET);
                    while(fgetc(f_i) != ')')
                        fseek(f_i, ftell(f_i)-2, SEEK_SET);

                    printf("%s", buf);
                    break;
                }
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

    free(buf);
    fclose(f_i);
    return 0;
}

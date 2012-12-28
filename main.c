#include "functions.h"

int main(int argc, char const *argv[])
{
    const char *help_options = "\n\tDocumentation\n \toptions: \n\t-u optional:\n\t-o <output file> // by default output.txt \n\n";


    if(argc < 2)
    {
        printf("%s", help_options);
        return 1;
    }                    

    const char * _i = getOption("-i",(char*)0, argc, argv);
    const char * _o = getOption("-o","output.txt", argc, argv);

    if ((char*)0 == _i || ('-' == _i[0] || '-' == _o[0]))
    {
        printf("%s", help_options);
        return 1;
    }

    FILE *f_i;
    f_i = fopen(_i, "r");
    if(f_i == NULL)
    {
        printf("! error: file %s not found!\n", _i);
        return 1;
    }

    char *buf = malloc(255*sizeof(char));
    if(buf == NULL)
    {
        printf("! error: not enough memory!\n");
        free(buf);
        fclose(f_i);
        return 1;
    }

    struct constants *globa_head = NULL;
    char c;
    while(c = getFileData(' ', buf, f_i), c != EOF){
        if(!strcmp(buf, "#define")){
            char *tmp = malloc(255*sizeof(char));
            getFileData(' ', tmp, f_i);

            //------------------------------------------------------------------
            struct constants *element = malloc(sizeof(struct constants));
            if(globa_head != NULL){

            }


            //------------------------------------------------------------------

            char j = fgetc(f_i);
            while(c = fgetc(f_i), true){
                if(j != '/' && c == '\n') break;
            }
        }
    }



    free(buf);
    fclose(f_i);
    return 0;
}

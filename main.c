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
    
    return 0;
}
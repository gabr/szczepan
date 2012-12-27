#include "functions.h"

int main(int argc, char const *argv[])
{

	const char * _i = getOption("-i",(char*)0, argc, argv);
    const char * _o = getOption("-o","output.txt", argc, argv);
	
	return 0;
}
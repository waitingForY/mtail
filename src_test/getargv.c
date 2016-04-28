#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
	char *path=argv[1];
	printf("%s\n",path);
	return 0;
}

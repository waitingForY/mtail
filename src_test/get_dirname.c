#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(void)
{
	char *path="test/testwjb*";
	char dirname[100];
	memset(dirname,0,100);
	int path_len=strlen(path);
	int i;
	for(i=0;i<path_len;i++)
	{
		dirname[i]=path[i];
		if(path[i]=='/')
		  break;
	}
	dirname[++i]='\0';
	char filename[100];
	memset(filename,0,100);
	int j=0;
	for(;i<path_len-1;i++)
	{
		filename[j++]=path[i];
	}
	filename[j]='\0';
	printf("%s\n",dirname);
	printf("%s\n",filename);
	return 0;
}

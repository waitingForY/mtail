#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int substr(char *dest,char *src)
{
	if(dest==NULL||src==NULL)
	  return 0;
	if(strlen(dest)>strlen(src))
	  return 0;
	int i;
	for(i=0;i<strlen(dest);i++)
	{
		if(dest[i]!=src[i])
		  break;
	}
	if(i<strlen(dest))
	  return 0;
	return 1;
}
int main(void)
{

	char *src="wo shi wj";
	char *dest="wo shi wjb";
	int is=substr(dest,src);
	printf("%d\n",is);
	return 0;
}

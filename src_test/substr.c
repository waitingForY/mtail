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
	int i=strlen(dest)-1;
	int j=strlen(src)-1;
	while(i>=0)
	{
		if(dest[i]!=src[j])
		  return 0;
		i--;
		j--;
	}
	return 1;
}
int main(void)
{

	char *src="/home/wjb/test/test.x";
	char *dest="test.x";
	int is=substr(dest,src);
	printf("%d\n",is);
	return 0;
}

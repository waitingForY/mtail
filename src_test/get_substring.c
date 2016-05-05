#include <stdio.h>
#include <string.h>

int main(void)
{
	char *path="/home/wjb/test/test.txt";
	char dirname[100];
	char filename[100];
	memset(dirname,0,100);
	memset(filename,0,100);
	int i;
	for(i=strlen(path)-1;i>=0;i--)
	{
		if(path[i]=='/')
		{
			strncpy(dirname,path,i+1);
			dirname[i+1]='\0';
			break;
		}
	}
	//printf("i=%d,strlen(dirname)=%d",i,strlen(dirname));
	strncpy(filename,path+strlen(dirname),strlen(path)-strlen(dirname));
	int n=strlen(filename);
	printf("%s\n",dirname);
	printf("%s,len =%d\n",filename,n);
	return 0;
}

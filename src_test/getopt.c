#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



char *get_cur_time()
{
	char time_str[100];
	memset(time_str,0,100);
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo=localtime(&rawtime);
	int len=strlen(asctime(timeinfo));
	strncpy(time_str,asctime(timeinfo),len);
	time_str[len-1]='\0';
	return asctime(timeinfo);
}

int main(int argc,char **argv)
{
	int opt;
	char head[512];
	memset(head,0,512);
	strcpy(head,"[");
	char *time_p=NULL;
	while((opt=getopt(argc,argv,"nt"))!=-1)
	{
		switch (opt)
		{
			case 'n':
				//printf("aaaa\n");
				time_p=get_cur_time();
				strcat(head,time_p);
				break;
			case 't':
				//printf("bbbb\n");
				strcat(head,"/home/wjb/src/");
				break;
			//default:
			//	fprintf(stderr,"Usage: %s [-n] [-t] name\n",argv[0]);
			//	exit(EXIT_FAILURE);
		}
	}
	strcat(head,"]");
	printf("%s\n",head);
	return 0;
}

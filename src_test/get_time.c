#include <stdio.h>
#include <time.h>

int main(void)
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo=localtime(&rawtime);
	char *curtime=asctime(timeinfo);
	printf("%s\n",curtime);
	return 0;
}

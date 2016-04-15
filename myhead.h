#ifndef _MYHEAD_H
#define _MYHEAD_H
#include <sys/types.h>

//define struct fileNode 
typedef struct fileNode
{
	off_t filesize;
	char *filename;
	off_t lastposition;
}FILENODE;


#endif //_MYHEAD_H

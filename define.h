#ifndef _DEFINE_H
#define _DEFINE_H
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct fileNode
{
	off_t filesize; /*file current size*/
	char *filename; /*flie name contain fullpath*/
	off_t lastposition; /*last read positions */
	struct fileNode *next; /*point the next file node*/
}FILENODE;

#define ERR_EXIT(m) do{perror(m);exit(EXIT_FAILURE);}while(0)
#define HAS_OPT 1
#define NO_OPT 0
#define OPT_POS_BETWEEN 0
#define OPT_POS_TAIL 1
#define MAX_STR_SIZE 512

#endif /*_DEFINE_H */

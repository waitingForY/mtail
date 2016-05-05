#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <glob.h>
#include "define.h"

#define PATH 1
#define COMMON 2
FILENODE *filelist=NULL;
char *myname;/*to stor the exe_name*/

/*
 *
 *globerr ---print error message for glob()
 * 
 */
int globerr(const char *path,int eerrno)
{
	fprintf(stderr,"%s:%s:%s\n",myname,path,strerror(eerrno));
	return 0;
}

/*
 *get_path ---expand command-line wildcards 
 */
glob_t get_path(int argc,char **argv)
{
	int i;
	int flags=0;
	glob_t results;
	int ret;
	if(argc == 1)
	{
		fprintf(stderr,"Usage: %s [-n] path ...\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	myname=argv[0];
	for(i=1;i<argc;i++)
	{
		flags |= (i > 1 ? GLOB_APPEND : 0);
		ret=glob(argv[i],flags,globerr,&results);
		if(ret!=0)
		{
			fprintf(stderr, "%s: problem with %s (%s),stopping early\n",
						myname,argv[i],
						(ret==GLOB_ABORTED?"filesystem problem":
						 ret==GLOB_NOMATCH?"no match of pattern":
						 ret==GLOB_NOSPACE?"no dynamic memory":
						 "unknown problem"));
			break;
		}
	}
	return results;
}



int path_paser(char *path)
{
	int path_len=strlen(path);
	if(path[path_len-1]=='/')
	{
		return PATH;
	}
	else if(path[path_len-1]=='*')
	{
		return COMMON;
	}
	else
	  return 0;

}


void init_filelist(FILENODE *head)
{
	head->filesize=0;
	head->filename=NULL;
	head->lastposition=0;
	head->next=NULL;
}

void filelist_clear(FILENODE *head)
{
	
}


void insert_filelist(FILENODE *node)
{
	if(filelist)
	{
		node->next=filelist;
		filelist=node;
	}
	else
	  filelist=node;
}



/*
 *
 *caculate the file size
 */

off_t file_size(char *filename)
{
	struct stat statbuf;
	if(stat(filename,&statbuf)==-1)
	  ERR_EXIT("stat");
	off_t size=statbuf.st_size;
	return size;

}


/*
 *tail file function
 *
 */
void tail_file_addhead(char *filename,off_t lastposition,off_t filesize)
{
	FILE *fstream;
	fstream=fopen(filename,"r");
	assert(fstream);

	int n;
	for(n=lastposition;n<filesize;)
	{
		/*char *head=(char *)malloc(sizeof(filename)+3);*/
		char head[100];
		memset(head,0,sizeof(head));
		strcpy(head,"[");
		strcat(head,filename);
		strcat(head,"] ");
		fflush(fstream);
		fseek(fstream,lastposition,SEEK_SET);
		char buf[1024];
		memset(buf,0,sizeof(buf));
		fgets(buf,sizeof(buf),fstream);
		char output[2048];
		memset(output,0,sizeof(output));
		strcpy(output,head);
		strcat(output,buf);
		lastposition=ftell(fstream);
		printf("%s",output);
		/*
		free(head);
		head=NULL;
		*/
		fflush(stdout);
		n+=strlen(buf);
		if(lastposition==filesize)
		{
			fclose(fstream);
			return;
		}
	}
	fclose(fstream);
	return;
}

void tail_file_nohead(char *filename,off_t lastposition,off_t filesize)
{
	FILE *fstream;
	fstream=fopen(filename,"r");
	assert(fstream);
	int n;
	for(n=lastposition;n<filesize;)
	{
		char output[2048];
		memset(output,0,sizeof(output));
		fflush(fstream);
		fseek(fstream,lastposition,SEEK_SET);
		fgets(output,sizeof(output),fstream);
		lastposition=ftell(fstream);
		printf("%s",output);
		fflush(stdout);
		n+=strlen(output);
		if(lastposition==filesize)
		{
			fclose(fstream);
			return;
		}
	}
	fclose(fstream);
	return;
}
int is_substr(char *src,char *dest)
{
	if(src==NULL||dest==NULL)
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

void do_tail_common(int argc,char **argv,char *path,int opt)
{
	/*
	 *save the dirname in variable dirname,and save filename in variable filename_common;
	 */
	char dirname[100];
	int path_len=strlen(path);
	int i;
	glob_t results=get_path(argc,argv);
	
	for(i=path_len-1;i>=0;i--)
	{
		//dirname[i]=path[i];
		if(path[i]=='/')
		{
			int j=0;
			while(j<=i)
			{
				dirname[j]=path[j];
				j++;
			}
			dirname[j]='\0';
			break;
		}
	}
	if(i==0)
	{
		printf("can not open the dir,please input the real name of dir!\n");
		exit(EXIT_FAILURE);
	}
	/*
	dirname[++i]='\0';
	char filename_common[100];
	memset(filename_common,0,100);
	int j=0;
	for(;i<path_len-1;i++)
	{
		filename_common[j++]=path[i];
	}
	filename_common[j]='\0';
	*/

	/*
	 *then cicle the whole file in the dir;
	 */

	int initlist_flag=1;
	while(1)
	{
		struct dirent *file;
		DIR *dir;
		if(!(dir=opendir(dirname)))
		  ERR_EXIT("opendir");
		/*
		 *if open the dir at the first time then init the filelist;
		 */

		if(initlist_flag)
		{
			initlist_flag=0;
			while((file=readdir(dir))!=NULL)
			{
				char *name=file->d_name;
				if(name[0]=='.')
				  continue;
				int i=0;
				while(i<results.gl_pathc)
				{
					char filename_common[100];
					memset(filename_common,0,100);
					int len=strlen(results.gl_pathv[i]);
					int dir_len=strlen(dirname);
					int j=0;
					while(j<len-dir_len)
					{
						filename_common[j]=results.gl_pathv[i][j+dir_len];
						j++;
					}
					filename_common[j]='\0';
					if(is_substr(name,filename_common))
					{
						FILENODE *new_file_node=(FILENODE *)malloc(sizeof(FILENODE));
						char *name=file->d_name;
						char *file_path_name=(char *)malloc(strlen(dirname)+strlen(name)+2);
						strcpy(file_path_name,dirname);
						strcat(file_path_name,name);
						off_t size=file_size(file_path_name);
						new_file_node->filesize=size;
						new_file_node->lastposition=size;
						new_file_node->filename=file_path_name;
						new_file_node->next=NULL;
						insert_filelist(new_file_node);
						break;
					}
					i++;
				}
			}
		}
		else
		{
			while((file=readdir(dir))!=NULL)
			{

				char *name=file->d_name;
				if(name[0]=='.')
				  continue;
				char *file_path_name=(char *)malloc(strlen(dirname)+strlen(name)+2);
				strcpy(file_path_name,dirname);
				strcat(file_path_name,name);
				off_t size=file_size(file_path_name);
				FILENODE *it=filelist;
				while(it)
				{
					if(strcmp((*it).filename,file_path_name)==0)
					{

						if((*it).filesize<size)
						{
							(*it).filesize=size;
							if(opt==1)
							{
								tail_file_addhead((*it).filename,(*it).lastposition,(*it).filesize);
							}
							if(opt==0)
							{
								tail_file_nohead((*it).filename,(*it).lastposition,(*it).filesize);
							}
							(*it).lastposition=size;
						}
						else if((*it).filesize>size)
						{
							(*it).filesize=size;
							(*it).lastposition=size;
						}
						break;
					}
					it=it->next;
				}
				/*
				 *if not found file int the list then insert the file info
				 */
				/*
				if(it==NULL)
				{
					FILENODE *node=(FILENODE *)malloc(sizeof(FILENODE));
					node->filesize=size;
					node->lastposition=size;
					node->filename=file_path_name;
					node->next=NULL;
					insert_filelist(node);
				}
				*/
			}

		}
		closedir(dir);
	}
}
void do_tail(char *path,int opt)
{
	int initlist_flag=1;
	while(1)
	{
		struct dirent *file;
		DIR *dir;
		if(!(dir=opendir(path)))
		  ERR_EXIT("opendir");
		/*
		 *if open the diretory at the first time then init the filelist
		 */

		if(initlist_flag)
		{
			initlist_flag=0;
			while((file=readdir(dir))!=NULL)
			{
				FILENODE *new_file_node=(FILENODE *)malloc(sizeof(FILENODE));
				char *name=file->d_name;
				if(name[0]=='.')
				{
					continue;
				}
				char *file_path_name=(char *)malloc(strlen(path)+strlen(name)+2);
				strcpy(file_path_name,path);
				strcat(file_path_name,name);
				off_t size=file_size(file_path_name);
				new_file_node->filesize=size;
				new_file_node->lastposition=size;
				new_file_node->filename=file_path_name;
				new_file_node->next=NULL;
				insert_filelist(new_file_node);
			}
		}

		/*
		 *
		 *if not the first to open the dir the jadge
		 */
		else
		{
			while((file=readdir(dir))!=NULL)
			{

				char *name=file->d_name;
				if(name[0]=='.')
				  continue;
				char *file_path_name=(char *)malloc(strlen(path)+strlen(name)+2);
				strcpy(file_path_name,path);
				strcat(file_path_name,name);
				off_t size=file_size(file_path_name);
				FILENODE *it=filelist;
				while(it)
				{
					if(strcmp((*it).filename,file_path_name)==0)
					{
						
						if((*it).filesize<size)
						{
							(*it).filesize=size;
							if(opt==1)
							{
								tail_file_addhead((*it).filename,(*it).lastposition,(*it).filesize);
							}
							if(opt==0)
							{
								tail_file_nohead((*it).filename,(*it).lastposition,(*it).filesize);
							}
							(*it).lastposition=size;
						}
						else if((*it).filesize>size)
						{
							(*it).filesize=size;
							(*it).lastposition=size;
						}
						break;
					}
					it=it->next;
				}
				/*
				 *
				 *if not found file int the list then insert the file info
				 */
				if(it==NULL)
				{
					FILENODE *node=(FILENODE *)malloc(sizeof(FILENODE));
					node->filesize=size;
					node->lastposition=size;
					node->filename=file_path_name;
					node->next=NULL;
					insert_filelist(node);
				}

			}

		}
		closedir(dir);
	}
	
}





int main(int argc,char **argv)
{
	
	
	if(argc<=1)
	{
		printf("argument is wrong!\n");
		printf("Usage:./tail [-n] dirpath/!\n");
		exit(EXIT_FAILURE);
	}
	
    if(argc<=2)
	{
		char *path=argv[1];
		//char *path="/home/wjb/src/apps/tail/test/test*";
		//char *array[2]={"/home/wjb/github/tail/tail","/home/wjb/src/apps/tail/test/test*"};
		if(path_paser(path)==PATH)
		  do_tail(path,0);
		else if(path_paser(path)==COMMON)
		  do_tail_common(argc,argv,path,0);
		else
		{
			printf("not a dir or common symbol!\n");
			exit(EXIT_FAILURE);
		}

	}
	else
	{
		char *path=NULL;
		if(argv[argc-1][0]=='-')
		  path=argv[1];
		else
		  path=argv[argc-1];
		int opt;
		while((opt=getopt(argc,argv,"n"))!=-1)
		{
			switch(opt)
			{
				case 'n':
					if(path_paser(path)==PATH)
					{
						do_tail(path,1);
					}
					else if(path_paser(path)==COMMON)
					{
						do_tail_common(argc,argv,path,1);
					}
					else
					{
						printf("not a dir or common symble!\n");
						exit(EXIT_FAILURE);
					}
					break;
					/*
				case 't':
					printf("hai mei shi xian!\n");
					break;
					*/
				default:
					fprintf(stderr,"Usage:%s [-n] dirname\n",argv[0]);
					exit(EXIT_FAILURE);
			}
		}
	}

	/*
	int opt;
	while((opt=getopt(argc,argv,"nt"))!=-1)
	{
		switch (opt)
		{
			case 'n':
				do_tail(path);
				break;
			case 't':
				printf("hai mei shixian !\n");
				break;
			default:
				fprintf(stderr,"Usage:%s [-t] [-n] dirname\n",argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	*/

	return 0;
}

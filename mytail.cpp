#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <vector>
#include <assert.h>
#include <fstream>
#include "myhead.h"

using namespace std;

//struct fileNode filelist[MAX_SIZE];

vector<FILENODE> filelist;

/*
 *caculate the file size 
 *return type must be off_t which define in <sys/types.h>
 */
off_t file_size(char *filename)
{
	struct stat statbuf;
	if(stat(filename,&statbuf)==-1)
	{
		cout<<"stat is wrong!"<<endl;
		exit(EXIT_FAILURE);
	}
	off_t size=statbuf.st_size;
	return size;
}



/*
 *find last line function
 *
 */

/*
int find_last_line(ifstream &infile)
{
	infile.seekg(0,ios::end);
	int filesize=infile.tellg();
	for(int i=1;i<filesize;i++)
	{
		infile.seekg(filesize-i-1,ios::beg);
		char c;
		infile.get(c);

		if(c==0x0A)
		  return infile.tellg();
	}
}
*/

/*
 *do_tail()
 *
 */

void do_tail(char *filename,off_t lastposition,off_t filesize)
{
	ifstream filein(filename);
	assert(filein);

	for(int n=lastposition;n<filesize;n++)
	{
		filein.clear();
		cout.clear();
		filein.seekg(lastposition,ios::beg);
		char output[1024];
		memset(output,0,sizeof(output));
		filein.getline(output,sizeof(output));
		lastposition=filein.tellg();
		cout<<output<<endl;
		cout.flush();
		if(lastposition==filesize)
		{
			filein.close();
			return;
		}

	}

}


/*
 *main() function
 */

int main(int argc,char **argv)
{

	if(argc==1)
	{
		cout<<"arguement is wrong!"<<endl;
		cout<<"use:  ./mytail dir/"<<endl;
		cout<<"and the arguement must be a dirtory,and must end of '/' !"<<endl;
		exit(EXIT_FAILURE);
	}
	
	char *path=argv[1];
	
	//char *path="test/";
	filelist.clear();

	bool initlist=true;//标志：用于判断是否是第一此打开目录
	
	//cicle the directory using while(1)
	while(1)
	{
		struct dirent *file;
		DIR *d;//opendir handle
		if(!(d=opendir(path)))
		{
			cout<<"open "<<path<<" error!"<<endl;
			exit(EXIT_FAILURE);
		}

		/*
		 *if open the diretory at the first time the init the filelist
		 */
		if(initlist)
		{
			initlist=false;
			while((file=readdir(d))!=NULL)
			{
				FILENODE *node=new FILENODE;
				char *name=file->d_name;
				if(name[0]=='.')
				  continue;
				char *file_path_name=(char *)malloc(strlen(path)+strlen(name)+2);
				strcpy(file_path_name,path);
				strcat(file_path_name,name);
				off_t size=file_size(file_path_name);
				node->filesize=size;
				node->lastposition=size;
				node->filename=file_path_name;
				filelist.push_back(*node);
				delete node;
			}
		}

		/*
		 *if not then jadge the eatch file current size with its old size
		 *and the do_tail(),at last update the lastposition
		 *如果不是第一次打开就比较文件列表中所有文件的当前大小与历史大小，
		 *
		 */
		else
		{
			while((file=readdir(d))!=NULL)
			{
				char *name=file->d_name;
				if(name[0]=='.')
				  continue;
				char *file_path_name=(char *)malloc(strlen(path)+strlen(name)+2);
				strcpy(file_path_name,path);
				strcat(file_path_name,name);
				off_t size=file_size(file_path_name);
				vector<FILENODE>::iterator it;
				for(it=filelist.begin();it!=filelist.end();++it)
				{
					if(strcmp((*it).filename,file_path_name)==0)
					{
						//if current size is biger than old....
						if((*it).filesize<size)
						{
							(*it).filesize=size;
							do_tail((*it).filename,(*it).lastposition,(*it).filesize);
							(*it).lastposition=size;
						}

						//if currentsize is smaller than old...
						else if((*it).filesize>size)
						{
							(*it).filesize=size;
							(*it).lastposition=size;
						}
						break;
					}
				}


				//not found file in the list then insert the file info
				//也就是说如果有新文件，那么就件新文件的信息插入filelist中
				if(it==filelist.end())
				{
					FILENODE *node=new FILENODE;
					node->filesize=size;
					node->lastposition=size;
					node->filename=file_path_name;
					filelist.push_back(*node);
					delete node;
				}

			}

		}
		closedir(d);
	}

	
}

/*
int main(int argc,char **argv)
{
	else
	{
		int last_position=-1;
		for(;;)
		{
			ifstream infile(argv[1]);
			int position=find_last_line(infile);		
			if(position>last_position)
			{
				infile.seekg(position,ios::beg);
				string in;
				getline(infile,in);
				cout<<in<<endl;
			}
			last_position=position;	
		}
	}
		return 0;
}
*/

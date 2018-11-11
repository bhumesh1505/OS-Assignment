#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main(int arg,char *argu[])
{
	int fd1,fd2,dis;
	char buf;
	char *file1=argu[1];

	if(arg<2)
	{
		printf("plzz enter only one arguments(one file name):\n");
	}
	else
	{
		fd1=open(file1,O_RDONLY);
		fd2=open(file1,O_RDWR);
		if(fd1==-1)
		{
			printf("file open error or file does not exit!!\n");			
		}
		else
		{
			int count=-1;
			int ch=0;
			while((read(fd1,&buf,1))>0)
			{
				count++;
				if(count%2==0)
				{	
					write(fd2,&buf,1);
					ch=ch+1;
				}
			}
			
			ftruncate(fd2,ch);
		}
		close(fd1);
		close(fd2);
	}
return 0;
}

#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main( int argc,char *argv[] )
{
	int fd,i;
	char buf;
		
	fd=open(argv[1],O_RDONLY);

	if(fd==-argc)
	{
		printf("file open error");
	}
	else
	{
		while((i=read(fd,&buf,1))>0)
		{
			write(1,&buf,1);
		}
		close(fd);
	}
}


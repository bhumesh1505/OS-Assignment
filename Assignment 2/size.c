#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
	int f1;
	char *src = argv[1];

	f1 = open(src, O_RDONLY);
	int size = lseek(f1, 0, SEEK_END);

	struct stat filedata;	
	stat(src, &filedata);

	printf("Size %d bytes\n", size);
	printf("Standard Block Size = %d bits\n",(int) filedata.st_blksize);

	int block_count = size * 8 / filedata.st_blksize;
	if( ( size * 8 ) % filedata.st_blksize  != 0)
	{
		block_count++; 
	}
	printf("No. of blocks are %d\n", block_count);

	close(f1);
	return 0;
}

#include <stdio.h>
#include <string.h>


int main() {

	FILE *fptr;  
	char ch,file[50];
	
	printf("Enter file name \n");
	scanf("%s",file);
	
	if( fptr = fopen( file , "r") )
	{
		int i = 0;
		while( (ch = getc(fptr)) != EOF ) 
		{
			if( i % 2 == 0)
			{
				printf("%c", ch);
			}
			i++;
		}
		fclose(fptr);
	}
	else
	{
		printf("Failed to open file\n");
	}
	return 0;
}

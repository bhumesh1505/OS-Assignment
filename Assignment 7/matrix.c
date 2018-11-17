#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include <sys/time.h>
#include<stdint.h>


#define SIZE 30
#define MT 30

int a[SIZE][SIZE],b[SIZE][SIZE];
long c[SIZE][SIZE];
int m , n , p , MAXTHREADS;
void read();
static void print(int start , int end,int tid);
void *multiply(void *arg);



long getMicrotime()
{
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return (currentTime.tv_sec * (int)1e6 + currentTime.tv_usec);
}

void multiply1()
{
	

  int  c, d, k, sum = 0;
  long s;
  int multiply[10][10];
	
    s = getMicrotime();

    for (c = 0; c < m; c++) {
      for (d = 0; d < p; d++) {
        for (k = 0; k < n; k++) {
          sum = sum + a[c][k]*b[k][d];
        }
 
        multiply[c][d] = sum;
        sum = 0;
      }
    }
 
    printf("Product of the matrices by simple method:\n");
 
    for (c = 0; c < m; c++) {
      for (d = 0; d < p; d++)
        printf("%d\t", multiply[c][d]);
 
      printf("\n");
    }
  
  long e = getMicrotime();
  printf("Total time taken is %ld microseconds\n",e-s );
}

pthread_t pt[MT];

int main()
{
	void *exit_status;
	long start , end ;
	float diff;
	int i,rp,j;
	printf("reading the values \n");
	read();
	printf("multiplying the matrices\n");
	start = getMicrotime();
	//printf("start time %ld \n",start );
	for(i = 0;i<MAXTHREADS;i++)
	{
		if(rp = pthread_create(&pt[i],NULL,*multiply,(void *)i))
		{
			printf("thread creation failed\n");
		}
	}
	
	for ( j = 0; j < MAXTHREADS; j++)
	{
		//printf("hey \n");
		pthread_join(pt[j],&exit_status);
		//printf("hey2 \n");
	}
	
	end = getMicrotime();
	//printf("Final time %ld\n",end );
	printf("TOTAL TIME TAKEN IS %ld microseconds\n",end-start);
	printf("Product of the matrices:\n");
	for ( i = 0; i < m; ++i)
	{
		for(j=0; j<p ;j++)
		{
			printf(" %ld\t",c[i][j] );
		}
		printf("\n");
	}
	multiply1();
	return 0;
}

void read()
{
	
	printf("Enter m and n for matrix A\n");
	scanf("%d%d",&m,&n);
	MAXTHREADS =  m ;
	printf("Enter matrix A \n");
	int i,j;
	for ( i = 0; i < m; ++i)
	{
		for ( j = 0; j< n; ++j)
		{
			scanf("%d",&a[i][j]);
		}
	}
	printf("Enter dimension p for matrix B\n");
	scanf("%d",&p);
	printf("Enter matrix B \n");
	for ( i = 0; i < n; ++i)
	{
		for ( j = 0; j< p; ++j)
		{
			scanf("%d",&b[i][j]);
		}
	}
	for ( i = 0; i < m; ++i)
	{
		for ( j = 0; j< p; ++j)
		{
			c[i][j] = 0;
		}
	}
}

void *multiply(void *arg)
{
	struct timeval t0, t1, dt;

    

	int i,j,k,l;
	l = (int) arg;
	int rpt,start,end ;
	
	i = l;
	start = i;
	end = start + 1 ;
	printf("THREAD %d CREATED \n", l);
	gettimeofday(&t0, NULL);
	long s =getMicrotime();
	for ( j = 0; j< p; ++j)
	{
		for(k=0;k< n;k++)
		{
			c[i][j] += a[i][k]*b[k][j];
		}
	}

	long e = getMicrotime();

	printf("Total time taken by a thread is %ld microseconds\n",e-s );

	print(start,end,l);
	return NULL;
}

void print(int start , int end ,int tid)
{
	int i,j;
	printf("THREAD %d PRINTING THE VALUES\n", tid);
	for ( i = start; i < end; ++i)
	{
		for(j=0; j< p;j++)
		{
			printf("c[%d][%d] : %ld\t",i,j,c[i][j] );
		}
	}
	printf("\n");
	printf("THREAD %d PRINTED THE VALUES\n", tid);
	
}



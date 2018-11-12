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

uint64_t GetTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}

long getMicrotime()
	{
		struct timeval currentTime;
		gettimeofday(&currentTime, NULL);
		return (currentTime.tv_sec * (int)1e6 + currentTime.tv_usec);
	}

void multiply1()
{
	

  int m, n, p, q, c, d, k, sum = 0;
  long s;
  int first[10][10], second[10][10], multiply[10][10];
 
  printf("Enter number of rows and columns of first matrix\n");
  scanf("%d%d", &m, &n);
  printf("Enter elements of first matrix\n");
 
  for (c = 0; c < m; c++)
    for (d = 0; d < n; d++)
      scanf("%d", &first[c][d]);
 
  printf("Enter number of rows and columns of second matrix\n");
  scanf("%d%d", &p, &q);
 
  if (n != p)
    printf("The matrices can't be multiplied with each other.\n");
  else
  {
    printf("Enter elements of second matrix\n");
 
    for (c = 0; c < p; c++)
      for (d = 0; d < q; d++)
        scanf("%d", &second[c][d]);
 	
    s = getMicrotime();

    for (c = 0; c < m; c++) {
      for (d = 0; d < q; d++) {
        for (k = 0; k < p; k++) {
          sum = sum + first[c][k]*second[k][d];
        }
 
        multiply[c][d] = sum;
        sum = 0;
      }
    }
 
    printf("Product of the matrices:\n");
 
    for (c = 0; c < m; c++) {
      for (d = 0; d < q; d++)
        printf("%d\t", multiply[c][d]);
 
      printf("\n");
    }
  }
  long e = getMicrotime();
  printf("Total time taken is %ld\n",e-s );
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
			printf("thread creation failed\n",i);
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
			printf(" %d\t",c[i][j] );
		}
		printf("\n");
	}
	multiply1();
	return 0;
}

void read()
{
	/*int i,j;
	for ( i = 0; i < SIZE; ++i)
	{
		for ( j = 0; j < SIZE; ++j)
		{
			a[i][j]=(i+2)/2;
			b[i][j]=(i+4)/2;
			c[i][j]=0;
		}
	}*/
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
	rpt = SIZE/MAXTHREADS;
	start = l;
	end = start + 1 ;
	printf("THREAD %d CREATED \n", l);
	gettimeofday(&t0, NULL);
	long s =getMicrotime();
	for ( i = start; i < end; ++i)
	{
		for ( j = 0; j< p; ++j)
		{
			for(k=0;k< n;k++)
			{
				c[i][j] += a[i][k]*b[k][j];
			}
		}
	}
	long e = getMicrotime();

	gettimeofday(&t1, NULL);

    timersub(&t1, &t0, &dt);

    printf("(thread %ld) took %d.%06d sec\n",
               (long)l, dt.tv_sec, dt.tv_usec);
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
			printf("c[%d][%d] : %d\t",i,j,c[i][j] );
		}
	}
	printf("\n");
	printf("THREAD %d PRINTED THE VALUES\n", tid);
	
}



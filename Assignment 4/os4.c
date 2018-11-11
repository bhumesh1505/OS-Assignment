#include<stdio.h>
#include<stdlib.h>

int logical_address;			// monotonically increasing
int totalFreeSpace;				

typedef struct List_PM				// free list in physical memory but can be used to represent allocatedlist of any given block
{
	int start;		// starting address
	int end;		// ending address
	struct List_PM* next;
}List_PM;
typedef struct LA_List		// list of blocks allocated in logical address
{
	int start;					// starting of logical address
	int size;				// size of block allocated
	List_PM* alloc_ptr;		// pointer to actual physical memory space
	struct LA_List* next;
}LA_List;

List_PM* initialize(int n);
List_PM* make_List_PM_Node(int start,int end);
LA_List* make_LA_List_Node(int x,List_PM** freeList);
List_PM* alloc_In_PM(int x,List_PM** freeList);						// allocate memory in physical memory
LA_List* alloc(int x,LA_List* la,List_PM** freeList);
List_PM* insert_In_Free_List(List_PM* freeList,int start,int end);
LA_List* Free(int logical_add,LA_List* la,List_PM** freeList);
void print_Logical_List(LA_List* la);
void print_Free_List(List_PM* freeList);


int main()
{
	int i,n,x,done=0,logical_add,select;
	LA_List* la=NULL;
	printf("Enter Total MEMORY\n");
	scanf("%d",&n);
	List_PM* freeList=initialize(n);
	while(!done)
	{
		printf("\t1.ALLOCATE\n");
		printf("\t2.FREE\n");
		printf("\t3.PRINT\n");
		printf("\t-1.Exit\n");
		scanf("%d",&select);
		switch(select)
		{
			case 1:
			{
				printf("Enter the number byte to be allocated:\n");
				scanf("%d",&x);
				la=alloc(x,la,&freeList);
				print_Logical_List(la);
				print_Free_List(freeList);
				break;
			}
			case 2:
			{
				printf("Enter the logical address that needs to be freed:\n");
				scanf("%d",&logical_add);
				la=Free(logical_add,la,&freeList);
				print_Logical_List(la);
				print_Free_List(freeList);
				break;
			}
			case 3:
			{
				print_Logical_List(la);
				print_Free_List(freeList);
				break;
			}
			case -1:
			{
				done = 1;
				break;
			}
			default:
			{
				printf("\n enter valid entry\n");
				break;
			}
		}
	}
}

List_PM* initialize(int n)
{
	printf("Initializing free list\n");
	logical_address = 0;
	totalFreeSpace = n;
	List_PM* lp=make_List_PM_Node(0,n-1);
	return lp;
}

List_PM* make_List_PM_Node(int start,int end)				// start and end are indices
{
	List_PM *lp = (List_PM *)malloc(sizeof(List_PM));
	lp->start = start;
	lp->end = end;
	lp->next = NULL;
	return lp;
}

LA_List* make_LA_List_Node(int x,List_PM** freeList)
{
	LA_List *ll = (LA_List *)malloc(sizeof(LA_List));
	ll->start = logical_address;
	ll->size = x;
	ll->alloc_ptr = alloc_In_PM(x, freeList);
	ll->next = NULL;
	return ll;
}

LA_List* alloc(int x,LA_List* la,List_PM** freeList)
{

	LA_List *retval = NULL, *la_ptr;
	if( x <= totalFreeSpace )
	{
		totalFreeSpace = totalFreeSpace - x;
		la_ptr = la;
		if(la_ptr == NULL)
		{
			retval = make_LA_List_Node(x, freeList);
		}
		else
		{
			retval = la;
			while((la_ptr->next) != NULL)
			{
				la_ptr = la_ptr->next;
			}
			la_ptr->next = make_LA_List_Node(x, freeList);
		}
		logical_address = logical_address + x;
	}
	else
	{
		printf("Insufficient Memory\n\n");
	}
	return retval;
}

List_PM* alloc_In_PM(int x,List_PM** freeList)
{
	List_PM *ptr = *freeList, *ret_ptr = NULL, *allocated_ptr = NULL;
	int start,end,done = 0;

	printf("\nALLOCATING MEM IN PHYSICAL MEMORY\n");
	List_PM *temp;
	while(ptr != NULL && done == 0)
	{
		start = ptr->start;
		end = ptr->end;

		if((end-start + 1) > x)			// we have to allocate a part of that block (split it)
		{								// + 1 bcoz start and end are indices, 0 to 99 indicates size of 100
			List_PM *p = make_List_PM_Node(start, start + x - 1);
			if(allocated_ptr == NULL)
			{
				allocated_ptr = p;
				ret_ptr = allocated_ptr;
			}
			else		// means this is not the starting pointer for that block to allocate
			{
				allocated_ptr->next = p;
				allocated_ptr = allocated_ptr->next;
			}
			ptr->start = ptr->start + x;
			done = 1;
		}
		else	// need to remove this node from free list as it is now completely used
		{
			List_PM *p = make_List_PM_Node(start, end);
			if(allocated_ptr == NULL)
			{
				allocated_ptr = p;
				ret_ptr = allocated_ptr;
			}
			else		// means this is not the starting pointer for that block to allocate
			{
				allocated_ptr->next = p;
				allocated_ptr = allocated_ptr->next;
			}

			temp = ptr;
			ptr = ptr->next;
			*freeList = ptr;
			free(temp);

			x = x - (end - start + 1);			// bracket is size of that block and start and end are indices
			if(x == 0)
			{
				done = 1;
			}
		}
	}
	return ret_ptr;
}

LA_List* Free(int logical_add,LA_List* la,List_PM** freeList)
{
	LA_List *ptr = la, *prev, *ret_ptr = la, *temp;
	int found = 0, list_start = 1, start, end;
	List_PM * lp;
	while(ptr != NULL && found == 0)
	{
		// print_Logical_List(la);
		if(ptr->start == logical_add)
		{
			lp = ptr->alloc_ptr;

			if(list_start == 1)
			{
				temp=ptr;
				ptr=ptr->next;
				free(temp);
				ret_ptr=ptr;
				list_start = 0;
			}
			else
			{
				temp=ptr;
				ptr=ptr->next;
				prev->next=ptr;
				free(temp);
			}
			// print_Logical_List(la);
			while(lp != NULL)
			{
				*freeList = insert_In_Free_List(*freeList, lp->start, lp->end);
				lp = lp->next;
			}

			found = 1;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	if(found == 0)
	{
		printf("\nINVALID ENTRY OF STARTING ADDRESS\n");
	}
	else
	{
		totalFreeSpace = totalFreeSpace + logical_add;
	}
	return ret_ptr;
}

List_PM* insert_In_Free_List(List_PM* freeList,int start,int end)
{
	printf("\nINSERTING IN FREE LIST\n");
	List_PM *ptr = freeList, *prev = NULL, *p, *ret_ptr = freeList;

	p = make_List_PM_Node(start, end);

	while(ptr != NULL && start > ptr->start)//find proper position to insert in freeList
	{
		prev = ptr;
		ptr = ptr->next;
	}
	if(prev == NULL) // insert before head of free list
	{
		if(ptr == NULL)	//if head of freeList was already NULL 
		{
			ret_ptr = p;	// new head of freeList becomes p only
		}
		else
		{
			if(end + 1 == ptr->start)	// merge both the free Blocks
			{
				ptr->start = start;
				free(p);
			}
			else
			{
				p->next = ptr;	// insert at start in free list
				ret_ptr = p;	// change the head of freeList
			}
		}
	}
	else	// insert in between free list
	{
		// insert before ptr pointer

		if(prev->end + 1 == start)// if u can combine "prev" and "block to be added"
		{
			prev->end = end;// just extend the end value of prev block
			if(ptr != NULL)	//node to be added is not going to be the last node of freeList
			{
				if(end+1 == ptr->start)	//if u can combine "block to be added" and "ptr"
				{
					prev->end = ptr->end;	
					prev->next = ptr->next;
					free(p);
					free(ptr);
				}
				else // delete p becouse it already has been combined with prev
				{
					free(p);
				}
			}
		}
		else// check if you can combine "block to be added" and "ptr" (if it exists)
		{
			prev ->next = p;
			if(ptr != NULL)	// if ptr exists
			{
				if(end+1 == ptr->start) // if you can combine "block to be added" and "ptr"
				{
					p->end = ptr->end;
					p->next = ptr->next;
					free(ptr);
				}
				else
				{
					p->next = ptr;
				}
			}
		}
	}
	return ret_ptr;
}

void print_Logical_List(LA_List* la)
{
	LA_List* ptr=la;
	List_PM* lp;
	printf("\n----------------------------------------------------------------------------------------------\n");
	printf("Logical Start Address\t\tChunk Size\t\tPhysical Distribution in Main Memory\n");
	printf("----------------------------------------------------------------------------------------------\n");
	while(ptr!=NULL)
	{
		printf("\t %d \t\t\t   ",ptr->start);
		printf("%d\t\t\t\t     ",ptr->size);
		lp=ptr->alloc_ptr;
		if(lp == NULL)
		{
			printf("could not allocate");
		}
		while(lp!=NULL)
		{
			printf("%d...%d,",lp->start,lp->end);
			lp=lp->next;
		}
		printf("\n");
		ptr=ptr->next;
	}
	printf("\n");
}

void print_Free_List(List_PM* freeList)
{
	List_PM* lp=freeList;
	printf("\n--------------------------------------------------------------------------------------------\n");
	printf("\t\tPhysical Distribution in Free List in Heap\n");
	printf("--------------------------------------------------------------------------------------------\n");
	while(lp!=NULL)
	{
		printf("\t\t\t\t%d.......%d\n",lp->start,lp->end);
		lp=lp->next;
	}
	printf("\n");
}

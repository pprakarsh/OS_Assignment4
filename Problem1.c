#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<asm/unistd.h>
#include<pthread.h>

struct cell
{
	int r; 
	int c;
	int** M1; int** M2; int** R;
	int k;
};

typedef struct cell cell;

void* multiply(void* curr_cell1)
{
	cell* curr_cell = (cell* )curr_cell1;
	int r = curr_cell->r; int c = curr_cell->c;
	int k = curr_cell->k;

	curr_cell->R[r][c] = 0;
	//printf("Inside Thread: r-%d c-%d k-%d \n", r, c, k);
	//fflush(stdout);
	for(int i = 0; i < k; i++)
	{
		curr_cell->R[r][c] += ((curr_cell->M1[r][i])*(curr_cell->M2[i][c]));
	}

	//printf("Exiting Thread %d,%d. R[r][c]=%d\n", r, c, curr_cell->R[r][c]);
	//fflush(stdout);
	
	free(curr_cell);
	pthread_exit(0);
}


int main()
{
	int** M1; 
	int** M2; 
	int** R;

	int n, k, m;	
	printf("Enter the value of n, k, m: \n");
	scanf("%d %d %d", &n, &k, &m);

	//Memory allocation to M1 and M2
	M1 = malloc(sizeof(int*)*n);
	M2 = malloc(sizeof(int*)*k);

	for(int i = 0; i < n; i++)
	{
		M1[i] = malloc(sizeof(int)*k);
	}	
	for(int i = 0; i < k; i++)
	{
		M2[i] = malloc(sizeof(int)*m);
	}


	//Memory allocation to Result array
	R = (int** )malloc(sizeof(int*)*n);
	for(int i = 0; i < n; i++)
	{
		R[i] = malloc(sizeof(int)*m);
	}

	
	//Creation of 2D Arrays M1 and M2
	printf("\nEnter values in M1\n");
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < k; j++)
		{
			scanf("%d", &M1[i][j]);
		}
	}
	printf("\nEnter values in M2\n");
	for(int i = 0; i < k; i++)
	{
		for(int j = 0; j < m; j++)
		{
			scanf("%d", &M2[i][j]);
		}
	}

	//Create 2D Array to save tid for join later
	pthread_t tid_values[n][m];
	
	//Calculating M1*M2 = R
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < m; j++)
		{
			cell* curr_cell = malloc(sizeof(cell));

			curr_cell->r = i; curr_cell->c = j; 
			curr_cell->M1 = M1; curr_cell->M2 = M2; curr_cell->R = R;
			curr_cell->k = k;

			pthread_t tid;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_create(&tid, &attr, multiply, curr_cell);
			tid_values[i][j] = tid;
//			printf("tid_values[%d][%d]: %d\n", i, j, tid_values[i][j]);
			fflush(stdout);
//			tid_values[i][j] = tid;
		}
	}

	/*for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < m; j++)
		{
			printf("Tid[%d][%d]: %ld\n", i, j, tid_values[i][j]);
		}
	}*/

	//Joining all threads
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < m; j++)
		{
			//printf("Joining Thread %d,%d\n", i, j);
			//fflush(stdout);
			pthread_join(tid_values[i][j], NULL);
			//printf("Joined Thread %d,%d\n", i, j);
			//fflush(stdout);
		}
	}
	
	//Printing Result Matrix
	printf("\nResult Matrix: \n");
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < m; j++)
		{
			printf("%d ", R[i][j]);
		}
		printf("\n");
	}
}


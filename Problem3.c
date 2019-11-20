#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<stdlib.h>

struct args
{
	int* A; int* B;
	int n; int m;
	int* ind_arr;
	int i; char arr;
};

void* find_ind(void* arg)
{
	struct args* args_thread = (struct args* )arg;
	if(args_thread->arr == 'A')
	{
		int ind = args_thread->m;
		for(int i = 0; i < args_thread->m; i++)
		{
			if(args_thread->B[i] > args_thread->A[args_thread->i])
			{
				ind = i;
				break;
			}
		}
		args_thread->ind_arr[(args_thread->i)+ind] = args_thread->A[args_thread->i];
	}
	else if(args_thread->arr == 'B')
	{
		int ind = args_thread->n;
		for(int i = 0; i < args_thread->n; i++)
		{
			if(args_thread->A[i] > args_thread->B[args_thread->i])
			{
				ind = i;
				break;
			}
		}
		args_thread->ind_arr[(args_thread->i)+ind] = args_thread->B[args_thread->i];
	}
}

int main()
{
	int n, m;
	printf("Enter the number of elements in A and B: ");
	scanf("%d %d", &n, &m);

	int* A = malloc(sizeof(int)*n);
	int* B = malloc(sizeof(int)*m);
	int* ind_arr = malloc(sizeof(int)*(n+m));

	printf("Enter elements of A\n");
	for(int i = 0; i < n; i++)
	{
		scanf("%d", &A[i]);
	}

	printf("Enter elements of B\n");
	for(int i = 0; i < m; i++)
	{
		scanf("%d", &B[i]);
	}

	pthread_t tid_values[n+m];
	struct args* table[n+m];

	//finding values of indices of elements of A in final array
	for(int i = 0; i < n; i++)
	{
		struct args* arg_thread = malloc(sizeof(struct args));
		arg_thread->n = n; arg_thread->m = m;
		arg_thread->A = A;
		arg_thread->B = B;
		arg_thread->ind_arr = ind_arr;
		arg_thread->arr = 'A';
		arg_thread->i = i;

		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr, find_ind, arg_thread);
		tid_values[i] = tid;
		table[i] = arg_thread;
	}
	
	//finding values of indices of elements of B in final array
	for(int i = 0; i < m; i++)
	{
		struct args* arg_thread = malloc(sizeof(struct args));
		arg_thread->n = n; arg_thread->m = m;
		arg_thread->A = A;
		arg_thread->B = B;
		arg_thread->ind_arr = ind_arr;
		arg_thread->arr = 'B';
		arg_thread->i = i;

		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr, find_ind, arg_thread);
		tid_values[i+n] = tid;
		table[i+n] = arg_thread;
	}


	for(int i = 0; i < n+m; i++)
	{
		pthread_join(tid_values[i], NULL);
		free(table[i]);
	}

	printf("Final sorted array is: \n");
	for(int i = 0; i < n+m; i++)
	{
		printf("%d ", ind_arr[i]);
	}
}

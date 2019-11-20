#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

struct ele
{
	int n, m;
	int t_no;
	int l1, l2;
};

int arr[100];

int no_digits(int num)
{
	int count = 0;
	while(num != 0)
	{
		num = num/10;
		count++;
	}
	return count;
}

int find_digit(int ind, int num)
{
	for(int i = 0; i < ind; i++)
	{
		num = num/10;	
	}
	return num%10;
}

void* add_mult(void* args)
{
	struct ele* element = (struct ele* )args;
	arr[element->t_no] = 0;

	for(int i = 0; i < element->l2; i++)
	{
		if(!(i > element->t_no || (element->l1 <= (element->t_no)-i)))
		{
			int n1, n2;
			n1 = find_digit(i, element->m);
			n2 = find_digit(element->t_no-i, element->n);
			arr[element->t_no] +=  n1*n2;	
		}	
	}
}

int main()
{
	int n, m;
	printf("Enter the value of n and m: ");
	scanf("%d %d", &n, &m);

	int l1 = no_digits(n);
	int l2 = no_digits(m);

	if(l1 < l2)
	{
		int temp = n;
		n = m;
		m = temp; 
		temp = l1; l1 = l2; l2 = temp;
	}

	
	struct ele* table[l1+l2-1];
	pthread_t tid_values[l1+l2-1];

	for(int i = 0; i < l1+l2-1; i++)
	{
		struct ele* args = (struct ele* )malloc(sizeof(struct ele));
		table[i] = args;
		args->n = n;
		args->m = m;
		args->t_no = i;
		args->l1 = l1; args->l2 = l2;

		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr, add_mult, args);
		tid_values[i] = tid;
	}	

	for(int i = 0; i < l1+l2-1; i++)
	{
		pthread_join(tid_values[i], NULL);
		free(table[i]);	
	}
	
	int carry = 0;
	int answer[l1+l2];
	for(int i = 0; i < l1+l2-1; i++)
	{
		if(((carry)+(arr[i]))/10 != 0)
		{
			answer[i] = ((carry)+(arr[i]))%10;
			carry = (carry+arr[i])/10;
		}
		else
		{
			answer[i] = carry + arr[i];
			carry = 0;
		}
	}
	if(carry != 0)
	{
		answer[l1+l2-1] = carry;
	}
	
	for(int i = l1+l2-1; i >= 0; i--)
	{
		printf("%d", answer[i]);	
	}
}

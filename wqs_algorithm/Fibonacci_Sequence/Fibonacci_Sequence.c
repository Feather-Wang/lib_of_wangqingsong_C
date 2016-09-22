#include <stdio.h>

int fabi(int n)
{
	if(n == 1 || n == 2)
		return 1;

	return fabi(n-1) +  fabi(n-2);
}

int main()
{
	int n;

	printf("Input NO : ");
	scanf("%d",&n);

	printf("fabi(n) : %d.\n",fabi(n));

	return 0;
}

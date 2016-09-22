#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{

	printf("PATH1 : %s.\n",getenv("PATH1"));
	printf("PATH2 : %s.\n",getenv("PATH2"));

	return 0;
}

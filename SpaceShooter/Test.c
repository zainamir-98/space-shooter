#include<stdio.h>
#include<time.h>



main() {
	srandom(time(NULL));
	int a = 3;
	int b = 5;
	printf("%d", rand() % (b - a) + a);
	system("pause");
}
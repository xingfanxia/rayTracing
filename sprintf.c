#include <stdio.h>
int main() {
	char* toprint;
	int var = 555;
	sprintf(toprint, "sphere%d", var);
	printf("%s \n ", toprint);
	return 0;
}
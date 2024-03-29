// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>

int main(void)
{
  int a = 7;
  int b = 5;

  printf("%d + %d = %d\n", a, b, a + b);
  printf("%d - %d = %d\n", a, b, a - b);
  printf("%d * %d = %d\n", a, b, a * b);
  printf("%d / %d = %d\n", a, b, a / b);  // Note - integer division
  printf("%d %% %d = %d\n", a, b, a % b); // Note - integer modulo

  printf("size of _Bool:       %zu\n", sizeof(_Bool));
  printf("size of char:        %zu\n", sizeof(char));
  printf("size of short:       %zu\n", sizeof(short));
  printf("size of int:         %zu\n", sizeof(int));
  printf("size of long:        %zu\n", sizeof(long));
  printf("size of long long:   %zu\n", sizeof(long long));
  printf("size of float:       %zu\n", sizeof(float));
  printf("size of double:      %zu\n", sizeof(double));
  printf("size of long double: %zu\n", sizeof(long double));
}

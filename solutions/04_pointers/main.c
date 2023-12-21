// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>

int global_int = 1;
int uninit_int;
const int const_int = 100;

int main(void)
{
  int local_int = 2;

  // Contents and address of objects
  //
  printf("Address %p : value %d\n", &global_int, global_int);
  printf("Address %p : value %d\n", &uninit_int, uninit_int);
  printf("Address %p : value %d\n", &const_int,  const_int);
  printf("Address %p : value %d\n", &local_int,  local_int);

  // Pointers to objects
  //
  int *ptr_global      = &global_int;
  int *ptr_uninit      = &uninit_int;
  const int *ptr_const = &const_int;
  int *ptr_local       = &local_int;

  // Pointer values
  //
  printf("ptr_global value: %p\n", ptr_global);
  printf("ptr_uninit value: %p\n", ptr_uninit);
  printf("ptr_const  value: %p\n", ptr_const);
  printf("ptr_local  value: %p\n", ptr_local);


  // Indirect access to objects
  //
  printf("global_int via pointer: %d\n", *ptr_global);
  printf("uninit_int via pointer: %d\n", *ptr_uninit);
  printf("const_int  via pointer: %d\n", *ptr_const);
  printf("local_int  via pointer: %d\n", *ptr_local);


  // Pointer addresses
  //
  printf("ptr_global address: %p\n", &ptr_global);
  printf("ptr_uninit address: %p\n", &ptr_uninit);
  printf("ptr_const  address: %p\n", &ptr_const);
  printf("ptr_local  address: %p\n", &ptr_local);


  // Pointer-to-pointer-to-int
  //
  int* *ptr_ptr = &ptr_local;

  printf("ptr_ptr:   %p\n", ptr_ptr);
  printf("*ptr_ptr:  %p\n", *ptr_ptr);
  printf("**ptr_ptr: %d\n", **ptr_ptr);
}

// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdbool.h> // to use bool, false and true instead of _Bool, 0, and 1
#include <stddef.h>
#include <stdio.h>

enum Programmes {
    White_Wash = '1',
    Colour_Wash,
    Mixed_Wash,
    Economy_Wash,
    Program_1,
    Program_2,
    Quit,
};

int main(void)
{
  bool done = false;

  do
  {
    puts("***************************");
    puts("Washing Machine Program   *");
    puts("***************************");
    puts("1 - White Wash");
    puts("2 - Colour Wash");
    puts("3 - Mixed Wash");
    puts("4 - Economy Wash");
    puts("5 - Program 1");
    puts("6 - Program 2");
    puts("7 - Exit to maintenance");
    puts("");
    printf("Select a program: ");

    // Read the input stream.  If the retrieved
    // character is a newline, discard it.  Keep
    // reading until a non-newline is retrieved
    //

    int selection;

    do {
        selection = getchar();
    }while(selection == '\n');

    puts("");

    switch(selection)
    {
    case White_Wash:
      puts("White wash running");
      break;

    case Colour_Wash:
      puts("Colour wash running");
      break;

    case Mixed_Wash:
      puts("Mixed wash running");
      break;

    case Economy_Wash:
      puts("Economy wash running");
      break;

    case Program_1:
      puts("User program 1 running");
      break;

    case Program_2:
      puts("User program 2 running");
      break;

    case Quit:
      done = true;
      break;

    default:
      puts("Unknown selection.  Try again.");
      break;
    }
    puts("");

  } while(!done);

  puts("Exiting...");
  return 0;
}

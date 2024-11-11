// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>

enum Programmes {
    White_Wash,
    Colour_Wash,
    Mixed_Wash,
    Economy_Wash,
    Program_1,
    Program_2,
    End_Programme,
};

int main(void)
{
    for (int i=0; i<End_Programme; i++) {
      printf("Program %d: ", i);

      switch(i) {
        case White_Wash:
          puts("white wash");
          break;

        case Colour_Wash:
          puts("Colour wash");
          break;

        case Mixed_Wash:
          puts("Mixed wash");
          break;

        case Economy_Wash:
          puts("Economy wash");
          break;

        case Program_1:
          puts("User program 1");
          break;

        case Program_2:
          puts("User program 2");
          break;
      }
    }
}

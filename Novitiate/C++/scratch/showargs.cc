// A program to display the functionality of passing command line
//   arguments to main().
// Brett Deaton 7.9.10

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  while(--argc)
    printf("%s\n", *(++argv));
  exit(EXIT_SUCCESS);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main (void)
{
  char *current_dir = getcwd (NULL, 0);

  if (current_dir != NULL)
    {

      printf ("%s\n", current_dir);
      free (current_dir);
    }
  else
    {
      perror ("pwd");
      return EXIT_FAILURE;
    }
  return EXIT_FAILURE;

}

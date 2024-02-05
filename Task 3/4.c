#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


#define RET_STATUSint exit(void);


int
main (void)
{
  int i = 0;
  i = 0;

  unsigned char buf1[1000] = { '\0' };
  unsigned char COM[10] = { '\0' };
  int j = 0;
  unsigned char token[100] = { '\0' };	// Change to '\0' instead of '\n'


  while (1)
  {
    i = 0;
    j = 0;
    memset (buf1, 0, 1000);
    memset (COM, 0, 10);
    memset (token, 0, 100);

    printf ("Enter the required  command >>>>>>>");

    fgets (buf1, 1000, stdin);


    while (buf1[i] != ' ' && buf1[i] != '\n')
    {
      COM[i] = buf1[i];
      i++;
    }

    if ((strcmp (COM, "exit")) == 0)
    {
      printf ("Etla3 Bara \n");
      break;
    }

    else if ((strcmp (COM, "echo")) == 0)
    {
      while (buf1[i] == ' ' || buf1[i] == '\n')
      {
	i++;
      }

      while (buf1[i] != '\n')
      {
	printf ("%c", buf1[i]);
	i++;
      }
      printf ("\n");
    }

    else if ((strcmp (COM, "pwd")) == 0)
    {

      char *current_dir = getcwd (NULL, 0);

      if (current_dir != NULL)
      {

	printf ("%s\n", current_dir);
	free (current_dir);
      }
      else
      {
	printf ("pwd failed:\n ");

      }

    }

    else if ((strcmp (COM, "cd")) == 0)
    {

      while (buf1[i] == ' ' || buf1[i] == '\n')
      {
	i++;
      }
      while (buf1[i] != '\n')
      {
	token[j] = buf1[i];
	i++;
	j++;
      }
      token[j] = '\0';		// Change to '\0' instead of '\n'


      if ((chdir (token)) == 0)
      {

	printf ("directory changed\n");
      }
      else
      {
	printf ("invalid dir\n");
      }

    }


    else
    {
      printf ("invaled command\n");
    }
  }
  return 0;
}

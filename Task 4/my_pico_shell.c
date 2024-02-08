#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define RET_STATUSint exit(void);

int
count_arg (char *buf_line, int l)
{
  int temp = 0;
  int i = 0;
  int n_args = 0;
  while (i < l)
  {
    while (buf_line[i] == ' ' || buf_line[i] == '\n')
    {
      i++;
      temp = 0;
    }

    while (buf_line[i] != ' ' && buf_line[i] != '\n')
    {
      i++;
      temp = 1;
    }
    if (temp == 1)
    {
      n_args++;
    }
    else
    {
    }
  }
  return (n_args - 1);
}

void
parsing (char *pInput, char *arr_tokens[], int *pTokensNum)
{

  int iterator = 0;
  arr_tokens[iterator] = strtok (pInput, " ");
  while (arr_tokens[iterator++] != NULL)
  {
    arr_tokens[iterator] = strtok (NULL, " ");
  }
  *pTokensNum = iterator;
}

int
main (void)
{
  while (1)
  {
    size_t n = 1;
    char *buf = (char *) malloc (sizeof (char) * n);
    size_t retv = 0;
    int pTokensNum = 0;

//getline
    printf ("Enter the required  command >>>>>>>");
    retv = getline (&buf, &n, stdin);
    if (retv == -1)
    {
      printf ("Malloc error\n ");
    }
    else
    {

    }
    buf[retv - 1] = '\0';
// count arguments

    size_t n_arg = 0;
    n_arg = count_arg (buf, retv);
    pTokensNum = n_arg;
    char **arr_tokens = (char **) malloc (sizeof (char) * n_arg);


    //parsing 
    parsing (buf, arr_tokens, &pTokensNum);

    //bilt in command 
    if ((strcmp (arr_tokens[0], "exit\0")) == 0)
    {
      printf ("Etla3 Bara \n");
      break;
    }

    else if ((strcmp (arr_tokens[0], "echo")) == 0)
    {
      int k = 0;
      for (k = 1; k < (pTokensNum - 1); k++)
      {
	printf ("%s ", arr_tokens[k]);
      }
      printf ("\n");

    }

    else if ((strcmp (arr_tokens[0], "pwd")) == 0)
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

    else if ((strcmp (arr_tokens[0], "cd")) == 0)
    {
      size_t l = strlen (arr_tokens[1]);
      char *path = (char *) malloc (sizeof (char) * (l + 1));
      strcpy (path, arr_tokens[1]);
      path[l] = '\0';
      if ((chdir (path) == 0))
      {
	printf ("directory changed\n");
      }
      else
      {
	printf ("invalid dir\n");
      }
    }

//external command
    else
    {

      pid_t returned_pid = fork ();
      if (returned_pid > 0)
      {
	//PARENT
	int wstatus;
	wait (&wstatus);
      }
      else if (returned_pid == 0)
      {
	//CHILD
	arr_tokens[pTokensNum] = NULL;
	execvp (arr_tokens[0], arr_tokens);
	printf ("Please Enter a valid COmmand\n");
	return 0;
      }
      else
      {
      }
      printf ("external command\n");
    }
    free (buf);
  }
  return 0;
}

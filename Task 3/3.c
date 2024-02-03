#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define RET_STATUS 0

int
main (int argc, char *argv[])
{
  int fd1, fd2, num_read, num_write;
  unsigned char buf[100];

  if (argc != 3)
    {
      fprintf (stderr, "Usage: %s src dest\n", argv[0]);
      return -1;
    }

  fd1 = open (argv[1], O_RDONLY);	// Corrected flags to O_RDONLY
  if (fd1 < 0)
    {
      perror ("Could not open the source file");
      return -1;
    }
  printf ("The source file opened with fd = %d\n", fd1);

  fd2 = open (argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd2 < 0)
    {
      perror ("Could not open the destination file");
      close (fd1);		// Close the source file if opening destination file fails
      return -1;
    }
  printf ("The destination file opened with fd = %d\n", fd2);

  while ((num_read = read (fd1, buf, 100)) != 0)
    {
      if (num_read < 0)
	{
	  perror ("Could not read from the source file");
	  close (fd1);
	  close (fd2);
	  return -1;
	}
      printf ("I read %d bytes\n", num_read);

      num_write = write (fd2, buf, num_read);
      if (num_write < 0)
	{
	  perror ("Could not write to the destination file");
	  close (fd1);
	  close (fd2);
	  return -1;
	}
      printf ("I wrote %d bytes\n", num_write);


      remove (argv[1]);
    }

  close (fd1);
  close (fd2);

  return RET_STATUS;
}

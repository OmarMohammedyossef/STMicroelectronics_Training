/*
===============================================================================
 * Name			: myfdik.c
 * Author		: Omar Elshamy
 * Description 		: Source File Fdisk system call
 * Created on		: Feb, 2024
===============================================================================
 */

/*Include standard libraries Files */
#define _LARGEFILE64_SOURCE
/*==========================================================================*/

/*Include standard libraries Files */
#include <inttypes.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
/*==========================================================================*/
/*
 *Make Partition Entrye as a Struct
 */
typedef struct
{
  uint8_t status;
  uint8_t first_chs[3];
  uint8_t partition_type;
  uint8_t last_chs[3];
  uint32_t lba;
  uint32_t sector_count;
} PartitionEntry;

/*==========================================================================*/
/*Main: take path of disk  */
int
main (int argc, char **argv)
{

  char buf[512];
  int fd = open (argv[1], O_RDONLY);
  uint8_t extended_flag = 5;
  read (fd, buf, 512);

  PartitionEntry *table_entry_ptr = (PartitionEntry *) & buf[446];

/* print Table's elements */

  printf ("%-5s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n", "Device",
	  "Boot", "Start", "End", "Sectors", "Size", "Id", "Type");

/* Description: Print MPR's parameters */

  for (int i = 0; i < 4; i++)
  {
    if (table_entry_ptr[i].sector_count == 0)
    {
      continue;
    }

    printf ("%s%-5d %-10c %-10u %-10u %-10u %0.1fG      %-10X\n",
	    argv[1],
	    i + 1,
	    table_entry_ptr[i].status == 0x80 ? '*' : ' ',
	    table_entry_ptr[i].lba,
	    table_entry_ptr[i].lba + table_entry_ptr[i].sector_count - 1,
	    table_entry_ptr[i].sector_count,
	    (double) (((uint64_t) table_entry_ptr[i].sector_count *
		       512) / (1024 * 1024 * 1024)),
	    table_entry_ptr[i].partition_type);
/* Check if there are extended partition  => save if's number */
    if (table_entry_ptr[i].partition_type == 5)
    {
      extended_flag = i;
    }
  }
  /* check if there are extended partition in partition's range (4 entry)   */
  if (extended_flag <= 4)
  {
/* jumb on extendded partition to read EPR  */
    uint32_t extended_lba = table_entry_ptr[extended_flag].lba;
    lseek64 (fd, (off64_t) extended_lba * 512, SEEK_SET);
    read (fd, buf, 512);
    table_entry_ptr = (PartitionEntry *) & buf[446];

/* Description: Print EPR's parameters */
    int i = 5;
    printf ("%s%-5d %-10c %-10u %-10u %-10u %0.1fG      %-10X\n",
	    argv[1],
	    i,
	    table_entry_ptr[0].status == 0x80 ? '*' : ' ',
	    table_entry_ptr[0].lba + extended_lba,
	    table_entry_ptr[0].lba + table_entry_ptr[0].sector_count - 1 +
	    extended_lba, table_entry_ptr[0].sector_count,
	    (double) (((uint64_t) table_entry_ptr[0].sector_count * 512) /
		      (1024 * 1024 * 1024)),
	    table_entry_ptr[0].partition_type);
    i++;
    while (1)
    {
/* check if extended partition points to another extended partition  */
      if ((table_entry_ptr[1].lba) == 0)
      {
	break;
      }
/* jumb on 2nd extendded partition to read EPR  */
      off64_t offset = extended_lba + table_entry_ptr[1].lba;
      lseek64 (fd, (off64_t) offset * 512, SEEK_SET);
      read (fd, buf, 512);
      table_entry_ptr = (PartitionEntry *) & buf[446];

/* Description: Print 2nd extendded partition EPR's parameters */
      printf ("%s%-5d %-10c %-10lu %-10lu %-10u %0.1fG      %-10X\n",
	      argv[1],
	      i,
	      table_entry_ptr[0].status == 0x80 ? '*' : ' ',
	      table_entry_ptr[0].lba + offset,
	      table_entry_ptr[0].lba + table_entry_ptr[0].sector_count - 1 +
	      offset, table_entry_ptr[0].sector_count,
	      (double) (((uint64_t) table_entry_ptr[0].sector_count * 512) /
			(1024 * 1024 * 1024)),
	      table_entry_ptr[0].partition_type);
      i++;
    }
  }
  return 0;
}

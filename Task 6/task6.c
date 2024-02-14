#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>
#define _LARGEFILE64_SOURCE	/* See feature_test_macros(7) */
#include <sys/types.h>
#include <unistd.h>


typedef struct
{
  uint8_t status;
  uint8_t first_chs[3];
  uint8_t partition_type;
  uint8_t last_chs[3];
  uint32_t lba;
  uint32_t sector_count;
} PartitionEntry;

int
main (int argc, char **argv)
{
  char buf[512];
  PartitionEntry *EBR_entry_ptr = NULL;
  int fd = open (argv[1], O_RDONLY);

  read (fd, buf, 512);

  PartitionEntry *table_entry_ptr = (PartitionEntry *) & buf[446];

  printf ("%-5s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n", "Device",
	  "Boot", "Start", "End", "Sectors", "Size", "Id", "Type");

  for (int i = 0; i < 4; i++)
  {

    printf ("%s%-5d %-10c %-10u %-10u %-10u %uG      %-10X\n",
	    argv[1],
	    i + 1,
	    table_entry_ptr[i].status == 0x80 ? '*' : ' ',
	    table_entry_ptr[i].lba,
	    table_entry_ptr[i].lba + table_entry_ptr[i].sector_count - 1,
	    table_entry_ptr[i].sector_count,
	    (uint32_t) (((uint64_t) table_entry_ptr[i].sector_count *
			 512) / (1024 * 1024 * 1024)),
	    table_entry_ptr[i].partition_type);



    if (table_entry_ptr[i].partition_type == 0x05)
    {
      lseek64 (fd, (off64_t *) 512 * table_entry_ptr[i].lba, SEEK_SET);
      EBR_entry_ptr = (PartitionEntry *) (table_entry_ptr[i].lba + 446);
      break;
    }
  }
  while (1)
  {
    int j = 0;
    printf ("%s%-5d %-10c %-10u %-10u %-10u %uG      %-10X\n",
	    argv[1],
	    j + 1,
	    EBR_entry_ptr[j].status == 0x80 ? '*' : ' ',
	    EBR_entry_ptr[j].lba,
	    EBR_entry_ptr[j].lba + EBR_entry_ptr[j].sector_count - 1,
	    EBR_entry_ptr[j].sector_count,
	    (uint32_t) (((uint64_t) EBR_entry_ptr[j].sector_count *
			 512) / (1024 * 1024 * 1024)),
	    EBR_entry_ptr[j].partition_type);
    /*extended */

    if ((EBR_entry_ptr + 16) != NULL)
    {
      EBR_entry_ptr[j].lba = (uint32_t *) * (EBR_entry_ptr[1].lba);
      EBR_entry_ptr = EBR_entry_ptr + 446;
    }
    else
    {
      break;
    }

  }

  return 0;
}

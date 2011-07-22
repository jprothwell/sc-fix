#include <stdio.h>
#include <stdlib.h>



int linux_sys_mode,linux_sys_close;

int main(int argc, char **argv)
{
  int status;

   if (argc!=2)
    {
      fprintf(stderr, "usage: tci_lower <mode>\n");
      exit(-1);
    }

  linux_sys_mode=atoi(argv[1]);
  switch(linux_sys_mode)
    {
    case 0:      
      fprintf(stderr, "TCI_UPPER: A\n");
      break;

    case 1:   
      fprintf(stderr, "TCI_UPPER: B\n");
      break;

    default:
      fprintf(stderr, "invalid mode (0 or 1)\n");
      return -1;
      break;
    }

  sys_init();
  prh_tci_init();


  while(1);
}


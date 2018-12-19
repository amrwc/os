#include "types.h"
#include "user.h"
#include "fs.h"

int main(int argc, char *argv[])
{
  int dirDescriptor = (argc > 1) ? opendir(argv[1]) : opendir("");

  if (dirDescriptor == 0)
  {
    printf("ls.c->main()->dirDescriptor == 0\n");
    exit();
  }

  struct _DirectoryEntry *dirEntry = malloc(32);
  // char name[8] = {0};
  // char ext[3] = {0};
  // char name[9] = {0};
  // char ext[4] = {0};
  // char line[12] = {0};
  char name[12] = {0};
  
  int i = 0;
  // int j;

  for (;;)
  {
    if (readdir(dirDescriptor, dirEntry) > -1)
    {
      if (dirEntry->Filename[0] == 0) break;

      // memmove(name, &dirEntry->Filename, strlen((char*)dirEntry->Filename));
      memmove(name, &dirEntry->Filename, 8);
      // memmove(line, &dirEntry->Filename, 8);
      // memmove(line + strlen(line), &dirEntry->Ext, 3);
      
      // printf("%s", dirEntry->Filename[0]);
      // printf("%s\n", name);
      // for (j = 0; j < 8; j++)
      // {
      //   if (dirEntry->Filename[j] == 0)
      //   {
      //     memmove(line, &dirEntry->Filename, j);
      //     break;
      //   }
      // }
      // if (i > 1) // If past the '.' and '..'.
      // {
      //   for (j = 1; j <= 12; j++)
      //   {
      //     // if (name + j == 0)
      //     // {
      //     //   memmove(name + j, &dirEntry->Ext, 3);
      //     //   break;
      //     // }
      //     printf("%c", name + j);
      //   }
      // }
      printf("%s\n", name);
      // printf("%s", line);


      // printf("%s.", name);
      // memmove(ext, &dirEntry->Ext, strlen((char*)dirEntry->Ext));
      // memmove(ext, &dirEntry->Ext, 3);
      // printf("%s\n", ext);
      // printf("%s.", dirEntry->Filename);
      // printf("%s\n", dirEntry->Ext);
    }

    i++;
  }

  if (closedir(dirDescriptor) != 0)
  {
    printf("ls.c->main(): closeDirectory() != 0\n");
    exit();
  }

	exit();
}

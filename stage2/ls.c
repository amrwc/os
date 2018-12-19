#include "types.h"
#include "user.h"
#include "fs.h"

int main(int argc, char *argv[])
{
  int dirDescriptor = opendir(argv[1]);

  if (dirDescriptor == 0)
  {
    printf("ls.c->main()->dirDescriptor == 0\n");
    exit();
  }

  struct _DirectoryEntry *dirEntry = malloc(32);
  char name[8] = {0};
  // char ext[3] = {0};
  // char name[9] = {0};
  // char ext[4] = {0};
  // char line[12] = {0};
  
  int i;
  // int j;

  for (;;)
  {
    if (readdir(dirDescriptor, dirEntry) > -1)
    {
      if (dirEntry->Filename[0] == 0) break;

      // memmove(name, &dirEntry->Filename, strlen((char*)dirEntry->Filename));
      memmove(name, &dirEntry->Filename, 7);
      // memmove(line, &dirEntry->Filename, 8);
      // memmove(line + strlen(line), &dirEntry->Ext, 3);
      
      // printf("%s", dirEntry->Filename[0]);
      printf("%s\n", name);
      // for (j = 0; j < 8; j++)
      // {
      //   if (dirEntry->Filename[j] == 0)
      //   {
      //     memmove(line, &dirEntry->Filename, j);
      //     break;
      //   }
      // }
      // if (j > 1)
      // {
      //   memmove(line + j, &dirEntry->Ext, 3);
      //   break;
      // }
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

  // TODO:
  // if (argc > 1) ? openDirectory(argv[1]) : openDirectory("")
  // readDirectory(, dirEntry);

  if (closedir(dirDescriptor) != 0)
  {
    printf("ls.c->main(): closeDirectory() != 0\n");
    exit();
  }

	exit();
}

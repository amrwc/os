#include "types.h"
#include "user.h"
#include "fs.h"

int main(int argc, char *argv[])
{
  int dirDescriptor;
  // struct _DirectoryEntry *dirEntry = malloc(32);

  // for(;;)
  // {
    dirDescriptor = opendir(argv[1]);

    if (dirDescriptor == 0)
    {
      printf("ls.c->main()->dirDescriptor == 0\n");
      exit();
      // break;
    }

    struct _DirectoryEntry *dirEntry = malloc(32);
    char name[8] = {0};
    char ext[3] = {0};
    do
    {
      if (readdir(dirDescriptor, dirEntry) > -1)
      {
        memmove(name, &dirEntry->Filename, 7);
        printf("%s ", name);
        memmove(ext, &dirEntry->Ext, 3);
        printf("%s ", ext);
        printf("%d\n", dirEntry->FileSize);
      }

      // printf("%s ", dirEntry->Filename);
      // printf("%s\n", dirEntry->Ext);
    } while(dirEntry->Filename[0] != 0);

    // TODO:
    // if (argc > 1) ? openDirectory(argv[1]) : openDirectory("")
    // readDirectory(, dirEntry);

    if (closedir(dirDescriptor) != 0)
    {
      printf("ls.c->main(): closeDirectory() != 0\n");
      exit();
      // break;
    }
  // }

	exit();
}

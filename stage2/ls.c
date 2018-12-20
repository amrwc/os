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
  char entryName[12] = {0}, ext[4] = {0};
  int i;

  for (;;)
  {
    if (readdir(dirDescriptor, dirEntry) > -1)
    {
      // If the Filename starts with zero,
      // we reached the end of the directory.
      if (dirEntry->Filename[0] == 0) break;

      memmove(entryName, &dirEntry->Filename, 8);
      memmove(ext, &dirEntry->Ext, 3);

      if (ext[0] == 0 || ext[0] == '.' || ext[0] == ' ')
      {
        // If it's a directory, append slash to the name.
        if (entryName[0] != '.')
        {
          for (i = 0; i < strlen(entryName); i++)
          {
            if (entryName[i] == ' ' || entryName[i] == 0)
            {
              entryName[i] = '/';
              break;
            }
          }
        }
        printf("%s\n", entryName);
      }
      else
      {
        // If the entry has an extension, append it to the name.
        for (i = 0; i < strlen(entryName); i++)
        {
          if (entryName[i] == ' ' || entryName[i] == 0)
          {
            entryName[i] = '.';
            memmove(entryName + i + 1, ext, strlen(ext));
            break;
          }
        }
        printf("%s\n", entryName);
      }
    }
  }

  if (closedir(dirDescriptor) != 0)
  {
    printf("ls.c->main(): closeDirectory() != 0\n");
  }

	exit();
}

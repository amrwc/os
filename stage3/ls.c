#include "types.h"
#include "user.h"
#include "fs.h"

struct Dir
{
  int dirDescriptor;
  int flag;
};

struct Dir openDirectory(int argc, char *argv[])
{
  int dirDescriptor = 0;
  int flag = 0; // Flags: "-l":= 1

  if (argc == 1)
  {
    // Empty ls
    dirDescriptor = opendir("");
  }
  else
  {
    if (argv[1][0] == '-')
    {
      // If 2nd argument is a flag...
      switch (argv[1][1])
      {
        case 'l' :
          flag = 1;
          break;
        default :
          break;
      }

      dirDescriptor = opendir("");
    }
    else if (argv[2][0] == '-')
    {
      // If 3rd argument is a flag...
      switch (argv[2][1])
      {
        case 'l' :
          flag = 1;
          break;
        default :
          break;
      }

      dirDescriptor = opendir(argv[1]);
    }
    else
    {
      dirDescriptor = opendir(argv[1]);
    }
  }

  struct Dir directory = {dirDescriptor, flag};
  return directory;
}

void printDirEntry(char *entryName, char *ext, int flag)
{
  int i;

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

    if (flag == 1)
    {
      printf("Flag == \"-l\"\n");
    }
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

    if (flag == 1)
    {
      printf("Flag == \"-l\"\n");
    }
  }
}

int main(int argc, char *argv[])
{
  struct Dir directory = openDirectory(argc, argv);

  if (directory.dirDescriptor == 0)
  {
    printf("ls.c->main()->dirDescriptor == 0\n");
    exit();
  }

  struct _DirectoryEntry *dirEntry = malloc(32);
  char entryName[12] = {0}, ext[4] = {0};

  for (;;)
  {
    if (readdir(directory.dirDescriptor, dirEntry) > -1)
    {
      // If the Filename starts with zero,
      // we reached the end of the directory.
      if (dirEntry->Filename[0] == 0) break;

      memmove(entryName, &dirEntry->Filename, 8);
      memmove(ext, &dirEntry->Ext, 3);

      printDirEntry(entryName, ext, directory.flag);
    }
    else
    {
      printf("ls.c->main(): readdir() <= -1\n");
      exit();
    }
  }

  if (closedir(directory.dirDescriptor) == -1)
  {
    printf("ls.c->main(): closedir() == -1\n");
  }

	exit();
}

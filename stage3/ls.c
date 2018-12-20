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

void printDirEntryDetails(struct _DirectoryEntry *dirEntry, char *entryName)
{
  /** 
   * Date:                                 Time:
   *  15          9       5         0       15      11          5         0
   * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   * |Y|Y|Y|Y|Y|Y|Y|M|M|M|M|D|D|D|D|D|     |h|h|h|h|h|m|m|m|m|m|m|s|s|s|s|s|
   * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   * \____________/\______/\_________/     \________/\__________/\_________/
   *      year      month      day            hour      minute     second
   */

  int dayCreated = dirEntry->DateCreated;
  dayCreated = dayCreated & 0b0000000000011111;

  int monthCreated = dirEntry->DateCreated;
  monthCreated = monthCreated & 0b0000000111100000;
  monthCreated = monthCreated >> 5;

  int yearCreated = dirEntry->DateCreated;
  yearCreated = (yearCreated >> 9) + 1980;

  printf("%s ", entryName);
  printf("%d/%d/%d ", dayCreated, monthCreated, yearCreated);
  printf("%d ", dirEntry->TimeCreated);
  printf("%d ", dirEntry->LastModDate);
  printf("%d ", dirEntry->LastModTime);
  printf("%db ", dirEntry->FileSize);
  printf("%d\n", dirEntry->Attrib);
}

void printDirEntry(struct _DirectoryEntry *dirEntry, int flag)
{
  char entryName[12] = {0}, ext[4] = {0};
  memmove(entryName, &dirEntry->Filename, 8);
  memmove(ext, &dirEntry->Ext, 3);

  if (ext[0] == 0 || ext[0] == '.' || ext[0] == ' ')
  {
    // If it's a directory, append slash to the name.
    if (entryName[0] != '.')
    {
      for (int i = 0; i < strlen(entryName); i++)
      {
        if (entryName[i] == ' ' || entryName[i] == 0)
        {
          entryName[i] = '/';
          break;
        }
      }

      if (flag == 1)
      {
        printDirEntryDetails(dirEntry, entryName);
      }
    }
    else
    {
      printf("%s\n", entryName);
    }
  }
  else
  {
    // If the entry has an extension, append it to the name.
    for (int i = 0; i < strlen(entryName); i++)
    {
      if (entryName[i] == ' ' || entryName[i] == 0)
      {
        entryName[i] = '.';
        memmove(entryName + i + 1, ext, strlen(ext));
        break;
      }
    }

    if (flag == 1)
    {
      printDirEntryDetails(dirEntry, entryName);
    }
    else
    {
      printf("%s\n", entryName);
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

  if (directory.flag == 1)
  {
    printf("Name     Created          Modified         Size       Attributes\n");
  }

  for (;;)
  {
    if (readdir(directory.dirDescriptor, dirEntry) > -1)
    {
      // If the Filename starts with zero,
      // we reached the end of the directory.
      if (dirEntry->Filename[0] == 0) break;

      printDirEntry(dirEntry, directory.flag);
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

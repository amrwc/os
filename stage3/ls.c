#include "types.h"
#include "user.h"
#include "fs.h"

typedef struct Dir
{
  int dirDescriptor;
  int flag;
} Dir;

Dir openDirectory(int argc, char *argv[])
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

  Dir directory = {dirDescriptor, flag};
  return directory;
}

void printDirEntryDetails(struct _DirectoryEntry *dirEntry)
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

  int day = dirEntry->DateCreated & 0b0000000000011111;
  int month = (dirEntry->DateCreated & 0b0000000111100000) >> 5;
  int year = (dirEntry->DateCreated >> 9) + 1980;
  printf("%d/%d/%d ", day, month, year);

  int second = (dirEntry->TimeCreated & 0b0000000000011111) * 2;
  int minute = (dirEntry->TimeCreated & 0b0000011111100000) >> 5;
  int hour = dirEntry->TimeCreated >> 11;
  (minute < 10)
    ? (second < 10)
      ? printf("%d:0%d:0%d  ", hour, minute, second)
      : printf("%d:0%d:%d  ", hour, minute, second)
    : (second < 10)
      ? printf("%d:%d:0%d  ", hour, minute, second)
      : printf("%d:%d:%d  ", hour, minute, second);

  day = dirEntry->LastModDate & 0b0000000000011111;
  month = (dirEntry->LastModDate & 0b0000000111100000) >> 5;
  year = (dirEntry->LastModDate >> 9) + 1980;
  printf("%d/%d/%d ", day, month, year);

  second = (dirEntry->LastModTime & 0b0000000000011111) * 2;
  minute = (dirEntry->LastModTime & 0b0000011111100000) >> 5;
  hour = dirEntry->LastModTime >> 11;
  (minute < 10)
    ? (second < 10)
      ? printf("%d:0%d:0%d  ", hour, minute, second)
      : printf("%d:0%d:%d  ", hour, minute, second)
    : (second < 10)
      ? printf("%d:%d:0%d  ", hour, minute, second)
      : printf("%d:%d:%d  ", hour, minute, second);

  // int dayCreated = dirEntry->DateCreated & 0b0000000000011111;
  // int monthCreated = (dirEntry->DateCreated & 0b0000000111100000) >> 5;
  // int yearCreated = (dirEntry->DateCreated >> 9) + 1980;

  // int secondCreated = dirEntry->TimeCreated & 0b0000000000011111;
  // int minuteCreated = (dirEntry->TimeCreated & 0b0000011111100000) >> 5;
  // int hourCreated = dirEntry->TimeCreated >> 11;

  // int dayModified = dirEntry->LastModDate & 0b0000000000011111;
  // int monthModified = (dirEntry->LastModDate & 0b0000000111100000) >> 5;
  // int yearModified = (dirEntry->LastModDate >> 9) + 1980;

  // int secondModified = dirEntry->LastModTime & 0b0000000000011111;
  // int minuteModified = (dirEntry->LastModTime & 0b0000011111100000) >> 5;
  // int hourModified = dirEntry->LastModTime >> 11;

  // printf("%d/%d/%d ", dayCreated, monthCreated, yearCreated);
  // (minuteCreated < 10)
  //   ? printf("%d:0%d  ", hourCreated, minuteCreated)
  //   : printf("%d:%d  ", hourCreated, minuteCreated);

  // printf("%d/%d/%d ", dayModified, monthModified, yearModified);
  // (minuteModified < 10)
  //   ? printf("%d:0%d  ", hourModified, minuteModified)
  //   : printf("%d:%d  ", hourModified, minuteModified);

  (dirEntry->Attrib & 0b00000001) ? printf("r") : printf("-");      // read only
  (dirEntry->Attrib & 0b00000010) ? printf("h") : printf("-");      // hidden
  (dirEntry->Attrib & 0b00000100) ? printf("s") : printf("-");      // system
  (dirEntry->Attrib & 0b00001000) ? printf("v") : printf("-");      // volume label
  (dirEntry->Attrib & 0b00010000) ? printf("s") : printf("-");      // subdirectory
  (dirEntry->Attrib & 0b00100000) ? printf("a  ") : printf("-  ");  // archive

  printf("%db\n", dirEntry->FileSize);
}

void printDirEntry(struct _DirectoryEntry *dirEntry, int flag)
{
  char entryName[12] = {0}, ext[4] = {0};
  memmove(entryName, &dirEntry->Filename, 8);
  memmove(ext, &dirEntry->Ext, 3);

  // If it's a subdirectory...
  if (ext[0] == 0 || ext[0] == '.' || ext[0] == ' ')
  {
    // ...and if it's not a 'dot entry'...
    if (entryName[0] != '.')
    {
      // ...append slash to the name.
      for (int i = 0; i < strlen(entryName); i++)
      {
        if (entryName[i] == ' ' || entryName[i] == 0)
        {
          entryName[i] = '/';
          break;
        }
      }

      if (flag == 1) // if '-l'
      {
        printf("%s  ", entryName);
        printDirEntryDetails(dirEntry);
      }
      else
      {
        // If no flags, display only the subdirectory's name.
        printf("%s\n", entryName);
      }
    }
    else
    {
      // Display 'dot entries'.
      printf("%s\n", entryName);
    }
  }
  else // If it's not a subdirectory...
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

    if (flag == 1) // if '-l'
    {
      printf("%s  ", entryName);
      printDirEntryDetails(dirEntry);
    }
    else
    {
      // If no flags, display the file's name with extension .
      printf("%s\n", entryName);
    }
  }
}

int main(int argc, char *argv[])
{
  Dir directory = openDirectory(argc, argv);

  if (directory.dirDescriptor == 0)
  {
    printf("ls.c->main()->dirDescriptor == 0\n");
    exit();
  }

  struct _DirectoryEntry *dirEntry = malloc(32);

  if (directory.flag == 1)
  {
    printf("Name      Created              Modified             Attrib  Size\n");
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

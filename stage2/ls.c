#include "types.h"
#include "user.h"
#include "fs.h"

int openDirectory(char *directory)
{
  return opendir(directory);
}

int readDirectory(int directoryDescriptor, struct _DirectoryEntry *dirEntry)
{
  return readdir(directoryDescriptor, dirEntry);
}

int closeDirectory(int directoryDescriptor)
{
  return closedir(directoryDescriptor);
}

int main(int argc, char *argv[])
{
  int dirDescriptor = openDirectory(argv[1]);

  if (dirDescriptor == 0)
  {
    printf("ls.c->main()->dirDescriptor == 0\n");
    exit();
  }

  // struct _DirectoryEntry *dirEntry;
  // dirEntry = malloc(32);
  struct _DirectoryEntry *dirEntry = malloc(32);

  readDirectory(dirDescriptor, dirEntry);

  printf("%s ", dirEntry->Filename);
  // printf("%d\n", dirEntry->FileSize);

	// int i;
	// for (i = 1; i < argc; i++)
	// {
	// 	printf("%s%s", argv[i], i + 1 < argc ? " " : "\n");
	// }

  // TODO:
  // if (argc > 1) ? openDirectory(argv[1]) : openDirectory("")
  // readDirectory(, dirEntry);

  if (closeDirectory(dirDescriptor) != 0)
  {
    printf("ls.c->main(): closeDirectory != 0\n");
    exit();
  }

	exit();
}

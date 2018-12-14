#include "types.h"
#include "user.h"
#include "fs.h"

int openDirectory(char *directory)
{
  printf("ls.c->openDirectory()->directory: %s\n", directory);

  int dirDescriptor = opendir(directory);

  if (dirDescriptor == 0)
  {
    printf("ls.c->openDirectory()->dirDescriptor == 0\n");
  }

  return dirDescriptor;
}

int readDirectory(int directoryDescriptor, struct _DirectoryEntry *dirEntry)
{
  readdir(directoryDescriptor, dirEntry);

  return 0;
}

int closeDirectory(int directoryDescriptor)
{
  closedir(directoryDescriptor);

  return 0;
}

int main(int argc, char *argv[])
{
  int dirDescriptor = openDirectory(argv[1]);
  // if (dirDescriptor == 0) return -1;
  if (dirDescriptor == 0)
  {
    // printf("ls.c->main()->dirDescriptor == 0\n");
    exit();
  }

  struct _DirectoryEntry *dirEntry;
  dirEntry = malloc(32);

  readDirectory(dirDescriptor, dirEntry);

	// int i;

	// for (i = 1; i < argc; i++)
	// {
	// 	printf("%s%s", argv[i], i + 1 < argc ? " " : "\n");
	// }

  // TODO:
  // if (argc > 1) ? openDirectory(argv[1]) : openDirectory("")
  // readDirectory(, dirEntry);
  closeDirectory(dirDescriptor);

	exit();
}

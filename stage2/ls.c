#include "types.h"
#include "user.h"
#include "fs.h"

int openDirectory(char *directory)
{
  // opendir(argv[0]);

  // return 0;
  return opendir(directory);
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
  int dirDescriptor = openDirectory(argv[0]);
  if (dirDescriptor == 0) return -1;

  struct _DirectoryEntry *dirEntry;

  readDirectory(dirDescriptor, dirEntry);

	// int i;

	// for (i = 1; i < argc; i++)
	// {
	// 	printf("%s%s", argv[i], i + 1 < argc ? " " : "\n");
	// }

  // TODO:
  // if (argc > 1) ? openDirectory(argv[1]) : openDirectory("")
  // readDirectory(, dirEntry);
  // closeDirectory();

	exit();
}

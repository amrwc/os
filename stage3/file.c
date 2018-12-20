#include "types.h"
#include "defs.h"
#include "param.h"
#include "fs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"

#include "mmu.h"
#include "proc.h"

Device devices[NDEV];

struct
{
	Spinlock		Lock;
	File			File[NFILE];
} FileTable;

void filesInitialise(void)
{
	File *f;

	spinlockInitialise(&FileTable.Lock, "FileTable");
	spinlockAcquire(&FileTable.Lock);
	for (f = FileTable.File; f < FileTable.File + NFILE; f++)
	{
		f->ReferenceCount = 0;
	}
	spinlockRelease(&FileTable.Lock);
}

// Allocate a file structure.
File* allocateFileStructure(void)
{
	File *f;

	spinlockAcquire(&FileTable.Lock);
	for (f = FileTable.File; f < FileTable.File + NFILE; f++) 
	{
		if (f->ReferenceCount == 0) 
		{
			f->ReferenceCount = 1;
			spinlockRelease(&FileTable.Lock);
			return f;
		}
	}
	spinlockRelease(&FileTable.Lock);
	return 0;
}

// Increment ref count for file f.
File* fileDup(File *f)
{
	spinlockAcquire(&FileTable.Lock);
	if (f->ReferenceCount < 1)
	{
		panic("fileDup");
	}
	f->ReferenceCount++;
	spinlockRelease(&FileTable.Lock);
	return f;
}

// Close file f.  (Decrement ref count, close when reaches 0.)
void fileClose(File *f)
{
	File ff;

	spinlockAcquire(&FileTable.Lock);
	if (f->ReferenceCount < 1)
	{
		panic("fileClose");
	}
	if (--f->ReferenceCount > 0) 
	{
		spinlockRelease(&FileTable.Lock);
		return;
	}
	ff = *f;
	f->ReferenceCount = 0;
	f->Type = FD_NONE;
	spinlockRelease(&FileTable.Lock);
	
	if (ff.Type == FD_PIPE)
	{
		pipeclose(ff.Pipe, ff.Writable);
	}
	else if (ff.Type == FD_FILE || ff.Type == FD_DIR) 
	{
		fsFat12Close(&ff);
	}

}

// Get metadata about file f.
int fileStat(File *f, Stat *st)
{
	if (f->Type == FD_FILE || f->Type == FD_DIR) 
	{
		//    ilock(f->ip);
		//    stati(f->ip, st);
		//    iunlock(f->ip);
		return 0;
	}
	return -1;
}

// Read from file f.
int fileRead(File *f, char *addr, int n)
{
	int r;

	if (f->Readable == 0)
	{
		return -1;
	}
	if (f->Type == FD_DEVICE)
	{
		return devices[f->DeviceID].read(f, addr, n);
	}
	else if (f->Type == FD_PIPE)
	{
		return piperead(f->Pipe, addr, n);
	}
	else if (f->Type == FD_FILE || f->Type == FD_DIR)
	{
		r = fsFat12Read(f, (unsigned char *)addr, n);
		return r;
	}
	panic("fileRead");
}

// Write to file f.
int fileWrite(File *f, char *addr, int n)
{

	if (f->Writable == 0)
	{
		return -1;
	}
	if (f->Type == FD_DEVICE)
	{
		return devices[f->DeviceID].write(f, addr, n);
	}
	else if (f->Type == FD_PIPE)
	{
		return pipewrite(f->Pipe, addr, n);
	}
	// Code to write to a disk file needs to be added
	panic("fileWrite");
}

// Allocate a file descriptor for the given file and
// store it in the process table for the current process.
static int fdAllocate(File *f)
{
	int fd;
	Process *curproc = myProcess();

	for (fd = 0; fd < NOFILE; fd++) 
	{
		if (curproc->OpenFile[fd] == 0) 
		{
			curproc->OpenFile[fd] = f; // Put the file in the process table.
			return fd;
		}
	}
	return -1;
}

int opendir(char *directory)
{
  int isSubdirectory;
  File *file;
  char *cwd = myProcess()->Cwd;

  if (strlen(directory) > 0)
  {
    isSubdirectory = 1;
    file = fsFat12Open(cwd, directory, isSubdirectory);
  }
  else
  {
    isSubdirectory = 1;
    char currentLocation[MAXCWDSIZE] = {0};

    // Get current location without the trailing slash.
    memmove(&currentLocation, cwd, strlen(cwd) - 1);

    file = fsFat12Open(cwd, currentLocation, isSubdirectory);
  }

  if (file == 0)
  {
    cprintf("file.c->opendir()->file == 0\n");
    return 0;
  }

  return fdAllocate(file); // Return file descriptor.
}

int readdir(int directoryDescriptor, struct _DirectoryEntry *dirEntry)
{
  File *file = myProcess()->OpenFile[directoryDescriptor];
  int fileReadResult = fileRead(file, (char*)dirEntry, 32);

  if (fileReadResult < 0)
  {
    cprintf("file.c->readdir(): File not readable.\n");
    return -1;
  }
  if (fileReadResult == 0)
  {
    cprintf("file.c->readdir(): Read no clusters.\n");
    return -1;
  }

  return 0;
}

int closedir(int directoryDescriptor)
{
  fileClose(myProcess()->OpenFile[directoryDescriptor]);
  myProcess()->OpenFile[directoryDescriptor] = 0;

  return 0;
}

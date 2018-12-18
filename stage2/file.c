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

  cprintf("%s  %d\n", f->Name, f->ReferenceCount);

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
			curproc->OpenFile[fd] = f; // Put the file in the process table?
			return fd;
		}
	}
	return -1;
}

int opendir(char *directory)
{
  // int isSubdirectory = strlen(directory) == 0 ? 0 : 1;
  int isSubdirectory = directory ? 0 : 1; // If directory is not null, assign 1.
  // cprintf("%d", isSubdirectory);
  // cprintf("%d", strlen(directory));
  // cprintf("file.c->opendir()->directory: %s\n", directory);

  char *cwd = myProcess()->Cwd;
  File *file = fsFat12Open(cwd, directory, isSubdirectory);

  if (file == 0)
  {
    cprintf("file.c->opendir()->file == 0\n");
    return 0;
  }

  // int dirDesc = fdAllocate(file);
  // cprintf("file.c->opendir()->dirDesc: %d\n", dirDesc);
  // return dirDesc;

  // cprintf("file.c->opendir(): file->ReferenceCount: %d\n", file->ReferenceCount);

  return fdAllocate(file); // Return file descriptor.
}

int readdir(int directoryDescriptor, struct _DirectoryEntry *dirEntry)
{
  File *file = myProcess()->OpenFile[directoryDescriptor];
  char buffer[32];

  if (fileRead(file, buffer, 32) < 0)
  {
    cprintf("file.c->readdir()->fileRead: -1\n");
    return -1;
  }

  memmove(dirEntry, buffer, 32);

  // cprintf("file.c->readdir(): file->ReferenceCount: %d\n", file->ReferenceCount);

  return 0;
}

int closedir(int directoryDescriptor)
{
  // File *file = myProcess()->OpenFile[directoryDescriptor];
  cprintf("file.c->closedir(): file->ReferenceCount: %d\n", myProcess()->OpenFile[directoryDescriptor]->ReferenceCount);

  // if (myProcess()->OpenFile[directoryDescriptor]) return -1;
  // fileClose(myProcess()->OpenFile[directoryDescriptor]);
  // File *file = myProcess()->OpenFile[directoryDescriptor];

  // fileClose(file);
  fileClose(myProcess()->OpenFile[directoryDescriptor]);
  myProcess()->OpenFile[directoryDescriptor] = 0;

  return 0;
}

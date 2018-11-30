//
// File-system system calls.


#include "types.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "mmu.h"
#include "proc.h"
#include "fs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"
#include "fcntl.h"

// Retrieve an argument to the system call that is an FD
//
// n   = The parameter number (0 = The first parameter, 1 = second parameter, etc).
// pfd = Returns the fd number (set to 0 if not required)
// pf  = Returns the pointer to the File structure (set to 0 if not required).

static int argfd(int n, int *pfd, File **pf)
{
	int fd;
	File *f;

	if (argint(n, &fd) < 0)
	{
		return -1;
	}
	if (fd < 0 || fd >= NOFILE || (f = myProcess()->OpenFile[fd]) == 0)
	{
		return -1;
	}
	if (pfd)
	{
		*pfd = fd;
	}
	if (pf)
	{
		*pf = f;
	}
	return 0;
}

// Allocate a file descriptor for the given file and
// store it in the process table for the current process.

static int fdalloc(File *f)
{
	int fd;
	Process *curproc = myProcess();

	for (fd = 0; fd < NOFILE; fd++) 
	{
		if (curproc->OpenFile[fd] == 0) 
		{
			curproc->OpenFile[fd] = f;
			return fd;
		}
	}
	return -1;
}

// Duplicate a file descriptor

int sys_dup(void)
{
	File *f;
	int fd;

	if (argfd(0, 0, &f) < 0)
	{
		return -1;
	}
	if ((fd = fdalloc(f)) < 0)
	{
		return -1;
	}
	fileDup(f);
	return fd;
}

// Read from file.

int sys_read(void)
{
	File *f;
	int n;
	char *p;

	if (argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argptr(1, &p, n) < 0)
	{
		return -1;
	}
	return fileRead(f, p, n);
}

// Write to file.  Only implemented for pipes and console at present. 

int sys_write(void)
{
	File *f;
	int n;
	char *p;
	if (argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argptr(1, &p, n) < 0)
	{
		return -1;
	}
	return fileWrite(f, p, n);
}

// Close file.

int sys_close(void)
{
	int fd;
	File *f;

	if (argfd(0, &fd, &f) < 0)
	{
		return -1;
	}
	myProcess()->OpenFile[fd] = 0;
	fileClose(f);
	return 0;
}

// Return file stats.  Not fully implemented.

int sys_fstat(void)
{
	File *f;
	Stat *st;

	if (argfd(0, 0, &f) < 0 || argptr(1, (void*)&st, sizeof(*st)) < 0)
	{
		return -1;
	}
	return fileStat(f, st);
}

// Open a file. 

int sys_open(void)
{
	char *path;
	int fd, omode;
	File * f;

	if (argstr(0, &path) < 0 || argint(1, &omode) < 0)
	{
		return -1;
	}
	
	Process *curproc = myProcess();
	// At the moment, only file reading is supported
	f = fsFat12Open(curproc->Cwd, path, 0);
	if (f == 0)
	{
		return -1;
	}
	fd = fdalloc(f);
	if (fd < 0)
	{
		fileClose(f);
		return -1;
	}
	f->Readable = !(omode & O_WRONLY);
	f->Writable = (omode & O_WRONLY) || (omode & O_RDWR);
	return fd;
}

// Execute a program

int sys_exec(void)
{
	char *path, *argv[MAXARG];
	int i;
	uint32_t uargv, uarg;
	char adjustedPath[200];
	
	if (argstr(0, &path) < 0 || argint(1, (int*)&uargv) < 0) 
	{
		return -1;
	}
	memset(argv, 0, sizeof(argv));
	for (i = 0;; i++) 
	{
		if (i >= NELEM(argv))
		{
			return -1;
		}
		if (fetchint(uargv + 4 * i, (int*)&uarg) < 0)
		{
			return -1;
		}
		if (uarg == 0) 
		{
			argv[i] = 0;
			break;
		}
		if (fetchstr(uarg, &argv[i]) < 0)
		{
			return -1;
		}
	}
	int pathLen = strlen(path);
	safestrcpy(adjustedPath, path, 200);
	if (path[pathLen - 4] != '.')
	{
		safestrcpy(adjustedPath + pathLen, ".exe", 200 - pathLen);
		adjustedPath[pathLen + 4] = 0;
	}
	return exec(adjustedPath, argv);
}

int sys_pipe(void)
{
	int *fd;
	File *rf, *wf;
	int fd0, fd1;

	if (argptr(0, (void*)&fd, 2 * sizeof(fd[0])) < 0)
	{
		return -1;
	}
	if (pipealloc(&rf, &wf) < 0)
	{
		return -1;
	}
	fd0 = -1;
	if ((fd0 = fdalloc(rf)) < 0 || (fd1 = fdalloc(wf)) < 0) 
	{
		if (fd0 >= 0)
		{
			myProcess()->OpenFile[fd0] = 0;
		}
		fileClose(rf);
		fileClose(wf);
		return -1;
	}
	fd[0] = fd0;
	fd[1] = fd1;
	return 0;
}

int sys_chdir(void)
{
  char *path;

  if (argstr(0, &path) < 1) 
	{
		return -1;
	}

  int pathLen = strlen(path);
  if (path[pathLen - 1] != '/' && path[pathLen - 1] != '\\')
  {
    // TODO: concatinating the thing.
    path += '/';
  }

  return 0;
}

int sys_getcwd(void)
{


  return 0;
}

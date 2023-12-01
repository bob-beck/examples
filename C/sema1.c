/*
 * Copyright (c) 2008-2013 Bob Beck <beck@obtuse.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Example of system V semaphore use for locking and unlocking access
 * System V semaphores can be shared between *processes* - do *NOT*
 * use them for thread synchronization - they are old and nasty and
 * horrible.
 *
 * You may compare doing this with the use of a posix semaphore
 * in the milk1.c -> milk2.c example in this directory (which
 * is much more sane...
 */

/*
 * compile with cc -o sema1 sema1.c
 * run with ./sema1& ./sema1
 */

#include <err.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define   SEMKEY 87654321L

int semid;

locker(int num)
{
      static struct sembuf lock_it[2] = {
                   0,0,0,
                   0,1,0
      };
      lock_it[0].sem_num = num; /* we TEST semaphore first, that it is 0 */
      lock_it[1].sem_num = num; /* Assuming that works we lock by 
				   raising the semaphore to 1 */
      if (semop(semid,&lock_it[0],2) < 0)
                   return 1;
      else
                   return 0;
}

unlocker(int num)
{
      static struct sembuf unlock_it[1] = {
                   0,-1,0
      };
      unlock_it[0].sem_num = num; /* We just unlock (lower) semaphore */
      if(semop(semid,&unlock_it[0],1) < 0)
                   return 1;
      else
                   return 0;
}

main()
{
	semid=semget(SEMKEY,1,IPC_CREAT|0660);
	if (locker(0) != 0)
		err(1, "Problems locking semaphore");
	else
		printf("Locked semaphore and pid is %d\n", getpid());
	sleep(10);
	if (unlocker(0) !=0)
		err(1, "Problems unlocking semaphore");
	else
		printf("Unlocked semaphore and pid is %d\n", getpid());
	/* this is a gross hack - we sleep for 30 seconds and then remove the
	 * semid */
	/* note this will make any other programs acessing it fail. */
	sleep(30);
	semctl(semid,0,IPC_RMID,NULL); /* remove shmid - fail silently */
	/*
	 * The problem with system V semaphores is that if we don't remove
	 * them, they stay around in the system "forever" or until
	 * the system administrator decides to find them and remove
	 * them with the command "ipcrm". - that's gross and it's why
	 * you should avoid using these things...
	 */
}

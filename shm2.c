/*
 * Copyright (c) 2008 Bob Beck <beck@obtuse.com>
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

/* Read from a shared memory segment, then destroy it */

/* 
 * compile with cc -o shm2 shm2.c
 * run with ./shm2
 */

#include <err.h>
#include <stdio.h>
#include <string.h>


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHMKEY 12345678L

int main()
{
        int shmid;
        char *shm_p;

	/* go find the shm identified by SHMKEY */
        if((shmid=shmget(SHMKEY,10,0)) == -1) 
                err(1, "shmget failed");

	/* attach what I just allocated */
        if((shm_p=shmat(shmid,0,0)) == (void *) -1)
                err(1, "shmat failed");

	/* hopefully, what's there is a C string or bad stuff will happen  */
	printf("Share mem contains \"%s\"\n", shm_p);

	/* detach ourselves */
        if(shmdt(shm_p) == -1) 
                err(1, "Could not detach...");
	/* now REMOVE the shm segment to not leave it lying around */
        if(shmctl(shmid,IPC_RMID,NULL) == -1)
                err(1, "Could not remove segment...");


	return(0);
}

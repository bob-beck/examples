/*
 * Copyright (c) 2013 Bob Beck <beck@obtuse.com>
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
 * milk2.c - a simplistic version of the roommates buying
 * milk problem using pthreads.
 *
 * to compile:
 * cc  -o milk2 milk2.c -lpthread
 *
 * and jut run it as ./milk2
 *
 * Same program as "milk1.c" but this one is protected using
 * a posix semaphore to only allow one thread into the critical
 * section at any one time.
  */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <semaphore.h>

#define NUM_THREADS	5

sem_t mysem; /* My semaphone - a posix semaphore */
int milk = 1;

void *CheckDaMilk(void *arg)
{
     int mynum = *((int *)arg);
     printf ("Hi I am thread %d\n", mynum);
     while(1) {
	   sem_wait(&mysem); /* "wait" (decrement) semaphore mysem */
	   if (milk > 0) {
		   sleep(1);
		   milk--;
		   printf("Now %d milks in the fridge, thread %d buying"
		       " milk..\n", milk, mynum);
		   milk++;
	   }
	   sem_post(&mysem); /* "free" (incremenet)  semaphore mysem */
	   pthread_yield();
    }
}

int
main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	int tids[NUM_THREADS];
	int rc, t;

	/*
	 * Initialize posix semaphore "mysem" to have 1 as the initial
	 * value
	 */
	sem_init(&mysem, 0, 1);

	for(t=0;t<NUM_THREADS;t++){
		tids[t]=t;
		printf("In main: creating thread %d\n", t);
		rc = pthread_create(&threads[t], NULL, CheckDaMilk, &tids[t]);
		if (rc)
			err(-1, "pthread_create failed!");
	}
	pthread_exit(NULL);
}

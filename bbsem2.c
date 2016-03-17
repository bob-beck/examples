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
 * An implementation of general semaphores using pthreads mutexes and
 * condition variables.  Note - this is intended as an example only,
 * You probably shouldn't roll your own semaphores, but rather use
 * the ones from semaphore.h.
 *
 * Unlike bbsem1 - this version does not busy wait.
 * This example is to show you how we use mutexes and
 * condition variables together. The mutex is used to protect
 * the critical section (where we change the semaphore value) and
 * the condition variable is used so we can wait for the value
 * of the semaphore to be non-zero so we can acquire it without
 * busy waiting.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

struct bbsem {
	pthread_mutex_t mutex;
	pthread_cond_t is_nonzero;
	int value;
};
 
struct bbsem *
bbseminit(int val)
{
	struct bbsem *tmp;
	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		return(NULL);
	pthread_cond_init(&tmp->is_nonzero, NULL);
	pthread_mutex_init(&tmp->mutex, NULL);
	tmp->value = val;
	return(tmp);
}

void
bbsemdestroy(struct bbsem *bp)
{
	if (pthread_mutex_destroy(&bp->mutex) != 0)
		err(1, "pthread_mutex_destroy failed:");
	if (pthread_cond_destroy(&bp->is_nonzero) != 0)
		err(1, "pthread_cond_destroy failed:");
	free(bp);
}

void
bbsemwait(struct bbsem *bp)
{
	pthread_mutex_lock(&bp->mutex);
	while (bp->value == 0)
		pthread_cond_wait(&bp->is_nonzero, &bp->mutex);
	/* come back from the wait with the mutex locked */
	bp->value--;
	pthread_mutex_unlock(&bp->mutex);
}

void
bbsemfree(struct bbsem *bp)
{
	pthread_mutex_lock(&bp->mutex);
	bp->value++;
	pthread_cond_signal(&bp->is_nonzero);
	pthread_mutex_unlock(&bp->mutex);
}


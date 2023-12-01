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

/*
 * Let's make a new process with fork()
 */

/*
 * compile with cc -o fork1 fork1.c, run with "./fork1" 
 *
 */
#include <sys/types.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t p;
	int seconds;

	printf("Process %d, starting up\n", getpid());

	p = fork();
	if (p == 0) {
		/* I am the child process */
		printf("Process %d (child), fork returned %d\n", getpid(), p);
		/* Use the process id to seed the random number generator
		 * I do this because it is convenient - and I do *NOT* care
		 * about this random number being unpredictable for use in
		 * any kind of security portion of this software. Please
		 * remember that if I know or could predict the PID the random
		 * number is as predictable as
		 * http://dilbert.com/strips/comic/2001-10-25/
		 */
		srandom(getpid());
		seconds = random() % 5;
		printf("Process %d (child) will be sleeping for %d seconds\n",
		    getpid(), seconds);
		sleep(seconds);
		printf("Process %d (child) now exiting\n", getpid());
		exit(0);
	}
	/* otherwise, I am in the parent */
	/*
	 * the fork could have failed (system resource limits, etc. so we
	 * need to check for that..
	 */
	if (p == -1)
		err(1, "Fork failed! Can't create child!");
	/*
	 * otherwise - p will have the pid of the child.
	 */
	printf("Process %d (parent), fork returned %d\n", getpid(), p);
	srandom(getpid());
	seconds = random() % 5;
	printf("Process %d (parent) will be sleeping for %d seconds\n",
	    getpid(), seconds);
	sleep(seconds);
	printf("Process %d (parent) now exiting\n", getpid());
	/*
	 * for this example, we won't yet worry about what happens when
	 * the child exits before the parent. - since eventually everyone
	 * exits, all works out in the end.
	 */
	exit(0);
}

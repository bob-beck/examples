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
 * Let's make a new process with fork() - and demonstrate that processes
 * inherit file descriptors. This will have a race to see what ends up
 * getting written into the file
 */

/*
 * compile with cc -o fork2 fork2.c, run with "./fork2"
 * you will need to make sure "forktest" exists in the directory you
 * run it in, you can create the file with "touch forktest"
 *
 */
#include <sys/types.h>

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
	pid_t p;
	int seconds;
	int fd, s;
	char *msg;

	fd = open("./forktest", O_WRONLY|O_TRUNC, 0);
	if (fd == -1)
		err(-1, "I can't open ./forktest");

	printf("Process %d, starting up\n", getpid());

	p = fork();
	if (p == 0) {
		/* I am the child process */
		msg = "Blah Blah\n";
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

		/* position at start of file */
		if (lseek(fd, 0, SEEK_SET) == -1)
			err(1, "lseek failed");

		/* let's write the message into the file */
		s = write(fd, msg, strlen(msg));
		if (s == -1)
			err(1, "write failed");
		if (s != strlen(msg))
			errx(1, "short write"); /* we could retry... */
		close(fd);

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
	msg = "Woof Woof\n";
	printf("Process %d (parent), fork returned %d\n", getpid(), p);
	srandom(getpid());
	seconds = random() % 5;
	printf("Process %d (parent) will be sleeping for %d seconds\n",
	    getpid(), seconds);
	sleep(seconds);

	/* position at start of file */
	if (lseek(fd, 0, SEEK_SET) == -1)
		err(1, "lseek failed");

	/* let's write the message into the file */
	s = write(fd, msg, strlen(msg));
	if (s == -1)
		err(1, "write failed");
	if (s != strlen(msg))
		errx(1, "short write"); /* we could retry... */
	close(fd);

	printf("Process %d (parent) now exiting\n", getpid());
	/*
	 * for this example, we won't yet worry about what happens when
	 * the child exits before the parent. - since eventually everyone
	 * exits, all works out in the end.
	 */
	exit(0);
}

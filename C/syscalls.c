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
 * System Calls are not without overhead - an example. 
 */

/*
 * compile with "cc -o syscalls syscalls.c".
 *
 * this program writes a whole bunch of 'a's into a file. 
 * 
 * how efficiently it does it depends on how many you let it write
 * with each system call - it's one parameter should be the write
 * size which can be between 1 and 8192 characters to write each time.
 * 
 * in order to see the overhead try the following, and use the 
 * /usr/bin/time command to measure how much time the program takes
 * during it's execution.
 * 
 * /usr/bin/time ./syscalls 4000
 * /usr/bin/time ./syscalls 1	
 *
 * Notice the difference in "sys" time taken - that's system
 * call processing time. We still wrote the same amount of data
 * we just did it with fewer system calls. 
 * 
 * play around with a few different values of the write size
 * to see what a difference it makes.
 */

#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_WRITE 8192

void usage()
{
	extern char * __progname;
	fprintf(stderr, "usage: %s size\n", __progname);
	exit(1);
}	

int main(int argc, char *argv[])
{
  	char filename[] = "/tmp/syscall.XXXXXXXXXX";
	int count = 5000000; /* approximate bytes we'll write */
	u_long wsize;
	char *buf, *ep;
	ssize_t w;
	int i, j, fd;

	/*
	 * grab a parameter from the command line 	
	 * so we can specify how big a write to do.
	 */
	if (argc != 2)
		usage();
	errno = 0;
        wsize = strtoul(argv[1], &ep, 10);
        if (*argv[1] == '\0' || *ep != '\0') {
		/* parameter wasn't a number, or was empty */
		fprintf(stderr, "%s - not a number\n", argv[1]);
		usage();
	}
        if ((errno == ERANGE && wsize == ULONG_MAX) || (wsize > MAX_WRITE)) {
		/* It's a number, but can't fit in an unsigned long 
		 * or it's just bigger than we want to allow
 		 */
		fprintf(stderr, "%s - value out of range\n", argv[1]);
		usage();	
	}

	/* allocate a buffer, and fill it with the letter 'a' */
	buf = malloc(wsize * sizeof(char));
	if (buf == NULL)
		err(1, "malloc failed");
	memset(buf, 'a', wsize);

	/* open up a temporary file to write into */
	if ((fd = mkstemp(filename)) == -1)
		err(1, "can't open a temporary file");

	fprintf(stderr, "Writing %d 'a' to my output\n", count);
	
	w = 0;
	j = 0;
	for (i=0; i < count; i+=w) {
		if ((w = write(fd, buf, wsize)) == -1) {
			if (errno != EINTR)
				err(1, "write failed"); 
			else
				w = 0;
		}
		j++;
	}	
	printf("I did %d system calls\n", j);
	unlink(filename); /* remove file when we're done */
	return(0);
}

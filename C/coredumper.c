/* Originally written by Theo de Raadt <deraadt@openbsd.org>
 * Modified by Bob Beck for instructional use.
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


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <err.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHUNKS	1000

char *dir;
int nproc;

/*
 * coredumper - dump nproc huge core files in parallel
 *
 * compile with
 * cc -o coredumper coredumper.c
 *
 * run with
 *
 * ./coredumper size nproc directory
 *
 * where "size" is the size of each core image to dump in megabytes
 * "nproc" is the number of core dumps to do in parallel, and
 * "directory" is the name of a directory where the places to dump cores
 *             can be created.
 *
 * This is a wonderfully evil program. The main program allocates "size"
 * megabytes of memory, and touches every page it allocates.
 *
 * it then forks nproc copies of itself, changes it's working directory
 * and calls abort in each one.
 *
 * The result is nproc processes all dumping "size" megabytes of
 * core images simultaneously. This program can seriously stress
 * a unix machine. You should not run it on a system you share with
 * anyone else - i.e. run it on your own machine, and don't cry if it
 * locks up or crashes.
 *
 * start with "coredumper 10 10 crap"
 * and proress to "coredumper 100 100 crap" or something like that
 * but beware of your memory limitations - if you start swapping these
 * you'll hurt a lot on many systems.
 *
 * note that some systems have issues where the will not actually write
 * out more than one core. technically this is a bug, but it may
 * prevent worse problems....
 *
 * You should likely *NOT* run this on a machine that is owned by
 * by someone other than yourself. In particular the university
 * could frown on you running this on a lab or other university computer
 * so you should not do that. 
 *
 */

int
main(int argc, char *argv[])
{
	char string[200];
	char *p;
	int i, ms, size, count, nbpg = getpagesize();

	if (argc != 4) {
		fprintf(stderr, "usage: coredumper size nproc dir\n");
		exit(1);
	}
	size = atoi(argv[1]);
	nproc = atoi(argv[2]);
	dir = argv[3];

	ms = CHUNKS * nbpg;
	count = (size * 1048576) / ms;

	while (count--) {
		p = malloc(ms);
		if (p == NULL) /* if we can't allocate any more, dump anyway */
			goto dump;
		for (i = 0; i < ms; i += nbpg)
			p[i] = 1; /* touch each page so it is dirty. */
	}
 dump:
	/*
	 * We do a printf this way, because we anticipate we might be
	 * here when we are out of memory and malloc won't give us any
	 * more. (see the loop above) so we use the pre-allocated
	 * variable (string) on the stack to sprintf into, and then
	 * call write to write it out, instead of directly calling printf.
	 * This is because we know printf will itself attempt to allocate
	 * memory with malloc and it could fail at this point.
	 */
	snprintf(string, sizeof string, "crashing...\n");
	write(STDOUT_FILENO, string, strlen(string));

	/* now comes the evil - we fork nproc copies of ourselves... */
	for (i = 0; i < nproc; i++) {
		switch (fork()) {
		case 0:
			/*
			 * In each copy of ourselves
			 * we first make a directory...
			 */
			snprintf(string, sizeof string,
			    "%s/crash-%d", dir, i);
			if (mkdir(string, 0770) != 0)
				err(1, "mkdir %s", string);
			/*
			 * We then change into that directory that we
			 * made...
			 */
			if (chdir(string) != 0)
				err(1, "chdir %s", string);
			/*
			 * We write a message that we are going to
			 * crash...
			 */
			snprintf(string, sizeof string,
			    "%d to %s/crash-%d\n", getpid(), dir, i);
			write(STDOUT_FILENO, string, strlen(string));
			/*
			 * we now call abort, to make ourselves dump core.
			 */
			abort();
			break;
		default:
			break;
		}
	}

	/*
	 * We now wait for all of our nproc copies of ourself
	 * to finish dumping core...
	 */
	snprintf(string, sizeof string, "waiting...\n");
	write(STDOUT_FILENO, string, strlen(string));
	for (i = 0; i < nproc; i++) {
		int j;
		wait(&j);
		if (j && WCOREDUMP(j))
			snprintf(string, sizeof string,
			    "finished %d with core dump\n", i);
		else
			snprintf(string, sizeof string,
			    "finished %d with no core dump\n", i);
		write(STDOUT_FILENO, string, strlen(string));
	}
	/*
	 * And print out a helpful message that we have successfully
	 * finished.
	 */
	snprintf(string, sizeof string, "done.\n");
	write(STDOUT_FILENO, string, strlen(string));
	return(0);
}

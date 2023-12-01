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
 * Signals example number 1
 */

/* 
 * compile with cc -o sig1 sig1.c, run with "./sig1 3" 
 *
 */

#include <err.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static sig_atomic_t do_reread;
static sig_atomic_t reset_count;

static struct sigaction sa, oldsa;

void usage()
{
	extern char * __progname;
	fprintf(stderr, "usage: %s resets\n", __progname);
	exit(1);
}	


static void handler(int signum) {
	reset_count++;
	do_reread = 1;
}	

char * getaline(char *buf, size_t size)
{
	printf("\nEnter a new string: ");
	if (fgets(buf, size, stdin) != NULL) {
		char *p;
		if ((p = strchr(buf, '\n')) == NULL) {
			fprintf(stderr, "input line too long\n");
			return(NULL);
		}
		*p = '\0';
		return(buf);
	} else
		return(NULL);
}


int main(int argc, char * argv[])
{
	char *ep, *buffer = NULL;
	char buf[80];
	unsigned long resets;

	/*
	 * grab a parameter from the command line 	
	 * so we can specify how many times we can be reset.
	 */
	if (argc != 2)
		usage();
	errno = 0;
        resets = strtoul(argv[1], &ep, 10);
        if (*argv[1] == '\0' || *ep != '\0') {
		/* parameter wasn't a number, or was empty */
		fprintf(stderr, "%s - not a number\n", argv[1]);
		usage();
	}
        if (errno == ERANGE && resets == ULONG_MAX) {
		/* It's a number, but can't fit in an unsigned long */
		fprintf(stderr, "%s - value out of range\n", argv[1]);
		usage();	
	}

	/* set up a signal handler to catch SIGINT - which is by default
	 * generated from Control-C on the keyboard. - we save whatever
	 * the old action was for SIGINT - and set up a new action to 
	 * call "handler" when we get a SIGINT.
	 */
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, NULL, &oldsa) == -1)
		err(1, "can't save old sigaction");
	if (sigaction(SIGINT, &sa, NULL) == -1)
		err(1, "can't do new sigaction");

	printf ("Hi there - I keep printing out a buffer...\n");
	printf ("You can change what the buffer is by pressing Ctrl-C.\n");
	printf ("You may change the buffer %ld times, after that, Ctrl-C\n",
	    resets);
	printf ("will make me exit.\n");
	while(1) {
		do_reread = 0;
		sleep(1);
		if (reset_count == resets) {
			/* restore old SIGINT signal handler */
			if (sigaction(SIGINT, &oldsa, NULL) == -1)
				err(1, "can't restore old signal handler");
			/* so now, Control-C will do the thing it used to do */
		}
		if (do_reread) {
			buffer = getaline(buf, sizeof(buf));
			do_reread = 0;
		}
		if (buffer == NULL)
			printf("my buffer is empty\n");
		else 
			printf("my buffer contains: %s\n", buffer);
		
	}
}

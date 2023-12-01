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

/* A little pointers and strings review */

/*
 * compile with "cc -o p1 p1.c", run with "./mem"
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char *fred;
char sam[]="I'm a lumberjack and I'm OK";

int main()
{
	char *george;
	char bob[]="I sleep all night and I work all day";

	/* an exercise in C variables, and pointers, and strings... */
	/* think about why this is all so.... */

	printf ("Variable fred, is at %p, size %d, contains value %p\n",
	    &fred, sizeof(fred), fred);
	printf ("Variable sam, is at %p, size %d, contains value %p, a string of length %d: %s\n",
	    &sam, sizeof(sam), sam, strlen(sam), sam);

	printf ("Variable george, is at %p, size %d, contains value %p\n", 
	    &george, sizeof(george), george);
	printf ("Variable bob, is at %p, size %d, contains value %p, a string of length %d: %s\n",
	    &bob, sizeof(bob), bob, strlen(bob), bob);

	george = sam;
	fred = bob;

	printf ("Variable george, is at %p, size %d, contains value %p, a string of length %d: %s\n", 
	    &george, sizeof(george), george, strlen(george), george);
	printf ("Variable fred, is at %p, size %d, contains value %p, a string of length %d: %s\n",
	     &fred, sizeof(fred), fred, strlen(fred), fred);
	exit(0);
}
	

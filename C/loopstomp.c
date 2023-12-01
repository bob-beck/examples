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
 * a simple stomper - let's whack the loop variable - note this
 * program can run forever - thanks to i getting clobbered.
 */

/*
 * compile with "cc -o loopstomp loopstomp.c", run with "./loopstomp"
 */

#include <stdio.h>
#include <stdlib.h>

/* may need to be adjusted depending on os and compiler... 
 * not guaranteed to work on some os's (especially OpenBSD)
 * these constants work fine in the cs lab.
 */
#define LEN 4 
#define END (LEN * 2)

int main()
{
	int i;
	int array[LEN];

	i = 0;
	while (i < END) {
		array[i] = 0;
		i++;
		/* you could add a few printf %p's in here to print out
		 * &i, and array+i to see what's going on... 
		 */
	}
	return(0);
}
	
	

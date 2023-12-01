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
 *  how far can we stomp? - this program should segfault, but it 
 *  will most likely spit out a lot more than 80 lines of output.
 *  notice you don't die immediately after the end of the array. 
 */

/*
 * compile with "cc -o seg1 seg1.c", run with "./seg1"
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
	char carr[80];
	int i;
	
	printf ("let's go a stomping... starting at %p, which ends at %p\n",
	    carr, carr + 80);

	while(1) {
		printf("stomping at %p\n", carr + i);
		carr[i++] = 'a';
	}
}
	
	

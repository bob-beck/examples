/*
 * Copyright (c) 2012 Bob Beck <beck@obtuse.com>
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
 * Integers have finite sizes.
 */

/*
 * compile this program twice, once with
 *
 * gcc -o intloop1 intloop1.c
 *
 * and then
 *
 * gcc -DBIGIJ -o intloop2 intloop2.c
 *
 * be sure you understand why they behave differently.
 */



#include <stdio.h>

int main()
{
#ifndef BIGIJ
	unsigned int i, j;
#else
	unsigned long i, j;
#endif

        printf("starting\n");
	for(j = i = 0; i < 0xffffffff; i+=4096) {
		if (i < j)
			printf("WTF? i=%u, j=%u\n", i, j);
                j = i;
        }
        printf("done\n");
}
  

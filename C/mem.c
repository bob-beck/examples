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
 * A simple example of where stuff gets put.  
 */

/*
 * compile with "cc -o mem mem.c", run with "./mem"
 */

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int aglobal;

int main()
{
	int iarr[1024];
	int *iarr2;

	iarr2 = malloc(1024 * sizeof(int));
	if (iarr2 == NULL)
		err(1, "malloc failed");
	
	printf("The address of main is %p\n", &main);
	printf("The address of aglobal is %p\n", &aglobal);
	printf("The address of iarr on main's stack is %p\n", iarr);
	printf("The address returned by malloc (heap) is %p\n", iarr2);
	printf("The address of the errno variable is %p\n", &errno);
	printf("The address of the err function is %p\n", &err);
	return(0);
}
	
	

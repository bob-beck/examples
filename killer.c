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
 * In unix, processes can kill other processes...
 */

/* 
 * compile with cc -o killer killer.c, run with "./killer&" 
 * note the "&" to run this program in the background.
 *
 * This program makes a child who becomes very busy. After 10
 * seconds the parent kill()'s the child. you should see 
 * a busy child for the first 10 seconds in ps -x, then 
 * only the parent for the next 10 seconds.
 * 
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t p;
	int i;

	printf("Process %d, starting up\n", getpid());

	p = fork();
	if (p == 0)
		while(1); /* run forever in a tight loop! */

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


	printf("You have 10 seconds to look at the process table\n");
	printf("with the command \"ps -x\"\n");
	while (i++ < 10)
		sleep(1);

	printf("Killing off child %d\n", p);
	if (kill(p, SIGTERM) == -1)
		err(1, "can't kill %d", p);

	printf("You have another 10 seconds to look at the process table\n");
	printf("with the command \"ps -x\"\n");
	i = 0;
	while (i++ < 10) {
		/* note, if you remove this wait, the killed child will show
		 * up as a zombie until the parent exits.. 
		 */
		waitpid(WAIT_ANY, NULL, WNOHANG);
		sleep(1);
	}
	exit(0);
}

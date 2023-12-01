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
 * Let's check out zombies, and waiting for our kids when they die.
 */

/* 
 * compile with cc -o zombie zombie.c, run with "./zombie&" 
 * note the "&" to run this program in the background.
 *
 * This program leaves a zombie for 30 seconds before exiting
 * if the environment variable "FLUFFY" is not set. It will 
 * wait() for the child exiting if FLUFFY is set. So try it
 * two ways.
 * 
 * unset FLUFFY
 * ./zombie &
 * 
 * export FLUFFY=fluff
 * ./zombie &
 * 
 */
#include <sys/types.h>
#include <sys/wait.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t p;
	int i = 0;
	int reap = 0; /* should we wait, to get rid of the zombie? */
	
	printf("Process %d, starting up\n", getpid());

	p = fork();
	if (p == 0) {
		/* I am the child process */
		exit(0); /* A responsible parent knows when I go away */
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

	if (getenv("FLUFFY") != NULL) {
		reap = 1;
		printf("I will be reaping my zombie children....\n");
	} else
		printf("I will be ignoring my zombie children....\n");

	printf("You have 10 seconds to look at the process table\n");
	printf("with the command \"ps -x\"\n");
	while (i++ < 10) {
		if (reap)
			waitpid(WAIT_ANY, NULL, WNOHANG);
		sleep(1);
	}
	exit(0);
}

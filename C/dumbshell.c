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
 * A very dumb unix shell - does nothing like command history or tty
 * management. all it does it run commands in a child. - this demonstrates
 * how a shell accepts commands on the command line, and runs them
 * using the execution path, doing a fork() and then an exec() to 
 * run the command. 
 */

/* 
 * compile with cc -o dumbshell dumbshell.c, run with "./dumbshell" 
 * you get out of it by typing "exit".
 *
 */
#include <sys/types.h>

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


char * getaline(char *buf, size_t size)
{
	printf("dumbshell>");
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

int runcmd(char *argv[]) {
	pid_t p;
	int status;

	if ((p = fork()) == 0) {
		/*
		 * I am the child process.
		 * we execvp the command from the command line
		 */
		if (execvp(argv[0], argv) == -1) {
			warn("command exec failed:");
			status = -1;
		}
	} else
		waitpid(p, &status, 0);
	return(status);
}  


#define MAX 80

int main()
{
	char *cp = NULL;
	char buf[MAX];
	char *tokens[MAX], *p, *last;

	/*
	 * A real shell is not quite as simple as this,
	 * it needs to handle signals, and other things.
	 * In particular, it can't change directories! ("cd" is 
	 * a builtin command to the usual unix shells).
	 */
	while(1) {
		int i = 0;
		cp = getaline(buf, sizeof(buf));
		if (cp == NULL || (strcmp(cp, "exit") == 0)) 
			err(0, "dumbshell exiting now\n");
		/*
		 * split the command given up into individual strings 
		 * so instead of buf containg one string of words 
		 * separated by spaces, we have tokens, an array
		 * of strings where each string is one token (word)
		 */
		for ((p = strtok_r(cp, " ", &last)); p;
	            (p = strtok_r(NULL, " ", &last))) {
			if (i < MAX - 1)
                        	tokens[i++] = p;
	        }
		runcmd(tokens);
	}
	return(0);
}

/*
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

/*
 * iagree.c - modified in class from the c379 "client.c" to show
 * how we can send useful stuff to a web server with sockets.
 *
 * compile with
 * gcc -o iagree iagree.c
 * or
 * gcc -DDEBUG -o iagree iagree.c
 *
 * In this case what we did was capture the http POST request that
 * is sent to the annoying captive proxy server on the "Guest@UofA"
 * network - the result that we developed in class from client.c
 * is a C program that can be run on the command line or in a shell
 * script that will attempt to send the "yes I agree" request.
 *
 * You could wrap the body of the main funciton in a while loop
 * that repeatedly tries and sleeps, and change the SIGALRM handler
 * to sigsetjmp/siglongjmp to sleep and retry if anything times out.
 *
 * Alternatively, you could simply wrapper this progam in a simple
 * shell script on your machine:
 *
 * #!/bin/sh
 *
 * while :; do
 *      iagree
 *      sleep 5
 * done
 *
 * if you compile the program into iagree and run the above program in
 * a shell script you will then happily be authenticated any time you
 * wander onto Guest@UofA without having to click the annoying button
 *
 * Of course, since this is C code, you could always compile it into
 * your dhcp server, or other system program to fire when it sees
 * certain networks. The posisbilities are endless if you start reading
 * your system code...
 *
 * integrating on your own laptop, or integrating onto your own laptop
 * is left as an exercise to the reader.
 */

#include <arpa/inet.h>

#include <netinet/in.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <err.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * The http 1.1 POST request we are interested in sending.  in this
 * case we got this from a tcpdump -A watching the traffic to the
 * 1.1.1.1 server as we hit the "I agree" button in our web browser.
 * (I used 'tcpdump -i <myinterface> -n -A -s 1500 host 1.1.1.1' for
 * this) We looked at it and put it here as a big ugly C string
 * because we are lazy. If we wanted to make a generic button pusher
 * we could put these in files and read them and try multiple
 * servers.. think about modifying the program to include the
 * acceptance string for your local starbucks, and other annoying
 * captive proxy networks.
 */
char iagree[] = "POST /login.html HTTP/1.1\r\nUser-Agent: curl/7.28.1\r\nHost: 1.1.1.1\r\nAccept: */*\r\nContent-Length: 15\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nbuttonClicked=4\r\n";

/* A signal handler so we exit after we get a SIGALRM */
struct sigaction sa;
static void alarmhandler(int signum) {
	/* signal handler for SIGALARM */
	exit(1);
}

int main(int argc, char *argv[])
{
	ssize_t written, w;
	struct sockaddr_in server_sa;
	char buffer[256];
	size_t maxread;
	ssize_t r, rc;
	u_short port;
	int sd;

	/*
	 * first set up "server_sa" to be the location of the server
	 * we know the server we are interested in is "1.1.1.1", and
	 * we know it is port 80, so we just hardcode that right in
	 * here.
	 */
	port = 80;
	memset(&server_sa, 0, sizeof(server_sa));
	server_sa.sin_family = AF_INET;
	server_sa.sin_port = htons(port);
	server_sa.sin_addr.s_addr = inet_addr("1.1.1.1");
	if (server_sa.sin_addr.s_addr == INADDR_NONE) {
		fprintf(stderr, "Invalid IP address %s\n", argv[1]);
		exit(1);
	}

	/* ok now get a socket. we don't care where... */
	if ((sd=socket(AF_INET,SOCK_STREAM,0)) == -1)
		err(1, "socket failed");

	/* let's set ourselves up to get a signal after 3 seconds */
	/*
	 * set up the sa to call "alarmhandler" above, and call sigaction
	 * to set it up as the signal handler for SIGALRM, then call "alarm"
	 * to give us a signal in 3 seconds.
	 */
	sa.sa_handler = alarmhandler;
        sigemptyset(&sa.sa_mask);
        if (sigaction(SIGALRM, &sa, NULL) == -1)
                err(1, "sigaction failed");
	alarm(3);

	/* connect the socket to the server described in "server_sa" */
	if (connect(sd, (struct sockaddr *)&server_sa, sizeof(server_sa))
	    == -1)
		err(1, "connect failed");

	/*
	 * finally, we are connected.
	 */

	/*
	 * Write the message to the client, being sure to
	 * handle a short write, or being interrupted by
	 * a signal before we could write anything.
	 */
	w = 0;
	written = 0;
	while (written < strlen(iagree)) {
		w = write(sd, iagree + written,
		    strlen(iagree) - written);
		if (w == -1) {
			if (errno != EINTR)
				err(1, "write failed");
		}
		else
			written += w;
	}

	/*
	 * Now let's read the answer back in case the
	 * server cares.  if we compile with -DDEBUG
	 * we can print out the answer so we can look at it
	 */

	r = -1;
	rc = 0;
	maxread = sizeof(buffer) - 1; /* leave room for a 0 byte */
	while ((r != 0) && rc < maxread) {
		r = read(sd, buffer + rc, maxread - rc);
		if (r == -1) {
			if (errno != EINTR)
				err(1, "read failed");
		} else
			rc += r;
	}
	/*
	 * we must make absolutely sure buffer has a terminating 0 byte
	 * if we are to use it as a C string
	 */
	buffer[rc] = '\0';
#ifdef DEBUG
	printf("Server sent:  %s",buffer);
#endif
	alarm(0);  /* cancel our alarm.. we made it through */
	close(sd);
	return(0);
}

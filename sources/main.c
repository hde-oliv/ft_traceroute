#include <stdio.h>

#include "ft_traceroute.h"

int clear(trace_t *t, int i) {
	freeaddrinfo(t->result);
	close(t->fd);

	return i;
}

int main(int argc, char **argv) {
	trace_t t = { 0 };

	if (argc != 2) {
		fprintf(stderr, "error on number of arguments\n");
		return 1;
	}

	char *arg = argv[1];

	if (!ft_strncmp(arg, "--help", 7)) {
		printf("Usage:\n");
		printf("  traceroute host\n\n");
		printf("Options:\n");
		printf("  --help          Read this help and exit:\n");
		printf("\n");
		printf("Arguments:\n");
		printf("    host          The host to traceroute to\n\n");
		return 0;
	} else {
		t.hostname = arg;
	}

	if (setup_socket(&t)) {
		fprintf(stderr, "error creating socket\n");
		return clear(&t, 1);
	}

	if (run_loop(&t)) {
		fprintf(stderr, "error running loop\n");
		return clear(&t, 1);
	}

	return clear(&t, 0);
}

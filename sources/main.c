#include "ft_traceroute.h"

int clear(trace_t *t, int i) {
	freeaddrinfo(t->result);
	close(t->fd);

	return i;
}

int main(void) {
	trace_t t  = { 0 };
	t.hostname = "google.com";

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

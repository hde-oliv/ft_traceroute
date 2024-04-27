#include "ft_traceroute.h"

int main(void) {
	trace_t t;

	t.hostname = "google.com";

	setup_socket(&t);

	printf("Got socket\n");

	close(t.fd);
	return 0;
}

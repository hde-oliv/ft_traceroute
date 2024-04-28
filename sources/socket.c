#include <string.h>

#include "ft_traceroute.h"

int setup_socket(trace_t *t) {
	int err;

	struct addrinfo	 hints;
	struct addrinfo *result = t->result;
	struct addrinfo *rp		= t->rp;

	hints.ai_family	  = AF_UNSPEC;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_flags	  = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_ICMP;

	err = getaddrinfo(t->hostname, NULL, &hints, &result);
	if (err) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
		return 1;
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		if (rp->ai_family == AF_INET) {
			struct sockaddr_in *tmp = (void *)rp->ai_addr;
			t->ip_str				= inet_ntoa(tmp->sin_addr);

			t->fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

			if (t->fd == -1) {
				fprintf(stderr, "socket: %s\n", strerror(errno));
				return 1;
			}
			break;
		}
	}

	if (rp == NULL) {
		fprintf(stderr, "could not connect with any address\n");
		return 1;
	}

	t->result = result;
	t->rp	  = rp;

	return 0;
}

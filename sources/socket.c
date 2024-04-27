#include "ft_traceroute.h"

void setup_socket(trace_t *t) {
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
		exit(1);
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		if (rp->ai_family == AF_INET) {
			struct sockaddr_in *tmp = (void *)rp->ai_addr;
			t->ip_str				= inet_ntoa(tmp->sin_addr);

			t->fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

			if (t->fd == -1) {
				perror("socket");
				exit(1);
			}
			break;
		}
	}

	if (rp == NULL) {
		fprintf(stderr, "could not connect with any address\n");
		exit(1);
	}
}

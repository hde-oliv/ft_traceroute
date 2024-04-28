#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ft_traceroute.h"

static void configure_socket(trace_t *t, int ttl) { setsockopt(t->fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)); }

int run_loop(trace_t *t) {
	struct timeval start, end = { 0 };
	char		   packet[P_SIZE];
	char		   response[M_SIZE];

	int flag = 1;

	print_init(t);
	for (int i = 1; (flag != 0) && (i <= M_HOPS); i++) {
		configure_socket(t, i);

		int		c	  = 0;
		batch_t batch = { 0 };

		while (c != 3) {
			setup_packet(&packet, P_SIZE, i);

			if (send_packet(t, &start, &packet, P_SIZE)) return 1;

			if (read_packet(t, &end, &response, M_SIZE)) return 1;

			store_packet(&response, &batch, c, &start, &end);

			c++;
		}

		print_response(i, &batch);
	}

	return 0;
}

#include "ft_traceroute.h"

static void configure_socket(trace_t *t, int ttl) { setsockopt(t->fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)); }

static void check_batch(batch_t *t, char *hostip, int *flag) {
	char		 ipstr[16];
	unsigned int ip = t->sum[0].ip;

	int a = (ip) & 0x000000FF;
	int b = (ip >> 8) & 0x000000FF;
	int c = (ip >> 16) & 0x000000FF;
	int d = (ip >> 24);

	snprintf(ipstr, 16, "%d.%d.%d.%d", a, b, c, d);

	if (!ft_strncmp(hostip, ipstr, 16)) *flag = 0;
}

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
		check_batch(&batch, t->ip_str, &flag);
	}

	return 0;
}

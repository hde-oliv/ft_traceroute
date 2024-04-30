#include <netinet/in.h>
#include <stdio.h>

#include "ft_traceroute.h"

int run_loop(trace_t *t) {
	struct timeval start, end = { 0 };
	char		   packet[P_SIZE];
	char		   response[M_SIZE];

	int flag = 1;
	int err;

	print_init(t);
	for (int i = 1; (flag != 0) && (i <= M_HOPS); i++) {
		configure_socket(t, i);

		int		c	  = 0;
		batch_t batch = { 0 };

		struct sockaddr_in res_addr;
		unsigned int	   res_len = sizeof(res_addr);
		char			   res_ip[INET6_ADDRSTRLEN];

		while (c != 3) {
			setup_packet(&packet, P_SIZE, i);

			gettimeofday(&start, NULL);
			err = sendto(t->fd, packet, P_SIZE, 0, t->rp->ai_addr, t->rp->ai_addrlen);

			if (err < 0) {
				fprintf(stderr, "sendto: %s", strerror(errno));
				return 1;
			}

			err = recvfrom(t->fd, response, M_SIZE, 0, (struct sockaddr *)&res_addr, &res_len);

			gettimeofday(&end, NULL);
			if (err < 0) {
				snprintf(res_ip, 2, "*");
			}

			inet_ntop(res_addr.sin_family, &res_addr.sin_addr, res_ip, sizeof(res_addr));

			store_packet(&response, &batch, res_ip, c, &start, &end);

			c++;
		}

		print_response(i, &batch);
		if (!ft_strncmp(t->ip_str, batch.sum[0].ip, 16)) flag = 0;
	}

	return 0;
}

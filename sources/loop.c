#include "ft_traceroute.h"

#define M_HOPS 30
#define P_SIZE 64

void run_loop(trace_t *t) {
	struct timeval start, end;
	char		   packet[P_SIZE];
	char		   response[P_SIZE + 20];

	int flag = 1;
	for (int i = 1; (flag != 0) && (i <= M_HOPS); i++) {
		setup_packet();
		send_packet();
		read_packet();
		validate_packet();
		print_response();
	}
}

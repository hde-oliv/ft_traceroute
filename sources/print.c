#include "ft_traceroute.h"

void print_response(int i, batch_t *b) {
	(void)b;
	printf(GREEN " %d " RESET, i);

	for (int idx = 0; idx < 3; idx++) {
		sum_t *t = &b->sum[idx];

		int d = (t->ip >> 24);
		int c = (t->ip >> 16) & 0x000000FF;
		int b = (t->ip >> 8) & 0x000000FF;
		int a = (t->ip) & 0x000000FF;

		printf(RED " %d.%d.%d.%d  " RESET, a, b, c, d);
		printf(GREEN "%.3lf" RESET " ms", t->time);
	}
	printf("\n");
}

void print_init(trace_t *t) {
	printf("traceroute to " RED "%s " RESET "(" CYAN "%s" RESET "), " GREEN "30 " RESET "hops max, " GREEN "60 " RESET
		   "byte packets\n",
		   t->hostname, t->ip_str);
}

void print_bytes(int size, void *ptr) {
	unsigned char *p = ptr;
	int			   i;

	for (i = 0; i < size; i++) {
		printf("%02hhX ", p[i]);
	}
	printf("\n");
}

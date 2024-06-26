#include <netinet/in.h>
#include <stdio.h>

#include "ft_traceroute.h"

void print_response(int i, batch_t *b) {
	printf(YELLOW " %d " RESET, i);

	char ipstr[INET6_ADDRSTRLEN];
	char last[INET6_ADDRSTRLEN];

	for (int idx = 0; idx < 3; idx++) {
		sum_t *t = &b->sum[idx];

		snprintf(ipstr, INET6_ADDRSTRLEN, "%s", t->ip);

		if (ft_strncmp(ipstr, last, 16)) {
			printf(" %s  ", ipstr);
		}

		printf(GREEN "%.3f" RESET " ms  ", t->time);

		snprintf(last, INET6_ADDRSTRLEN, "%s", ipstr);
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

int ft_strncmp(const char *s1, const char *s2, size_t n) {
	size_t i;

	i = 0;
	while ((s1[i] == s2[i]) && (s1[i] != '\0') && (s2[i] != '\0')) i++;
	if (i < n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	else
		return (0);
}

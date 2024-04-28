#include <netinet/ip.h>

#include "ft_traceroute.h"

unsigned short get_cksum(unsigned short *b, int len) {
	unsigned sum;
	for (sum = 0; len > 1; len -= 2) {
		sum += *b++;
	}

	if (len == 1) {
		sum += *(unsigned char *)b;
	}

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return ~sum;
}

void fill_packet(void *p, int p_size) {
	struct timeval ts;

	gettimeofday(&ts, NULL);

	memcpy(p, &ts.tv_sec, 8);
	p += 8;
	memcpy(p, &ts.tv_usec, 8);
	p += 8;

	p_size = p_size - 8 - 8 - 8;

	for (int i = 0; i < p_size; i++) {
		*(char *)p++ = i;
	}
}

void setup_packet(void *p, size_t p_siz, short seq) {
	icmp_t *h = (icmp_t *)p;

	h->type	 = ICMP_ECHO;
	h->code	 = 0;
	h->id	 = (short)getpid();
	h->seq	 = htons(seq);
	h->cksum = 0;

	fill_packet(&h->data, p_siz);

	h->cksum = get_cksum((unsigned short *)p, p_siz);
}

int validate_packet(void *s, void *r, short p_siz) {
	icmp_t *rp = (r + 20);
	icmp_t *sp = s;

	(void)rp;
	(void)sp;
	(void)p_siz;
	int err = 0;

	/* if (rp->type != ICMP_ECHOREPLY && rp->type != ICMP_TIME_EXCEEDED) { */
	/* 	fprintf(stderr, "validate: invalid type %hu\n", rp->type); */
	/* 	err = 1; */
	/* } */
	/* if (rp->code != 0) { */
	/* 	fprintf(stderr, "validate: invalid code\n"); */
	/* 	err = 1; */
	/* } */
	/* if (rp->id != sp->id) { */
	/* 	fprintf(stderr, "validate: invalid id %hu | %hu\n", rp->id, sp->id); */
	/* 	err = 1; */
	/* } */
	/* if (rp->seq != sp->seq) { */
	/* 	fprintf(stderr, "validate: invalid seq\n"); */
	/* 	err = 1; */
	/* } */
	/* if (rp->code != get_cksum((unsigned short *)rp, p_siz)) { */
	/* 	fprintf(stderr, "validate: invalid cksum\n"); */
	/* 	err = 1; */
	/* } */

	return err;
}

int send_packet(trace_t *t, void *time, void *packet, int p_siz) {
	struct timeval *ts = time;
	gettimeofday(ts, NULL);

	int err;
	err = sendto(t->fd, packet, p_siz, 0, t->rp->ai_addr, t->rp->ai_addrlen);
	if (err < 0) {
		fprintf(stderr, "sendto: %s", strerror(errno));
		return 1;
	}

	return 0;
}

int read_packet(trace_t *t, void *time, void *packet, int p_siz) {
	struct timeval *ts = time;
	gettimeofday(ts, NULL);

	int err;
	err = recvfrom(t->fd, packet, p_siz, 0, NULL, NULL);
	if (err < 0) {
		fprintf(stderr, "recvfrom: %s", strerror(errno));
		return 1;
	}

	return 0;
}

void store_packet(void *p, batch_t *b, int i, void *s, void *e) {
	sum_t		 *sum  = &b->sum[i];
	struct iphdr *ip   = p;
	icmp_t		 *icmp = p + 20;

	sum->ip	  = ip->saddr;
	sum->type = icmp->type;

	struct timeval *start = s;
	struct timeval *end	  = e;

	double sts = ((start->tv_sec * 1000) + (start->tv_usec / 1000.0));
	double ets = ((end->tv_sec * 1000) + (end->tv_usec / 1000.0));
	sum->time  = ets - sts;
}

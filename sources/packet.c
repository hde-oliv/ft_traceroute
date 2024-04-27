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

void fill_packet(char *p, int p_size) {
	struct timeval ts;

	gettimeofday(&ts, NULL);

	memcpy(p, &ts.tv_sec, 8);
	p += 8;
	memcpy(p, &ts.tv_nsec, 8);
	p += 8;

	p_size = p_size - 8 - 8 - 8;

	for (int i = 0; i < p_size; i++) {
		*p++ = i;
	}
}

void setup_packet(void *p, size_t p_siz, short seq) {
	icmp_header_t *h = (icmp_header_t *)p;

	h->type	 = ICMP_ECHO;
	h->code	 = 0;
	h->id	 = (short)getpid();
	h->seq	 = htons(seq);
	h->cksum = 0;

	fill_packet(&h->data, p_siz);

	h->cksum = get_cksum((unsigned short *)p, p_siz);
}

void validate_packet(void *s, void *r, short p_siz) {
	// NOTE: +20 Because of ipv4 header
	struct icmp *rp = (struct icmp *)(r + 20);
	struct icmp *sp = (struct icmp *)s;

	int err = 0;

	if (rp->icmp_type != ICMP_ECHOREPLY) {
		fprintf(stderr, "invalid type %hu\n", rp->icmp_type);
		err = 1;
	}
	if (rp->icmp_code != 0) {
		fprintf(stderr, "invalid code\n");
		err = 1;
	}
	if (rp->icmp_id != sp->icmp_id) {
		fprintf(stderr, "invalid id\n");
		err = 1;
	}
	if (rp->icmp_seq != sp->icmp_seq) {
		fprintf(stderr, "invalid seq\n");
		err = 1;
	}
	if (rp->icmp_code != get_cksum((unsigned short *)rp, p_siz)) {
		fprintf(stderr, "invalid cksum\n");
		err = 1;
	}

	if (err) {
		errno = EBADMSG;
		panic("validate_packet");
	}
}

void send_packet(void *time, void *packet, int p_siz) {
	ssize_t err;

	err = clock_gettime(CLOCK_REALTIME, (struct timespec *)time);
	if (err == -1) panic("clock_gettime");

	err = sendto(loop.sockfd, packet, p_siz, 0, loop.rp->ai_addr, loop.rp->ai_addrlen);
	if (err < 0) panic("sendto");

	loop.stats.send++;
}

void read_packet(void *time, void *packet, int p_siz) {
	ssize_t err;

	err = recvfrom(loop.sockfd, packet, p_siz, 0, loop.rp->ai_addr, &loop.rp->ai_addrlen);
	if (err < 0) panic("recvfrom");

	err = clock_gettime(CLOCK_REALTIME, (struct timespec *)time);

	if (err == -1) panic("clock_gettime");

	loop.stats.recv++;
}

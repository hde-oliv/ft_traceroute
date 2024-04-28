#pragma once

#include <arpa/inet.h>
#include <asm-generic/errno.h>
#include <bits/time.h>
#include <errno.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <uchar.h>
#include <unistd.h>

#define CYAN "\033[0;36m"
#define RED "\033[0;31m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define RESET "\033[0m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"

#define M_HOPS 30
#define P_SIZE 68
#define M_SIZE 65535

typedef struct s_trace {
	struct addrinfo *result;
	struct addrinfo *rp;
	int				 fd;
	char			*ip_str;
	char			*hostname;
} trace_t;

typedef struct s_sum {
	unsigned int  ip;
	double		  time;
	unsigned char type;
} sum_t;

typedef struct s_batch {
	sum_t sum[3];
} batch_t;

typedef struct s_icmp {
	unsigned char  type;
	unsigned char  code;
	unsigned short cksum;
	unsigned short id;
	unsigned short seq;
	unsigned char  data;
} icmp_t;

typedef struct s_ip {
	unsigned char  ver;
	unsigned char  ds;
	unsigned short len;
	unsigned short id;
	unsigned short flag;
	unsigned char  ttl;
	unsigned char  proto;
	unsigned short cksum;
	unsigned int   src;
	unsigned int   dest;
} ip_t;

// Socket
int setup_socket(trace_t *t);

// Loop
int run_loop(trace_t *t);

// Packet
void setup_packet(void *p, size_t p_siz, short seq);
int	 send_packet(trace_t *t, void *time, void *packet, int p_siz);
int	 read_packet(trace_t *t, void *time, void *packet, int p_siz);
int	 validate_packet(void *s, void *r, short p_siz);
void store_packet(void *p, batch_t *b, int i, void *s, void *e);

// Panic
void panic(char *s);

// Print
void print_response(int i, batch_t *b);
void print_init(trace_t *t);
void print_bytes(int size, void *ptr);

#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct s_trace {
	struct addrinfo *result;
	struct addrinfo *rp;
	int				 fd;
	char			*ip_str;
	char			*hostname;
} trace_t;

typedef struct s_icmp {
	char  type;
	char  code;
	short chksum;
	short id;
	short seq;
	char  data;
} icmp_t;

// Socket
void setup_socket(trace_t *t);

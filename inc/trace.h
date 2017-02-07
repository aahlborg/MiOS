// Trace and debug

#ifndef TRACE_H
#define TRACE_H

#include <stdio.h>

void _trace_hex_dump(char * buf_p, int len);

#ifdef DEBUG
#define TRACE_DEBUG(...) printf(__VA_ARGS__)
#else
#define TRACE_DEBUG(...)
#endif

#define TRACE_DUMP(X, Y) _trace_hex_dump((char *)&(X), (Y))

#endif

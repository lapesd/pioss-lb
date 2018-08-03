/*
 * pioss-util.h
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#ifndef PIOSS_UTIL_H_
#define PIOSS_UTIL_H_

#include <stdlib.h>

#include "pioss-types.h"

/*** MACROS *******************************************************************/

#define perr(fmt, ...) pioss_err (__FILE__, __LINE__, fmt, __VA_ARGS__)
#define pdbg(fmt, ...) pioss_dbg (fmt, __VA_ARGS__)

/*** PROTOTYPES ***************************************************************/

void
pioss_err (const char *file, int line, const char *fmt, ...);
void
pioss_dbg (const char *fmt, ...);
void *
pioss_malloc (size_t size);
void *
pioss_calloc (size_t nelem, size_t elsize);
char *
auint32_tostr (const uint32_t *auint32, int n);
char *
astr_tostr (const char **astr, int n, int ttl_len);
char *
bytes_tostr (uint64_t bytes);
pioss_time_t
curtime ();
void
print_splash ();
void
print_param (const param_t param);
void
print_stats (const stats_t stats);
void
fprint_stats (const stats_t stats, const param_t param);

#endif /* PIOSS_UTIL_H_ */

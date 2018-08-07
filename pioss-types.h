/*
 * pioss-types.h
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#ifndef PIOSS_TYPES_H_
#define PIOSS_TYPES_H_

#include <stdint.h>
#include <stdbool.h>

/*** CONSTANTS ****************************************************************/

#define KIB 1024
#define MIB (1024 * KIB)
#define GIB (1024 * MIB)
#define TIB (1024LL * GIB)

#define MAX_LB_TYPE_NAME 16
#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

/*** TYPES ********************************************************************/

typedef uint32_t nid_t;

typedef double pioss_time_t;

typedef struct pioss_param_s
{
  uint32_t niop; /* number of I/O processes */
  uint64_t nbytes_per_iop; /* bytes transferred per I/O process */
  bool is_shared_file; /* true = single shared file; false = file per process */
  uint32_t ndts; /* number of data servers */
  uint32_t stripe_width; /* number of data servers per file */
  uint64_t stripe_size; /* size of a file stripe fragment */
  char lb_type[MAX_LB_TYPE_NAME + 1]; /* load balancer type */
  uint32_t lb_seed; /* seed for load balancer RNG */
  char foutput[PATH_MAX + 1]; /* append statistics to the file */
  bool is_debug; /* print debug messages */
} param_t;

typedef struct pioss_stats_s
{
  pioss_time_t start_time;
  pioss_time_t end_time;
  uint64_t max_nbytes;
  uint32_t max_nfiles;
} stats_t;

#endif /* PIOSS_TYPES_H_ */

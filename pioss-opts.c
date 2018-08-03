/*
 * pioss-opts.c
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

#include "pioss-opts.h"
#include "pioss-util.h"

/*** PROTOTYPES ***************************************************************/

static void
usage (char **argv);
static uint32_t
parse_opt_uint32 (char *opt);
static uint64_t
parse_opt_uint64 (char *opt);

/*** VARIABLES ****************************************************************/

static struct option long_opts[] =
  {
    { "niop", optional_argument, NULL, 'n' },
    { "nbytes_per_iop", optional_argument, NULL, 'b' },
    { "shared_file", optional_argument, NULL, 'f' },
    { "ndts", optional_argument, NULL, 'm' },
    { "stripe_width", optional_argument, NULL, 'w' },
    { "stripe_size", optional_argument, NULL, 'z' },
    { "lb_type", optional_argument, NULL, 't' },
    { "lb_seed", optional_argument, NULL, 's' },
    { "output", optional_argument, NULL, 'o' },
    { "debug", no_argument, NULL, 'd' },
    { "help", no_argument, NULL, 'h' },
    { NULL, 0, NULL, 0 } };

static char short_opts[] = "n:b:f:m:w:z:t:s:o:dh";

/*** FUNCTIONS ****************************************************************/

/** PUBLIC **/
void
parse_opts (int argc, char **argv, param_t *param)
{
  int c, opt_idx;

  c = 0;
  opt_idx = 0;
  while ((c = getopt_long (argc, argv, short_opts, long_opts, &opt_idx)) != -1)
    {
      switch (c)
	{
	case 'n':
	  param->niop = parse_opt_uint32 (optarg);
	  break;
	case 'b':
	  param->nbytes_per_iop = parse_opt_uint64 (optarg);
	  break;
	case 'f':
	  param->is_shared_file = true;
	  break;
	case 'm':
	  param->ndts = parse_opt_uint32 (optarg);
	  break;
	case 'w':
	  param->stripe_width = parse_opt_uint32 (optarg);
	  break;
	case 'z':
	  param->stripe_size = parse_opt_uint64 (optarg);
	  break;
	case 't':
	  if (strlen (optarg) > MAX_LB_TYPE_NAME)
	    perr("LB type must have at most %d characters: %s",
		 MAX_LB_TYPE_NAME, optarg);
	  else
	    strncpy(param->lb_type, optarg, MAX_LB_TYPE_NAME);
	  break;
	case 's':
	  param->lb_seed = parse_opt_uint32 (optarg);
	  break;
	case 'o':
	  if (strlen (optarg) > PATH_MAX)
	    perr("Output path must have at most %d characters: %s", PATH_MAX,
		 optarg);
	  else
	    strncpy(param->foutput, optarg, PATH_MAX);
	  break;
	case 'd':
	  param->is_debug = true;
	  break;
	case 'h':
	  usage (argv);
	  break;
	default:
	  perr("Unrecognized option: %c", c);
	}

      opt_idx = 0;
    }
} // parse_opts ()

/** PRIVATE **/
static void
usage (char **argv)
{
  fprintf (stderr, "usage: %s [OPTIONS]\n\n", *argv);
  fputs (
      "OPTIONS:\n"
      "--niop=NUM | -n NUM           : number of I/O generating processes\n"
      "--nbytes_per_iop=NUM | -b NUM : bytes transferred per I/O process\n"
      "--shared_file | -f            : on = single shared file; off = file per process\n"
      "--ndts=NUM | -m NUM           : number of data servers\n"
      "--stripe_width=NUM | -w NUM   : number of data servers per file\n"
      "--stripe_size=NUM | -z NUM    : size of a file stripe fragment\n"
      "--lb_type=STR | -t STR        : load balancer type\n"
      "--lb_seed=NUM | -s NUM        : seed for load balancer RNG\n"
      "--output=STR | -o STR         : append statistics to the file\n"
      "--debug | -d                  : print debug messages\n"
      "--help | -h                   : diplays command-line options\n",
      stderr);
  fflush (stderr);

  exit (EXIT_FAILURE);
} // usage ()

static uint32_t
parse_opt_uint32 (char *opt)
{
  uint32_t val;
  char *endptr;

  val = strtoul (opt, &endptr, 10);

  if ((errno == ERANGE && val == UINT_MAX) || (errno != 0 && val == 0))
    perr("%s: %u; aborting options parsing!", strerror(errno), val);

  if (endptr == opt || *endptr != '\0')
    perr("Invalid option value: %s", opt);

  return val;
} // parse_opt_uint32 ()

static uint64_t
parse_opt_uint64 (char *opt)
{
  uint64_t val;
  char *endptr;

  val = strtoull (opt, &endptr, 10);

  if ((errno == ERANGE && val == ULLONG_MAX) || (errno != 0 && val == 0))
    perr("%s: %llu; aborting options parsing!", strerror(errno), val);

  if (endptr == opt || *endptr != '\0')
    perr("Invalid option value: %s", opt);

  return val;
} // parse_opt_uint64 ()


/*
 * pioss-util.c
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <assert.h>

#include "pioss-util.h"

/*** CONSTANTS ****************************************************************/

#define ERR_MSG_FMT "[ERROR]: %s (%s:%d)\n"
#define DBG_MSG_FMT "[DEBUG]: %s\n"

/*** FUNCTIONS ****************************************************************/

void
pioss_err (const char *file, int line, const char *fmt, ...)
{
  va_list ap;
  char *str = NULL;
  int len;

  va_start(ap, fmt);

  len = vsnprintf(str, 0, fmt, ap);
  if (len > 0)
    {
      va_start(ap, fmt);
      len++;
      str = pioss_malloc (len);
      vsnprintf(str, len, fmt, ap);
    }

  va_end(ap);

  fprintf (stderr, ERR_MSG_FMT, str, file, line);
  fflush (stderr);

  exit (EXIT_FAILURE);
} // pioss_err ()

void
pioss_dbg (const char *fmt, ...)
{
  va_list ap;
  char *str = NULL;
  int len;

  va_start(ap, fmt);

  len = vsnprintf(str, 0, fmt, ap);
  if (len > 0)
    {
      va_start(ap, fmt);
      len++;
      str = pioss_malloc (len);
      vsnprintf(str, len, fmt, ap);
    }

  va_end(ap);

  fprintf (stderr, DBG_MSG_FMT, str);
  fflush (stderr);
} // pioss_dbg ()

void *
pioss_malloc (size_t size)
{
  void *ptr;

  ptr = malloc (size);
  assert(ptr);

  return ptr;
} // pioss_malloc ()

void *
pioss_calloc (size_t nelem, size_t elsize)
{
  void *ptr;

  ptr = calloc (nelem, elsize);
  assert(ptr);

  return ptr;
} // pioss_calloc ()

char *
auint32_tostr (const uint32_t *auint32, int n)
{
  char *str, *fmt;
  int ttl_len, len;
  char **astr;
  int i;

  str = NULL;
  fmt = "%u";
  ttl_len = 0;

  if (auint32 && n > 0)
    {
      astr = malloc (n * sizeof(char *));
      assert(astr);

      for (i = 0; i < n; i++)
	{
	  len = snprintf(astr[i], 0, fmt, auint32[i]);
	  if (len > 0)
	    {
	      len++;
	      astr[i] = malloc (len);
	      assert(astr[i]);
	      snprintf(astr[i], len, fmt, auint32[i]);
	      ttl_len += len;
	    }
	}

      str = astr_tostr ((const char **) astr, n, ttl_len);

      for (i = 0; i < n; i++)
	free (astr[i]);
      free (astr);
    }

  return str;
} // auint32_tostr ()

char *
astr_tostr (const char **astr, int n, int ttl_len)
{
  char *str, *str_ptr;
  int i, len;

  str = NULL;
  if (astr && n > 0 && ttl_len > 0)
    {
      /* (* 2) for ", " between elements; (+ 4) for brackets; (+ 1) for end */
      ttl_len = (ttl_len - 1) * 2 + 4 + 1;
      str = malloc (ttl_len);
      assert(str);
      str_ptr = str;

      *str_ptr = '[';
      str_ptr++;
      *str_ptr = ' ';
      str_ptr++;
      for (i = 0; i < n; i++)
	{
	  if (i > 0)
	    {
	      *str_ptr = ',';
	      str_ptr++;
	      *str_ptr = ' ';
	      str_ptr++;
	    }

	  len = strlen (astr[i]);
	  strncpy(str_ptr, astr[i], len);
	  str_ptr += len;
	}
      *str_ptr = ' ';
      str_ptr++;
      *str_ptr = ']';
      str_ptr++;
    }

  return str;
} // astr_tostr ()

char *
bytes_tostr (uint64_t bytes)
{
  char *str, *fmt, *unit;
  double b_trans;
  int len;

  fmt = "%g %s";
  if (bytes >= TIB)
    {
      b_trans = bytes / (double) TIB;
      unit = "TiB";
    }
  else if (bytes >= GIB)
    {
      b_trans = bytes / (double) GIB;
      unit = "GiB";
    }
  else if (bytes >= MIB)
    {
      b_trans = bytes / (double) MIB;
      unit = "MiB";
    }
  else if (bytes >= KIB)
    {
      b_trans = bytes / (double) KIB;
      unit = "KiB";
    }
  else
    {
      b_trans = bytes;
      unit = "bytes";
    }

  len = snprintf(str, 0, fmt, b_trans, unit);
  if (len > 0)
    {
      len++;
      str = pioss_malloc (len);
      snprintf(str, len, fmt, b_trans, unit);
    }

  return str;
} // bytes_tostr ()

pioss_time_t
curtime ()
{
  pioss_time_t time;
  struct timeval tv;

  gettimeofday (&tv, NULL);
  time = tv.tv_sec + (tv.tv_usec / 1e6);

  return time;
} // curtime ()

void
print_splash ()
{
  fputs ("PIOSS - Parallel I/O and Storage System simulation model.\n\n",
  stdout);
  fflush (stdout);
} // print_splash ()

void
print_param (const param_t param)
{
  char *fmt;

  fmt = "*****************************************************************\n"
      "PARAMS:\n"
      "  Num. I/O processes         : %u\n"
      "  Num. bytes per I/O process : %llu\n"
      "  Shared file                : %s\n"
      "  Num. data servers          : %u\n"
      "  Stripe width               : %u\n"
      "  Stripe size                : %llu\n"
      "  LB type                    : %s\n"
      "  LB seed                    : %u\n"
      "  Output to file             : %s\n"
      "*****************************************************************\n\n";

  fprintf (stdout, fmt, param.niop, param.nbytes_per_iop,
	   ((param.is_shared_file) ? "true" : "false"), param.ndts,
	   param.stripe_width, param.stripe_size, param.lb_type, param.lb_seed,
	   param.foutput);
  fflush (stdout);
} // print_param ()

void
print_stats (const stats_t stats)
{
  char *fmt, *bytes_str;

  fmt = "Simulation executed in %0.6f seconds\n"
      "Num. bytes in the most loaded data server: %s\n"
      "Num. of file in the most selected data server: %u\n";
  bytes_str = bytes_tostr (stats.max_nbytes);
  pdbg("%s", bytes_str);

  fprintf (stdout, fmt, (stats.end_time - stats.start_time), bytes_str,
	   stats.max_nfiles);
  fflush (stdout);

  free (bytes_str);
} // print_stats ()

/*
 * Output format:
 *
 * niop,nbytes_per_iop,is_shared_file,ndts,stripe_width,stripe_size,lb_type,
 * lb_seed,runtime,max_nbytes,max_nfiles
 */
void
fprint_stats (const stats_t stats, const param_t param)
{
  char *fmt;
  FILE *csv;

  fmt = "%u,%llu,%s,%u,%u,%llu,%s,%u,%0.6f,%llu,%u\n";

  if (strlen (param.foutput) > 0)
    {
      csv = fopen (param.foutput, "a");
      if (!csv)
	perr("Unable to open file: %s", param.foutput);

      fprintf (csv, fmt, param.niop, param.nbytes_per_iop,
	       ((param.is_shared_file) ? "shared_file" : "file_per_process"),
	       param.ndts, param.stripe_width, param.stripe_size, param.lb_type,
	       param.lb_seed, (stats.end_time - stats.start_time),
	       stats.max_nbytes, stats.max_nfiles);
      fflush (csv);

      fclose (csv);
    }
} // fprint_stats ()

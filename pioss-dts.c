/*
 * pioss-dts.c
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#include <stdio.h>

#include "pioss-dts.h"
#include "pioss-util.h"

/*** TYPES ********************************************************************/

typedef struct pioss_dts_st_s
{
  nid_t id;
  uint64_t ttl_nbytes;
  uint32_t *filetable;
  int _filetable_len;
} dts_st_t;

/*** PROTOTYPES ***************************************************************/

static char *
dts_st_tostr (const dts_st_t dts_st);

/*** VARIABLES ****************************************************************/

static dts_st_t *g_dts_st = NULL;

/*** FUNCTIONS ****************************************************************/

/** PUBLIC **/
void
dts_init (const param_t param)
{
  nid_t dts;
  uint32_t nfiles;

  nfiles = (param.is_shared_file) ? 1 : param.niop;
  g_dts_st = pioss_malloc (param.ndts * sizeof(dts_st_t));
  for (dts = 0; dts < param.ndts; dts++)
    {
      g_dts_st[dts].id = dts;
      g_dts_st[dts].ttl_nbytes = 0;
      g_dts_st[dts].filetable = pioss_calloc (nfiles, sizeof(uint32_t));
      g_dts_st[dts]._filetable_len = nfiles;
    }
} // dts_init ()

void
dts_xfer (uint32_t file, uint64_t nbytes, nid_t dts, const param_t param)
{
  char *dts_str;

  g_dts_st[dts].ttl_nbytes += nbytes;
  g_dts_st[dts].filetable[file] = 1;

  if (param.is_debug)
    {
      dts_str = dts_st_tostr (g_dts_st[dts]);
      pdbg("%s", dts_str);
      free (dts_str);
    }
} // dts_xfer ()

void
dts_final (const param_t param, stats_t *stats)
{
  nid_t dts;
  uint32_t ttl_nfiles, file;

  for (dts = 0; dts < param.ndts; dts++)
    {
      if (g_dts_st[dts].ttl_nbytes > stats->max_nbytes)
	stats->max_nbytes = g_dts_st[dts].ttl_nbytes;

      ttl_nfiles = 0;
      for (file = 0; file < g_dts_st[dts]._filetable_len; file++)
	ttl_nfiles += g_dts_st[dts].filetable[file];

      if (ttl_nfiles > stats->max_nfiles)
	stats->max_nfiles = ttl_nfiles;

      free (g_dts_st[dts].filetable);
    }

  free (g_dts_st);
} // dts_final ()

/** PRIVATE **/
static char *
dts_st_tostr (const dts_st_t dts_st)
{
  char *str, *fmt;
  int len;
  uint32_t ttl_nfiles, file;

  ttl_nfiles = 0;
  for (file = 0; file < dts_st._filetable_len; file++)
    ttl_nfiles += dts_st.filetable[file];

  fmt = "dts %u: ttl_nbytes = %llu, ttl_nfiles = %u";
  len = snprintf(str, 0, fmt, dts_st.id, dts_st.ttl_nbytes, ttl_nfiles);
  if (len > 0)
    {
      len++;
      str = pioss_malloc (len);
      snprintf(str, len, fmt, dts_st.id, dts_st.ttl_nbytes, ttl_nfiles);
    }

  return str;
} // dts_st_tostr ()

/*
 * pioss-mds.c
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#include "pioss-mds.h"
#include "pioss-lb.h"
#include "pioss-util.h"

/*** TYPES ********************************************************************/

typedef struct pioss_mds_st_s
{
  nid_t *dts;
  nid_t **filetable;
  uint32_t _filetable_len;
} mds_st_t;

/*** VARIABLES ****************************************************************/

static mds_st_t g_mds_st =
  { .dts = NULL, .filetable = NULL };

static const lb_t *g_lb = NULL;

/*** FUNCTIONS ****************************************************************/

void
mds_init (const param_t param)
{
  nid_t dts;
  uint32_t nfiles, file;

  g_mds_st.dts = pioss_malloc (param.ndts * sizeof(nid_t));
  for (dts = 0; dts < param.ndts; dts++)
    g_mds_st.dts[dts] = dts;

  nfiles = (param.is_shared_file) ? 1 : param.niop;
  g_mds_st.filetable = pioss_malloc (nfiles * sizeof(nid_t *));
  for (file = 0; file < nfiles; file++)
    g_mds_st.filetable[file] = NULL;
  g_mds_st._filetable_len = nfiles;

  g_lb = pioss_lb_lookup (param.lb_type);
  if (g_lb)
    g_lb->init (param.lb_seed);
  else
    perr("Unrecognized load balancer type: '%s'.", param.lb_type);
} // mds_init ()

nid_t *
mds_open_file (uint32_t file, const param_t param)
{
  nid_t *dts_set = NULL;

  if (!g_mds_st.filetable[file])
    {
      g_mds_st.filetable[file] = pioss_malloc (
	  param.stripe_width * sizeof(nid_t));
      g_lb->get_dts (g_mds_st.dts, param.ndts, g_mds_st.filetable[file],
		     param.stripe_width);
    }

  dts_set = g_mds_st.filetable[file];

  return dts_set;
} // mds_open_file ()

void
mds_final (const param_t param)
{
  uint32_t file;

  free (g_mds_st.dts);

  for (file = 0; file < g_mds_st._filetable_len; file++)
    free (g_mds_st.filetable[file]);
  free (g_mds_st.filetable);
} // mds_final ()

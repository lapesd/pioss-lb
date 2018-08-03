/*
 * pioss-iop.c
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#include <stdio.h>

#include "pioss-iop.h"
#include "pioss-mds.h"
#include "pioss-dts.h"
#include "pioss-util.h"

/*** TYPES ********************************************************************/

typedef struct pioss_iop_st_s
{
  nid_t id;
  uint32_t file;
  uint64_t buf_size;
  uint64_t offset;
  uint64_t xfer_size;
  nid_t *dts_set;
  int _dts_set_len;
} iop_st_t;

/*** PROTOTYPES ***************************************************************/

static char *
iop_st_tostr (const iop_st_t iop_st);

/*** VARIABLES ****************************************************************/

static iop_st_t iop_st =
  { .id = 0, .file = 0, .buf_size = 0, .offset = 0, .xfer_size = 0, .dts_set =
  NULL };

/*** FUNCTIONS ****************************************************************/

/** PUBLIC **/
void
iop_init (nid_t id, const param_t param)
{
  iop_st.id = id;
  iop_st.file = (param.is_shared_file) ? 0 : id;
  iop_st.buf_size = param.nbytes_per_iop;
  iop_st.offset = (param.is_shared_file) ? (id * param.nbytes_per_iop) : 0;
  iop_st.xfer_size = param.stripe_size;
  iop_st.dts_set = mds_open_file (iop_st.file, param);
  iop_st._dts_set_len = param.stripe_width;
} // iop_init ()

void
iop_xfer (const param_t param)
{
  nid_t dts;
  uint32_t idx;

  while (iop_st.buf_size > 0)
    {
      iop_st.xfer_size = param.stripe_size
	  - (iop_st.offset % param.stripe_size);
      if (iop_st.xfer_size > iop_st.buf_size)
	iop_st.xfer_size = iop_st.buf_size;

      idx = (iop_st.offset / param.stripe_size) % param.stripe_width;
      dts = iop_st.dts_set[idx];

      if (param.is_debug)
	pdbg("iop %u -> dts %u; file = %u, xfer_size = %llu, offset = %llu",
	     iop_st.id, dts, iop_st.file, iop_st.xfer_size, iop_st.offset);
      dts_xfer (iop_st.file, iop_st.xfer_size, dts, param);

      iop_st.buf_size -= iop_st.xfer_size;
      iop_st.offset += iop_st.xfer_size;
    }
} // iop_xfer ()

/** PRIVATE **/
static char *
iop_st_tostr (const iop_st_t iop_st)
{
  char *str, *fmt, *dts_str;
  int len;

  fmt = "id %u: file = %u, buf_size = %llu, offset = %llu, xfer_size = %llu, "
      "*dts_set (%p) = %s";
  dts_str = auint32_tostr (iop_st.dts_set, iop_st._dts_set_len);
  len = snprintf(str, 0, fmt, iop_st.id, iop_st.file, iop_st.buf_size,
		 iop_st.offset, iop_st.xfer_size, iop_st.dts_set, dts_str);
  if (len > 0)
    {
      len++;
      str = pioss_malloc (len);
      snprintf(str, len, fmt, iop_st.id, iop_st.file, iop_st.buf_size,
	       iop_st.offset, iop_st.xfer_size, iop_st.dts_set, dts_str);
    }

  free (dts_str);

  return str;
} // iop_st_tostr ()

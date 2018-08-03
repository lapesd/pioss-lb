/*
 * pioss-lb-randseq.c
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "pioss-lb.h"
#include "pioss-lb-randseq.h"
#include "tinymt32.h"

/*** CONSTANTS ****************************************************************/

#define LB_TYPE_RANDSEQ "RAND+SEQ"

/*** PROTOTYPES ***************************************************************/

static void
lb_randseq_init (uint32_t seed);

static void
lb_randseq_get_dts (const nid_t *dts_in, uint32_t ndts, nid_t *dts_out,
		    uint32_t sw);

/*** VARIABLES ****************************************************************/

lb_t lb_randseq =
      { (LB_TYPE_RANDSEQ), (init_f) lb_randseq_init,
	  (get_dts_f) lb_randseq_get_dts };

tinymt32_t g;

/*** FUNCTIONS ****************************************************************/

/** PUBLIC **/
void
lb_randseq_register ()
{
  pioss_lb_register (&lb_randseq);
} // lb_randseq_register ()

/** PRIVATE **/
static void
lb_randseq_init (uint32_t seed)
{
  tinymt32_init (&g, seed);
} // lb_randseq_init ()

static void
lb_randseq_get_dts (const nid_t *dts_in, uint32_t ndts, nid_t *dts_out,
		    uint32_t sw)
{
  nid_t next;
  uint32_t i;

  next = tinymt32_generate_uint32 (&g) % ndts;
  for (i = 0; i < sw; i++)
    {
      dts_out[i] = dts_in[next];
      next = (next + 1) % ndts;
    }
} // lb_randseq_get_dts ()


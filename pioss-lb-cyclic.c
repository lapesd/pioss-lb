/*
 * pioss-lb-cyclic.c
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#include <stdlib.h>
#include <string.h>

#include "pioss-lb.h"
#include "pioss-lb-cyclic.h"
#include "tinymt32.h"

/*** CONSTANTS ****************************************************************/

#define LB_TYPE_CYCLIC "CYCLIC"

/*** PROTOTYPES ***************************************************************/

static void
lb_cyclic_init (uint32_t seed);

static void
lb_cyclic_get_dts (const nid_t *dts_in, uint32_t ndts, nid_t *dts_out,
		   uint32_t sw);

/*** VARIABLES ****************************************************************/

static lb_t lb_cyclic =
  { (LB_TYPE_CYCLIC), (init_f) lb_cyclic_init, (get_dts_f) lb_cyclic_get_dts };

static tinymt32_t g;

static int next_idx;

/*** FUNCTIONS ****************************************************************/

/** PUBLIC **/
void
lb_cyclic_register ()
{
  pioss_lb_register (&lb_cyclic);
} // lb_cyclic_register ()

/** PRIVATE **/
static void
lb_cyclic_init (uint32_t seed)
{
  tinymt32_init (&g, seed);

  next_idx = -1;
} // lb_cyclic_init ()

static void
lb_cyclic_get_dts (const nid_t *dts_in, uint32_t ndts, nid_t *dts_out,
		   uint32_t sw)
{
  uint32_t i;

  if (next_idx == -1)
    next_idx = tinymt32_generate_uint32 (&g) % ndts;

  for (i = 0; i < sw; i++)
    {
      dts_out[i] = dts_in[next_idx];
      next_idx = (next_idx + 1) % ndts;
    }
} // lb_cyclic_get_dts ()

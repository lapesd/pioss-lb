/*
 * pioss-lb-rand.c
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "pioss-lb.h"
#include "pioss-lb-rand.h"
#include "pioss-util.h"
#include "tinymt32.h"

/*** CONSTANTS ****************************************************************/

#define LB_TYPE_RAND "RAND"

/*** PROTOTYPES ***************************************************************/

static void
lb_rand_init (uint32_t seed);

static void
lb_rand_get_dts (const nid_t *dts_in, uint32_t ndts, nid_t *dts_out,
		 uint32_t sw);

/*** VARIABLES ****************************************************************/

lb_t lb_rand =
  { (LB_TYPE_RAND), (init_f) lb_rand_init, (get_dts_f) lb_rand_get_dts };

tinymt32_t g;

/*** FUNCTIONS ****************************************************************/

/** PUBLIC **/
void
lb_rand_register ()
{
  pioss_lb_register (&lb_rand);
} // lb_rand_register ()

/** PRIVATE **/
static void
lb_rand_init (uint32_t seed)
{
  tinymt32_init (&g, seed);
} // lb_rand_init ()

static void
lb_rand_get_dts (const nid_t *dts_in, uint32_t ndts, nid_t *dts_out,
		 uint32_t sw)
{
  nid_t *dts;
  nid_t i, o, swp;

  dts = pioss_malloc (ndts * sizeof(nid_t));
  memcpy(dts, dts_in, ndts * sizeof(nid_t));

  for (i = 0; i < ndts; i++)
    {
      o = tinymt32_generate_uint32 (&g) % ndts;
      swp = dts[i];
      dts[i] = dts[o];
      dts[o] = swp;
    }

  memcpy(dts_out, dts, sw * sizeof(nid_t));

  free (dts);
} // lb_rand_get_dts ()

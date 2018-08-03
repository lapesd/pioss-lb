/*
 * pioss-lb.h
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#ifndef PIOSS_LB_H_
#define PIOSS_LB_H_

#include "pioss-types.h"

/*** TYPES ********************************************************************/

typedef void
(*init_f) (uint32_t seed);

typedef void
(*get_dts_f) (const nid_t *dts_in, uint32_t ndts, nid_t *dts_out, uint32_t sw);

typedef struct pioss_lb_s
{
  char *type;
  init_f init;
  get_dts_f get_dts;
} lb_t;

/*** PROTOTYPES ***************************************************************/

void
pioss_lb_register (const lb_t *lb);
const lb_t *
pioss_lb_lookup (const char *type);

#endif /* PIOSS_LB_H_ */

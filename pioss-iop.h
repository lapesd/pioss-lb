/*
 * pioss-iop.h
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#ifndef PIOSS_IOP_H_
#define PIOSS_IOP_H_

#include "pioss-types.h"

/*** PROTOTYPES ***************************************************************/

void
iop_init (nid_t id, const param_t param);
void
iop_xfer (const param_t param);

#endif /* PIOSS_IOP_H_ */

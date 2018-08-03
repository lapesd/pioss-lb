/*
 * pioss-dts.h
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#ifndef PIOSS_DTS_H_
#define PIOSS_DTS_H_

#include "pioss-types.h"

/*** PROTOTYPES ***************************************************************/

void
dts_init (const param_t param);
void
dts_xfer (uint32_t file, uint64_t nbytes, nid_t dts, const param_t param);
void
dts_final (const param_t param, stats_t *stats);

#endif /* PIOSS_DTS_H_ */

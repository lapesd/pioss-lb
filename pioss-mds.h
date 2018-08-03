/*
 * pioss-mds.h
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#ifndef PIOSS_MDS_H_
#define PIOSS_MDS_H_

#include "pioss-types.h"

/*** PROTOTYPES ***************************************************************/

void
mds_init (const param_t param);
nid_t *
mds_open_file (uint32_t file, const param_t param);
void
mds_final (const param_t param);

#endif /* PIOSS_MDS_H_ */

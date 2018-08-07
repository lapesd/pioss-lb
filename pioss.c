/*
 * pioss.c
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#include "pioss.h"
#include "pioss-opts.h"
#include "pioss-mds.h"
#include "pioss-lb.h"
#include "pioss-lb-rand.h"
#include "pioss-lb-randseq.h"
#include "pioss-dts.h"
#include "pioss-iop.h"
#include "pioss-util.h"

/*** MAIN *********************************************************************/

int
main (int argc, char **argv)
{
  param_t param =
    { .niop = 1, .nbytes_per_iop = 1 * MIB, .is_shared_file = false, .ndts = 2,
	.stripe_width = 1, .stripe_size = 64 * KIB, .lb_type = "RAND",
	.lb_seed = 0, .foutput = "", .is_debug = false };
  stats_t stats =
    { .max_nbytes = 0, .max_nfiles = 0 };

  print_splash ();

  lb_rand_register ();
  lb_randseq_register ();

  parse_opts (argc, argv, &param);

  print_param (param);

  pioss_exec (param, &stats);

  pioss_lb_regclean();

  print_stats (stats);
  fprint_stats (stats, param);
} // main ()

/*** FUNCTIONS ****************************************************************/

/** PUBLIC **/
void
pioss_exec (const param_t param, stats_t *stats)
{
  nid_t iop;

  /* initializes servers state */
  mds_init (param);
  dts_init (param);

  stats->start_time = curtime ();


  /* simulates I/O requests iterating through I/O processes */
  for (iop = 0; iop < param.niop; iop++)
    {
      iop_init (iop, param);
      iop_xfer (param);
    }

  stats->end_time = curtime ();

  /* finalize states */
  mds_final (param);
  dts_final (param, stats);
} // pioss_exec ()

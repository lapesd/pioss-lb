/*
 * pioss-lb.c
 *
 * Author: Camilo <eduardo.camilo@posgrad.ufsc.br>
 */

#include <string.h>

#include "pioss-lb.h"
#include "pioss-util.h"

/*** TYPES ********************************************************************/

typedef struct pioss_lblist_s lblist_t;

struct pioss_lblist_s
{
  const lb_t *lb;
  lblist_t *next;
};

/*** VARIABLES ****************************************************************/

lblist_t lb_head =
  { .lb = NULL, .next = NULL };
lblist_t *lb_tail = NULL;

/*** FUNCTIONS ****************************************************************/

void
pioss_lb_register (const lb_t *lb)
{
  lblist_t *new_tail;

  if (!lb_tail)
    {
      lb_head.lb = lb;
      lb_tail = &lb_head;
    }
  else
    {
      new_tail = pioss_malloc (sizeof(lblist_t));
      new_tail->lb = lb;
      new_tail->next = NULL;

      lb_tail->next = new_tail;
      lb_tail = new_tail;
    }
} // pioss_lb_register ()

const lb_t *
pioss_lb_lookup (const char *type)
{
  const lb_t *lb = NULL;
  lblist_t *next = &lb_head;

  while (!lb && next)
    {
      if (strcmp (type, next->lb->type) == 0)
	lb = next->lb;
      else
	next = next->next;
    }

  return lb;
} // pioss_lb_lookup ()

void
pioss_lb_regclean ()
{
  lblist_t *next, *swp;

  next = lb_head.next;
  while (next != NULL)
    {
      swp = next;
      next = swp->next;
      swp->next = NULL;
      free (swp);
    }
  lb_tail = NULL;
} // pioss_lb_regclean ()

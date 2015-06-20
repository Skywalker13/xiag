/*
 * XIAG: XML Interpreter for Adventure Games
 * Copyright (C) 2008 Mathieu Schroeter <mathieu.schroeter@gamesover.ch>
 *
 * This file is part of XIAG.
 *
 * XIAG is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XIAG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdlib.h>

#include "xiag.h"
#include "interface.h"
#include "story.h"
#include "module.h"
#include "inventory.h"
#include "logs.h"

#define MODULE_NAME "xiag"


static xiag_t *
xiag_new (void)
{
  xiag_t *xiag;

  xiag = calloc (1, sizeof (xiag_t));
  if (!xiag)
    return NULL;

  return xiag;
}

static void
xiag_shutdown (xiag_t *xiag)
{
  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  inventory_free ();
  module_shutdown (xiag);
  story_shutdown (xiag);
  interface_shutdown (xiag);
  free (xiag);
}

int
main (int argc, char **argv)
{
  xiag_t *xiag;
  int res;

  (void) argc;
  (void) argv;

  xiag = xiag_new ();
  if (!xiag)
    return -1;

  res = interface_loader (xiag, INTERFACE_TYPE_TERMINAL);
  if (res)
    goto out;

  res = story_loader (xiag);
  if (res)
    goto out;

  /* TODO: savegame loader */
  res = story_load_firstid (xiag); /* no save, then load the first position */
  if (res)
    goto out;

  res = module_loader (xiag);
  if (res)
    goto out;

  interface_main_loop (xiag);

 out:
  xiag_shutdown (xiag);
  return res;
}

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
#include "logs.h"
#include "interface.h"

#include "interfaces/terminal.h"

#define MODULE_NAME "interface"

struct interface_s {
  interface_type_t type;
  interface_funcs_t *funcs;
  void *private;
};


char *
interface_story_get (xiag_t *xiag)
{
  interface_t *interface = xiag->interface;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  if (interface->funcs->story_get)
    return interface->funcs->story_get (xiag, interface->private);

  return NULL;
}

void
interface_main_loop (xiag_t *xiag)
{
  interface_t *interface = xiag->interface;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  if (interface->funcs->main_loop)
    interface->funcs->main_loop (xiag, interface->private);
}

static int
interface_init (xiag_t *xiag)
{
  interface_t *interface = xiag->interface;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  if (interface->funcs->init)
    return interface->funcs->init (xiag, interface->private);

  return 0;
}

static void
interface_uninit (xiag_t *xiag)
{
  interface_t *interface = xiag->interface;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  if (!interface)
    return;

  if (interface->funcs && interface->funcs->uninit)
    interface->funcs->uninit (xiag, interface->private);
}

static interface_t *
interface_new (interface_type_t type)
{
  interface_t *interface;

  interface = calloc (1, sizeof (interface_t));
  if (!interface)
    return NULL;

  interface->type = type;

  return interface;
}

static void
interface_free (interface_t *interface)
{
  if (!interface)
    return;

  if (interface->funcs)
    free (interface->funcs);
  free (interface);
}

int
interface_loader (xiag_t *xiag, interface_type_t type)
{
  interface_t *interface;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  if (!xiag)
    return -1;

  xiag->interface = interface_new (type);
  interface = xiag->interface;

  if (!interface)
    return -1;

  switch (type)
  {
  case INTERFACE_TYPE_TERMINAL:
    interface->funcs = terminal_register_funcs ();
    interface->private = terminal_register_private ();
    break;

  case INTERFACE_TYPE_GRAPHICAL:
    XIAG_LOG (xiag, XIAG_MSG_WARNING,
              MODULE_NAME, "the graphical interface is unimplemented");
    break;

  default:
    XIAG_LOG (xiag, XIAG_MSG_ERROR, MODULE_NAME, "unknown interface");
    break;
  }

  if (!interface->funcs)
    return -1;

  return interface_init (xiag);
}

void
interface_shutdown (xiag_t *xiag)
{
  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  if (!xiag)
    return;

  interface_uninit (xiag);
  interface_free (xiag->interface);
}

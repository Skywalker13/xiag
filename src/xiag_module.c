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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "xiag.h"
#include "xiag_module.h"
#include "entity.h"
#include "inventory.h"
#include "engine.h"
#include "logs.h"

#define MODULE_NAME "xiag_module"


void
xiag_module_log (XIAG_HANDLE *handle, xiag_module_msg_t level,
                 const char *file, int line, const char *format, ...)
{
  xiag_verbosity_level_t l;
  xiag_t *xiag = handle;
  va_list va;
  char str[256];

  switch (level)
  {
  case XIAG_MODULE_MSG_VERBOSE:
    l = XIAG_MSG_VERBOSE;
    break;

  default:
  case XIAG_MODULE_MSG_INFO:
    l = XIAG_MSG_INFO;
    break;

  case XIAG_MODULE_MSG_WARNING:
    l = XIAG_MSG_WARNING;
    break;

  case XIAG_MODULE_MSG_ERROR:
    l = XIAG_MSG_ERROR;
    break;

  case XIAG_MODULE_MSG_CRITICAL:
    l = XIAG_MSG_CRITICAL;
    break;
  }

  va_start (va, format);
  vsnprintf (str, sizeof (str), format, va);
  xlog (xiag, l, MODULE_NAME, file, line, str);
  va_end (va);
}

xiag_module_id_type_t
xiag_module_id_type_get (XIAG_HANDLE *handle, char *id)
{
  engine_id_type_t type;
  xiag_t *xiag = handle;

  type = engine_id_type_get (xiag, id);
  switch (type)
  {
  case ENGINE_ID_CHAPTER:
    return XIAG_MODULE_ID_CHAPTER;

  case ENGINE_ID_SCENE:
    return XIAG_MODULE_ID_SCENE;

  case ENGINE_ID_ROOM:
    return XIAG_MODULE_ID_ROOM;

  case ENGINE_ID_ENTITY:
    return XIAG_MODULE_ID_ENTITY;

  default:
    return XIAG_MODULE_ID_UNKNOWN;
  }
}

void
xiag_module_id_split (const char *id, char **id_chapter, char **id_scene,
                                      char **id_room, char **id_entity)
{
  engine_id_split (id, id_chapter, id_scene, id_room, id_entity);
}

int
xiag_module_entity_inventory_get (XIAG_HANDLE *handle, char *id)
{
  entity_t *entity;
  xiag_t *xiag = handle;
  entity_inventory_t inventory;

  entity = engine_entity_search (xiag, id);
  if (!entity)
    return -1;

  inventory = entity_inventory_get (entity);
  switch (inventory)
  {
  default:
  case ENTITY_INVENTORY_NULL:
    return -1;

  case ENTITY_INVENTORY_NO:
    return 0;

  case ENTITY_INVENTORY_YES:
    return 1;
  }
}

const char *
xiag_module_entity_state_get (XIAG_HANDLE *handle, char *id)
{
  entity_t *entity;
  xiag_t *xiag = handle;

  entity = engine_entity_search (xiag, id);
  if (!entity)
    return NULL;

  return entity_state_get (entity);
}

char *
xiag_module_idpos_get (XIAG_HANDLE *handle)
{
  xiag_t *xiag = handle;

  return engine_idpos_get (xiag);
}

unsigned int
xiag_module_inventory_size (XIAG_HANDLE *handle)
{
  (void) handle;

  return inventory_size ();
}

char *
xiag_module_inventory_id (XIAG_HANDLE *handle, unsigned int item)
{
  (void) handle;

  return inventory_id_get (item);
}

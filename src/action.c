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
#include <string.h>

#include "xiag.h"
#include "module.h"
#include "engine.h"
#include "logs.h"
#include "result.h"
#include "action.h"

#define MODULE_NAME "action"


result_t *
action_look (xiag_t *xiag, char *id)
{
  int res;
  char id_room[256];
  char *id_look = id;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  /*
   * Get the current room if ID is undefined, then we can `look` the room
   * without argument.
   */
  if (!id_look)
  {
    snprintf (id_room, sizeof (id_room),
              "%s.%s.%s", xiag->id_chapter, xiag->id_scene, xiag->id_room);
    id_look = id_room;
  }

  if (!engine_idpos_check (xiag, id_look))
    return NULL;

  /*
   * LOOK is an action where the ID can be NULL. For example it is very useful
   * in order to look globaly the entities in a room. And not only an entity
   * itself. But this way is only available for modules, especially "texts".
   */
  if (engine_id_type_get (xiag, id_look) == ENGINE_ID_ENTITY)
  {
    res = engine_look (xiag, id_look);
    if (res)
      return NULL;
  }

  return module_look (xiag, id_look);
}

result_t *
action_take (xiag_t *xiag, char *id)
{
  int res;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  if (!id)
    return NULL;

  if (!engine_idpos_check (xiag, id))
    return NULL;

  res = engine_take (xiag, id);
  if (res)
    return NULL;

  return module_take (xiag, id);
}

result_t *
action_use (xiag_t *xiag, char *id, char *what_id)
{
  int res;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  if (!id)
    return NULL;

  if (!engine_idpos_check (xiag, id))
    return NULL;

  /* what_id must be in the inventory */
  if (what_id && !engine_inventory_check (xiag, what_id))
  {
    XIAG_LOG (xiag, XIAG_MSG_INFO,
              MODULE_NAME, "%s not in the inventory", what_id);
    return NULL;
  }

  res = engine_use (xiag, id, what_id);
  if (res)
    return NULL;

  return module_use (xiag, id, what_id);
}

result_t *
action_speak (xiag_t *xiag, char *id)
{
  int res;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  if (!id)
    return NULL;

  if (!engine_idpos_check (xiag, id))
    return NULL;

  res = engine_speak (xiag, id);
  if (res)
    return NULL;

  return module_speak (xiag, id);
}

result_t *
action_move (xiag_t *xiag, char *id)
{
  int res;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  if (!id)
    return NULL;

  if (!engine_idpos_check (xiag, id))
    return NULL;

  res = engine_move (xiag, id);
  if (res)
    return NULL;

  return module_move (xiag, id);
}

xiag_module_terminal_t *
action_data_terminal (xiag_t *xiag,
                      const char *mod_name, result_t *result)
{
  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  for (; result; result = result->next)
    if (!strcmp (result->mod_name, mod_name))
      return  module_data_terminal (xiag, mod_name, result->mod_data);

  return NULL;
}

xiag_module_graphical_t *
action_data_graphical (xiag_t *xiag,
                       const char *mod_name, result_t *result)
{
  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  for (; result; result = result->next)
    if (!strcmp (result->mod_name, mod_name))
      return  module_data_graphical (xiag, mod_name, result->mod_data);

  return NULL;
}

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
#include <stdio.h>
#include <string.h>

#include "texts_entity.h"
#include "texts_room.h"
#include "texts_story.h"
#include "texts_engine.h"
#include "texts.h"

typedef struct texts_result_s {
  char *name;
  char *text;
} texts_result_t;


static texts_t texts;

static int
texts_init (XIAG_HANDLE *handle)
{
  MOD_LOG (handle, XIAG_MODULE_MSG_INFO, __FUNCTION__);

  texts.handle = handle;
  texts_story_loader (&texts, strdup ("/usr/local/share/xiag/story/example.texts.en.xml"));

  return 0;
}

static void
texts_uninit (void)
{
  MOD_LOG (texts.handle, XIAG_MODULE_MSG_INFO, __FUNCTION__);

  texts_story_shutdown (&texts);
}

static void *
texts_action (const char *id, texts_action_t action, const char *what_id)
{
  texts_entity_t *entity;
  texts_result_t *res;
  char *tmp;
  const char *name;

  entity = texts_engine_entity_search (&texts, (char *) id);
  if (!entity)
    return NULL;

  res = malloc (sizeof (texts_result_t));
  if (!res)
    return NULL;

  name = texts_entity_name_get (entity);
  res->name = name ? strdup (name) : NULL;

  switch (action)
  {
  case TEXTS_ACTION_LOOK:
    tmp = texts_entity_look (&texts, entity);
    break;

  case TEXTS_ACTION_TAKE:
    tmp = texts_entity_take (&texts, entity);
    break;

  case TEXTS_ACTION_USE:
    tmp = texts_entity_use (&texts, entity, what_id);
    break;

  case TEXTS_ACTION_SPEAK:
    tmp = texts_entity_speak (&texts, entity);
    break;

  case TEXTS_ACTION_MOVE:
    tmp = texts_entity_move (&texts, entity);
    break;

  default:
    tmp = NULL;
    break;
  }
  res->text = tmp;

  return res;
}

static void *
texts_look (const char *id)
{
  xiag_module_id_type_t type;

  MOD_LOG (texts.handle, XIAG_MODULE_MSG_INFO, __FUNCTION__);

  type = xiag_module_id_type_get (texts.handle, (char *) id);
  switch (type)
  {
  case XIAG_MODULE_ID_ENTITY:
    return texts_action (id, TEXTS_ACTION_LOOK, NULL);

  case XIAG_MODULE_ID_ROOM:
  {
    char *tmp;
    const char *name;
    texts_result_t *res;
    texts_room_t *room = texts_engine_room_search (&texts, (char *) id);
    if (!room)
      return NULL;

    res = malloc (sizeof (texts_result_t));
    if (!res)
      return NULL;

    name = texts_room_name_get (room);
    res->name = name ? strdup (name) : NULL;

    tmp = texts_room_look (&texts, room);
    res->text = tmp;

    return res;
  }

  default:
    return NULL;
  }
}

static void *
texts_take (const char *id)
{
  MOD_LOG (texts.handle, XIAG_MODULE_MSG_INFO, __FUNCTION__);

  return texts_action (id, TEXTS_ACTION_TAKE, NULL);
}

static void *
texts_use (const char *id, const char *what_id)
{
  MOD_LOG (texts.handle, XIAG_MODULE_MSG_INFO, __FUNCTION__);

  return texts_action (id, TEXTS_ACTION_USE, what_id);
}

static void *
texts_speak (const char *id)
{
  MOD_LOG (texts.handle, XIAG_MODULE_MSG_INFO, __FUNCTION__);

  return texts_action (id, TEXTS_ACTION_SPEAK, NULL);
}

static void *
texts_move (const char *id)
{
  MOD_LOG (texts.handle, XIAG_MODULE_MSG_INFO, __FUNCTION__);

  return texts_action (id, TEXTS_ACTION_MOVE, NULL);
}

static xiag_module_terminal_t *
texts_data_terminal (void *data)
{
  texts_result_t *res = data;
  xiag_module_terminal_t *data_term;

  MOD_LOG (texts.handle, XIAG_MODULE_MSG_INFO, __FUNCTION__);

  if (!res)
    return NULL;

  data_term = calloc (1, sizeof (xiag_module_terminal_t));
  if (!data_term)
    return NULL;

  data_term->name = res->name;
  data_term->text = res->text;

  return data_term;
}

static void
texts_data_free (void *data)
{
  texts_result_t *res = data;

  MOD_LOG (texts.handle, XIAG_MODULE_MSG_INFO, __FUNCTION__);

  if (!res)
    return;

  if (res->name)
    free (res->name);
  if (res->text)
    free (res->text);
  free (res);
}

static char *
texts_item2id (void *item)
{
  char *name = item, *idpos;
  char res[256] = "";
  const char *id;
  unsigned int size;

  MOD_LOG (texts.handle, XIAG_MODULE_MSG_INFO, __FUNCTION__);

  if (!item)
    return NULL;

  /*
   * FIXME: an entity with the same name can exist in the inventory and in
   *        the room.
   */

  /* search in the inventory in priority */
  size = xiag_module_inventory_size (texts.handle);
  MOD_LOG (texts.handle, XIAG_MODULE_MSG_INFO, "inventory size: %u", size);
  for (; size; size--)
  {
    texts_entity_t *entity;
    const char *rname;
    char *id = xiag_module_inventory_id (texts.handle, size);
    if (!id)
      continue;

    entity = texts_engine_entity_search (&texts, id);
    if (!entity)
    {
      free (id);
      continue;
    }

    rname = texts_entity_name_get (entity);
    if (!strcmp (name, rname))
    {
      MOD_LOG (texts.handle,
               XIAG_MODULE_MSG_INFO, "%s found in the inventory", name);
      return id; /* item found */
    }
    free (id);
  }

  /* or search in the current position (room) */
  idpos = xiag_module_idpos_get (texts.handle);
  if (!idpos)
    return NULL;

  id = texts_engine_name2id (&texts, idpos, name);
  if (id)
    snprintf (res, sizeof (res), "%s.%s", idpos, id);

  free (idpos);
  return strlen (res) ? strdup (res) : NULL;
}

static xiag_module_t texts_mod = {
  .init           = texts_init,
  .uninit         = texts_uninit,
  .look           = texts_look,
  .take           = texts_take,
  .use            = texts_use,
  .speak          = texts_speak,
  .move           = texts_move,
  .data_terminal  = texts_data_terminal,
  .data_graphical = NULL,
  .data_free      = texts_data_free,
  .item2id        = texts_item2id,
};

XIAG_MODULE_REGISTER (&texts_mod, "texts", "0.0.1", "Text XML support for XIAG")

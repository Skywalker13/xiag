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
#include <string.h>

#include "texts_room.h"

struct texts_room_s {
  struct texts_room_s *next;
  char *id;
  char *name;
  texts_entity_t *entities;
  texts_condition_t *conditions;
};


const char *
texts_room_name_get (texts_room_t *room)
{
  if (!room)
    return NULL;

  return room->name;
}

texts_entity_t *
texts_room_entities_get (texts_room_t *room)
{
  if (!room)
    return NULL;

  return room->entities;
}

static texts_room_t *
texts_room_new (char *id, char *name,
                texts_entity_t *entities, texts_condition_t *conditions)
{
  texts_room_t *room;

  if (!id)
    return NULL;

  room = calloc (1, sizeof (texts_room_t));
  if (!room)
    return NULL;

  room->id = id;
  room->name = name;
  room->entities = entities;
  room->conditions = conditions;

  return room;
}

void
texts_room_free (texts_room_t *room)
{
  texts_room_t *room_p;

  while (room)
  {
    texts_entity_free (room->entities);
    texts_condition_free (room->conditions);
    if (room->id)
      free (room->id);
    if (room->name)
      free (room->name);
    room_p = room;
    room = room->next;
    free (room_p);
  }
}

texts_room_t *
texts_room_add (texts_room_t *room, char *id, char *name,
                texts_entity_t *entities, texts_condition_t *conditions)
{
  if (!room)
  {
    room = texts_room_new (id, name, entities, conditions);
    return room;
  }

  while (room->next)
    room = room->next;
  room->next = texts_room_new (id, name, entities, conditions);
  return room->next;
}

texts_room_t *
texts_room_search_fromid (texts_room_t *room, char *id)
{
  if (!id)
    return NULL;

  for (; room; room = room->next)
    if (!strcmp (room->id, id))
      return room;

  return NULL;
}

char *
texts_room_look (texts_t *texts, texts_room_t *room)
{
  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  if (!room)
    return NULL;

  if (!room->conditions)
    return NULL; /* NOP */

  return texts_condition_look (texts, room->conditions);
}

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

#include "room.h"

struct room_s {
  struct room_s *next;
  scene_t *parent;
  char *id;
  entity_t *entities;
};


entity_t *
room_entities_get (room_t *room)
{
  if (!room)
    return NULL;

  return room->entities;
}

const char *
room_id_get (room_t *room)
{
  if (!room)
    return NULL;

  return room->id;
}

void
room_entities_set (room_t *room, entity_t *entities)
{
  if (!room)
    return;

  room->entities = entities;
}

scene_t *
room_parent_get (room_t *room)
{
  if (!room)
    return NULL;

  return room->parent;
}

static room_t *
room_new (char *id, scene_t *parent)
{
  room_t *room;

  if (!id)
    return NULL;

  room = calloc (1, sizeof (room_t));
  if (!room)
    return NULL;

  room->id = id;
  room->parent = parent;

  return room;
}

void
room_free (room_t *room)
{
  room_t *room_p;

  while (room)
  {
    entity_free (room->entities);
    if (room->id)
      free (room->id);
    room_p = room;
    room = room->next;
    free (room_p);
  }
}

room_t *
room_add (room_t *room, char *id, scene_t *parent)
{
  if (!room)
  {
    room = room_new (id, parent);
    return room;
  }

  while (room->next)
    room = room->next;
  room->next = room_new (id, parent);
  return room->next;
}

room_t *
room_search_fromid (room_t *room, char *id)
{
  if (!id)
    return NULL;

  for (; room; room = room->next)
    if (!strcmp (room->id, id))
      return room;

  return NULL;
}

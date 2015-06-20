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

#include "entity.h"
#include "room.h"
#include "scene.h"
#include "chapter.h"
#include "inventory.h"


typedef struct inventory_s {
  struct inventory_s *next;
  entity_t *entity;
} inventory_t;

static inventory_t *g_inventory;


static inventory_t *
inventory_add (entity_t *entity)
{
  inventory_t *inv;

  inv = calloc (1, sizeof (inventory_t));
  if (!inv)
    return NULL;

  inv->entity = entity;

  return inv;
}

void
inventory_free (void)
{
  inventory_t *inv_p, *inv = g_inventory;

  while (inv)
  {
    inv_p = inv;
    inv = inv->next;
    free (inv_p);
  }

  g_inventory = NULL;
}

void
inventory_add_entity (entity_t *entity)
{
  inventory_t *inv = g_inventory;

  if (!entity)
    return;

  if (!inv)
    g_inventory = inventory_add (entity);
  else
  {
    while (inv->next)
      inv = inv->next;
    inv->next = inventory_add (entity);
  }
}

void
inventory_remove_entity (entity_t *entity)
{
  inventory_t *inv = g_inventory, *inv_p = NULL;

  if (!entity)
    return;

  while (inv)
  {
    if (inv->entity == entity)
      break;
    inv_p = inv;
    inv = inv->next;
  }

  if (!inv)
    return;

  if (inv_p)
    inv_p->next = inv->next;
  else
    g_inventory = inv->next;
  free (inv);
}

int
inventory_entity_is_available (entity_t *entity)
{
  inventory_t *inv = g_inventory;

  if (!entity)
    return 0;

  while (inv)
  {
    if (inv->entity == entity)
      return 1;
    inv = inv->next;
  }

  return 0;
}

unsigned int
inventory_size (void)
{
  int s = 0;
  inventory_t *inv;

  for (inv = g_inventory; inv; inv = inv->next)
    s++;

  return s;
}

char *
inventory_id_get (unsigned int item)
{
  char id[256];
  inventory_t *inv;
  chapter_t *chapter;
  scene_t *scene;
  room_t *room;

  if (!item)
    return NULL;

  for (inv = g_inventory, --item; inv && item > 0; inv = inv->next, item--)
    ;

  if (item || !inv) /* end reached but nothing found */
    return NULL;

  room = entity_parent_get (inv->entity);
  scene = room_parent_get (room);
  chapter = scene_parent_get (scene);

  snprintf (id, sizeof (id), "%s.%s.%s.%s",
            chapter_id_get (chapter),
            scene_id_get (scene),
            room_id_get (room),
            entity_id_get (inv->entity));
  return strdup (id);
}

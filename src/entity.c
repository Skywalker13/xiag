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

#include "logs.h"
#include "entity.h"
#include "inventory.h"

#define MODULE_NAME "entity"

struct entity_s {
  struct entity_s *next;
  room_t *parent;
  char *id;
  char *state;
  condition_t *conditions;
};


void
entity_inventory_set (entity_t *entity, entity_inventory_t inventory)
{
  if (!entity)
    return;

  if (inventory == ENTITY_INVENTORY_YES)
    inventory_add_entity (entity);
  else if (inventory == ENTITY_INVENTORY_NO)
    inventory_remove_entity (entity);
}

entity_inventory_t
entity_inventory_get (entity_t *entity)
{
  if (!entity)
    return ENTITY_INVENTORY_NULL;

  if (inventory_entity_is_available (entity))
    return ENTITY_INVENTORY_YES;
  return ENTITY_INVENTORY_NO;
}

void
entity_state_set (entity_t *entity, char *state)
{
  if (!entity)
    return;

  if (entity->state)
    free (entity->state);

  entity->state = state;
}

const char *
entity_state_get (entity_t *entity)
{
  if (!entity)
    return NULL;

  return entity->state;
}

const char *
entity_id_get (entity_t *entity)
{
  if (!entity)
    return NULL;

  return entity->id;
}

room_t *
entity_parent_get (entity_t *entity)
{
  if (!entity)
    return NULL;

  return entity->parent;
}

static entity_t *
entity_new (char *id, entity_inventory_t inventory, char *state,
            condition_t *conditions, room_t *parent)
{
  entity_t *entity;

  if (!id)
    return NULL;

  entity = calloc (1, sizeof (entity_t));
  if (!entity)
    return NULL;

  entity->id = id;
  entity->state = state;
  entity->conditions = conditions;
  entity->parent = parent;

  if (inventory == ENTITY_INVENTORY_YES)
    inventory_add_entity (entity);

  return entity;
}

void
entity_free (entity_t *entity)
{
  entity_t *entity_p;

  while (entity)
  {
    condition_free (entity->conditions);
    if (entity->id)
      free (entity->id);
    if (entity->state)
      free (entity->state);
    entity_p = entity;
    entity = entity->next;
    free (entity_p);
  }
}

entity_t *
entity_add (entity_t *entity,
            char *id, entity_inventory_t inventory, char *state,
            condition_t *conditions, room_t *parent)
{
  if (!entity)
  {
    entity = entity_new (id, inventory, state, conditions, parent);
    return entity;
  }

  while (entity->next)
    entity = entity->next;
  entity->next = entity_new (id, inventory, state, conditions, parent);
  return entity->next;
}

entity_t *
entity_search_fromid (entity_t *entity, char *id)
{
  if (!id)
    return NULL;

  for (; entity; entity = entity->next)
    if (!strcmp (entity->id, id))
      return entity;

  return NULL;
}

int
entity_look (xiag_t *xiag, entity_t *entity)
{
  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (!entity)
    return -1;

  if (!entity->conditions)
    return 0; /* NOP */

  return condition_look (xiag, entity->conditions);
}

int
entity_take (xiag_t *xiag, entity_t *entity)
{
  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (!entity)
    return -1;

  if (!entity->conditions)
    return 0; /* NOP */

  return condition_take (xiag, entity->conditions);
}

int
entity_use (xiag_t *xiag, entity_t *entity, const char *what_id)
{
  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (!entity)
    return -1;

  if (!entity->conditions)
    return 0; /* NOP */

  return condition_use (xiag, entity->conditions, what_id);
}

int
entity_speak (xiag_t *xiag, entity_t *entity)
{
  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (!entity)
    return -1;

  if (!entity->conditions)
    return 0; /* NOP */

  return condition_speak (xiag, entity->conditions);
}

int
entity_move (xiag_t *xiag, entity_t *entity)
{
  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (!entity)
    return -1;

  if (!entity->conditions)
    return 0; /* NOP */

  return condition_move (xiag, entity->conditions);
}

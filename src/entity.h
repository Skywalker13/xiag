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

#ifndef ENTITY_H
#define ENTITY_H

typedef struct entity_s entity_t;

typedef enum entity_inventory {
  ENTITY_INVENTORY_NULL = -1,
  ENTITY_INVENTORY_NO,
  ENTITY_INVENTORY_YES,
} entity_inventory_t;

#include "room.h"
#include "condition.h"

void entity_inventory_set (entity_t *entity, entity_inventory_t inventory);
entity_inventory_t entity_inventory_get (entity_t *entity);
void entity_state_set (entity_t *entity, char *state);
const char *entity_state_get (entity_t *entity);
const char *entity_id_get (entity_t *entity);
room_t *entity_parent_get (entity_t *entity);

void entity_free (entity_t *entity);
entity_t *entity_add (entity_t *entity,
                      char *id, entity_inventory_t inventory, char *state,
                      condition_t *conditions, room_t *parent);

entity_t *entity_search_fromid (entity_t *entity, char *id);

int entity_look (xiag_t *xiag, entity_t *entity);
int entity_take (xiag_t *xiag, entity_t *entity);
int entity_use (xiag_t *xiag, entity_t *entity, const char *what_id);
int entity_speak (xiag_t *xiag, entity_t *entity);
int entity_move (xiag_t *xiag, entity_t *entity);

#endif /* ENTITY_H */

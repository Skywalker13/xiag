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

#ifndef ROOM_H
#define ROOM_H

typedef struct room_s room_t;

#include "scene.h"
#include "entity.h"

void room_free (room_t *room);
room_t *room_add (room_t *room, char *id, scene_t *parent);

entity_t *room_entities_get (room_t *room);
room_t *room_search_fromid (room_t *room, char *id);
const char *room_id_get (room_t *room);
void room_entities_set (room_t *room, entity_t *entities);
scene_t *room_parent_get (room_t *room);

#endif /* ROOM_H */

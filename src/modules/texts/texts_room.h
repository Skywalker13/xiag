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

#ifndef TEXTS_ROOM_H
#define TEXTS_ROOM_H

#include "texts_entity.h"

typedef struct texts_room_s texts_room_t;

const char *texts_room_name_get (texts_room_t *room);

void texts_room_free (texts_room_t *room);
texts_room_t *texts_room_add (texts_room_t *room, char *id, char *name,
                              texts_entity_t *entities,
                              texts_condition_t *conditions);

texts_entity_t *texts_room_entities_get (texts_room_t *room);
texts_room_t *texts_room_search_fromid (texts_room_t *room, char *id);

char *texts_room_look (texts_t *texts, texts_room_t *room);

#endif /* TEXTS_ROOM_H */

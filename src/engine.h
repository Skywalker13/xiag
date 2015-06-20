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

#ifndef ENGINE_H
#define ENGINE_H

#include "entity.h"

typedef enum engine_id_type {
  ENGINE_ID_UNKNOWN,
  ENGINE_ID_CHAPTER,
  ENGINE_ID_SCENE,
  ENGINE_ID_ROOM,
  ENGINE_ID_ENTITY,
} engine_id_type_t;


int engine_look (xiag_t *xiag, char *id);
int engine_take (xiag_t *xiag, char *id);
int engine_use (xiag_t *xiag, char *id, char *what_id);
int engine_speak (xiag_t *xiag, char *id);
int engine_move (xiag_t *xiag, char *id);

engine_id_type_t engine_id_type_get (xiag_t *xiag, char *id);
void engine_id_split (const char *id, char **id_chapter, char **id_scene,
                                      char **id_room, char **id_entity);
entity_t *engine_entity_search (xiag_t *xiag, char *id);
int engine_idpos_check (xiag_t *xiag, char *id);
void engine_idpos_goto (xiag_t *xiag, char *id);
int engine_inventory_check (xiag_t *xiag, char *id);
char *engine_idpos_get (xiag_t *xiag);

#endif /* ENGINE_H */

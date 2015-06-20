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

#ifndef TEXTS_ENTITY_H
#define TEXTS_ENTITY_H

typedef struct texts_entity_s texts_entity_t;

#include "texts_condition.h"

const char *texts_entity_id_get (texts_entity_t *entity);
const char *texts_entity_name_get (texts_entity_t *entity);

void texts_entity_free (texts_entity_t *entity);
texts_entity_t *texts_entity_add (texts_entity_t *entity,
                                  char *id, char *name,
                                  texts_condition_t *conditions);

texts_entity_t *texts_entity_search_fromid (texts_entity_t *entity, char *id);
texts_entity_t *texts_entity_search_fromname (texts_entity_t *entity,
                                              char *name);

char *texts_entity_look (texts_t *texts, texts_entity_t *entity);
char *texts_entity_take (texts_t *texts, texts_entity_t *entity);
char *texts_entity_use (texts_t *texts, texts_entity_t *entity,
                              const char *what_id);
char *texts_entity_speak (texts_t *texts, texts_entity_t *entity);
char *texts_entity_move (texts_t *texts, texts_entity_t *entity);

#endif /* TEXTS_ENTITY_H */

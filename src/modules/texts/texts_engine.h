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

#ifndef TEXTS_ENGINE_H
#define TEXTS_ENGINE_H

#include "texts_entity.h"

texts_room_t *texts_engine_room_search (texts_t *texts, char *id);
texts_entity_t *texts_engine_entity_search (texts_t *texts, char *id);
const char *texts_engine_name2id (texts_t *texts, char *id_room, char *name);

#endif /* TEXTS_ENGINE_H */

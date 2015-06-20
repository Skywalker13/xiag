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

#ifndef TEXTS_H
#define TEXTS_H

#include "xiag_module.h"

typedef struct texts_story_s texts_story_t;

typedef enum texts_action {
  TEXTS_ACTION_LOOK,
  TEXTS_ACTION_TAKE,
  TEXTS_ACTION_USE,
  TEXTS_ACTION_USE_WITH,
  TEXTS_ACTION_SPEAK,
  TEXTS_ACTION_MOVE,
} texts_action_t;

typedef struct texts_s {
  XIAG_HANDLE *handle;
  texts_story_t *story;
} texts_t;

#endif /* TEXTS_H */

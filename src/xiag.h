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

#ifndef XIAG_H
#define XIAG_H

typedef struct interface_s interface_t;
typedef struct story_s story_t;
typedef struct module_s module_t;

typedef enum xiag_verbosity_level {
  XIAG_MSG_VERBOSE,
  XIAG_MSG_INFO,
  XIAG_MSG_WARNING,
  XIAG_MSG_ERROR,
  XIAG_MSG_CRITICAL,
  XIAG_MSG_NONE,
} xiag_verbosity_level_t;

typedef enum xiag_action {
  XIAG_ACTION_LOOK,
  XIAG_ACTION_TAKE,
  XIAG_ACTION_USE,
  XIAG_ACTION_USE_WITH,
  XIAG_ACTION_SPEAK,
  XIAG_ACTION_MOVE,
} xiag_action_t;

typedef struct xiag_s {
  interface_t *interface;
  story_t *story;
  module_t *modules;
  xiag_verbosity_level_t verbosity;
  /* current room ID in the story */
  char *id_chapter;
  char *id_scene;
  char *id_room;
} xiag_t;

#define PATH_BUFFER  512

#endif /* XIAG_H */

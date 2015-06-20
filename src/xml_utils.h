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

#ifndef XML_UTILS_H
#define XML_UTILS_H

#include "exmlparser.h"

#define XML_ID         "id"
#define XML_NAME       "name"
#define XML_INVENTORY  "inventory"
#define XML_STATE      "state"
#define XML_GOTO       "goto"
#define XML_ENTITY     "entity"
#define XML_ROOM       "room"
#define XML_SCENE      "scene"
#define XML_CHAPTER    "chapter"

#define XML_LOOK       "look"
#define XML_TAKE       "take"
#define XML_USE        "use"
#define XML_USE_WITH   "use_with"
#define XML_SPEAK      "speak"
#define XML_MOVE       "move"

xml_node_t *exml_find_node (xml_node_t *root, char *name);

#endif /* XML_UTILS_H */

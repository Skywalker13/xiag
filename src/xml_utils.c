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

#include "xml_utils.h"


xml_node_t *
exml_find_node (xml_node_t *root, char *name)
{
  xml_node_t *node = NULL;

  if (!root || !root->name || !name)
    return NULL;

  if (!strcmp (root->name, name))
    return root;

  node = exml_find_node (root->child, name);
  if (node)
    return node;

  node = exml_find_node (root->next, name);
  if (node)
    return node;

  return NULL;
}

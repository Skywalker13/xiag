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

#include "scene.h"

struct scene_s {
  struct scene_s *next;
  chapter_t *parent;
  char *id;
  room_t *rooms;
};


room_t *
scene_rooms_get (scene_t *scene)
{
  if (!scene)
    return NULL;

  return scene->rooms;
}

const char *
scene_id_get (scene_t *scene)
{
  if (!scene)
    return NULL;

  return scene->id;
}

void
scene_rooms_set (scene_t *scene, room_t *rooms)
{
  if (!scene)
    return;

  scene->rooms = rooms;
}

chapter_t *
scene_parent_get (scene_t *scene)
{
  if (!scene)
    return NULL;

  return scene->parent;
}

static scene_t *
scene_new (char *id, chapter_t *parent)
{
  scene_t *scene;

  if (!id)
    return NULL;

  scene = calloc (1, sizeof (scene_t));
  if (!scene)
    return NULL;

  scene->id = id;
  scene->parent = parent;

  return scene;
}

void
scene_free (scene_t *scene)
{
  scene_t *scene_p;

  while (scene)
  {
    room_free (scene->rooms);
    if (scene->id)
      free (scene->id);
    scene_p = scene;
    scene = scene->next;
    free (scene_p);
  }
}

scene_t *
scene_add (scene_t *scene, char *id, chapter_t *parent)
{
  if (!scene)
  {
    scene = scene_new (id, parent);
    return scene;
  }

  while (scene->next)
    scene = scene->next;
  scene->next = scene_new (id, parent);
  return scene->next;
}

scene_t *
scene_search_fromid (scene_t *scene, char *id)
{
  if (!id)
    return NULL;

  for (; scene; scene = scene->next)
    if (!strcmp (scene->id, id))
      return scene;

  return NULL;
}

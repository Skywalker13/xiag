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

#include "texts_scene.h"

struct texts_scene_s {
  struct texts_scene_s *next;
  char *id;
  char *name;
  texts_room_t *rooms;
};


texts_room_t *
texts_scene_rooms_get (texts_scene_t *scene)
{
  if (!scene)
    return NULL;

  return scene->rooms;
}

static texts_scene_t *
texts_scene_new (char *id, char *name, texts_room_t *rooms)
{
  texts_scene_t *scene;

  if (!id)
    return NULL;

  scene = calloc (1, sizeof (texts_scene_t));
  if (!scene)
    return NULL;

  scene->id = id;
  scene->name = name;
  scene->rooms = rooms;

  return scene;
}

void
texts_scene_free (texts_scene_t *scene)
{
  texts_scene_t *scene_p;

  while (scene)
  {
    texts_room_free (scene->rooms);
    if (scene->id)
      free (scene->id);
    if (scene->name)
      free (scene->name);
    scene_p = scene;
    scene = scene->next;
    free (scene_p);
  }
}

texts_scene_t *
texts_scene_add (texts_scene_t *scene,
                 char *id, char *name, texts_room_t *rooms)
{
  if (!scene)
  {
    scene = texts_scene_new (id, name, rooms);
    return scene;
  }

  while (scene->next)
    scene = scene->next;
  scene->next = texts_scene_new (id, name, rooms);
  return scene->next;
}

texts_scene_t *
texts_scene_search_fromid (texts_scene_t *scene, char *id)
{
  if (!id)
    return NULL;

  for (; scene; scene = scene->next)
    if (!strcmp (scene->id, id))
      return scene;

  return NULL;
}

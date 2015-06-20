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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xiag.h"
#include "story.h"
#include "chapter.h"
#include "scene.h"
#include "room.h"
#include "entity.h"
#include "logs.h"
#include "engine.h"

#define MODULE_NAME "engine"


engine_id_type_t
engine_id_type_get (xiag_t *xiag, char *id)
{
  char *it = id;
  char *it_tmp = NULL;
  int parts = 0;

  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, "%s (%s)", __FUNCTION__, id);

  /* test if the string is valid */
  if (!id || *id == '\0' || *id == '.' || *(id + strlen (id) - 1) == '.')
    return ENGINE_ID_UNKNOWN;

  while ((it = strchr (it + 1, '.')))
  {
    /* two '.' must not be consecutive */
    if (it_tmp && it - it_tmp < 2)
      return ENGINE_ID_UNKNOWN;

    it_tmp = it;
    parts++;
  }

  switch (parts)
  {
  case 0:
    return ENGINE_ID_CHAPTER;

  case 1:
    return ENGINE_ID_SCENE;

  case 2:
    return ENGINE_ID_ROOM;

  case 3:
    return ENGINE_ID_ENTITY;

  default:
    return ENGINE_ID_UNKNOWN;
  }
}

static inline void *
engine_id_alloc (const char *it1, const char *it2)
{
  char *result;
  size_t size = it2 - it1;

  result = malloc (size + 1);
  if (!result)
    return NULL;
  memcpy (result, it1, size);
  *(result + size) = '\0';
  return result;
}

void
engine_id_split (const char *id, char **id_chapter, char **id_scene,
                                 char **id_room, char **id_entity)
{
  char *it1 = NULL, *it2 = NULL, *it3 = NULL;

  it1 = strchr (id, '.');
  if (!it1)
  {
    *id_chapter = strdup (id);
    return;
  }

  *id_chapter = engine_id_alloc (id, it1);

  it2 = strchr (it1 + 1, '.');
  if (!it2)
  {
    *id_scene = strdup (it1 + 1);
    return;
  }

  *id_scene = engine_id_alloc (it1 + 1, it2);

  it3 = strchr (it2 + 1, '.');
  if (!it3)
  {
    *id_room = strdup (it2 + 1);
    return;
  }

  *id_room = engine_id_alloc (it2 + 1, it3);
  *id_entity = strdup (it3 + 1);
}

entity_t *
engine_entity_search (xiag_t *xiag, char *id)
{
  char *id_chapter = NULL, *id_scene = NULL, *id_room = NULL, *id_entity = NULL;
  chapter_t *chapter, *chapters;
  scene_t *scene, *scenes;
  room_t *room, *rooms;
  entity_t *entity = NULL, *entities;

  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (engine_id_type_get (xiag, id) != ENGINE_ID_ENTITY)
    return NULL;

  engine_id_split (id, &id_chapter, &id_scene, &id_room, &id_entity);
  XIAG_LOG (xiag, XIAG_MSG_VERBOSE,
            MODULE_NAME, "chapter:%s scene:%s room:%s entity:%s",
                         id_chapter, id_scene, id_room, id_entity);

  chapters = story_chapters_get (xiag);
  if (!chapters)
    goto out;

  chapter = chapter_search_fromid (chapters, id_chapter);
  if (!chapter)
    goto out;

  scenes = chapter_scenes_get (chapter);
  if (!scenes)
    goto out;

  scene = scene_search_fromid (scenes, id_scene);
  if (!scene)
    goto out;

  rooms = scene_rooms_get (scene);
  if (!rooms)
    goto out;

  room = room_search_fromid (rooms, id_room);
  if (!room)
    goto out;

  entities = room_entities_get (room);
  if (!entities)
    goto out;

  entity = entity_search_fromid (entities, id_entity);

 out:
  if (!entity)
    XIAG_LOG (xiag, XIAG_MSG_WARNING, MODULE_NAME, "bad entity: %s", id);

  free (id_chapter);
  free (id_scene);
  free (id_room);
  free (id_entity);
  return entity;
}

int
engine_inventory_check (xiag_t *xiag, char *id)
{
  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (engine_id_type_get (xiag, id) != ENGINE_ID_ENTITY)
    return 0;

  entity_t *entity = engine_entity_search (xiag, id);
  if (!entity)
    return 0;

  if (entity_inventory_get (entity) == ENTITY_INVENTORY_YES)
    return 1;

  return 0;
}

int
engine_idpos_check (xiag_t *xiag, char *id)
{
  int res = 0;
  char *id_chapter = NULL, *id_scene = NULL, *id_room = NULL, *id_entity = NULL;

  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (engine_inventory_check (xiag, id))
    return 1;

  engine_id_split (id, &id_chapter, &id_scene, &id_room, &id_entity);

  if (!id_room)
    goto out;

  if (strcmp (id_chapter, xiag->id_chapter))
    goto out;

  if (strcmp (id_scene, xiag->id_scene))
    goto out;

  if (strcmp (id_room, xiag->id_room))
    goto out;

  res = 1;

 out:
  if (!res)
    XIAG_LOG (xiag, XIAG_MSG_WARNING, MODULE_NAME, "%s unavailable", id);

  if (id_chapter)
    free (id_chapter);
  if (id_scene)
    free (id_scene);
  if (id_room)
    free (id_room);
  if (id_entity)
    free (id_entity);
  return res;
}

void
engine_idpos_goto (xiag_t *xiag, char *id)
{
  char *id_chapter = NULL, *id_scene = NULL, *id_room = NULL;

  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (engine_id_type_get (xiag, id) != ENGINE_ID_ROOM)
    return;

  engine_id_split (id, &id_chapter, &id_scene, &id_room, NULL);

  if (xiag->id_chapter)
    free (xiag->id_chapter);
  xiag->id_chapter = strdup (id_chapter);
  if (xiag->id_scene)
    free (xiag->id_scene);
  xiag->id_scene = strdup (id_scene);
  if (xiag->id_room)
    free (xiag->id_room);
  xiag->id_room = strdup (id_room);

  free (id_chapter);
  free (id_scene);
  free (id_room);
}

char *
engine_idpos_get (xiag_t *xiag)
{
  char *idpos;

  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (!xiag->id_chapter || !xiag->id_scene || !xiag->id_room)
    return NULL;

  idpos = malloc (strlen (xiag->id_chapter)
                  + strlen (xiag->id_scene)
                  + strlen (xiag->id_room) + 3);
  if (!idpos)
    return NULL;

  sprintf (idpos, "%s.%s.%s", xiag->id_chapter, xiag->id_scene, xiag->id_room);
  return idpos;
}

static int
engine_action (xiag_t *xiag, xiag_action_t a, char *id, char *what_id)
{
  entity_t *entity;

  entity = engine_entity_search (xiag, id);
  if (!entity)
    return -1;

  switch (a)
  {
  case XIAG_ACTION_LOOK:
    return entity_look (xiag, entity);

  case XIAG_ACTION_TAKE:
    return entity_take (xiag, entity);

  case XIAG_ACTION_USE:
    return entity_use (xiag, entity, what_id);

  case XIAG_ACTION_SPEAK:
    return entity_speak (xiag, entity);

  case XIAG_ACTION_MOVE:
    return entity_move (xiag, entity);

  default:
    return -1;
  }
}

int
engine_look (xiag_t *xiag, char *id)
{
  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  return engine_action (xiag, XIAG_ACTION_LOOK, id, NULL);
}

int
engine_take (xiag_t *xiag, char *id)
{
  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  return engine_action (xiag, XIAG_ACTION_TAKE, id, NULL);
}

int
engine_use (xiag_t *xiag, char *id, char *what_id)
{
  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  return engine_action (xiag, XIAG_ACTION_USE, id, what_id);
}

int
engine_speak (xiag_t *xiag, char *id)
{
  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  return engine_action (xiag, XIAG_ACTION_SPEAK, id, NULL);
}

int
engine_move (xiag_t *xiag, char *id)
{
  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  return engine_action (xiag, XIAG_ACTION_MOVE, id, NULL);
}

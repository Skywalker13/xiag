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

#include "texts.h"
#include "texts_story.h"
#include "texts_chapter.h"
#include "texts_scene.h"
#include "texts_room.h"
#include "texts_entity.h"
#include "texts_engine.h"


texts_room_t *
texts_engine_room_search (texts_t *texts, char *id)
{
  char *id_chapter = NULL, *id_scene = NULL, *id_room = NULL;
  texts_chapter_t *chapter, *chapters;
  texts_scene_t *scene, *scenes;
  texts_room_t *room, *rooms;

  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  if (xiag_module_id_type_get (texts->handle, id) != XIAG_MODULE_ID_ROOM)
    return NULL;

  xiag_module_id_split (id, &id_chapter, &id_scene, &id_room, NULL);
  MOD_LOG (texts->handle,
           XIAG_MODULE_MSG_VERBOSE, "chapter:%s scene:%s room:%s",
                                    id_chapter, id_scene, id_room);

  chapters = texts_story_chapters_get (texts);
  if (!chapters)
    goto out;

  chapter = texts_chapter_search_fromid (chapters, id_chapter);
  if (!chapter)
    goto out;

  scenes = texts_chapter_scenes_get (chapter);
  if (!scenes)
    goto out;

  scene = texts_scene_search_fromid (scenes, id_scene);
  if (!scene)
    goto out;

  rooms = texts_scene_rooms_get (scene);
  if (!rooms)
    goto out;

  room = texts_room_search_fromid (rooms, id_room);

 out:
  free (id_chapter);
  free (id_scene);
  free (id_room);
  return room;
}

texts_entity_t *
texts_engine_entity_search (texts_t *texts, char *id)
{
  char *id_chapter = NULL, *id_scene = NULL, *id_room = NULL, *id_entity = NULL;
  char id_room_buf[256];
  texts_room_t *room;
  texts_entity_t *entity = NULL, *entities;

  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  if (xiag_module_id_type_get (texts->handle, id) != XIAG_MODULE_ID_ENTITY)
    return NULL;

  xiag_module_id_split (id, &id_chapter, &id_scene, &id_room, &id_entity);
  MOD_LOG (texts->handle,
           XIAG_MODULE_MSG_VERBOSE, "chapter:%s scene:%s room:%s entity:%s",
                                    id_chapter, id_scene, id_room, id_entity);

  snprintf (id_room_buf,
            sizeof (id_room_buf), "%s.%s.%s", id_chapter, id_scene, id_room);

  room = texts_engine_room_search (texts, id_room_buf);
  if (!room)
    goto out;

  entities = texts_room_entities_get (room);
  if (!entities)
    goto out;

  entity = texts_entity_search_fromid (entities, id_entity);

 out:
  free (id_chapter);
  free (id_scene);
  free (id_room);
  free (id_entity);
  return entity;
}

const char *
texts_engine_name2id (texts_t *texts, char *idpos, char *name)
{
  texts_room_t *room;
  texts_entity_t *entities, *entity;

  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  if (!name || !idpos)
    return NULL;

  room = texts_engine_room_search (texts, idpos);
  if (!room)
    return NULL;

  entities = texts_room_entities_get (room);
  if (!entities)
    return NULL;

  entity = texts_entity_search_fromname (entities, name);
  if (!name)
    return NULL;

  return texts_entity_id_get (entity);
}

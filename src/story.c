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
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "xiag.h"
#include "interface.h"
#include "xml_utils.h"
#include "chapter.h"
#include "scene.h"
#include "room.h"
#include "entity.h"
#include "condition.h"
#include "logs.h"
#include "story.h"

#define MODULE_NAME "story"

struct story_s {
  char *xml_file;
  chapter_t *chapters;
};


static inline int
get_inventory_value (const char *value)
{
  if (!strcmp (value, "yes"))
    return ENTITY_INVENTORY_YES;
  else if (!strcmp (value, "no"))
    return ENTITY_INVENTORY_NO;
  return ENTITY_INVENTORY_NULL;
}

static condition_result_t *
story_xml_load_results (xiag_t *xiag, xml_node_t *root_node)
{
  xml_node_t *node;
  condition_result_t *cond_result_first = NULL;
  condition_result_t *cond_result = NULL;

  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  for (node = root_node; node; node = node->next)
  {
    xml_property_t *props;
    char *id = NULL, *state = NULL;
    entity_inventory_t inventory = ENTITY_INVENTORY_NULL;
    res_type_t type = RESULT_TYPE_UNKNOWN;

    if (!strcmp (node->name, XML_INVENTORY))
    {
      type = RESULT_TYPE_INVENTORY;
      inventory = get_inventory_value (node->data);
    }
    else if (!strcmp (node->name, XML_STATE))
    {
      type = RESULT_TYPE_STATE;
      state = strdup (node->data);
    }
    else if (!strcmp (node->name, XML_GOTO))
      type = RESULT_TYPE_GOTO;
    else
    {
      XIAG_LOG (xiag, XIAG_MSG_ERROR,
                MODULE_NAME, "unknown tag (result): <%s>", node->name);
      continue;
    }

    /*
     * If there are more than one property, then the XML file is wrong
     * FIXME: fix id if it is uncomplete in the value.
     */
    for (props = node->props; props; props = props->next)
      if (!strcmp (props->name, XML_ENTITY) ||
          !strcmp (props->name, XML_ROOM)   ||
          !strcmp (props->name, XML_SCENE)  ||
          !strcmp (props->name, XML_CHAPTER))
        id = strdup (props->value);

    cond_result =
      condition_result_add (cond_result, type, id, inventory, state);

    if (!cond_result_first)
      cond_result_first = cond_result;
  }

  return cond_result_first;
}

static void
story_xml_load_condition_action (xiag_t *xiag, condition_t **condition,
                                 xml_node_t *root_node, xiag_action_t action)
{
  condition_t *condition_first = *condition;

  xml_property_t *props;
  char *id = NULL, *state = NULL;
  entity_inventory_t inventory = ENTITY_INVENTORY_NULL;
  condition_result_t *res = NULL;

  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  for (props = root_node->props; props; props = props->next)
  {
    if (!strcmp (props->name, XML_ENTITY))
      id = strdup (props->value);
    else if (!strcmp (props->name, XML_INVENTORY))
      inventory = get_inventory_value (props->value);
    else if (!strcmp (props->name, XML_STATE))
      state = strdup (props->value);
    else
      XIAG_LOG (xiag, XIAG_MSG_ERROR,
                MODULE_NAME, "unknown property (condition): %s", props->name);
  }

  if (root_node->child)
    res = story_xml_load_results (xiag, root_node->child);

  *condition = condition_add (*condition, action, id, inventory, state, res);

  if (!condition_first)
    condition_first = *condition;
  else
    *condition = condition_first;
}

static condition_t *
story_xml_load_conditions (xiag_t *xiag, xml_node_t *root_node)
{
  xml_node_t *node;
  condition_t *cond = NULL;

  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  for (node = root_node; node; node = node->next)
  {
    if (!strcmp (node->name, XML_LOOK))
      story_xml_load_condition_action (xiag, &cond, node, XIAG_ACTION_LOOK);
    else if (!strcmp (node->name, XML_TAKE))
      story_xml_load_condition_action (xiag, &cond, node, XIAG_ACTION_TAKE);
    else if (!strcmp (node->name, XML_USE))
      story_xml_load_condition_action (xiag, &cond, node, XIAG_ACTION_USE);
    else if (!strcmp (node->name, XML_USE_WITH))
      story_xml_load_condition_action (xiag, &cond, node, XIAG_ACTION_USE_WITH);
    else if (!strcmp (node->name, XML_SPEAK))
      story_xml_load_condition_action (xiag, &cond, node, XIAG_ACTION_SPEAK);
    else if (!strcmp (node->name, XML_MOVE))
      story_xml_load_condition_action (xiag, &cond, node, XIAG_ACTION_MOVE);
    else
      XIAG_LOG (xiag, XIAG_MSG_ERROR,
                MODULE_NAME, "unknown tag (action): <%s>", node->name);
  }

  return cond;
}

static entity_t *
story_xml_load_entities (xiag_t *xiag, xml_node_t *root_node, room_t *parent)
{
  xml_node_t *node;
  entity_t *entity_first = NULL;
  entity_t *entity = NULL;

  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  for (node = root_node; node; node = node->next)
  {
    xml_property_t *props;
    char *id = NULL, *state = NULL;
    entity_inventory_t inventory = ENTITY_INVENTORY_NULL;
    condition_t *conditions = NULL;

    if (strcmp (node->name, XML_ENTITY))
    {
      XIAG_LOG (xiag, XIAG_MSG_ERROR,
                MODULE_NAME, "unknown tag in the room: <%s>", node->name);
      continue;
    }

    for (props = node->props; props; props = props->next)
    {
      if (!strcmp (props->name, XML_ID))
        id = strdup (props->value);
      else if (!strcmp (props->name, XML_INVENTORY))
        inventory = get_inventory_value (props->value);
      else if (!strcmp (props->name, XML_STATE))
        state = strdup (props->value);
      else
        XIAG_LOG (xiag, XIAG_MSG_ERROR,
                  MODULE_NAME, "unknown property (entity): %s", props->name);
    }

    if (node->child)
      conditions = story_xml_load_conditions (xiag, node->child);

    entity = entity_add (entity, id, inventory, state, conditions, parent);

    if (!entity_first)
      entity_first = entity;
  }

  return entity_first;
}

static room_t *
story_xml_load_rooms (xiag_t *xiag, xml_node_t *root_node, scene_t *parent)
{
  xml_node_t *node;
  room_t *room_first = NULL;
  room_t *room = NULL;

  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  for (node = root_node; node; node = node->next)
  {
    xml_property_t *props;
    char *id = NULL;
    entity_t *entities = NULL;

    if (strcmp (node->name, XML_ROOM))
    {
      XIAG_LOG (xiag, XIAG_MSG_ERROR,
                MODULE_NAME, "unknown tag in the scene: <%s>", node->name);
      continue;
    }

    for (props = node->props; props; props = props->next)
      if (!strcmp (props->name, XML_ID))
        id = strdup (props->value);

    room = room_add (room, id, parent);

    if (node->child)
      entities = story_xml_load_entities (xiag, node->child, room);

    room_entities_set (room, entities);

    if (!room_first)
      room_first = room;
  }

  return room_first;
}

static scene_t *
story_xml_load_scenes (xiag_t *xiag, xml_node_t *root_node, chapter_t *parent)
{
  xml_node_t *node;
  scene_t *scene_first = NULL;
  scene_t *scene = NULL;

  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  for (node = root_node; node; node = node->next)
  {
    xml_property_t *props;
    char *id = NULL;
    room_t *rooms = NULL;

    if (strcmp (node->name, XML_SCENE))
    {
      XIAG_LOG (xiag, XIAG_MSG_ERROR,
                MODULE_NAME, "unknown tag in the chapter: <%s>", node->name);
      continue;
    }

    for (props = node->props; props; props = props->next)
      if (!strcmp (props->name, XML_ID))
        id = strdup (props->value);

    scene = scene_add (scene, id, parent);

    if (node->child)
      rooms = story_xml_load_rooms (xiag, node->child, scene);

    scene_rooms_set (scene, rooms);

    if (!scene_first)
      scene_first = scene;
  }

  return scene_first;
}

static chapter_t *
story_xml_load_chapters (xiag_t *xiag, xml_node_t *root_node)
{
  xml_node_t *node;
  chapter_t *chapter_first = NULL;
  chapter_t *chapter = NULL;

  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  for (node = root_node; node; node = node->next)
  {
    xml_property_t *props;
    char *id = NULL;
    scene_t *scenes = NULL;

    if (strcmp (node->name, XML_CHAPTER))
    {
      XIAG_LOG (xiag, XIAG_MSG_ERROR,
                MODULE_NAME, "unknown tag in the root: <%s>", node->name);
      continue;
    }

    for (props = node->props; props; props = props->next)
      if (!strcmp (props->name, XML_ID))
        id = strdup (props->value);

    chapter = chapter_add (chapter, id);

    if (node->child)
      scenes = story_xml_load_scenes (xiag, node->child, chapter);

    chapter_scenes_set (chapter, scenes);

    if (!chapter_first)
      chapter_first = chapter;
  }

  return chapter_first;
}

static int
story_xml_load (xiag_t *xiag)
{
  story_t *story = xiag->story;
  FILE *xml;
  char *xml_data;
  xml_node_t *root_node;
  struct stat st;

  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  xml = fopen (story->xml_file, "r");
  if (!xml)
  {
    XIAG_LOG (xiag, XIAG_MSG_ERROR,
              MODULE_NAME, "story %s unreadable", story->xml_file);
    return -1;
  }

  stat (story->xml_file, &st);
  xml_data = malloc (st.st_size + 1);
  if (!xml_data)
  {
    fclose (xml);
    return -1;
  }

  fread (xml_data, 1, st.st_size, xml);
  *(xml_data + st.st_size) = '\0';

  exml_parser_init (xml_data, strlen (xml_data), XML_PARSER_CASE_SENSITIVE);
  if (!exml_parser_build_tree (&root_node))
  {
    story->chapters = story_xml_load_chapters (xiag, root_node->child);
    exml_parser_free_tree (root_node);
  }
  free (xml_data);

  fclose (xml);
  return 0;
}

static story_t *
story_new (void)
{
  story_t *story;

  story = calloc (1, sizeof (story_t));

  return story;
}

static void
story_free (story_t *story)
{
  if (!story)
    return;

  free (story->xml_file);
  free (story);
}

int
story_loader (xiag_t *xiag)
{
  story_t *story;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  if (!xiag)
    return -1;

  xiag->story = story_new ();
  if (!xiag->story)
    return -1;

  story = xiag->story;

  /* load the story */
  story->xml_file = interface_story_get (xiag);
  if (!story->xml_file)
    return -1;

  if (story_xml_load (xiag))
    return -1;

  return 0;
}

void
story_shutdown (xiag_t *xiag)
{
  story_t *story;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  if (!xiag || !xiag->story)
    return;

  story = xiag->story;
  chapter_free (story->chapters);
  story_free (story);

  if (xiag->id_chapter)
    free (xiag->id_chapter);
  if (xiag->id_scene)
    free (xiag->id_scene);
  if (xiag->id_room)
    free (xiag->id_room);
}

chapter_t *
story_chapters_get (xiag_t *xiag)
{
  if (!xiag || !xiag->story)
    return NULL;

  return xiag->story->chapters;
}

int
story_load_firstid (xiag_t *xiag)
{
  story_t *story = xiag->story;
  chapter_t *chapter;
  scene_t *scene;
  room_t *room;
  const char *id_chapter;
  const char *id_scene;
  const char *id_room;

  chapter = story->chapters;
  if (!chapter)
    return -1;

  id_chapter = chapter_id_get (chapter);
  xiag->id_chapter = id_chapter ? strdup (id_chapter) : NULL;

  scene = chapter_scenes_get (chapter);
  if (!scene)
    return -1;

  id_scene = scene_id_get (scene);
  xiag->id_scene = id_scene ? strdup (id_scene) : NULL;

  room = scene_rooms_get (scene);
  if (!room)
    return -1;

  id_room = room_id_get (room);
  xiag->id_room = id_room ? strdup (id_room) : NULL;

  XIAG_LOG (xiag, XIAG_MSG_INFO,
            MODULE_NAME, "current id: %s.%s.%s", id_chapter, id_scene, id_room);

  return 0;
}

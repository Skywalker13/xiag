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

#include "xml_utils.h"

#include "texts.h"
#include "texts_chapter.h"
#include "texts_scene.h"
#include "texts_room.h"
#include "texts_entity.h"
#include "texts_condition.h"
#include "texts_story.h"

struct texts_story_s {
  char *xml_file;
  texts_chapter_t *chapters;
};


static inline int
get_inventory_value (const char *value)
{
  if (!strcmp (value, "yes"))
    return 1;
  else if (!strcmp (value, "no"))
    return 0;
  return -1;
}

static void
texts_story_xml_load_condition_action (texts_t *texts,
                                       texts_condition_t **condition,
                                       xml_node_t *root_node,
                                       texts_action_t action, char *entity_id)
{
  texts_condition_t *condition_first = *condition;

  xml_property_t *props;
  char *id = NULL, *state = NULL;
  int inventory = -1;
  char *text = strdup (root_node->data);

  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  for (props = root_node->props; props; props = props->next)
  {
    if (!strcmp (props->name, XML_ENTITY))
      id = strdup (props->value);
    else if (!strcmp (props->name, XML_INVENTORY))
      inventory = get_inventory_value (props->value);
    else if (!strcmp (props->name, XML_STATE))
      state = strdup (props->value);
    else
      MOD_LOG (texts->handle, XIAG_MODULE_MSG_ERROR,
               "unknown property (condition): %s", props->name);
  }

  if (!id && entity_id)
    id = strdup (entity_id);

  *condition =
    texts_condition_add (*condition, action, id, inventory, state, text);

  if (!condition_first)
    condition_first = *condition;
  else
    *condition = condition_first;
}

static texts_condition_t *
texts_story_xml_load_conditions (texts_t *texts,
                                 xml_node_t *root_node, char *id)
{
  xml_node_t *node;
  texts_condition_t *cond = NULL;

  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  for (node = root_node; node; node = node->next)
  {
    if (!strcmp (node->name, XML_LOOK))
      texts_story_xml_load_condition_action (texts, &cond,
                                             node, TEXTS_ACTION_LOOK, id);
    else if (!strcmp (node->name, XML_TAKE))
      texts_story_xml_load_condition_action (texts, &cond,
                                             node, TEXTS_ACTION_TAKE, id);
    else if (!strcmp (node->name, XML_USE))
      texts_story_xml_load_condition_action (texts, &cond,
                                             node, TEXTS_ACTION_USE, id);
    else if (!strcmp (node->name, XML_USE_WITH))
      texts_story_xml_load_condition_action (texts, &cond,
                                             node, TEXTS_ACTION_USE_WITH, id);
    else if (!strcmp (node->name, XML_SPEAK))
      texts_story_xml_load_condition_action (texts, &cond,
                                             node, TEXTS_ACTION_SPEAK, id);
    else if (!strcmp (node->name, XML_MOVE))
      texts_story_xml_load_condition_action (texts, &cond,
                                             node, TEXTS_ACTION_MOVE, id);
/* FIXME
    else
      MOD_LOG (texts->handle, XIAG_MODULE_MSG_ERROR,
               "unknown tag (action): <%s>", node->name);
*/
  }

  return cond;
}

static texts_entity_t *
texts_story_xml_load_entities (texts_t *texts,
                               xml_node_t *root_node, char *room_id)
{
  xml_node_t *node;
  texts_entity_t *entity_first = NULL;
  texts_entity_t *entity = NULL;

  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  for (node = root_node; node; node = node->next)
  {
    xml_property_t *props;
    char entity_id[256];
    char *id = NULL, *name = NULL;
    texts_condition_t *conditions = NULL;

    if (strcmp (node->name, XML_ENTITY))
    {
/* FIXME
      MOD_LOG (texts->handle, XIAG_MODULE_MSG_ERROR,
               "unknown tag in the room: <%s>", node->name);
*/
      continue;
    }

    for (props = node->props; props; props = props->next)
    {
      if (!strcmp (props->name, XML_ID))
        id = strdup (props->value);
      else if (!strcmp (props->name, XML_NAME))
        name = strdup (props->value);
      else
        MOD_LOG (texts->handle, XIAG_MODULE_MSG_ERROR,
                 "unknown property (entity): %s", props->name);
    }

    snprintf (entity_id, sizeof (entity_id), "%s.%s", room_id, id);

    if (node->child)
      conditions =
        texts_story_xml_load_conditions (texts, node->child, entity_id);

    entity = texts_entity_add (entity, id, name, conditions);

    if (!entity_first)
      entity_first = entity;
  }

  return entity_first;
}

static texts_room_t *
texts_story_xml_load_rooms (texts_t *texts,
                            xml_node_t *root_node, char *scene_id)
{
  xml_node_t *node;
  texts_room_t *room_first = NULL;
  texts_room_t *room = NULL;

  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  for (node = root_node; node; node = node->next)
  {
    xml_property_t *props;
    char room_id[256];
    char *id = NULL, *name = NULL;
    texts_entity_t *entities = NULL;
    texts_condition_t *conditions = NULL;

    if (strcmp (node->name, XML_ROOM))
    {
      MOD_LOG (texts->handle, XIAG_MODULE_MSG_ERROR,
               "unknown tag in the scene: <%s>", node->name);
      continue;
    }

    for (props = node->props; props; props = props->next)
    {
      if (!strcmp (props->name, XML_ID))
        id = strdup (props->value);
      else if (!strcmp (props->name, XML_NAME))
        name = strdup (props->value);
    }

    snprintf (room_id, sizeof (room_id), "%s.%s", scene_id, id);

    /*
     * FIXME: same tags are read two times!
     */
    if (node->child)
    {
      entities = texts_story_xml_load_entities (texts, node->child, room_id);
      /*
       * FIXME: must load conditions _only_ for LOOK.
       */
      conditions =
        texts_story_xml_load_conditions (texts, node->child, NULL);
    }

    room = texts_room_add (room, id, name, entities, conditions);

    if (!room_first)
      room_first = room;
  }

  return room_first;
}

static texts_scene_t *
texts_story_xml_load_scenes (texts_t *texts,
                             xml_node_t *root_node, char *chapter_id)
{
  xml_node_t *node;
  texts_scene_t *scene_first = NULL;
  texts_scene_t *scene = NULL;

  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  for (node = root_node; node; node = node->next)
  {
    xml_property_t *props;
    char scene_id[256];
    char *id = NULL, *name = NULL;
    texts_room_t *rooms = NULL;

    if (strcmp (node->name, XML_SCENE))
    {
      MOD_LOG (texts->handle, XIAG_MODULE_MSG_ERROR,
               "unknown tag in the chapter: <%s>", node->name);
      continue;
    }

    for (props = node->props; props; props = props->next)
    {
      if (!strcmp (props->name, XML_ID))
        id = strdup (props->value);
      else if (!strcmp (props->name, XML_NAME))
        name = strdup (props->value);
    }

    snprintf (scene_id, sizeof (scene_id), "%s.%s", chapter_id, id);

    if (node->child)
      rooms = texts_story_xml_load_rooms (texts, node->child, scene_id);

    scene = texts_scene_add (scene, id, name, rooms);

    if (!scene_first)
      scene_first = scene;
  }

  return scene_first;
}

static texts_chapter_t *
texts_story_xml_load_chapters (texts_t *texts, xml_node_t *root_node)
{
  xml_node_t *node;
  texts_chapter_t *chapter_first = NULL;
  texts_chapter_t *chapter = NULL;

  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  for (node = root_node; node; node = node->next)
  {
    xml_property_t *props;
    char *id = NULL, *name = NULL;
    texts_scene_t *scenes = NULL;

    if (strcmp (node->name, XML_CHAPTER))
    {
      MOD_LOG (texts->handle, XIAG_MODULE_MSG_ERROR,
               "unknown tag in the root: <%s>", node->name);
      continue;
    }

    for (props = node->props; props; props = props->next)
    {
      if (!strcmp (props->name, XML_ID))
        id = strdup (props->value);
      else if (!strcmp (props->name, XML_NAME))
        name = strdup (props->value);
    }

    if (node->child)
      scenes = texts_story_xml_load_scenes (texts, node->child, id);

    chapter = texts_chapter_add (chapter, id, name, scenes);

    if (!chapter_first)
      chapter_first = chapter;
  }

  return chapter_first;
}

static int
texts_story_xml_load (texts_t *texts)
{
  texts_story_t *story = texts->story;
  FILE *xml;
  char *xml_data;
  xml_node_t *root_node;
  struct stat st;

  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  xml = fopen (story->xml_file, "r");
  if (!xml)
  {
    MOD_LOG (texts->handle, XIAG_MODULE_MSG_ERROR,
             "story text %s unreadable", story->xml_file);
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
    story->chapters = texts_story_xml_load_chapters (texts, root_node->child);
    exml_parser_free_tree (root_node);
  }
  free (xml_data);

  fclose (xml);
  return 0;
}

static texts_story_t *
texts_story_new (void)
{
  texts_story_t *story;

  story = calloc (1, sizeof (texts_story_t));

  return story;
}

static void
texts_story_free (texts_story_t *story)
{
  if (!story)
    return;

  free (story->xml_file);
  free (story);
}

int
texts_story_loader (texts_t *texts, char *xml_file)
{
  texts_story_t *story;

  MOD_LOG (texts->handle, XIAG_MODULE_MSG_INFO, __FUNCTION__);

  if (!texts)
    return -1;

  texts->story = texts_story_new ();
  if (!texts->story)
    return -1;

  story = texts->story;

  /* load the text story */
  story->xml_file = xml_file;
  if (!story->xml_file)
    return -1;

  if (texts_story_xml_load (texts))
    return -1;

  return 0;
}

void
texts_story_shutdown (texts_t *texts)
{
  texts_story_t *story;

  MOD_LOG (texts->handle, XIAG_MODULE_MSG_INFO, __FUNCTION__);

  if (!texts || !texts->story)
    return;

  story = texts->story;
  texts_chapter_free (story->chapters);
  texts_story_free (story);
}

texts_chapter_t *
texts_story_chapters_get (texts_t *texts)
{
  if (!texts || !texts->story)
    return NULL;

  return texts->story->chapters;
}

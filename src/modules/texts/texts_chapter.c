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

#include "texts_chapter.h"

struct texts_chapter_s {
  struct texts_chapter_s *next;
  char *id;
  char *name;
  texts_scene_t *scenes;
};


texts_scene_t *
texts_chapter_scenes_get (texts_chapter_t *chapter)
{
  if (!chapter)
    return NULL;

  return chapter->scenes;
}

static texts_chapter_t *
texts_chapter_new (char *id, char *name, texts_scene_t *scenes)
{
  texts_chapter_t *chapter;

  if (!id)
    return NULL;

  chapter = calloc (1, sizeof (texts_chapter_t));
  if (!chapter)
    return NULL;

  chapter->id = id;
  chapter->name = name;
  chapter->scenes = scenes;

  return chapter;
}

void
texts_chapter_free (texts_chapter_t *chapter)
{
  texts_chapter_t *chapter_p;

  while (chapter)
  {
    texts_scene_free (chapter->scenes);
    if (chapter->id)
      free (chapter->id);
    if (chapter->name)
      free (chapter->name);
    chapter_p = chapter;
    chapter = chapter->next;
    free (chapter_p);
  }
}

texts_chapter_t *
texts_chapter_add (texts_chapter_t *chapter,
             char *id, char *name, texts_scene_t *scene)
{
  if (!chapter)
  {
    chapter = texts_chapter_new (id, name, scene);
    return chapter;
  }

  while (chapter->next)
    chapter = chapter->next;
  chapter->next = texts_chapter_new (id, name, scene);
  return chapter->next;
}

texts_chapter_t *
texts_chapter_search_fromid (texts_chapter_t *chapter, char *id)
{
  if (!id)
    return NULL;

  for (; chapter; chapter = chapter->next)
    if (!strcmp (chapter->id, id))
      return chapter;

  return NULL;
}

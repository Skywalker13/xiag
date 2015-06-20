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

#include "chapter.h"

struct chapter_s {
  struct chapter_s *next;
  char *id;
  scene_t *scenes;
};


scene_t *
chapter_scenes_get (chapter_t *chapter)
{
  if (!chapter)
    return NULL;

  return chapter->scenes;
}

const char *
chapter_id_get (chapter_t *chapter)
{
  if (!chapter)
    return NULL;

  return chapter->id;
}

void
chapter_scenes_set (chapter_t *chapter, scene_t *scenes)
{
  if (!chapter)
    return;

  chapter->scenes = scenes;
}

static chapter_t *
chapter_new (char *id)
{
  chapter_t *chapter;

  if (!id)
    return NULL;

  chapter = calloc (1, sizeof (chapter_t));
  if (!chapter)
    return NULL;

  chapter->id = id;

  return chapter;
}

void
chapter_free (chapter_t *chapter)
{
  chapter_t *chapter_p;

  while (chapter)
  {
    scene_free (chapter->scenes);
    if (chapter->id)
      free (chapter->id);
    chapter_p = chapter;
    chapter = chapter->next;
    free (chapter_p);
  }
}

chapter_t *
chapter_add (chapter_t *chapter, char *id)
{
  if (!chapter)
  {
    chapter = chapter_new (id);
    return chapter;
  }

  while (chapter->next)
    chapter = chapter->next;
  chapter->next = chapter_new (id);
  return chapter->next;
}

chapter_t *
chapter_search_fromid (chapter_t *chapter, char *id)
{
  if (!id)
    return NULL;

  for (; chapter; chapter = chapter->next)
    if (!strcmp (chapter->id, id))
      return chapter;

  return NULL;
}

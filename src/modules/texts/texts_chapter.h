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

#ifndef TEXTS_CHAPTER_H
#define TEXTS_CHAPTER_H

#include "texts_scene.h"

typedef struct texts_chapter_s texts_chapter_t;

void texts_chapter_free (texts_chapter_t *chapter);
texts_chapter_t *texts_chapter_add (texts_chapter_t *chapter,
                                    char *id, char *name,
                                    texts_scene_t *scenes);

texts_scene_t *texts_chapter_scenes_get (texts_chapter_t *chapter);
texts_chapter_t *texts_chapter_search_fromid (texts_chapter_t *chapter,
                                              char *id);

#endif /* TEXTS_CHAPTER_H */

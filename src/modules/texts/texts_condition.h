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

#ifndef TEXTS_CONDITION_H
#define TEXTS_CONDITION_H

#include "texts.h"

typedef struct texts_condition_s texts_condition_t;

void texts_condition_free (texts_condition_t *condition);
texts_condition_t *texts_condition_add (texts_condition_t *condition,
                                        texts_action_t action,
                                        char *id, int inventory, char *state,
                                        char *text);

char *texts_condition_look (texts_t *texts, texts_condition_t *condition);
char *texts_condition_take (texts_t *texts, texts_condition_t *condition);
char *texts_condition_use (texts_t *texts, texts_condition_t *condition,
                                 const char *what_id);
char *texts_condition_speak (texts_t *texts, texts_condition_t *condition);
char *texts_condition_move (texts_t *texts, texts_condition_t *condition);

#endif /* TEXTS_CONDITION_H */

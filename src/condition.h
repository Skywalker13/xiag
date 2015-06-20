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

#ifndef CONDITION_H
#define CONDITION_H

#include "xiag.h"

typedef struct condition_s condition_t;
typedef struct condition_result_s condition_result_t;

typedef enum res_type {
  RESULT_TYPE_UNKNOWN = 0,
  RESULT_TYPE_INVENTORY,
  RESULT_TYPE_STATE,
  RESULT_TYPE_GOTO,
} res_type_t;


void condition_free (condition_t *condition);
condition_t *condition_add (condition_t *condition, xiag_action_t action,
                            char *id, entity_inventory_t inventory,
                            char *state, condition_result_t *res);
condition_result_t *condition_result_add (condition_result_t *cond_result,
                                          res_type_t type, char *id,
                                          entity_inventory_t inventory,
                                          char *state);

int condition_look (xiag_t *xiag, condition_t *condition);
int condition_take (xiag_t *xiag, condition_t *condition);
int condition_use (xiag_t *xiag, condition_t *condition, const char *what_id);
int condition_speak (xiag_t *xiag, condition_t *condition);
int condition_move (xiag_t *xiag, condition_t *condition);

#endif /* CONDITION_H */

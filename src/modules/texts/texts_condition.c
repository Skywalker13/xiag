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

#include "texts_condition.h"

typedef struct texts_condition_action_s {
  struct texts_condition_action_s *next;
  /* properties */
  char *id;
  int inventory;
  char *state;
  int with;
  /* value */
  char *text;
} texts_condition_action_t;

struct texts_condition_s {
  texts_condition_action_t *look;
  texts_condition_action_t *take;
  texts_condition_action_t *use;
  texts_condition_action_t *speak;
  texts_condition_action_t *move;
};


static texts_condition_action_t *
texts_condition_action_new (char *id, int inventory, char *state, int with,
                            char *text)
{
  texts_condition_action_t *cond_action;

  cond_action = calloc (1, sizeof (texts_condition_action_t));
  if (!cond_action)
    return NULL;

  cond_action->id = id;
  cond_action->inventory = inventory;
  cond_action->state = state;
  cond_action->with = with;
  cond_action->text = text;

  return cond_action;
}

static void
texts_condition_action_free (texts_condition_action_t *cond_action)
{
  texts_condition_action_t *cond_action_p;

  while (cond_action)
  {
    if (cond_action->text)
      free (cond_action->text);
    if (cond_action->id)
      free (cond_action->id);
    if (cond_action->state)
      free (cond_action->state);
    cond_action_p = cond_action;
    cond_action = cond_action->next;
    free (cond_action_p);
  }
}

static texts_condition_action_t *
texts_condition_action_add (texts_condition_action_t *cond_action,
                            char *id, int inventory, char *state, int with,
                            char *text)
{
  if (!cond_action)
  {
    cond_action = texts_condition_action_new (id, inventory, state, with, text);
    return cond_action;
  }

  while (cond_action->next)
    cond_action = cond_action->next;
  cond_action->next =
    texts_condition_action_new (id, inventory, state, with, text);
  return cond_action->next;
}

static texts_condition_t *
texts_condition_new (void)
{
  texts_condition_t *condition;

  condition = calloc (1, sizeof (texts_condition_t));

  return condition;
}

void
texts_condition_free (texts_condition_t *condition)
{
  if (!condition)
    return;

  texts_condition_action_free (condition->look);
  texts_condition_action_free (condition->take);
  texts_condition_action_free (condition->use);
  texts_condition_action_free (condition->speak);
  texts_condition_action_free (condition->move);
  free (condition);
}

texts_condition_t *
texts_condition_add (texts_condition_t *condition, texts_action_t action,
                     char *id, int inventory, char *state, char *text)
{
  texts_condition_action_t *cond_action;
  int with = 0;

  if (!condition)
    condition = texts_condition_new ();

  switch (action)
  {
  case TEXTS_ACTION_LOOK:
    cond_action = texts_condition_action_add (condition->look,
                                              id, inventory, state, 0, text);
    if (!condition->look)
      condition->look = cond_action;
    break;

  case TEXTS_ACTION_TAKE:
    cond_action = texts_condition_action_add (condition->take,
                                              id, inventory, state, 0, text);
    if (!condition->take)
      condition->take = cond_action;
    break;

  case TEXTS_ACTION_USE_WITH:
    with = 1;
  case TEXTS_ACTION_USE:
    cond_action = texts_condition_action_add (condition->use,
                                              id, inventory, state, with, text);
    if (!condition->use)
      condition->use = cond_action;
    break;

  case TEXTS_ACTION_SPEAK:
    cond_action = texts_condition_action_add (condition->speak,
                                              id, inventory, state, 0, text);
    if (!condition->speak)
      condition->speak = cond_action;
    break;

  case TEXTS_ACTION_MOVE:
    cond_action = texts_condition_action_add (condition->move,
                                              id, inventory, state, 0, text);
    if (!condition->move)
      condition->move = cond_action;
    break;
  }

  return condition;
}

static char *
texts_condition_action (texts_t *texts,
                        texts_condition_action_t *cond_act, const char *what_id)
{
  char *text = NULL;

  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  for (; cond_act; cond_act = cond_act->next)
  {
    if (cond_act->id) /* check if the condition is really conditional */
    {
      int inventory;
      const char *state;

      /*
       * When the action is 'use', an ID can be used to know what entity
       * is used on the main entity. If what_id is not the same as the ID
       * in the condition, then we must ignore it!
       */
      if ((cond_act->with && !what_id)
          || (!cond_act->with && what_id)
          || (cond_act->with && what_id && strcmp (cond_act->id, what_id)))
        continue;

      inventory =
        xiag_module_entity_inventory_get (texts->handle, cond_act->id);
      if (cond_act->inventory > -1 && cond_act->inventory != inventory)
        continue; /* inventory is not the same */

      state = xiag_module_entity_state_get (texts->handle, cond_act->id);
      if (cond_act->state && strcmp (cond_act->state, state))
        continue; /* state is not the same */
    }

    if (cond_act->text)
    {
      if (text)
        text = realloc (text, strlen (text) + strlen (cond_act->text) + 2);
      else
      {
        text = malloc (strlen (cond_act->text) + 1);
        *text = '\0';
      }
      sprintf (text, "%s%s%s", text, strlen (text) ? " " : "", cond_act->text);
    }
  }

  return text;
}

char *
texts_condition_look (texts_t *texts, texts_condition_t *condition)
{
  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  if (!condition)
    return NULL;

  if (!condition->look)
    return NULL; /* NOP */

  return texts_condition_action (texts, condition->look, NULL);
}

char *
texts_condition_take (texts_t *texts, texts_condition_t *condition)
{
  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  if (!condition)
    return NULL;

  if (!condition->take)
    return NULL; /* NOP */

  return texts_condition_action (texts, condition->take, NULL);
}

char *
texts_condition_use (texts_t *texts,
                     texts_condition_t *condition, const char *what_id)
{
  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  if (!condition)
    return NULL;

  if (!condition->use)
    return NULL; /* NOP */

  return texts_condition_action (texts, condition->use, what_id);
}

char *
texts_condition_speak (texts_t *texts, texts_condition_t *condition)
{
  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  if (!condition)
    return NULL;

  if (!condition->speak)
    return NULL; /* NOP */

  return texts_condition_action (texts, condition->speak, NULL);
}

char *
texts_condition_move (texts_t *texts, texts_condition_t *condition)
{
  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  if (!condition)
    return NULL;

  if (!condition->move)
    return NULL; /* NOP */

  return texts_condition_action (texts, condition->move, NULL);
}

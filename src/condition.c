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

#include "engine.h"
#include "entity.h"
#include "logs.h"
#include "condition.h"

#define MODULE_NAME "condition"

struct condition_result_s {
  struct condition_result_s *next;
  char *id;
  res_type_t type;
  union {
    char *state;
    entity_inventory_t inventory;
  };
};

typedef struct condition_action_s {
  struct condition_action_s *next;
  char *id;
  entity_inventory_t inventory;
  char *state;
  int with;
  condition_result_t *results;
} condition_action_t;

struct condition_s {
  condition_action_t *look;
  condition_action_t *take;
  condition_action_t *use;
  condition_action_t *speak;
  condition_action_t *move;
};


static condition_result_t *
condition_result_new (res_type_t type,
                      char *id, entity_inventory_t inventory, char *state)
{
  condition_result_t *cond_result;

  cond_result = calloc (1, sizeof (condition_result_t));
  if (!cond_result)
    return NULL;

  cond_result->type = type;
  cond_result->id = id;

  switch (type)
  {
  case RESULT_TYPE_INVENTORY:
    cond_result->inventory = inventory;
    break;

  case RESULT_TYPE_STATE:
    cond_result->state = state;
    break;

  default:
    break;
  }

  return cond_result;
}

static void
condition_result_free (condition_result_t *cond_result)
{
  condition_result_t *cond_result_p;

  while (cond_result)
  {
    if (cond_result->id)
      free (cond_result->id);
    if (cond_result->type == RESULT_TYPE_STATE)
      if (cond_result->state)
        free (cond_result->state);
    cond_result_p = cond_result;
    cond_result = cond_result->next;
    free (cond_result_p);
  }
}

condition_result_t *
condition_result_add (condition_result_t *cond_result, res_type_t type,
                      char *id, entity_inventory_t inventory, char *state)
{
  if (!cond_result)
  {
    cond_result = condition_result_new (type, id, inventory, state);
    return cond_result;
  }

  while (cond_result->next)
    cond_result = cond_result->next;
  cond_result->next = condition_result_new (type, id, inventory, state);
  return cond_result->next;
}

static condition_action_t *
condition_action_new (char *id, entity_inventory_t inventory, char *state,
                      int with, condition_result_t *res)
{
  condition_action_t *cond_action;

  cond_action = calloc (1, sizeof (condition_action_t));
  if (!cond_action)
    return NULL;

  cond_action->id = id;
  cond_action->inventory = inventory;
  cond_action->state = state;
  cond_action->with = with;
  cond_action->results = res;

  return cond_action;
}

static void
condition_action_free (condition_action_t *cond_action)
{
  condition_action_t *cond_action_p;

  while (cond_action)
  {
    condition_result_free (cond_action->results);
    if (cond_action->id)
      free (cond_action->id);
    if (cond_action->state)
      free (cond_action->state);
    cond_action_p = cond_action;
    cond_action = cond_action->next;
    free (cond_action_p);
  }
}

static condition_action_t *
condition_action_add (condition_action_t *cond_action,
                      char *id, entity_inventory_t inventory, char *state,
                      int with, condition_result_t *res)
{
  if (!cond_action)
  {
    cond_action = condition_action_new (id, inventory, state, with, res);
    return cond_action;
  }

  while (cond_action->next)
    cond_action = cond_action->next;
  cond_action->next = condition_action_new (id, inventory, state, with, res);
  return cond_action->next;
}

static condition_t *
condition_new (void)
{
  condition_t *condition;

  condition = calloc (1, sizeof (condition_t));

  return condition;
}

void
condition_free (condition_t *condition)
{
  if (!condition)
    return;

  condition_action_free (condition->look);
  condition_action_free (condition->take);
  condition_action_free (condition->use);
  condition_action_free (condition->speak);
  condition_action_free (condition->move);
  free (condition);
}

condition_t *
condition_add (condition_t *condition, xiag_action_t action,
               char *id, entity_inventory_t inventory, char *state,
               condition_result_t *res)
{
  condition_action_t *cond_action;
  int with = 0;

  if (!condition)
    condition = condition_new ();

  switch (action)
  {
  case XIAG_ACTION_LOOK:
    cond_action =
      condition_action_add (condition->look, id, inventory, state, 0, res);
    if (!condition->look)
      condition->look = cond_action;
    break;

  case XIAG_ACTION_TAKE:
    cond_action =
      condition_action_add (condition->take, id, inventory, state, 0, res);
    if (!condition->take)
      condition->take = cond_action;
    break;

  case XIAG_ACTION_USE_WITH:
    with = 1;
  case XIAG_ACTION_USE:
    cond_action =
      condition_action_add (condition->use, id, inventory, state, with, res);
    if (!condition->use)
      condition->use = cond_action;
    break;

  case XIAG_ACTION_SPEAK:
    cond_action =
      condition_action_add (condition->speak, id, inventory, state, 0, res);
    if (!condition->speak)
      condition->speak = cond_action;
    break;

  case XIAG_ACTION_MOVE:
    cond_action =
      condition_action_add (condition->move, id, inventory, state, 0, res);
    if (!condition->move)
      condition->move = cond_action;
    break;
  }

  return condition;
}

static inline entity_t *
condition_get_entity (xiag_t *xiag, char *id)
{
  entity_t *entity = engine_entity_search (xiag, id);
  if (!entity)
  {
    XIAG_LOG (xiag, XIAG_MSG_ERROR,
              MODULE_NAME, "entity unknow, result is broken. "
                           "Please, fix the story file");
    return NULL;
  }
  return entity;
}

static int
condition_results_apply (xiag_t *xiag, condition_result_t *cond_res)
{
  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (!cond_res)
    return -1;

  for (; cond_res; cond_res = cond_res->next)
  {
    char *state = NULL;

    switch (cond_res->type)
    {
    case RESULT_TYPE_INVENTORY:
    {
      entity_t *entity = condition_get_entity (xiag, cond_res->id);
      if (!entity)
        return -1;

      XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME,
                "set inventory of %s to %i", cond_res->id, cond_res->inventory);
      entity_inventory_set (entity, cond_res->inventory);
      break;
    }

    case RESULT_TYPE_STATE:
    {
      entity_t *entity = condition_get_entity (xiag, cond_res->id);
      if (!entity)
        return -1;

      XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME,
                "set state of %s to '%s'", cond_res->id, cond_res->state);
      if (cond_res->state)
        state = strdup (cond_res->state);
      entity_state_set (entity, state);
      break;
    }

    case RESULT_TYPE_GOTO:
      XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, "goto %s", cond_res->id);
      engine_idpos_goto (xiag, cond_res->id);
      break;

    default:
      XIAG_LOG (xiag, XIAG_MSG_WARNING,
                MODULE_NAME, "unsupported type (%i)", cond_res->type);
      break;
    }
  }

  return 0;
}

static int
condition_action (xiag_t *xiag,
                  condition_action_t *cond_act, const char *what_id)
{
  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  for (; cond_act; cond_act = cond_act->next)
  {
    int res;

    if (cond_act->id) /* check if the condition is really conditional */
    {
      entity_t *entity = condition_get_entity (xiag, cond_act->id);
      if (!entity)
        return -1;

      /*
       * When the action is 'use', an ID can be used to know what entity
       * is used on the main entity. If what_id is not the same as the ID
       * in the condition, then we must ignore it!
       */
      if ((cond_act->with && !what_id)
          || (!cond_act->with && what_id)
          || (cond_act->with && what_id && strcmp (cond_act->id, what_id)))
        continue;

      if (cond_act->inventory != ENTITY_INVENTORY_NULL
          && cond_act->inventory != entity_inventory_get (entity))
        continue; /* inventory is not the same */
      if (cond_act->state
          && strcmp (cond_act->state, entity_state_get (entity)))
        continue; /* state is not the same */
    }

    XIAG_LOG (xiag, XIAG_MSG_VERBOSE,
              MODULE_NAME, "condition values of %s, inventory:%i state:%s",
                           cond_act->id, cond_act->inventory, cond_act->state);

    res = condition_results_apply (xiag, cond_act->results);
    if (res)
    {
      XIAG_LOG (xiag, XIAG_MSG_ERROR,
                MODULE_NAME, "results can not be applied, condition is broken. "
                             "Please, fix the story file");
      return -1; /* Oops: results can not be applied correctly */
    }
  }

  return 0;
}

int
condition_look (xiag_t *xiag, condition_t *condition)
{
  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (!condition)
    return -1;

  if (!condition->look)
    return 0; /* NOP */

  return condition_action (xiag, condition->look, NULL);
}

int
condition_take (xiag_t *xiag, condition_t *condition)
{
  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (!condition)
    return -1;

  if (!condition->take)
    return 0; /* NOP */

  return condition_action (xiag, condition->take, NULL);
}

int
condition_use (xiag_t *xiag, condition_t *condition, const char *what_id)
{
  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (!condition)
    return -1;

  if (!condition->use)
    return 0; /* NOP */

  return condition_action (xiag, condition->use, what_id);
}

int
condition_speak (xiag_t *xiag, condition_t *condition)
{
  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (!condition)
    return -1;

  if (!condition->speak)
    return 0; /* NOP */

  return condition_action (xiag, condition->speak, NULL);
}

int
condition_move (xiag_t *xiag, condition_t *condition)
{
  XIAG_LOG (xiag, XIAG_MSG_VERBOSE, MODULE_NAME, __FUNCTION__);

  if (!condition)
    return -1;

  if (!condition->move)
    return 0; /* NOP */

  return condition_action (xiag, condition->move, NULL);
}

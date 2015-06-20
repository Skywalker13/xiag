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

#include "texts_entity.h"

struct texts_entity_s {
  struct texts_entity_s *next;
  char *id;
  char *name;
  texts_condition_t *conditions;
};


const char *
texts_entity_id_get (texts_entity_t *entity)
{
  if (!entity)
    return NULL;

  return entity->id;
}

const char *
texts_entity_name_get (texts_entity_t *entity)
{
  if (!entity)
    return NULL;

  return entity->name;
}

static texts_entity_t *
texts_entity_new (char *id, char *name, texts_condition_t *conditions)
{
  texts_entity_t *entity;

  if (!id)
    return NULL;

  entity = calloc (1, sizeof (texts_entity_t));
  if (!entity)
    return NULL;

  entity->id = id;
  entity->name = name;
  entity->conditions = conditions;

  return entity;
}

void
texts_entity_free (texts_entity_t *entity)
{
  texts_entity_t *entity_p;

  while (entity)
  {
    texts_condition_free (entity->conditions);
    if (entity->id)
      free (entity->id);
    if (entity->name)
      free (entity->name);
    entity_p = entity;
    entity = entity->next;
    free (entity_p);
  }
}

texts_entity_t *
texts_entity_add (texts_entity_t *entity,
                  char *id, char *name, texts_condition_t *conditions)
{
  if (!entity)
  {
    entity = texts_entity_new (id, name, conditions);
    return entity;
  }

  while (entity->next)
    entity = entity->next;
  entity->next = texts_entity_new (id, name, conditions);
  return entity->next;
}

texts_entity_t *
texts_entity_search_fromid (texts_entity_t *entity, char *id)
{
  if (!id)
    return NULL;

  for (; entity; entity = entity->next)
    if (!strcmp (entity->id, id))
      return entity;

  return NULL;
}

texts_entity_t *
texts_entity_search_fromname (texts_entity_t *entity, char *name)
{
  if (!name)
    return NULL;

  for (; entity; entity = entity->next)
    if (!strcmp (entity->name, name))
      return entity;

  return NULL;
}

char *
texts_entity_look (texts_t *texts, texts_entity_t *entity)
{
  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  if (!entity)
    return NULL;

  if (!entity->conditions)
    return NULL; /* NOP */

  return texts_condition_look (texts, entity->conditions);
}

char *
texts_entity_take (texts_t *texts, texts_entity_t *entity)
{
  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  if (!entity)
    return NULL;

  if (!entity->conditions)
    return NULL; /* NOP */

  return texts_condition_take (texts, entity->conditions);
}

char *
texts_entity_use (texts_t *texts, texts_entity_t *entity, const char *what_id)
{
  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  if (!entity)
    return NULL;

  if (!entity->conditions)
    return NULL; /* NOP */

  return texts_condition_use (texts, entity->conditions, what_id);
}

char *
texts_entity_speak (texts_t *texts, texts_entity_t *entity)
{
  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  if (!entity)
    return NULL;

  if (!entity->conditions)
    return NULL; /* NOP */

  return texts_condition_speak (texts, entity->conditions);
}

char *
texts_entity_move (texts_t *texts, texts_entity_t *entity)
{
  MOD_LOG (texts->handle, XIAG_MODULE_MSG_VERBOSE, __FUNCTION__);

  if (!entity)
    return NULL;

  if (!entity->conditions)
    return NULL; /* NOP */

  return texts_condition_move (texts, entity->conditions);
}

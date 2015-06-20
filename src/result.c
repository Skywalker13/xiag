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

#include "xiag.h"
#include "module.h"
#include "logs.h"
#include "result.h"

#define MODULE_NAME "result"


static result_t *
result_new (const char *name, void *data)
{
  result_t *result;

  result = calloc (1, sizeof (result_t));
  if (!result)
    return NULL;

  result->mod_name = name;
  result->mod_data = data;

  return result;
}

void
result_free (xiag_t *xiag, result_t *result)
{
  result_t *result_n;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  while (result)
  {
    module_data_free (xiag, result->mod_name, result->mod_data);
    result_n = result->next;
    free (result);
    result = result_n;
  }
}

void
result_add (xiag_t *xiag, result_t **result, const char *name, void *data)
{
  result_t *result_n = *result;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  if (!*result)
  {
    *result = result_new (name, data);
    return;
  }

  while (result_n->next)
    result_n = result_n->next;
  result_n->next = result_new (name, data);
}

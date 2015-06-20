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

#include "xiag.h"
#include "action.h"
#include "result.h"
#include "utils.h"
#include "logs.h"
#include "terminal.h"

#define MODULE_NAME "terminal"


static int
terminal_init (xiag_t *xiag, void *private)
{
  (void) xiag;
  (void) private;

  return 0;
}

static void
terminal_uninit (xiag_t *xiag, void *private)
{
  (void) xiag;
  (void) private;
}

static char *
terminal_story_get (xiag_t *xiag, void *private)
{
  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  (void) private;

  return strdup (XIAG_DATA_DIR "/story/example.script.xml");
}

/*
 * TODO: to rewrite all this crappy stuff..
 */
static void
terminal_main_loop (xiag_t *xiag, void *private)
{
  char cmd[256];

  (void) private;

  printf ("XIAG / Terminal Client\n");

  while (1)
  {
    char *it, *id = NULL;
    result_t *res = NULL;
    xiag_module_terminal_t *data;

    printf ("Action > ");
    fgets (cmd, sizeof (cmd), stdin);
    it = strchr (cmd, '\n');
    if (it)
      *it = '\0';

    if (!strncmp (cmd, "look", 4))
    {
      if (strlen (cmd + 5))
      {
        id = utils_item2id (xiag, "texts", cmd + 5);
        if (id)
          res = action_look (xiag, id);
      }
      else
        res = action_look (xiag, NULL);
    }
    else if (!strncmp (cmd, "take", 4))
    {
      id = utils_item2id (xiag, "texts", cmd + 5);
      if (id)
        res = action_take (xiag, id);
    }
    else if (!strncmp (cmd, "use", 3))
    {
      it = strstr (cmd, " with ");
      if (it)
      {
        *it = '\0';
        id = utils_item2id (xiag, "texts", it + 6);
        char *id2 = utils_item2id (xiag, "texts", cmd + 4);
        if (id && id2)
          res = action_use (xiag, id, id2);
        if (id2)
          free (id2);
      }
      else
      {
        id = utils_item2id (xiag, "texts", cmd + 4);
        if (id)
          res = action_use (xiag, id, NULL);
      }
    }
    else if (!strncmp (cmd, "speak", 5))
    {
      id = utils_item2id (xiag, "texts", cmd + 6);
      if (id)
        res = action_speak (xiag, id);
    }
    else if (!strncmp (cmd, "move", 4))
    {
      id = utils_item2id (xiag, "texts", cmd + 5);
      if (id)
        res = action_move (xiag, id);
    }
    else if (!strcmp (cmd, "quit"))
      break;

    data = action_data_terminal (xiag, "texts", res);
    if (data)
    {
      printf ("\n %s\n %s\n\n", data->name, data->text);
      free (data);
    }

    result_free (xiag, res);
    if (id)
    {
      free (id);
      id = NULL;
    }
  }
}

interface_funcs_t *
terminal_register_funcs (void)
{
  interface_funcs_t *funcs;

  funcs = calloc (1, sizeof (interface_funcs_t));
  if (!funcs)
    return NULL;

  funcs->init      = terminal_init;
  funcs->uninit    = terminal_uninit;
  funcs->story_get = terminal_story_get;
  funcs->main_loop = terminal_main_loop;

  return funcs;
}

void *
terminal_register_private (void)
{
  return NULL;
}

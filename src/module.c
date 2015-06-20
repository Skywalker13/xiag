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
#include <dlfcn.h>
#include <dirent.h>

#include "xiag.h"
#include "xiag_module.h"
#include "logs.h"
#include "result.h"
#include "module.h"

#define MODULE_NAME "module"

struct module_s {
  struct module_s *next;
  xiag_mod_register_t *mod;
  void *handle;
};


result_t *
module_look (xiag_t *xiag, char *id)
{
  module_t *module;
  result_t *results = NULL;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  for (module = xiag->modules; module; module = module->next)
  {
    void *data;
    xiag_mod_register_t *mod = module->mod;
    if (!mod)
      continue;

    XIAG_LOG (xiag, XIAG_MSG_INFO,
              MODULE_NAME, "module [%s]: look %s", mod->name, id);

    if (mod->funcs->look)
      data = mod->funcs->look (id);

    result_add (xiag, &results, mod->name, data);
  }

  return results;
}

result_t *
module_take (xiag_t *xiag, char *id)
{
  module_t *module;
  result_t *results = NULL;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  for (module = xiag->modules; module; module = module->next)
  {
    void *data;
    xiag_mod_register_t *mod = module->mod;
    if (!mod)
      continue;

    XIAG_LOG (xiag, XIAG_MSG_INFO,
              MODULE_NAME, "module [%s]: take %s", mod->name, id);

    if (mod->funcs->take)
      data = mod->funcs->take (id);

    result_add (xiag, &results, mod->name, data);
  }

  return results;
}

result_t *
module_use (xiag_t *xiag, char *id, char *what_id)
{
  module_t *module;
  result_t *results = NULL;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  for (module = xiag->modules; module; module = module->next)
  {
    void *data;
    xiag_mod_register_t *mod = module->mod;
    if (!mod)
      continue;

    XIAG_LOG (xiag, XIAG_MSG_INFO,
              MODULE_NAME, "module [%s]: use %s%s%s",
              mod->name, what_id ? what_id : "", what_id ? " with " : "", id);

    if (mod->funcs->use)
      data = mod->funcs->use (id, what_id);

    result_add (xiag, &results, mod->name, data);
  }

  return results;
}

result_t *
module_speak (xiag_t *xiag, char *id)
{
  module_t *module;
  result_t *results = NULL;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  for (module = xiag->modules; module; module = module->next)
  {
    void *data;
    xiag_mod_register_t *mod = module->mod;
    if (!mod)
      continue;

    XIAG_LOG (xiag, XIAG_MSG_INFO,
              MODULE_NAME, "module [%s]: speak %s", mod->name, id);

    if (mod->funcs->speak)
      data = mod->funcs->speak (id);

    result_add (xiag, &results, mod->name, data);
  }

  return results;
}

result_t *
module_move (xiag_t *xiag, char *id)
{
  module_t *module;
  result_t *results = NULL;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  for (module = xiag->modules; module; module = module->next)
  {
    void *data;
    xiag_mod_register_t *mod = module->mod;
    if (!mod)
      continue;

    XIAG_LOG (xiag, XIAG_MSG_INFO,
              MODULE_NAME, "module [%s]: move %s", mod->name, id);

    if (mod->funcs->move)
      data = mod->funcs->move (id);

    result_add (xiag, &results, mod->name, data);
  }

  return results;
}

xiag_module_terminal_t *
module_data_terminal (xiag_t *xiag, const char *mod_name, void *mod_data)
{
  module_t *module;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  for (module = xiag->modules; module; module = module->next)
  {
    xiag_mod_register_t *mod = module->mod;
    if (!mod)
      continue;

    if (!strcmp (mod->name, mod_name) && mod->funcs->data_terminal)
      return mod->funcs->data_terminal (mod_data);
  }

  return NULL;
}

xiag_module_graphical_t *
module_data_graphical (xiag_t *xiag, const char *mod_name, void *mod_data)
{
  module_t *module;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  for (module = xiag->modules; module; module = module->next)
  {
    xiag_mod_register_t *mod = module->mod;
    if (!mod)
      continue;

    if (!strcmp (mod->name, mod_name) && mod->funcs->data_graphical)
      return mod->funcs->data_graphical (mod_data);
  }

  return NULL;
}

void
module_data_free (xiag_t *xiag, const char *mod_name, void *mod_data)
{
  module_t *module;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  for (module = xiag->modules; module; module = module->next)
  {
    xiag_mod_register_t *mod = module->mod;
    if (!mod)
      continue;

    if (!strcmp (mod->name, mod_name) && mod->funcs->data_free)
    {
      mod->funcs->data_free (mod_data);
      break;
    }
  }
}

char *
module_item2id (xiag_t *xiag, const char *mod_name, void *item)
{
  module_t *module;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  for (module = xiag->modules; module; module = module->next)
  {
    xiag_mod_register_t *mod = module->mod;
    if (!mod)
      continue;

    if (!strcmp (mod->name, mod_name) && mod->funcs->item2id)
      return mod->funcs->item2id (item);
  }

  return NULL;
}

static void
module_init (xiag_t *xiag)
{
  module_t *module;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  for (module = xiag->modules; module; module = module->next)
  {
    xiag_mod_register_t *mod = module->mod;
    if (!mod)
      continue;

    XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME,
              "module [%s] (%s) \"%s\": init", mod->name, mod->ver, mod->desc);

    if (mod->funcs->init)
      mod->funcs->init (xiag);
  }
}

static void
module_uninit (xiag_t *xiag)
{
  module_t *module;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  for (module = xiag->modules; module; module = module->next)
  {
    xiag_mod_register_t *mod = module->mod;
    if (!mod)
      continue;

    XIAG_LOG (xiag, XIAG_MSG_INFO,
              MODULE_NAME, "module [%s]: uninit", mod->name);

    if (mod->funcs->uninit)
      mod->funcs->uninit ();

    dlclose (module->handle);
  }
}

static module_t *
module_new (xiag_mod_register_t *mod, void *handle)
{
  module_t *module;

  module = calloc (1, sizeof (module_t));
  if (!module)
    return NULL;

  module->mod = mod;
  module->handle = handle;

  return module;
}

static void
module_free (module_t *module)
{
  module_t *module_n;

  while (module)
  {
    module_n = module->next;
    free (module);
    module = module_n;
  }
}

static void
module_add (module_t **module, xiag_mod_register_t *mod, void *handle)
{
  module_t *module_n = *module;

  if (!*module)
  {
    *module = module_new (mod, handle);
    return;
  }

  while (module_n->next)
    module_n = module_n->next;
  module_n->next = module_new (mod, handle);
}

static int
module_search (xiag_t *xiag)
{
  int nb = 0;
  DIR *dirp;
  struct dirent *dp;
  char mod_so[PATH_BUFFER];

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  dirp = opendir (XIAG_MODULE_DIR);
  if (!dirp)
    return 0;

  while ((dp = readdir (dirp)))
  {
    void *handle;
    xiag_mod_register_t *(*mod_register) (void);

    if (!strcmp (dp->d_name, ".") || !strcmp (dp->d_name, ".."))
      continue;

    /* FIXME: d_type is not available with all filesystems, prefer stat() instead */
    if (dp->d_type != DT_REG) /* check if that is a file */
      continue;

    if (!strstr (dp->d_name, ".so"))
      continue;

    snprintf (mod_so, sizeof (mod_so), XIAG_MODULE_DIR "/%s", dp->d_name);

    XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, "module found: %s", mod_so);

    handle = dlopen (mod_so, RTLD_LAZY);
    if (!handle)
    {
      XIAG_LOG (xiag, XIAG_MSG_ERROR,
                MODULE_NAME, "broken shared object %s", mod_so);
      continue;
    }

    mod_register = dlsym (handle, MOD_FUNC_REGISTER);
    if (mod_register)
    {
      xiag_mod_register_t *mod = mod_register ();
      if (mod)
      {
        module_add (&xiag->modules, mod, handle);
        nb++;
        continue;
      }
    }
    else
      XIAG_LOG (xiag, XIAG_MSG_ERROR,
                MODULE_NAME, "register function not found in %s", mod_so);

    dlclose (handle);
  }

  closedir (dirp);
  return nb;
}

int
module_loader (xiag_t *xiag)
{
  int nb;

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  if (!xiag)
    return -1;

  nb = module_search (xiag);

  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME,
            "%i module%s to be initialized", nb, nb > 1 ? "s" : "");

  if (!nb)
    return 0;

  module_init (xiag);

  return 0;
}

void
module_shutdown (xiag_t *xiag)
{
  XIAG_LOG (xiag, XIAG_MSG_INFO, MODULE_NAME, __FUNCTION__);

  if (!xiag)
    return;

  module_uninit (xiag);
  module_free (xiag->modules);
}

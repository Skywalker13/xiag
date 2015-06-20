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

#ifndef MODULE_H
#define MODULE_H

#include "xiag_module.h"
#include "result.h"

int module_loader (xiag_t *xiag);
void module_shutdown (xiag_t *xiag);

result_t *module_look (xiag_t *xiag, char *id);
result_t *module_take (xiag_t *xiag, char *id);
result_t *module_use (xiag_t *xiag, char *id, char *what_id);
result_t *module_speak (xiag_t *xiag, char *id);
result_t *module_move (xiag_t *xiag, char *id);

xiag_module_terminal_t *module_data_terminal (xiag_t *xiag,
                                              const char *mod_name,
                                              void *mod_data);
xiag_module_graphical_t *module_data_graphical (xiag_t *xiag,
                                                const char *mod_name,
                                                void *mod_data);
void module_data_free (xiag_t *xiag, const char *mod_name, void *mod_data);

char *module_item2id (xiag_t *xiag, const char *mod_name, void *item);

#endif /* MODULE_H */

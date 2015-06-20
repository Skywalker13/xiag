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

#ifndef ACTION_H
#define ACTION_H

#include "xiag_module.h"
#include "result.h"

result_t *action_look (xiag_t *xiag, char *id);
result_t *action_take (xiag_t *xiag, char *id);
result_t *action_use (xiag_t *xiag, char *id, char *what_id);
result_t *action_speak (xiag_t *xiag, char *id);
result_t *action_move (xiag_t *xiag, char *id);

xiag_module_terminal_t *action_data_terminal (xiag_t *xiag,
                                              const char *mod_name,
                                              result_t *result);
xiag_module_graphical_t *action_data_graphical (xiag_t *xiag,
                                                const char *mod_name,
                                                result_t *result);

#endif /* ACTION_H */

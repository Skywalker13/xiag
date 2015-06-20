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

#ifndef INTERFACE_H
#define INTERFACE_H

typedef enum interface_type {
  INTERFACE_TYPE_TERMINAL,
  INTERFACE_TYPE_GRAPHICAL,
} interface_type_t;

typedef struct interface_funcs_s {
  int (*init) (xiag_t *xiag, void *private);
  void (*uninit) (xiag_t *xiag, void *private);
  char *(*story_get) (xiag_t *xiag, void *private);
  void (*main_loop) (xiag_t *xiag, void *private);
} interface_funcs_t;


char *interface_story_get (xiag_t *xiag);
void interface_main_loop (xiag_t *xiag);

int interface_loader (xiag_t *xiag, interface_type_t type);
void interface_shutdown (xiag_t *xiag);

#endif /* INTERFACE_H */

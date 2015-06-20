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
#include <stdarg.h>

#include "xiag.h"

#define NORMAL   "\033[0m"
#define COLOR(x) "\033[" #x ";1m"
#define BOLD     COLOR(1)
#define F_RED    COLOR(31)
#define F_GREEN  COLOR(32)
#define F_YELLOW COLOR(33)
#define F_BLUE   COLOR(34)
#define B_RED    COLOR(41)

int
xlog_test (xiag_t *xiag, xiag_verbosity_level_t level)
{
  if (!xiag)
    return 0;

  /* do we really want logging ? */
  if (xiag->verbosity == XIAG_MSG_NONE)
    return 0;

  if (level < xiag->verbosity)
    return 0;

  return 1;
}

void
xlog (xiag_t *xiag, xiag_verbosity_level_t level,
      const char *module, const char *file, int line, const char *format, ...)
{
  static const char const *c[] = {
    [XIAG_MSG_VERBOSE]  = F_BLUE,
    [XIAG_MSG_INFO]     = F_GREEN,
    [XIAG_MSG_WARNING]  = F_YELLOW,
    [XIAG_MSG_ERROR]    = F_RED,
    [XIAG_MSG_CRITICAL] = B_RED,
  };
  static const char const *l[] = {
    [XIAG_MSG_VERBOSE]  = "Verb",
    [XIAG_MSG_INFO]     = "Info",
    [XIAG_MSG_WARNING]  = "Warn",
    [XIAG_MSG_ERROR]    = "Err",
    [XIAG_MSG_CRITICAL] = "Crit",
  };
  va_list va;

  if (!xiag || !format)
    return;

  if (!xlog_test (xiag, level))
    return;

  va_start (va, format);

  fprintf (stderr, "[" BOLD "%s" NORMAL "] [%s:%i] %s%s" NORMAL ": ",
           module, file, line, c[level], l[level]);

  vfprintf (stderr, format, va);
  fprintf (stderr, "\n");
  va_end (va);
}

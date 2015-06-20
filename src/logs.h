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

#ifndef XIAG_LOGS_H
#define XIAG_LOGS_H

#include "xiag.h"

int xlog_test (xiag_t *xiag, xiag_verbosity_level_t level);
void xlog (xiag_t *xiag, xiag_verbosity_level_t level,
           const char *module, const char *file, int line,
           const char *format, ...);

#define XIAG_LOG(xiag, level, module, format, args...) \
        xlog (xiag, level, module, __FILE__, __LINE__, format, ##args)

#endif /* XIAG_LOGS_H */

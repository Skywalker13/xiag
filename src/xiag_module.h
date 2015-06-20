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

#ifndef XIAG_MODULE_H
#define XIAG_MODULE_H

#define XIAG_HANDLE void

typedef enum xiag_module_id_type {
  XIAG_MODULE_ID_UNKNOWN,
  XIAG_MODULE_ID_CHAPTER,
  XIAG_MODULE_ID_SCENE,
  XIAG_MODULE_ID_ROOM,
  XIAG_MODULE_ID_ENTITY,
} xiag_module_id_type_t;

typedef enum xiag_module_msg {
  XIAG_MODULE_MSG_VERBOSE,
  XIAG_MODULE_MSG_INFO,
  XIAG_MODULE_MSG_WARNING,
  XIAG_MODULE_MSG_ERROR,
  XIAG_MODULE_MSG_CRITICAL,
} xiag_module_msg_t;

typedef struct xiag_module_terminal_s {
  const char *name;
  const char *text;
} xiag_module_terminal_t;

typedef struct xiag_module_graphical_s {
  const char *dummy; /* TODO */
} xiag_module_graphical_t;

typedef struct xiag_module_s {
  /* un/initialization */
  int (*init) (XIAG_HANDLE *handle);
  void (*uninit) (void);
  /* send actions */
  void *(*look) (const char *id);
  void *(*take) (const char *id);
  void *(*use) (const char *id, const char *what_id);
  void *(*speak) (const char *id);
  void *(*move) (const char *id);
  /* data for interfaces */
  xiag_module_terminal_t *(*data_terminal) (void *data);
  xiag_module_graphical_t *(*data_graphical) (void *data);
  void (*data_free) (void *data);
  /* other */
  char *(*item2id) (void *item);
} xiag_module_t;

typedef struct xiag_mod_register_s {
  xiag_module_t *funcs;
  const char *name;
  const char *ver;
  const char *desc;
} xiag_mod_register_t;


/**
 * \brief Get the type of ID in function of a complete id string.
 *
 * An ID is a string formed with 4 parts seperated by a dot. Each part is
 * a level in the story structure. For example:
 *  chapter1.scene1.room1.entity1
 * But the ID can be limited to represent a chapter, a scene or a room.
 *  chapter1.scene1.room1; chapter1.scene1 or just chapter1
 *
 * \param[in] handle        XIAG handle.
 * \param[in] id            Retrieve the type of this ID.
 * \return The type.
 */
xiag_module_id_type_t xiag_module_id_type_get (XIAG_HANDLE *handle, char *id);

/**
 * \brief In some cases, that will be useful to split a complete ID.
 *
 * Each part must be freed when no longer used.
 *
 * \param[in] id            ID to split.
 * \param[out] id_chapter   Pointer where the chapter part is saved.
 * \param[out] id_scene     Pointer where the scene part is saved.
 * \param[out] id_room      Pointer where the room part is saved.
 * \param[out] id_entity    Pointer where the entity part is saved.
 */
void xiag_module_id_split (const char *id, char **id_chapter, char **id_scene,
                                           char **id_room, char **id_entity);

/**
 * \brief Get the inventory from an ID (entity).
 *
 * Possible values are :
 *  -1: unused
 *   0: is in the room
 *   1: is in the inventory
 *
 * \param[in] handle        XIAG handle.
 * \param[in] id            Entity where the inventory will be retrieved.
 * \return The inventory value (see above).
 */
int xiag_module_entity_inventory_get (XIAG_HANDLE *handle, char *id);

/**
 * \brief Get the state from an ID (entity).
 *
 * Possible values are infinite, this field is free. Only the link between
 * the XML story and the XML texts is needed.
 *
 * \param[in] handle        XIAG handle.
 * \param[in] id            Entity where the state will be retrieved.
 * \return The state.
 */
const char *xiag_module_entity_state_get (XIAG_HANDLE *handle, char *id);

/**
 * \brief Get the current position ID (room).
 *
 * The value is in the format:
 *  chapter.scene.room
 *
 * \param[in] handle        XIAG handle.
 * \return The ID.
 */
char *xiag_module_idpos_get (XIAG_HANDLE *handle);

/**
 * \brief Get the number of entities in the inventory.
 *
 * \param[in] handle        XIAG handle.
 * \return The size.
 */
unsigned int xiag_module_inventory_size (XIAG_HANDLE *handle);

/**
 *
 */
char *xiag_module_inventory_id (XIAG_HANDLE *handle, unsigned int item);

void xiag_module_log (XIAG_HANDLE *handle, xiag_module_msg_t level,
                      const char *file, int line, const char *format, ...);
#define MOD_LOG(handle, level, format, args...) \
        xiag_module_log (handle, level, __FILE__, __LINE__, format, ##args)

#define MOD_FUNC_REGISTER "mod_register"
#define XIAG_MODULE_REGISTER(m, n, v, d)  \
  xiag_mod_register_t *                   \
  mod_register (void)                     \
  {                                       \
    static xiag_mod_register_t mod = {    \
      .funcs = m,                         \
      .name  = n,                         \
      .ver   = v,                         \
      .desc  = d,                         \
    };                                    \
    return &mod;                          \
  }

#endif /* XIAG_MODULE_H */

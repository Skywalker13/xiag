/*
 *  Copyright (C) 2002-2003 the xine project
 *
 *  This file is part of xine, a free video player.
 *
 *  xine is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  xine is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 *
 *  $Id: xmlparser.h,v 1.4 2006/02/14 02:25:01 dsalt Exp $
 *
 */
#ifndef XML_PARSER_H
#define XML_PARSER_H

/* parser modes */
#define XML_PARSER_CASE_INSENSITIVE  0
#define XML_PARSER_CASE_SENSITIVE    1

/* return codes */
#define XML_PARSER_OK                0
#define XML_PARSER_ERROR             1


/* xml property */
typedef struct xml_property_s {
	char *name;
	char *value;
	struct xml_property_s *next;
} xml_property_t;

/* xml node */
typedef struct xml_node_s {
	char *name;
	char *data;
	struct xml_property_s *props;
	struct xml_node_s *child;
	struct xml_node_s *next;
} xml_node_t;

void exml_parser_init(const char * buf, int size, int mode);

int exml_parser_build_tree(xml_node_t **root_node);

void exml_parser_free_tree(xml_node_t *root_node);

char *exml_parser_get_property (const xml_node_t *node, const char *name);
int   exml_parser_get_property_int (const xml_node_t *node, const char *name, 
				   int def_value);
int exml_parser_get_property_bool (const xml_node_t *node, const char *name, 
				  int def_value);

/* for output:
 * returns an escaped string (free() it when done)
 * input must be in ASCII or UTF-8
 */

typedef enum {
  XML_ESCAPE_NO_QUOTE,
  XML_ESCAPE_SINGLE_QUOTE,
  XML_ESCAPE_DOUBLE_QUOTE
} xml_escape_quote_t;
char *exml_escape_string (const char *s, xml_escape_quote_t quote_type);

/* for debugging purposes: dump read-in xml tree in a nicely
 * indented fashion
 */

void exml_parser_dump_tree (const xml_node_t *node) ;

#endif

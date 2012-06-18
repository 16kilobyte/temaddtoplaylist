/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lib.h
 * Copyright (C) Kator James 2010 <kator95@gmail.com>
 * 
 * totemaddtoplaylist is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * totemaddtoplaylist is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <glib-object.h>

#define KATOR_NAUTILUS_TYPE (KATOR_get_nautilus_type())

typedef struct _NautilusATPL      KatorNautilus;
typedef struct _NautilusATPLClass KatorNautilusClass;

struct _NautilusATPL {
	GObject __parent;
};

struct _NautilusATPLClass {
	GObjectClass __parent;
};

GType KATOR_get_nautilus_type     (void);

void KATOR_nautilus_register_type (GTypeModule * module);

static void KATOR_nautilus_ATPL_class_init (KatorNautilusClass *atplclass);

static void KATOR_nautilus_ATPL_instance_init (KatorNautilus *atpl);

static void KATOR_nautilus_ATPL_menu_provider_iface_init(NautilusMenuProviderIface *iface);

static GList * KATOR_nautilus_ATPL_get_file_items (NautilusMenuProvider *provider, GtkWidget *window, GList *files);

static void KATOR_totem_add_to_playlist_callback (NautilusMenuItem *item, gpointer user_data);

static gboolean is_media (NautilusFileInfo *file);

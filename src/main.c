/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lib.h
 * Copyright (C) Kator James 2012 <kator95@gmail.com>
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

#include <libnautilus-extension/nautilus-extension-types.h>
#include <libnautilus-extension/nautilus-file-info.h>
#include <libnautilus-extension/nautilus-menu-provider.h>
#include <stdlib.h>
#include <string.h>

#include "totemaddtoplaylist.h"

/* Some global declarations */

static GType kator_ext_type = 0;
static GObjectClass *parent_class = NULL;
static char * program = "totem --enqueue ";
static char * running = "pidof totem";

/* Implementing the module_initialize interface */

void
nautilus_module_initialize (GTypeModule*module)
{
	KATOR_nautilus_register_type (module);
	//printf("Module initialized\n");

}

/* Implementing the module_shutdown interface */

void
nautilus_module_shutdown (void)
{
	//printf("Our addtoplaylist module is destroyed\n");
}



/* Implementing the module_list_types interface */
void 
nautilus_module_list_types (const GType **types, int *num_types)
{
	static GType type_list[1];
	
	type_list[0] = KATOR_NAUTILUS_TYPE;
	*types = type_list;
	*num_types = 1;
}

void KATOR_nautilus_register_type(GTypeModule * module)
{
		//printf("registered\n");
		static const GTypeInfo info = {
		sizeof (KatorNautilusClass),
		(GBaseInitFunc) NULL,
		(GBaseFinalizeFunc) NULL,
		(GClassInitFunc) KATOR_nautilus_ATPL_class_init,
		NULL, 
		NULL,
		sizeof (KatorNautilus),
		0,
		(GInstanceInitFunc) KATOR_nautilus_ATPL_instance_init,
	};

	static const GInterfaceInfo menu_provider_iface_info = {
		(GInterfaceInitFunc) KATOR_nautilus_ATPL_menu_provider_iface_init,
		NULL,
		NULL
	};

	kator_ext_type = g_type_module_register_type (module, G_TYPE_OBJECT, "KatorNautilusATPL",&info, 0);

	g_type_module_add_interface (module,
				     kator_ext_type,
				     NAUTILUS_TYPE_MENU_PROVIDER,
				     &menu_provider_iface_info);
}

static void KATOR_nautilus_ATPL_class_init (KatorNautilusClass *atplclass)
{
parent_class = g_type_class_peek_parent (atplclass);
}

static void KATOR_nautilus_ATPL_instance_init (KatorNautilus *atpl){

}


static void	KATOR_nautilus_ATPL_menu_provider_iface_init(NautilusMenuProviderIface *iface)
{
	iface->get_file_items = KATOR_nautilus_ATPL_get_file_items;

}

static GList *
KATOR_nautilus_ATPL_get_file_items (NautilusMenuProvider *provider, GtkWidget *window, GList *files)
{
	//printf("Calling our addtoplaylist Module\n");
	GList    *items = NULL;
	GList    *scan = NULL;
	GList *l = NULL;
	NautilusMenuItem *item;
	int count =  0;
	int exit_status = 0;


	/* This extension only operates on selections that include only
	 * MP3,ogg, wma, avi, files */
	for (l = files; l != NULL; l = l->next) {
	        NautilusFileInfo *file = NAUTILUS_FILE_INFO (l->data);

		//if at least one of the following extentions is highlighted, show add to totem playlist
		if (nautilus_file_info_is_mime_type (file, "audio/mpeg") || nautilus_file_info_is_mime_type (file, "audio/x-vorbis+ogg") || nautilus_file_info_is_mime_type (file, "audio/x-ms-wma") || nautilus_file_info_is_mime_type (file, "audio/x-wav")) {
			//nautilus_file_info_is_directory
		     count++;
		}
		
	}

		if(count == 0)return NULL;

	exit_status = system(running);

	if(exit_status != 0)return NULL;
	

	for (scan = files; scan; scan = scan->next) {
		NautilusFileInfo *file = scan->data;
		gchar            *scheme;
		gboolean          local;

		scheme = nautilus_file_info_get_uri_scheme (file);
		local = strncmp (scheme, "file", 4) == 0;
		g_free (scheme);

		if (!local)
			return NULL;
	}
	
	
	
	item = nautilus_menu_item_new ("KatorNautilus::KatorNautilusATPL", "Add to playlist", "Add sounds to Totem playlist", "video-x-generic");
		g_signal_connect (item, "activate", G_CALLBACK (KATOR_totem_add_to_playlist_callback), provider);
			
		g_object_set_data_full (G_OBJECT (item), "files", nautilus_file_info_list_copy (files),	(GDestroyNotify) nautilus_file_info_list_free);
		items = g_list_append (items, item);
	
	return items;
}

static void KATOR_totem_add_to_playlist_callback (NautilusMenuItem *item,
              gpointer          user_data)
{
	//printf("Module handler for addtoplaylist has been called\n");
	GList            *files;
	NautilusFileInfo *file;
	gchar            *uri;
	char  prog[250] = "";	   /* Segfaults for a filename greater than 90 chars*/

	strcpy(prog,program);
	
	files = g_object_get_data (G_OBJECT (item), "files");

	for(;files != NULL; files = files->next){
		strcpy(prog,program);
		
		file = files->data;
		uri = nautilus_file_info_get_uri (file);
		
				if(prog == NULL)return;

			if(is_media (file)){
				strcat(prog,(char *)uri);
				//printf("Command to run = %s\n",prog);
				system (prog);
			
			}
		strcpy(prog,(char *)""); /* Unset */
			
	}
	
}

GType
KATOR_get_nautilus_type (void)
{
	return kator_ext_type;
}

static gboolean is_media (NautilusFileInfo *file)
{
	gchar *mimeType;
	gboolean isMedia;
	
	mimeType = nautilus_file_info_get_mime_type (file);
	
	isMedia = g_str_has_prefix (nautilus_file_info_get_mime_type (file), "audio/mpeg") || g_str_has_prefix (nautilus_file_info_get_mime_type (file), "audio/x-vorbis+ogg") || g_str_has_prefix (nautilus_file_info_get_mime_type (file), "audio/x-ms-wma") || g_str_has_prefix (nautilus_file_info_get_mime_type (file), "audio/x-wav");
	
	//g_free (isMedia); segfaulting
	
	return isMedia;	
}

/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpsettingsbox.h
 * Copyright (C) 2008 Michael Natterer <mitch@gimp.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __GIMP_SETTINGS_BOX_H__
#define __GIMP_SETTINGS_BOX_H__


#define GIMP_TYPE_SETTINGS_BOX            (gimp_settings_box_get_type ())
#define GIMP_SETTINGS_BOX(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_SETTINGS_BOX, GimpSettingsBox))
#define GIMP_SETTINGS_BOX_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_SETTINGS_BOX, GimpSettingsBoxClass))
#define GIMP_IS_SETTINGS_BOX(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_SETTINGS_BOX))
#define GIMP_IS_SETTINGS_BOX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_SETTINGS_BOX))
#define GIMP_SETTINGS_BOX_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_SETTINGS_BOX, GimpSettingsBoxClass))


typedef struct _GimpSettingsBoxClass GimpSettingsBoxClass;

struct _GimpSettingsBox
{
  GtkHBox        parent_instance;

  GtkWidget     *combo;
  GtkWidget     *menu;
  GtkWidget     *import_item;
  GtkWidget     *export_item;
  GtkWidget     *file_dialog;
  GtkWidget     *editor_dialog;

  Gimp          *gimp;
  GObject       *config;
  GimpContainer *container;
  gchar         *filename;

  gchar         *import_dialog_title;
  gchar         *export_dialog_title;
  gchar         *file_dialog_help_id;
  gchar         *default_folder;
  gchar         *last_filename;
};

struct _GimpSettingsBoxClass
{
  GtkHBoxClass  parent_class;

  void (* file_dialog_setup) (GimpSettingsBox      *box,
                              GtkFileChooserDialog *dialog,
                              gboolean              export);
  void (* import)            (GimpSettingsBox      *box,
                              const gchar          *filename);
  void (* export)            (GimpSettingsBox      *box,
                              const gchar          *filename);
};


GType       gimp_settings_box_get_type    (void) G_GNUC_CONST;

GtkWidget * gimp_settings_box_new         (Gimp            *gimp,
                                           GObject         *config,
                                           GimpContainer   *container,
                                           const gchar     *filename,
                                           const gchar     *import_dialog_title,
                                           const gchar     *export_dialog_title,
                                           const gchar     *file_dialog_help_id,
                                           const gchar     *default_folder,
                                           const gchar     *last_filename);

void        gimp_settings_box_add_current (GimpSettingsBox *box);


#endif  /*  __GIMP_SETTINGS_BOX_H__  */

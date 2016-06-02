/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimpenumcombobox.c
 * Copyright (C) 2004  Sven Neumann <sven@gimp.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <gtk/gtk.h>

#include "gimpwidgetstypes.h"

#include "gimpenumcombobox.h"
#include "gimpenumstore.h"

#include "libgimp/libgimp-intl.h"


enum
{
  PROP_0,
  PROP_MODEL
};


static void  gimp_enum_combo_box_set_property (GObject      *object,
                                               guint         prop_id,
                                               const GValue *value,
                                               GParamSpec   *pspec);
static void  gimp_enum_combo_box_get_property (GObject      *object,
                                               guint         prop_id,
                                               GValue       *value,
                                               GParamSpec   *pspec);


G_DEFINE_TYPE (GimpEnumComboBox, gimp_enum_combo_box,
               GIMP_TYPE_INT_COMBO_BOX)

#define parent_class gimp_enum_combo_box_parent_class


static void
gimp_enum_combo_box_class_init (GimpEnumComboBoxClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = gimp_enum_combo_box_set_property;
  object_class->get_property = gimp_enum_combo_box_get_property;

  /*  override the "model" property of GtkComboBox  */
  g_object_class_install_property (object_class,
                                   PROP_MODEL,
                                   g_param_spec_object ("model", NULL, NULL,
                                                        GIMP_TYPE_ENUM_STORE,
                                                        GIMP_PARAM_READWRITE));
}

static void
gimp_enum_combo_box_init (GimpEnumComboBox *combo_box)
{
}

static void
gimp_enum_combo_box_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
  GtkComboBox *combo_box = GTK_COMBO_BOX (object);

  switch (prop_id)
    {
    case PROP_MODEL:
      gtk_combo_box_set_model (combo_box, g_value_get_object (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gimp_enum_combo_box_get_property (GObject    *object,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
  GtkComboBox *combo_box = GTK_COMBO_BOX (object);

  switch (prop_id)
    {
    case PROP_MODEL:
      g_value_set_object (value, gtk_combo_box_get_model (combo_box));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}


/**
 * gimp_enum_combo_box_new:
 * @enum_type: the #GType of an enum.
 *
 * Creates a #GtkComboBox readily filled with all enum values from a
 * given @enum_type. The enum needs to be registered to the type
 * system. It should also have %GimpEnumDesc descriptions registered
 * that contain translatable value names. This is the case for the
 * enums used in the GIMP PDB functions.
 *
 * This is just a convenience function. If you need more control over
 * the enum values that appear in the combo_box, you can create your
 * own #GimpEnumStore and use gimp_enum_combo_box_new_with_model().
 *
 * Return value: a new #GimpEnumComboBox.
 *
 * Since: GIMP 2.4
 **/
GtkWidget *
gimp_enum_combo_box_new (GType enum_type)
{
  GtkListStore *store;
  GtkWidget    *combo_box;

  g_return_val_if_fail (G_TYPE_IS_ENUM (enum_type), NULL);

  store = gimp_enum_store_new (enum_type);

  combo_box = g_object_new (GIMP_TYPE_ENUM_COMBO_BOX,
                            "model", store,
                            NULL);

  g_object_unref (store);

  return combo_box;
}

/**
 * gimp_enum_combo_box_new_with_model
 * @enum_store: a #GimpEnumStore to use as the model
 *
 * Creates a #GtkComboBox for the given @enum_store.
 *
 * Return value: a new #GimpEnumComboBox.
 *
 * Since: GIMP 2.4
 **/
GtkWidget *
gimp_enum_combo_box_new_with_model (GimpEnumStore *enum_store)
{
  g_return_val_if_fail (GIMP_IS_ENUM_STORE (enum_store), NULL);

  return g_object_new (GIMP_TYPE_ENUM_COMBO_BOX,
                       "model", enum_store,
                       NULL);
}

/**
 * gimp_enum_combo_box_set_stock_prefix:
 * @combo_box:    a #GimpEnumComboBox
 * @stock_prefix: a prefix to create icon stock ID from enum values
 *
 * Attempts to create stock icons for all items in the @combo_box. See
 * gimp_enum_store_set_icons() to find out what to use as @stock_prefix.
 *
 * Since: GIMP 2.4
 **/
void
gimp_enum_combo_box_set_stock_prefix (GimpEnumComboBox *combo_box,
                                      const gchar      *stock_prefix)
{
  GtkTreeModel *model;

  g_return_if_fail (GIMP_IS_ENUM_COMBO_BOX (combo_box));

  model = gtk_combo_box_get_model (GTK_COMBO_BOX (combo_box));

  gimp_enum_store_set_stock_prefix (GIMP_ENUM_STORE (model), stock_prefix);
}

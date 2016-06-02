/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
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

#ifndef __GIMP_ITEM_PROP_UNDO_H__
#define __GIMP_ITEM_PROP_UNDO_H__


#include "gimpitemundo.h"


#define GIMP_TYPE_ITEM_PROP_UNDO            (gimp_item_prop_undo_get_type ())
#define GIMP_ITEM_PROP_UNDO(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_ITEM_PROP_UNDO, GimpItemPropUndo))
#define GIMP_ITEM_PROP_UNDO_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_ITEM_PROP_UNDO, GimpItemPropUndoClass))
#define GIMP_IS_ITEM_PROP_UNDO(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_ITEM_PROP_UNDO))
#define GIMP_IS_ITEM_PROP_UNDO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_ITEM_PROP_UNDO))
#define GIMP_ITEM_PROP_UNDO_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_ITEM_PROP_UNDO, GimpItemPropUndoClass))


typedef struct _GimpItemPropUndoClass GimpItemPropUndoClass;

struct _GimpItemPropUndo
{
  GimpItemUndo  parent_instance;

  gchar        *name;
  gint          offset_x;
  gint          offset_y;
  gboolean      visible;
  gboolean      linked;
  gchar        *parasite_name;
  GimpParasite *parasite;
};

struct _GimpItemPropUndoClass
{
  GimpItemUndoClass  parent_class;
};


GType   gimp_item_prop_undo_get_type (void) G_GNUC_CONST;


#endif /* __GIMP_ITEM_PROP_UNDO_H__ */

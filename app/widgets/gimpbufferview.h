/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpbufferview.h
 * Copyright (C) 2001 Michael Natterer <mitch@gimp.org>
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

#ifndef __GIMP_BUFFER_VIEW_H__
#define __GIMP_BUFFER_VIEW_H__


#include "gimpcontainereditor.h"


#define GIMP_TYPE_BUFFER_VIEW            (gimp_buffer_view_get_type ())
#define GIMP_BUFFER_VIEW(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_BUFFER_VIEW, GimpBufferView))
#define GIMP_BUFFER_VIEW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_BUFFER_VIEW, GimpBufferViewClass))
#define GIMP_IS_BUFFER_VIEW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_BUFFER_VIEW))
#define GIMP_IS_BUFFER_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_BUFFER_VIEW))
#define GIMP_BUFFER_VIEW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_BUFFER_VIEW, GimpBufferViewClass))


typedef struct _GimpBufferViewClass  GimpBufferViewClass;

struct _GimpBufferView
{
  GimpContainerEditor  parent_instance;

  GtkWidget           *global_view;
  GtkWidget           *global_label;

  GtkWidget           *paste_button;
  GtkWidget           *paste_into_button;
  GtkWidget           *paste_as_new_button;
  GtkWidget           *delete_button;
};

struct _GimpBufferViewClass
{
  GimpContainerEditorClass  parent_class;
};


GType       gimp_buffer_view_get_type (void) G_GNUC_CONST;

GtkWidget * gimp_buffer_view_new      (GimpViewType     view_type,
                                       GimpContainer   *container,
                                       GimpContext     *context,
                                       gint             view_size,
                                       gint             view_border_width,
                                       GimpMenuFactory *menu_factory);


#endif  /*  __GIMP_BUFFER_VIEW_H__  */

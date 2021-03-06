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

#include "config.h"

#include <gegl.h>

#include "core-types.h"

#include "base/gimplut.h"
#include "base/lut-funcs.h"

#include "gegl/gimp-gegl-utils.h"

/* temporary */
#include "gimp.h"
#include "gimpimage.h"

#include "gimpdrawable.h"
#include "gimpdrawable-invert.h"
#include "gimpdrawable-operation.h"
#include "gimpdrawable-process.h"
#include "gimpprogress.h"

#include "gimp-intl.h"


void
gimp_drawable_invert (GimpDrawable *drawable,
                      GimpProgress *progress)
{
  g_return_if_fail (GIMP_IS_DRAWABLE (drawable));
  g_return_if_fail (gimp_item_is_attached (GIMP_ITEM (drawable)));
  g_return_if_fail (progress == NULL || GIMP_IS_PROGRESS (progress));

  if (gimp_use_gegl (GIMP_ITEM (drawable)->image->gimp))
    {
      GeglNode    *invert;
      const gchar *name;

      name = gimp_gegl_check_version (0, 0, 21) ? "gegl:invert" : "invert";

      invert = g_object_new (GEGL_TYPE_NODE, "operation", name, NULL);

      gimp_drawable_apply_operation (drawable, progress, _("Invert"),
                                     invert, TRUE);
      g_object_unref (invert);
    }
  else
    {
      GimpLut *lut = invert_lut_new (gimp_drawable_bytes (drawable));

      gimp_drawable_process_lut (drawable, progress, _("Invert"), lut);
      gimp_lut_free (lut);
    }
}

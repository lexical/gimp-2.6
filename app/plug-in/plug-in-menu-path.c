/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * plug-in-menu-path.c
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

#include <string.h>

#include <glib-object.h>

#include "libgimpbase/gimpbase.h"

#include "plug-in-types.h"

#include "plug-in-menu-path.h"


typedef struct _MenuPathMapping MenuPathMapping;

struct _MenuPathMapping
{
  const gchar *orig_path;
  const gchar *label;
  const gchar *mapped_path;
};


static const MenuPathMapping menu_path_mappings[] =
{
  { "<Toolbox>/Xtns/Languages",       NULL, "<Image>/Filters/Languages"           },
  { "<Toolbox>/Xtns/Extensions",      NULL, "<Image>/Filters/Extensions"          },

  { "<Toolbox>/Xtns/Buttons",         NULL, "<Image>/File/Create/Buttons"         },
  { "<Toolbox>/Xtns/Logos",           NULL, "<Image>/File/Create/Logos"           },
  { "<Toolbox>/Xtns/Misc",            NULL, "<Image>/File/Create/Misc"            },
  { "<Toolbox>/Xtns/Patterns",        NULL, "<Image>/File/Create/Patterns"        },
  { "<Toolbox>/Xtns/Web Page Themes", NULL, "<Image>/File/Create/Web Page Themes" },

  { "<Toolbox>/Xtns", "Buttons",            "<Image>/File/Create"                 },
  { "<Toolbox>/Xtns", "Logos",              "<Image>/File/Create"                 },
  { "<Toolbox>/Xtns", "Misc",               "<Image>/File/Create"                 },
  { "<Toolbox>/Xtns", "Patterns",           "<Image>/File/Create"                 },
  { "<Toolbox>/Xtns", "Web Page Themes",    "<Image>/File/Create"                 },

  { "<Toolbox>/Xtns",                 NULL, "<Image>/Filters/Extensions"          },
  { "<Toolbox>/Help",                 NULL, "<Image>/Help"                        },

  { "<Toolbox>/File/Acquire",         NULL, "<Image>/File/Create/Acquire"         },
  { "<Toolbox>",                      NULL, "<Image>"                             },
  { "<Image>/File/Acquire",           NULL, "<Image>/File/Create/Acquire"         },
  { "<Image>/File/New",               NULL, "<Image>/File/Create"                 },
  { NULL, NULL, NULL                                                              }
};


gchar *
plug_in_menu_path_map (const gchar *menu_path,
                       const gchar *menu_label)
{
  const MenuPathMapping *mapping;
  gchar                 *stripped_label = NULL;

  g_return_val_if_fail (menu_path != NULL, NULL);

  if (menu_label)
    stripped_label = gimp_strip_uline (menu_label);

  for (mapping = menu_path_mappings; mapping->orig_path; mapping++)
    {
      if (g_str_has_prefix (menu_path, mapping->orig_path))
        {
          gint   orig_len = strlen (mapping->orig_path);
          gchar *mapped_path;

          /*  if the mapping has a label, only map if the passed label
           *  is identical and the paths' lengths match exactly.
           */
          if (mapping->label &&
              (! stripped_label               ||
               strlen (menu_path) != orig_len ||
               strcmp (mapping->label, stripped_label)))
            {
              continue;
            }

          if (strlen (menu_path) > orig_len)
            mapped_path = g_strconcat (mapping->mapped_path,
                                       menu_path + orig_len,
                                       NULL);
          else
            mapped_path = g_strdup (mapping->mapped_path);

#if GIMP_UNSTABLE
          {
            gchar *orig;
            gchar *mapped;

            if (menu_label)
              {
                orig   = g_strdup_printf ("%s/%s", menu_path,   stripped_label);
                mapped = g_strdup_printf ("%s/%s", mapped_path, stripped_label);
              }
            else
              {
                orig   = g_strdup (menu_path);
                mapped = g_strdup (mapped_path);
              }

            g_printerr (" mapped '%s' to '%s'\n", orig, mapped);

            g_free (orig);
            g_free (mapped);
          }
#endif

          g_free (stripped_label);

          return mapped_path;
        }
    }

  g_free (stripped_label);

  return g_strdup (menu_path);
}
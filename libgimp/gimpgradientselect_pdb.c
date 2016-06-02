/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2003 Peter Mattis and Spencer Kimball
 *
 * gimpgradientselect_pdb.c
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

/* NOTE: This file is auto-generated by pdbgen.pl */

#include "config.h"

#include "gimp.h"

/**
 * gimp_gradients_popup:
 * @gradient_callback: The callback PDB proc to call when gradient selection is made.
 * @popup_title: Title of the gradient selection dialog.
 * @initial_gradient: The name of the gradient to set as the first selected.
 * @sample_size: Size of the sample to return when the gradient is changed.
 *
 * Invokes the Gimp gradients selection.
 *
 * This procedure opens the gradient selection dialog.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradients_popup (const gchar *gradient_callback,
                      const gchar *popup_title,
                      const gchar *initial_gradient,
                      gint         sample_size)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-gradients-popup",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, gradient_callback,
                                    GIMP_PDB_STRING, popup_title,
                                    GIMP_PDB_STRING, initial_gradient,
                                    GIMP_PDB_INT32, sample_size,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradients_close_popup:
 * @gradient_callback: The name of the callback registered for this pop-up.
 *
 * Close the gradient selection dialog.
 *
 * This procedure closes an opened gradient selection dialog.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradients_close_popup (const gchar *gradient_callback)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-gradients-close-popup",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, gradient_callback,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_gradients_set_popup:
 * @gradient_callback: The name of the callback registered for this pop-up.
 * @gradient_name: The name of the gradient to set as selected.
 *
 * Sets the current gradient in a gradient selection dialog.
 *
 * Sets the current gradient in a gradient selection dialog.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_gradients_set_popup (const gchar *gradient_callback,
                          const gchar *gradient_name)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-gradients-set-popup",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, gradient_callback,
                                    GIMP_PDB_STRING, gradient_name,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

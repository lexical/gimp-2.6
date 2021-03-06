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

#ifndef __GIMP_PDB_ERROR_H__
#define __GIMP_PDB_ERROR_H__


typedef enum
{
  GIMP_PDB_PROCEDURE_NOT_FOUND,
  GIMP_PDB_INVALID_ARGUMENT,
  GIMP_PDB_INVALID_RETURN_VALUE,
  GIMP_PDB_CANCELLED,
  GIMP_PDB_INTERNAL_ERROR
} GimpPdbErrorCode;


#define GIMP_PDB_ERROR (gimp_pdb_error_quark ())

GQuark  gimp_pdb_error_quark (void) G_GNUC_CONST;


#endif /* __GIMP_PDB_ERROR_H__ */

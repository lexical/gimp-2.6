/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2003 Peter Mattis and Spencer Kimball
 *
 * gimpproceduraldb_pdb.c
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

#include <string.h>

#include "gimp.h"

/**
 * gimp_procedural_db_temp_name:
 *
 * Generates a unique temporary PDB name.
 *
 * This procedure generates a temporary PDB entry name that is
 * guaranteed to be unique.
 *
 * Returns: A unique temporary name for a temporary PDB entry.
 */
gchar *
gimp_procedural_db_temp_name (void)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gchar *temp_name = NULL;

  return_vals = gimp_run_procedure ("gimp-procedural-db-temp-name",
                                    &nreturn_vals,
                                    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    temp_name = g_strdup (return_vals[1].data.d_string);

  gimp_destroy_params (return_vals, nreturn_vals);

  return temp_name;
}

/**
 * gimp_procedural_db_dump:
 * @filename: The dump filename.
 *
 * Dumps the current contents of the procedural database
 *
 * This procedure dumps the contents of the procedural database to the
 * specified file. The file will contain all of the information
 * provided for each registered procedure.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_procedural_db_dump (const gchar *filename)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-procedural-db-dump",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, filename,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_procedural_db_query:
 * @name: The regex for procedure name.
 * @blurb: The regex for procedure blurb.
 * @help: The regex for procedure help.
 * @author: The regex for procedure author.
 * @copyright: The regex for procedure copyright.
 * @date: The regex for procedure date.
 * @proc_type: The regex for procedure type: { 'Internal GIMP procedure', 'GIMP Plug-In', 'GIMP Extension', 'Temporary Procedure' }.
 * @num_matches: The number of matching procedures.
 * @procedure_names: The list of procedure names.
 *
 * Queries the procedural database for its contents using regular
 * expression matching.
 *
 * This procedure queries the contents of the procedural database. It
 * is supplied with seven arguments matching procedures on { name,
 * blurb, help, author, copyright, date, procedure type}. This is
 * accomplished using regular expression matching. For instance, to
 * find all procedures with \"jpeg\" listed in the blurb, all seven
 * arguments can be supplied as \".*\", except for the second, which
 * can be supplied as \".*jpeg.*\". There are two return arguments for
 * this procedure. The first is the number of procedures matching the
 * query. The second is a concatenated list of procedure names
 * corresponding to those matching the query. If no matching entries
 * are found, then the returned string is NULL and the number of
 * entries is 0.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_procedural_db_query (const gchar   *name,
                          const gchar   *blurb,
                          const gchar   *help,
                          const gchar   *author,
                          const gchar   *copyright,
                          const gchar   *date,
                          const gchar   *proc_type,
                          gint          *num_matches,
                          gchar       ***procedure_names)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;
  gint i;

  return_vals = gimp_run_procedure ("gimp-procedural-db-query",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, name,
                                    GIMP_PDB_STRING, blurb,
                                    GIMP_PDB_STRING, help,
                                    GIMP_PDB_STRING, author,
                                    GIMP_PDB_STRING, copyright,
                                    GIMP_PDB_STRING, date,
                                    GIMP_PDB_STRING, proc_type,
                                    GIMP_PDB_END);

  *num_matches = 0;
  *procedure_names = NULL;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    {
      *num_matches = return_vals[1].data.d_int32;
      *procedure_names = g_new (gchar *, *num_matches);
      for (i = 0; i < *num_matches; i++)
        (*procedure_names)[i] = g_strdup (return_vals[2].data.d_stringarray[i]);
    }

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_procedural_db_proc_exists:
 * @procedure_name: The procedure name.
 *
 * Checks if the specified procedure exists in the procedural database
 *
 * This procedure checks if the specified procedure is registered in
 * the procedural database.
 *
 * Returns: Whether a procedure of that name is registered.
 *
 * Since: GIMP 2.6
 */
gboolean
gimp_procedural_db_proc_exists (const gchar *procedure_name)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean exists = FALSE;

  return_vals = gimp_run_procedure ("gimp-procedural-db-proc-exists",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, procedure_name,
                                    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    exists = return_vals[1].data.d_int32;

  gimp_destroy_params (return_vals, nreturn_vals);

  return exists;
}

/**
 * _gimp_procedural_db_proc_info:
 * @procedure_name: The procedure name.
 * @blurb: A short blurb.
 * @help: Detailed procedure help.
 * @author: Author(s) of the procedure.
 * @copyright: The copyright.
 * @date: Copyright date.
 * @proc_type: The procedure type.
 * @num_args: The number of input arguments.
 * @num_values: The number of return values.
 *
 * Queries the procedural database for information on the specified
 * procedure.
 *
 * This procedure returns information on the specified procedure. A
 * short blurb, detailed help, author(s), copyright information,
 * procedure type, number of input, and number of return values are
 * returned. For specific information on each input argument and return
 * value, use the gimp_procedural_db_proc_arg() and
 * gimp_procedural_db_proc_val() procedures.
 *
 * Returns: TRUE on success.
 */
gboolean
_gimp_procedural_db_proc_info (const gchar      *procedure_name,
                               gchar           **blurb,
                               gchar           **help,
                               gchar           **author,
                               gchar           **copyright,
                               gchar           **date,
                               GimpPDBProcType  *proc_type,
                               gint             *num_args,
                               gint             *num_values)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-procedural-db-proc-info",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, procedure_name,
                                    GIMP_PDB_END);

  *blurb = NULL;
  *help = NULL;
  *author = NULL;
  *copyright = NULL;
  *date = NULL;
  *proc_type = 0;
  *num_args = 0;
  *num_values = 0;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    {
      *blurb = g_strdup (return_vals[1].data.d_string);
      *help = g_strdup (return_vals[2].data.d_string);
      *author = g_strdup (return_vals[3].data.d_string);
      *copyright = g_strdup (return_vals[4].data.d_string);
      *date = g_strdup (return_vals[5].data.d_string);
      *proc_type = return_vals[6].data.d_int32;
      *num_args = return_vals[7].data.d_int32;
      *num_values = return_vals[8].data.d_int32;
    }

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_procedural_db_proc_arg:
 * @procedure_name: The procedure name.
 * @arg_num: The argument number.
 * @arg_type: The type of argument.
 * @arg_name: The name of the argument.
 * @arg_desc: A description of the argument.
 *
 * Queries the procedural database for information on the specified
 * procedure's argument.
 *
 * This procedure returns information on the specified procedure's
 * argument. The argument type, name, and a description are retrieved.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_procedural_db_proc_arg (const gchar     *procedure_name,
                             gint             arg_num,
                             GimpPDBArgType  *arg_type,
                             gchar          **arg_name,
                             gchar          **arg_desc)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-procedural-db-proc-arg",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, procedure_name,
                                    GIMP_PDB_INT32, arg_num,
                                    GIMP_PDB_END);

  *arg_type = 0;
  *arg_name = NULL;
  *arg_desc = NULL;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    {
      *arg_type = return_vals[1].data.d_int32;
      *arg_name = g_strdup (return_vals[2].data.d_string);
      *arg_desc = g_strdup (return_vals[3].data.d_string);
    }

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_procedural_db_proc_val:
 * @procedure_name: The procedure name.
 * @val_num: The return value number.
 * @val_type: The type of return value.
 * @val_name: The name of the return value.
 * @val_desc: A description of the return value.
 *
 * Queries the procedural database for information on the specified
 * procedure's return value.
 *
 * This procedure returns information on the specified procedure's
 * return value. The return value type, name, and a description are
 * retrieved.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_procedural_db_proc_val (const gchar     *procedure_name,
                             gint             val_num,
                             GimpPDBArgType  *val_type,
                             gchar          **val_name,
                             gchar          **val_desc)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-procedural-db-proc-val",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, procedure_name,
                                    GIMP_PDB_INT32, val_num,
                                    GIMP_PDB_END);

  *val_type = 0;
  *val_name = NULL;
  *val_desc = NULL;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    {
      *val_type = return_vals[1].data.d_int32;
      *val_name = g_strdup (return_vals[2].data.d_string);
      *val_desc = g_strdup (return_vals[3].data.d_string);
    }

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * _gimp_procedural_db_get_data:
 * @identifier: The identifier associated with data.
 * @bytes: The number of bytes in the data.
 * @data: A byte array containing data.
 *
 * Returns data associated with the specified identifier.
 *
 * This procedure returns any data which may have been associated with
 * the specified identifier. The data is a variable length array of
 * bytes. If no data has been associated with the identifier, an error
 * is returned.
 *
 * Returns: TRUE on success.
 */
gboolean
_gimp_procedural_db_get_data (const gchar  *identifier,
                              gint         *bytes,
                              guint8      **data)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-procedural-db-get-data",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, identifier,
                                    GIMP_PDB_END);

  *bytes = 0;
  *data = NULL;

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    {
      *bytes = return_vals[1].data.d_int32;
      *data = g_new (guint8, *bytes);
      memcpy (*data,
              return_vals[2].data.d_int8array,
              *bytes * sizeof (guint8));
    }

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_procedural_db_get_data_size:
 * @identifier: The identifier associated with data.
 *
 * Returns size of data associated with the specified identifier.
 *
 * This procedure returns the size of any data which may have been
 * associated with the specified identifier. If no data has been
 * associated with the identifier, an error is returned.
 *
 * Returns: The number of bytes in the data.
 */
gint
gimp_procedural_db_get_data_size (const gchar *identifier)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gint bytes = 0;

  return_vals = gimp_run_procedure ("gimp-procedural-db-get-data-size",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, identifier,
                                    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    bytes = return_vals[1].data.d_int32;

  gimp_destroy_params (return_vals, nreturn_vals);

  return bytes;
}

/**
 * _gimp_procedural_db_set_data:
 * @identifier: The identifier associated with data.
 * @bytes: The number of bytes in the data.
 * @data: A byte array containing data.
 *
 * Associates the specified identifier with the supplied data.
 *
 * This procedure associates the supplied data with the provided
 * identifier. The data may be subsequently retrieved by a call to
 * 'procedural-db-get-data'.
 *
 * Returns: TRUE on success.
 */
gboolean
_gimp_procedural_db_set_data (const gchar  *identifier,
                              gint          bytes,
                              const guint8 *data)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-procedural-db-set-data",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, identifier,
                                    GIMP_PDB_INT32, bytes,
                                    GIMP_PDB_INT8ARRAY, data,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

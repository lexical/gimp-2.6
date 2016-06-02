/* gimptool in C
 * Copyright (C) 2001-2007 Tor Lillqvist
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

/*
 * Gimptool rewritten in C, originally for Win32, where end-users who
 * might want to build and install a plug-in from source don't
 * necessarily have any Bourne-compatible shell to run the gimptool
 * script in. Later fixed up to replace the gimptool script on all
 * platforms.
 *
 * Yes, this program leaks dynamically allocated strings without
 * hesitation. So what, it runs only for a minimal time.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/stat.h>

#include <glib.h>

#include "libgimpbase/gimpversion.h"

#ifdef G_OS_WIN32
#include "libgimpbase/gimpwin32-io.h"
#endif


static gboolean     silent  = FALSE;
static gboolean     dry_run = FALSE;
static gchar       *prefix;
static const gchar *exec_prefix;

static gboolean     msvc_syntax = FALSE;
static const gchar *env_cc;
static const gchar *env_cflags;
static const gchar *env_ldflags;
static const gchar *env_libs;


#ifdef G_OS_WIN32
#define EXEEXT ".exe"
#else
#define EXEEXT ""
#endif

#ifdef G_OS_WIN32
#define COPY win32_command ("copy")
#define REMOVE win32_command ("del")
#else
#define COPY "cp"
#define REMOVE "rm -f"
#endif

static struct {
  const gchar *option;
  const gchar *value;
} dirs[] = {
  { "prefix",         PREFIX         },
  { "exec-prefix",    EXEC_PREFIX    },
  { "bindir",         BINDIR         },
  { "sbindir",        SBINDIR        },
  { "libexecdir",     LIBEXECDIR     },
  { "datadir",        DATADIR        },
  { "datarootdir",    DATAROOTDIR    },
  { "sysconfdir",     SYSCONFDIR     },
  { "sharedstatedir", SHAREDSTATEDIR },
  { "localstatedir",  LOCALSTATEDIR  },
  { "libdir",         LIBDIR         },
  { "infodir",        INFODIR        },
  { "mandir",         MANDIR         },
#if 0
  /* For --includedir we want the includedir of the developer package,
   * not an includedir under the runtime installation prefix.
   */
  { "includedir",     INCLUDEDIR     },
#endif
  { "gimpplugindir",  GIMPPLUGINDIR  },
  { "gimpdatadir",    GIMPDATADIR    }
};

#ifdef G_OS_WIN32

static gchar *
win32_command (const gchar *command)
{
  const gchar *comspec = getenv ("COMSPEC");

  if (!comspec)
    comspec = "cmd.exe";

  return g_strdup_printf ("%s /c %s", comspec, command);
}

#endif

static gboolean
starts_with_dir (const gchar *string,
		 const gchar *test)
{
  return g_str_has_prefix (string, g_strconcat (test, "/", NULL)) ||
    strcmp (string, test) == 0;
}

static gchar *
one_line_output (const gchar *program,
		 const gchar *args)
{
  gchar *command = g_strconcat (program, " ", args, NULL);
  FILE  *pipe    = popen (command, "r");
  gchar  line[1000];

  if (pipe == NULL)
    {
      g_printerr ("Cannot run '%s'\n", command);
      exit (EXIT_FAILURE);
    }

  if (fgets (line, sizeof (line), pipe) == NULL)
    line[0] = '\0';

  if (strlen (line) > 0 && line [strlen (line) - 1] == '\n')
    line [strlen (line) - 1] = '\0';
  if (strlen (line) > 0 && line [strlen (line) - 1] == '\r')
    line [strlen (line) - 1] = '\0';

  pclose (pipe);

  if (strlen (line) == 0)
    {
      g_printerr ("No output from '%s'\n", command);
      exit (EXIT_FAILURE);
    }

  return g_strdup (line);
}

static gchar *
pkg_config (const gchar *args)
{
#ifdef G_OS_WIN32
  if (msvc_syntax)
    return one_line_output ("pkg-config --msvc-syntax", args);
#endif

  return one_line_output ("pkg-config", args);
}

static const gchar *
get_runtime_prefix (gchar slash)
{
#ifdef G_OS_WIN32

  /* Don't use the developer package prefix, but deduce the
   * installation-time prefix from where gimp-x.y.exe can be found.
   */

  gchar *path;
  gchar *p, *r;

  path = g_find_program_in_path ("gimp-" GIMP_APP_VERSION ".exe");

  if (path == NULL)
    path = g_find_program_in_path ("gimp.exe");

  if (path != NULL)
    {
      r = strrchr (path, G_DIR_SEPARATOR);
      if (r != NULL)
	{
	  *r = '\0';
	  if (strlen (path) >= 4 &&
	      g_ascii_strcasecmp (r - 4, G_DIR_SEPARATOR_S "bin") == 0)
	    {
	      r[-4] = '\0';
	      prefix = path;
	      if (slash == '/')
		{
		  /* Use forward slashes, less quoting trouble in Makefiles */
		  while ((p = strchr (prefix, '\\')) != NULL)
		    *p = '/';
		}
	      return prefix;
	    }
	}
    }

  g_printerr ("Cannot determine GIMP " GIMP_APP_VERSION " installation location\n");

  exit (EXIT_FAILURE);
#else
  /* On Unix assume the executable package is in the same prefix as the developer stuff */
  return pkg_config ("--variable=prefix gimp-2.0");
#endif
}

static const gchar *
get_exec_prefix (gchar slash)
{
#ifdef G_OS_WIN32
  if (exec_prefix != NULL)
    return exec_prefix;

  /* On Win32, exec_prefix is always same as prefix. Or is it? Maybe not,
   * but at least in tml's prebuilt stuff it is. If somebody else does
   * it another way, feel free to hack this.
   */
  return (exec_prefix = get_runtime_prefix (slash));
#else
  return EXEC_PREFIX;
#endif
}

static const gchar *
expand_and_munge (const gchar *value)
{
  const gchar *retval;

  if (starts_with_dir (value, "${prefix}"))
    retval = g_strconcat (PREFIX, value + strlen ("${prefix}"), NULL);
  else if (starts_with_dir (value, "${exec_prefix}"))
    retval = g_strconcat (EXEC_PREFIX, value + strlen ("${exec_prefix}"), NULL);
  else
    retval = g_strdup (value);

  if (starts_with_dir (retval, EXEC_PREFIX))
    retval = g_strconcat (get_exec_prefix ('/'), retval + strlen (EXEC_PREFIX), NULL);

  if (starts_with_dir (retval, PREFIX))
    retval = g_strconcat (get_runtime_prefix ('/'), retval + strlen (PREFIX), NULL);

  return retval;
}

static void
find_out_env_flags (void)
{
  gchar *p;

  if ((p = getenv ("CC")) != NULL && *p != '\0')
    env_cc = p;
  else if (msvc_syntax)
    env_cc = "cl -MD";
  else
    env_cc = CC;

  if (g_ascii_strncasecmp (env_cc, "cl", 2) == 0)
    msvc_syntax = TRUE;

  if ((p = getenv ("CFLAGS")) != NULL)
    env_cflags = p;
  else
    env_cflags = "";

  if ((p = getenv ("LDFLAGS")) != NULL)
    env_ldflags = p;
  else
    env_ldflags = "";

  if ((p = getenv ("LIBS")) != NULL && *p != '\0')
    env_libs = p;
  else
    env_libs = "";
}

static void
usage (int exit_status)
{
  g_print ("\
Usage: gimptool-2.0 [OPTION]...\n\
\n\
General options:\n\
  --help                  print this message\n\
  --quiet, --silent       don't echo build commands\n\
  --version               print the version of GIMP associated with this script\n\
  -n, --just-print, --dry-run, --recon\n\
                          don't actually run any commands; just print them\n\
Developer options:\n\
  --cflags                print the compiler flags that are necessary to\n\
                          compile a plug-in\n\
  --libs                  print the linker flags that are necessary to link a\n\
                          plug-in\n\
  --prefix=PREFIX         use PREFIX instead of the installation prefix that\n\
                          GIMP was built when computing the output for --cflags\n\
                          and --libs\n\
  --exec-prefix=PREFIX    use PREFIX instead of the installation exec prefix\n\
                          that GIMP was built when computing the output for\n\
                          --cflags and --libs\n\
  --msvc-syntax           print flags in MSVC syntax\n\
\n\
Installation directory options:\n\
  --prefix --exec-prefix --bindir --sbindir --libexecdir --datadir --sysconfdir\n\
  --sharedstatedir --localstatedir --libdir --infodir --mandir --includedir\n\
  --gimpplugindir --gimpdatadir\n\
\n\
The --cflags and --libs options can be appended with -noui to get appropriate\n\
settings for plug-ins which do not use GTK+.\n\
\n\
User options:\n\
  --build plug-in.c               build a plug-in from a source file\n\
  --install plug-in.c             same as --build, but installs the built\n\
                                  plug-in as well\n\
  --install-bin plug-in           install a compiled plug-in\n\
  --install-script script.scm     install a script-fu script\n\
\n\
  --uninstall-bin plug-in         remove a plug-in again\n\
  --uninstall-script plug-in      remove a script-fu script\n\
\n\
The --install and --uninstall options have \"admin\" counterparts (with\n\
prefix --install-admin instead of --install) that can be used instead to\n\
install/uninstall a plug-in or script in the machine directory instead of a\n\
user directory.\n\
\n\
For plug-ins which do not use GTK+, the --build and --install options can be\n\
appended with -noui for appropriate settings. For plug-ins that use GTK+ but\n\
not libgumpui, append -nogimpui.\n");
  exit (exit_status);
}

static gchar *
get_includedir (void)
{
  return pkg_config ("--variable=includedir gimp-2.0");
}

static void
do_includedir (void)
{
  g_print ("%s\n", get_includedir ());
}

static gchar *
get_cflags (void)
{
  return pkg_config ("--cflags gimpui-2.0");
}

static void
do_cflags (void)
{
  g_print ("%s\n", get_cflags ());
}

static gchar *
get_cflags_noui (void)
{
  return pkg_config ("--cflags gimp-2.0");
}

static void
do_cflags_noui (void)
{
  g_print ("%s\n", get_cflags_noui ());
}

static gchar *
get_cflags_nogimpui (void)
{
  return pkg_config ("--cflags gimp-2.0 gtk+-2.0");
}

static void
do_cflags_nogimpui (void)
{
  g_print ("%s\n", get_cflags_nogimpui ());
}

static gchar *
get_libs (void)
{
  return pkg_config ("--libs gimpui-2.0");
}

static void
do_libs (void)
{
  g_print ("%s\n", get_libs ());
}

static gchar *
get_libs_noui (void)
{
  return pkg_config ("--libs gimp-2.0");
}

static void
do_libs_noui (void)
{
  g_print ("%s\n", get_libs_noui ());
}

static gchar *
get_libs_nogimpui (void)
{
  return pkg_config ("--libs gimp-2.0 gtk+-2.0");
}

static void
do_libs_nogimpui (void)
{
  g_print ("%s\n", get_libs_nogimpui ());
}

static void
maybe_run (gchar *cmd)
{
  if (!silent)
    g_print ("%s\n", cmd);

  if (!dry_run)
    system (cmd);
}

static void
do_build_2 (const gchar *cflags,
	    const gchar *libs,
	    const gchar *install_dir,
	    const gchar *what)
{
  gchar       *cmd;
  const gchar *dest_dir;
  const gchar *output_flag;
  gchar       *dest_exe;
  const gchar *here_comes_linker_flags = "";
  const gchar *windows_subsystem_flag = "";
  gchar       *p, *q;

  if (install_dir != NULL)
    dest_dir = g_strconcat (install_dir, "/", NULL);
  else
    dest_dir = "";

  dest_exe = g_strdup (what);

  p = strrchr (dest_exe, '.');
  if (p == NULL ||
      !(strcmp (p, ".c")   == 0 ||
        strcmp (p, ".cc")  == 0 ||
        strcmp (p, ".cpp") == 0))
    {
      g_printerr ("plug-in source %s is not a C or C++ file?\n", what);
      exit (EXIT_FAILURE);
    }

  *p = '\0';
  q = strrchr (dest_exe, G_DIR_SEPARATOR);
#ifdef G_OS_WIN32
  {
    gchar *r = strrchr (dest_exe, '/');
    if (r != NULL && (q == NULL || r > q))
      q = r;
  }
#endif
  if (q == NULL)
    q = dest_exe;
  else
    q++;

  dest_exe = g_strconcat (q, EXEEXT, NULL);

  if (msvc_syntax)
    {
      output_flag = "-Fe";
      here_comes_linker_flags = " -link";
      windows_subsystem_flag = " -subsystem:windows";
    }
  else
    {
      output_flag = "-o ";
#ifdef G_OS_WIN32
      windows_subsystem_flag = " -mwindows";
#endif
    }

  cmd = g_strdup_printf ("%s %s %s %s%s%s %s%s %s%s %s %s",
			 env_cc,
			 env_cflags,
			 cflags,
			 output_flag,
			 dest_dir,
			 dest_exe,
			 what,
			 here_comes_linker_flags,
			 env_ldflags,
			 windows_subsystem_flag,
			 libs,
			 env_libs);
  maybe_run (cmd);
}

static void
do_build (const gchar *what)
{
  do_build_2 (get_cflags (), get_libs (), NULL, what);
}

static void
do_build_noui (const gchar *what)
{
  do_build_2 (get_cflags_noui (), get_libs_noui (), NULL, what);
}

static void
do_build_nogimpui (const gchar *what)
{
  do_build (what);
}

static gchar *
get_user_plugin_dir (gboolean forward_slashes)
{
#ifdef G_OS_WIN32
  /* In the --install-bin and --uninstall modes we want
   * to use backward slashes on Win32, because we invoke
   * the COPY and DEL commands directly with system().
   */

  const gchar slash = forward_slashes ? '/' : '\\';
#else
  const gchar slash = '/';
#endif

  return g_strdup_printf ("%s%c" GIMPDIR "%cplug-ins",
			  g_get_home_dir (), slash, slash);
}

static void
do_install (const gchar *what)
{
  do_build_2 (get_cflags (), get_libs (), get_user_plugin_dir (FALSE), what);
}

static void
do_install_noui (const gchar *what)
{
  do_build_2 (get_cflags_noui (), get_libs_noui (), get_user_plugin_dir (FALSE), what);
}

static void
do_install_nogimpui (const gchar *what)
{
  do_install (what);
}

static gchar *
get_sys_plugin_dir (gboolean forward_slashes)
{
#ifdef G_OS_WIN32
  const gchar slash = forward_slashes ? '/' : '\\';
#else
  const gchar slash = '/';
#endif

  return g_strdup_printf ("%s%clib%cgimp%c" GIMP_PLUGIN_VERSION "%cplug-ins",
			  get_runtime_prefix (slash),
			  slash, slash, slash, slash);
}

static void
do_install_admin (const gchar *what)
{
  do_build_2 (get_cflags (), get_libs (), get_sys_plugin_dir (TRUE), what);
}

static void
do_install_admin_noui (const gchar *what)
{
  do_build_2 (get_cflags_noui (), get_libs_noui (), get_sys_plugin_dir (TRUE), what);
}

static void
do_install_admin_nogimpui (const gchar *what)
{
  do_build_2 (get_cflags (), get_libs (), get_sys_plugin_dir (TRUE), what);
}

static void
do_install_bin_2 (const gchar *dir,
		  const gchar *what)
{
  g_mkdir_with_parents (dir,
                        S_IRUSR | S_IXUSR | S_IWUSR |
                        S_IRGRP | S_IXGRP |
                        S_IROTH | S_IXOTH);

  maybe_run (g_strconcat (COPY, " ", what, " ", dir, NULL));
}

static void
do_install_bin (const gchar *what)
{
  do_install_bin_2 (get_user_plugin_dir (FALSE), what);
}

static void
do_install_admin_bin (const gchar *what)
{
  do_install_bin_2 (get_sys_plugin_dir (FALSE), what);
}

static void
do_uninstall (const gchar *dir,
	      const gchar *what)
{
  maybe_run (g_strconcat (REMOVE, " ", dir, G_DIR_SEPARATOR_S, what, NULL));
}

static const gchar *
maybe_append_exe (const gchar *what)
{
#ifdef G_OS_WIN32
  gchar *p = strrchr (what, '.');

  if (p == NULL || g_ascii_strcasecmp (p, ".exe") != 0)
    return g_strconcat (what, ".exe", NULL);
#endif

  return what;
}

static void
do_uninstall_bin (const gchar *what)
{
  do_uninstall (get_user_plugin_dir (FALSE), maybe_append_exe (what));
}

static void
do_uninstall_admin_bin (const gchar *what)
{
  do_uninstall (get_sys_plugin_dir (FALSE), maybe_append_exe (what));
}

static gchar *
get_user_script_dir (gboolean forward_slashes)
{
#ifdef G_OS_WIN32
  const gchar slash = forward_slashes ? '/' : '\\';
#else
  const gchar slash = '/';
#endif

  return g_strdup_printf ("%s%c" GIMPDIR "%cscripts",
			  g_get_home_dir (), slash, slash);
}

static void
do_install_script (const gchar *what)
{
  do_install_bin_2 (get_user_script_dir (FALSE), what);
}

static gchar *
get_sys_script_dir (gboolean forward_slashes)
{
#ifdef G_OS_WIN32
  const gchar slash = forward_slashes ? '/' : '\\';
#else
  const gchar slash = '/';
#endif

  return g_strdup_printf ("%s%cshare%cgimp%c%d.%d%cscripts",
			  get_runtime_prefix (slash),
			  slash, slash, slash,
			  GIMP_MAJOR_VERSION, GIMP_MINOR_VERSION,
			  slash);
}

static void
do_install_admin_script (const gchar *what)
{
  do_install_bin_2 (get_sys_script_dir (FALSE), what);
}

static void
do_uninstall_script (const gchar *what)
{
  do_uninstall (get_user_script_dir (FALSE), what);
}

static void
do_uninstall_admin_script (const gchar *what)
{
  do_uninstall (get_sys_script_dir (FALSE), what);
}

int
main (int    argc,
      char **argv)
{
  gint argi;
  gint i;

  if (argc == 1)
    usage (EXIT_SUCCESS);

  /* First scan for flags that affect our behaviour globally, but
   * are still allowed late on the command line.
   */
  argi = 0;
  while (++argi < argc)
    {
      if (strcmp (argv[argi], "-n") == 0 ||
	  strcmp (argv[argi], "--just-print") == 0 ||
	  strcmp (argv[argi], "--dry-run") == 0 ||
	  strcmp (argv[argi], "--recon") == 0)
        {
          dry_run = TRUE;
        }
      else if (strcmp (argv[argi], "--help") == 0)
        {
          usage (EXIT_SUCCESS);
        }
      else if (g_str_has_prefix (argv[argi], "--prefix="))
        {
          prefix = argv[argi] + strlen ("--prefix=");
        }
      else if (g_str_has_prefix (argv[argi], "--exec-prefix="))
        {
          exec_prefix = argv[argi] + strlen ("--exec_prefix=");
        }
      else if (strcmp (argv[argi], "--msvc-syntax") == 0)
        {
          msvc_syntax = TRUE;
        }
    }

#ifndef G_OS_WIN32
  if (msvc_syntax)
    {
      g_printerr ("Ignoring --msvc-syntax\n");
      msvc_syntax = FALSE;
    }
#endif

  find_out_env_flags ();

  /* Second pass, actually do something. */
  argi = 0;
  while (++argi < argc)
    {
      for (i = 0; i < G_N_ELEMENTS (dirs); i++)
        {
          if (strcmp (argv[argi],
                      g_strconcat ("--", dirs[i].option, NULL)) == 0)
            break;
        }

      if (i < G_N_ELEMENTS (dirs))
        {
          g_print ("%s\n", expand_and_munge (dirs[i].value));
        }
      else if (strcmp (argv[argi], "--quiet") == 0 ||
               strcmp (argv[argi], "--silent") == 0)
        {
          silent = TRUE;
        }
      else if (strcmp (argv[argi], "--version") == 0)
	{
	  g_print ("%d.%d.%d\n",
                   GIMP_MAJOR_VERSION, GIMP_MINOR_VERSION, GIMP_MICRO_VERSION);
	  exit (EXIT_SUCCESS);
	}
      else if (strcmp (argv[argi], "-n") == 0 ||
	       strcmp (argv[argi], "--just-print") == 0 ||
	       strcmp (argv[argi], "--dry-run") == 0 ||
	       strcmp (argv[argi], "--recon") == 0)
	;			/* Already handled */
      else if (strcmp (argv[argi], "--includedir") == 0)
	do_includedir ();
      else if (strcmp (argv[argi], "--cflags") == 0)
	do_cflags ();
      else if (strcmp (argv[argi], "--cflags-noui") == 0)
	do_cflags_noui ();
      else if (strcmp (argv[argi], "--cflags-nogimpui") == 0)
	do_cflags_nogimpui ();
      else if (strcmp (argv[argi], "--libs") == 0)
	do_libs ();
      else if (strcmp (argv[argi], "--libs-noui") == 0)
	do_libs_noui ();
      else if (strcmp (argv[argi], "--libs-nogimpui") == 0)
	do_libs_nogimpui ();
      else if (g_str_has_prefix (argv[argi], "--prefix="))
	;
      else if (g_str_has_prefix (argv[argi], "--exec-prefix="))
	;
      else if (strcmp (argv[argi], "--msvc-syntax") == 0)
	;
      else if (strcmp (argv[argi], "--build") == 0)
	do_build (argv[++argi]);
      else if (strcmp (argv[argi], "--build-noui") == 0)
	do_build_noui (argv[++argi]);
      else if (strcmp (argv[argi], "--build-nogimpui") == 0)
	do_build_nogimpui (argv[++argi]);
      else if (strcmp (argv[argi], "--install") == 0)
	do_install (argv[++argi]);
      else if (strcmp (argv[argi], "--install-noui") == 0)
	do_install_noui (argv[++argi]);
      else if (strcmp (argv[argi], "--install-nogimpui") == 0)
	do_install_nogimpui (argv[++argi]);
      else if (strcmp (argv[argi], "--install-admin") == 0)
	do_install_admin (argv[++argi]);
      else if (strcmp (argv[argi], "--install-admin-noui") == 0)
	do_install_admin_noui (argv[++argi]);
      else if (strcmp (argv[argi], "--install-admin-nogimpui") == 0)
	do_install_admin_nogimpui (argv[++argi]);
      else if (strcmp (argv[argi], "--install-bin") == 0)
	do_install_bin (argv[++argi]);
      else if (strcmp (argv[argi], "--install-admin-bin") == 0)
	do_install_admin_bin (argv[++argi]);
      else if (strcmp (argv[argi], "--uninstall-bin") == 0)
	do_uninstall_bin (argv[++argi]);
      else if (strcmp (argv[argi], "--uninstall-admin-bin") == 0)
	do_uninstall_admin_bin (argv[++argi]);
      else if (strcmp (argv[argi], "--install-script") == 0)
	do_install_script (argv[++argi]);
      else if (strcmp (argv[argi], "--install-admin-script") == 0)
	do_install_admin_script (argv[++argi]);
      else if (strcmp (argv[argi], "--uninstall-script") == 0)
	do_uninstall_script (argv[++argi]);
      else if (strcmp (argv[argi], "--uninstall-admin-script") == 0)
	do_uninstall_admin_script (argv[++argi]);
      else
	{
	  g_printerr ("Unrecognized switch %s\n", argv[argi]);
	  usage (EXIT_FAILURE);
	}
    }

  exit (EXIT_SUCCESS);
}
/*
 * Local Variables:
 * mode: c
 * End:
 */

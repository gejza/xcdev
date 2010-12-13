dnl Macros for auto-configure script.
dnl ---------------------------------------------------------------------------
dnl AM_MFIS_DEBUG version: 1
dnl --------------
dnl Usage: Just AC_MFIS_DEBUG
AC_DEFUN([AC_MFIS_DEBUG], [
    AC_MSG_CHECKING(if you want to enable debug-code)
    AC_ARG_ENABLE(debug,
        AS_HELP_STRING([--enable-debug], [compile w/ debugging (if \$CFLAGS is set, add -g there, too)]))
    AC_MSG_RESULT($with_debug)
    AC_MSG_NOTICE([host: ], $host_os)
    AS_IF([test "x$enable_debug" = "xyes"], [
        case $host_os in
        ultrix*|osf*)
            CF_STRIP_O_OPT(CFLAGS)
            if test -z "$GCC" ; then
                CFLAGS="$CFLAGS -g3"
            fi
            ;;
        esac
    ],[
        case $host_os in
        osf*) #(vi
            test -z "$GCC" && CFLAGS="$CFLAGS -std1"
            CF_STRIP_G_OPT(CFLAGS)
            ;;
        *)
            CF_STRIP_G_OPT(CFLAGS)
            ;;
        esac
    ])
])dnl

dnl ---------------------------------------------------------------------------
dnl AM_MFIS_WARN version: 1
dnl --------------
dnl Usage: Just AC_MFIS_WARN
AC_DEFUN([AC_MFIS_WARN], [
    # warnings
    if test -n "$GCC"
    then
    AC_MSG_CHECKING(if you want to turn on gcc warnings)
    AC_ARG_ENABLE(warnings,
                  AS_HELP_STRING([--enable-warnings], [GCC compiler warnings]))
    AS_IF([test "x$enable_warnings" = "xyes"], [
        AC_MSG_RESULT([enabling warnings])
        CF_GCC_ATTRIBUTES
        CF_GCC_WARNINGS
        ])
    fi

])dnl

dnl ---------------------------------------------------------------------------
dnl AM_MFIS_OPT version: 1
dnl --------------
dnl Usage: Just AC_MFIS_OPT
AC_DEFUN([AC_MFIS_OPT], [
    # optimalization
    AC_ARG_ENABLE(optimization,
        AS_HELP_STRING([--enable-optimization], [compile optimized without debug logging]))
    AS_IF([test "x$enable_optimization" = "xyes"], [
        AC_MSG_RESULT([enabling optimization.])
        CXXEXTRAFLAGS="${CXXEXTRAFLAGS} -O2"
        CPPFLAGS="${CPPFLAGS} -UDEBUG"
    ],[
        AC_MSG_RESULT([disabling optimization.])
        CPPFLAGS="${CPPFLAGS} -DDEBUG=1"
    ])
])dnl


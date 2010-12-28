dnl $Id$
dnl config.m4 for extension xcrd

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(xcrd, for xcrd support,
[  --with-xcrd             Include xcrd support])


if test "$PHP_XCRD" != "no"; then
  dnl Write more examples of tests here...
    PHP_REQUIRE_CXX()

    PHP_ADD_LIBRARY(stdc++, 1, XCRD_SHARED_LIBADD)
    # --with-xcrd -> check with-path
    SEARCH_PATH="/usr/local /usr"     # you might want to change this
    SEARCH_FOR="include/xc/version.h"  # you most likely want to change this
    if test -r $PHP_XCRD; then # path given as parameter
        XCRD_DIR=$PHP_XCRD
        AC_MSG_RESULT(found in $PHP_XCRD)
    else # search default path list
     AC_MSG_CHECKING([for xcrd files in default path])
     for i in $SEARCH_PATH ; do
       if test -r $i/$SEARCH_FOR; then
         XCRD_DIR=$i
         AC_MSG_RESULT(found in $i)
       fi
     done
    fi

    if test -z "$XCRD_DIR"; then
        AC_MSG_RESULT([not found])
        AC_MSG_ERROR([Please reinstall the xcrd distribution])
    fi

    # --with-xcrd -> add include path
    PHP_ADD_INCLUDE($XCRD_DIR/include)

    # --with-xcrd -> check for lib and symbol presence
    LIBNAME=xcrd # you may want to change this
    LIBSYMBOL=xcrd_version # you most likely want to change this

    PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
    [
        PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $XCRD_DIR/lib, XCRD_SHARED_LIBADD)
        AC_DEFINE(HAVE_XCRDLIB,1,[ ])
    ],[
        AC_MSG_ERROR([wrong xcrd lib version or lib not found])
    ],[
       -L$XCRD_DIR/lib -lm
    ])

  PHP_SUBST(XCRD_SHARED_LIBADD)

  PHP_NEW_EXTENSION(xcrd, xcrd.cc utils.cc, $ext_shared)
fi

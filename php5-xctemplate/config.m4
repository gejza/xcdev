dnl $Id$
dnl config.m4 for extension xctemplate

PHP_ARG_WITH(xctemplate, for xctemplate support,
[  --with-xctemplate[=FILE]       Include xctemplate support. File is the optional path to xctemplate-config])
PHP_ARG_ENABLE(xctemplate-debug, whether to enable debugging support in xctemplate,
[  --enable-xctemplate-debug        xctemplate: Enable debugging support in xctemplate], no, no)

dnl Check whether the extension is enabled at all
if test "$PHP_XCTEMPLATE" != "no"; then
  
  PHP_REQUIRE_CXX()


 dnl Check for xctemplate-config. First try any path that was given to us, then look in $PATH
  AC_MSG_CHECKING([for xctemplate-config])
  XCTEMPLATE_CONFIG="xctemplate-config"
  if test "$PHP_XCTEMPLATE" != "yes"; then
    XCTEMPLATE_PATH=$PHP_XCTEMPLATE
  else
    XCTEMPLATE_PATH=`$php_shtool path $XCTEMPLATE_CONFIG`
  fi
  
  dnl If a usable xctemplate-config was found, use it
  if test -f "$XCTEMPLATE_PATH" && test -x "$XCTEMPLATE_PATH" && $XCTEMPLATE_PATH --version > /dev/null 2>&1; then
    AC_MSG_RESULT([$XCTEMPLATE_PATH])
    XCTEMPLATE_LIB_NAME=xctemplate
    XCTEMPLATE_INCDIRS=`$XCTEMPLATE_PATH --includedir`
    XCTEMPLATE_LIBS=`$XCTEMPLATE_PATH --libs`
    
    dnl Check that the library works properly
    PHP_CHECK_LIBRARY($XCTEMPLATE_LIB_NAME, xctemplate_version,
    [
      dnl Add the necessary include dirs
      PHP_EVAL_INCLINE($XCTEMPLATE_INCDIRS)
      dnl Add the necessary libraries and library dirs
      PHP_EVAL_LIBLINE($XCTEMPLATE_LIBS, XCTEMPLATE_SHARED_LIBADD)
    ],[
      dnl Bail out
      AC_MSG_ERROR([xctemplate library not found. Check config.log for more information.])
    ],[$XCTEMPLATE_LIBS]
    )
  else
    dnl No usable xctemplate-config, bail  
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please check your xctemplate installation.])
  fi


    PHP_ADD_LIBRARY(stdc++, 1, XCTEMPLATE_SHARED_LIBADD)

  
  dnl Check whether to enable debugging
  if test "$PHP_XCTEMPLATE_DEBUG" != "no"; then
    dnl Yes, so set the C macro
    AC_DEFINE(USE_XCTEMPLATE_DEBUG,1,[Include debugging support in xctemplate])
  fi
  
  dnl Finally, tell the build system about the extension and what files are needed
  PHP_NEW_EXTENSION(xctemplate, main.cc template.cc, $ext_shared)
  PHP_SUBST(XCTEMPLATE_SHARED_LIBADD)
fi


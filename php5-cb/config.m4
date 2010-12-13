PHP_ARG_ENABLE(cb, whether to enable cb support,
[ --enable-cb   Enable cb support])

if test -z "$PHP_DEBUG"; then
  AC_ARG_ENABLE(debug,
  [  --enable-debug          compile with debugging symbols],[
    PHP_DEBUG=$enableval
  ],[
    PHP_DEBUG=no
  ]) 
fi

if test "$PHP_CB" != "no"; then
  AC_DEFINE(HAVE_CB, 1, [Whether you have cb])
  PHP_REQUIRE_CXX()
  PHP_ADD_LIBRARY(stdc++, 1, CB_SHARED_LIBADD)
  PHP_ADD_LIBRARY(xc, 1, CB_SHARED_LIBADD)
  PHP_ADD_LIBRARY(xclog, 1, CB_SHARED_LIBADD)
  PHP_ADD_LIBRARY(xccb, 1, CB_SHARED_LIBADD)
  PHP_SUBST(CB_SHARED_LIBADD)
  PHP_NEW_EXTENSION(cb, cb.cc utils.cc, $ext_shared)
fi

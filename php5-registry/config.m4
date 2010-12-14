PHP_ARG_ENABLE(registry, whether to enable registry support,
[ --enable-registry   Enable registry support])

if test -z "$PHP_DEBUG"; then
  AC_ARG_ENABLE(debug,
  [  --enable-debug          compile with debugging symbols],[
    PHP_DEBUG=$enableval
  ],[
    PHP_DEBUG=no
  ]) 
fi

if test "$PHP_REGISTRY" != "no"; then
  AC_DEFINE(HAVE_REGISTRY, 1, [Whether you have registry])
  PHP_REQUIRE_CXX()
  PHP_ADD_LIBRARY(stdc++, 1, REGISTRY_SHARED_LIBADD)
  PHP_ADD_LIBRARY(xc, 1, REGISTRY_SHARED_LIBADD)
  PHP_ADD_LIBRARY(xclog, 1, REGISTRY_SHARED_LIBADD)
  PHP_ADD_LIBRARY(xcreg, 1, REGISTRY_SHARED_LIBADD)
  PHP_SUBST(REGISTRY_SHARED_LIBADD)
  PHP_NEW_EXTENSION(registry, registry.cc utils.cc, $ext_shared)
fi

PHP_ARG_WITH(pancake, for pancake support, [  --with-pancake     Enable pancake support])

if test "$PHP_PANCAKE" != "no"; then
  PHP_REQUIRE_CXX
  AC_LANG_CPLUSPLUS
  PHP_ADD_LIBRARY(stdc++,,PANCAKE_SHARED_LIBADD)
  export OLD_CPPFLAGS="$CPPFLAGS"
  export CPPFLAGS="$CPPFLAGS $INCLUDES -DHAVE_HELLO"
  AC_MSG_CHECKING(PHP version)
  AC_TRY_COMPILE([#include <php_version.h>], [
  #if PHP_VERSION_ID < 40000
  #error  this extension requires at least PHP version 4.0.0
  #endif
  ],
  [AC_MSG_RESULT(ok)],
  [AC_MSG_ERROR([need at least PHP 4.0.0])])

  export CPPFLAGS="$OLD_CPPFLAGS"


  PHP_SUBST(HELLO_SHARED_LIBADD)
  AC_DEFINE(HAVE_HELLO, 1, [ ])

  SEARCH_PATH="/usr/local /usr /opt/local"
  SEARCH_FOR="/include/opencv/cv.h"

  if test -r $PHP_PANCAKE/$SEARCH_FOR; then
    PANCAKE_DIR=$PHP_PANCAKE
  else
    AC_MSG_CHECKING([for pancake in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        PANCAKE_DIR=$i
        AC_MSG_RESULT(found in $i)
	break
      fi
    done
  fi

  if test -z "$PANCAKE_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the OpenCV distribution])
  fi

  PHP_ADD_INCLUDE($PANCAKE_DIR/include)

  AC_CHECK_HEADER([$PANCAKE_DIR/include/opencv/cv.h], [], AC_MSG_ERROR('opencv/cv.h' header not found))
  AC_CHECK_HEADER([$PANCAKE_DIR/include/opencv/highgui.h], [], AC_MSG_ERROR('opencv/highgui.h' header not found))

  PHP_CHECK_LIBRARY(opencv_core, cvLoad,
  [
    PHP_ADD_LIBRARY_WITH_PATH(opencv_core, $PANCAKE_DIR/lib, PANCAKE_SHARED_LIBADD)
    PHP_CHECK_LIBRARY(opencv_objdetect, cvHaarDetectObjects,
    [
      PHP_ADD_LIBRARY_WITH_PATH(opencv_objdetect, $PANCAKE_DIR/lib, PANCAKE_SHARED_LIBADD)
      AC_DEFINE(HAVE_PANCAKE, 1, [ ])
    ],[
      AC_MSG_ERROR([Wrong OpenCV version or OpenCV not found]) 
      ],[
    ]),
  ],[
    AC_MSG_ERROR([wrong OpenCV version or OpenCV not found])
  ],[
  ])

  PHP_SUBST(PANCAKE_SHARED_LIBADD)
  AC_DEFINE(HAVE_PANCAKE, 1, [ ])
  PHP_NEW_EXTENSION(pancake, pancake.cpp, $ext_shared)
fi


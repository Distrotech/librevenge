#!/bin/sh

accheck=`autoconf --version | grep 2.13`
if test "x$accheck" != "x"; then
    echo "error: you appear to be using autoconf 2.13"
    echo "       the automake build system requires autoconf >= 2.50"
    exit 1
fi

aclocal --version > /dev/null 2> /dev/null || {
    echo "error: aclocal not found"
    exit 1
}
automake --version > /dev/null 2> /dev/null || {
    echo "error: automake not found"
    exit 1
}

amcheck=`automake --version | grep 'automake (GNU automake) 1.5'`
if test "x$amcheck" = "xautomake (GNU automake) 1.5"; then
    echo "warning: you appear to be using automake 1.5"
    echo "         this version has a bug - GNUmakefile.am dependencies are not generated"
fi

libtoolize --force --copy || {
    echo "error: libtoolize failed"
    exit 1
}
aclocal $ACLOCAL_FLAGS || {
    echo "error: aclocal $ACLOCAL_FLAGS failed"
    exit 1
}
autoheader || {
    echo "error: autoheader failed"
    exit 1
}
automake -a -c --foreign || {
    echo "warning: automake failed"
}
autoconf || {
    echo "error: autoconf failed"
    exit 1
}

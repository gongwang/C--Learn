#!/bin/sh
# Convenience script for regenerating Makefile and invoking it.

# Usage: ./autogen.sh [debug/release] [clean]
if [ "$1" = "debug" ]; then
    CPPFLAGS='-DDEBUG'
    CXXFLAGS='-O0 -g'
    LDFLAGS='-O0'
elif [ "$1" = "release" ]; then
    CPPFLAGS='-DNDEBUG'
    CXXFLAGS='-O2'
    LDFLAGS='-O2'
else
    echo "invalid argument, command format: ./autogen.sh [debug/release] [clean]"
    exit
fi
export CPPFLAGS CXXFLAGS LDFLAGS

if [ "$2" = "clean" ]; then
    if [ -f Makefile ] ; then
        make distclean
        [ $? -ne 0 ] && autoreconf --install --force && ./configure && make distclean
    fi
    [ -e aclocal.m4 ] && rm -rf aclocal.m4
    [ -e autom4te.cache ] && rm -rf autom4te.cache
    [ -e config.h.in ] && rm -rf config.h.in
    [ -e configure ] && rm -rf configure
    [ -e depcomp ] && rm -rf depcomp
    [ -e install-sh ] && rm -rf install-sh
    [ -e Makefile.in ] && rm -rf Makefile.in
    [ -e missing ] && rm -rf missing
    
    [ -e config.h.in~ ] && rm -rf config.h.in~
    [ -e config.guess ] && rm -rf config.guess
    [ -e config.sub ] && rm -rf config.sub
    [ -e ltmain.sh ] && rm -rf ltmain.sh
else
    [ ! -f Makefile ] && autoreconf --install --force && ./configure
    make  
fi


#---------------------------------------------------------------------------
# testts.pro
#
# Build configuration file for Zyzzyva tests using qmake.
#
# Copyright 2006-2012 Boshvark Software, LLC.
#
# This file is part of Zyzzyva.
#
# Zyzzyva is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# Zyzzyva is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#---------------------------------------------------------------------------

TEMPLATE = app
TARGET = test_zyzzyva
CONFIG += qt thread warn_on qtestlib
QT += sql xml

ROOT = ../..
DESTDIR = $$ROOT/bin
INCLUDEPATH += $$ROOT/src/libzyzzyva

include($$ROOT/zyzzyva.pri)

unix {
    LIBS = -lzyzzyva -L$$ROOT/bin
}
win32 {
    LIBS = -lzyzzyva2 -L$$ROOT/bin
}

# Source files
SOURCES = \
    WordEngineTest.cpp

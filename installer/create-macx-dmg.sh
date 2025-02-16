#!/bin/bash
#-----------------------------------------------------------------------------
# create-macx-dmg.sh
#
# Create a disk image file for Mac OS X.
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
#-----------------------------------------------------------------------------

set -e

. $(pwd)/installer/create-macx-bundle.sh

ZVER=$(cat $(pwd)/VERSION)
APPDIR=$BUILD_DIR/Zyzzyva-$ZVER.app
DMGFILE=Zyzzyva-$ZVER.dmg
mv $BUILD_DIR/Zyzzyva.app $APPDIR

# Create disk image
echo "Creating disk image..."
rm -rf $DMGFILE
hdiutil create -srcfolder $APPDIR -volname Zyzzyva-$ZVER $DMGFILE > /dev/null
hdiutil attach $DMGFILE > /dev/null
DEVS=$(hdiutil attach $DMGFILE | cut -f 1)
DEV=$(echo $DEVS | cut -f 1 -d ' ')
hdiutil detach $DEV > /dev/null
hdiutil internet-enable -yes $DMGFILE > /dev/null
hdiutil convert $DMGFILE -format UDZO -o Zyzzyva-output.dmg > /dev/null
# 10.3.9 uses -format ADC
mv Zyzzyva-output.dmg $DMGFILE
mv $DMGFILE $BUILD_DIR

mv $APPDIR $BUILD_DIR/Zyzzyva.app

echo "Done.  Disk image is called $BUILD_DIR/$DMGFILE."

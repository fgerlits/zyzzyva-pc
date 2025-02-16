#!/bin/bash
#-----------------------------------------------------------------------------
# create-macx-installer-dmg.sh
#
# Create a disk image file for the Mac OS X installer.
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

ZVER=2.1.0
OUTDIR=$HOME/dev/zyzzyva-install
DMGFILE=$OUTDIR/Zyzzyva-$ZVER-osx-installer.dmg

mkdir -p $OUTDIR
mv /Applications/BitRock\ InstallBuilder\ Enterprise\ 4.1.0/output/Zyzzyva* $OUTDIR

# Create disk image
echo "Creating disk image..."
rm -rf $DMGFILE
hdiutil create -srcfolder $OUTDIR/Zyzzyva-$ZVER-osx-installer.app \
    -volname Zyzzyva-$ZVER-osx-installer $DMGFILE
hdiutil attach $DMGFILE
DEVS=$(hdiutil attach $DMGFILE | cut -f 1)
DEV=$(echo $DEVS | cut -f 1 -d ' ')
hdiutil detach $DEV
hdiutil internet-enable -yes $DMGFILE
hdiutil convert $DMGFILE -format UDZO -o $OUTDIR/out.dmg
mv $OUTDIR/out.dmg $DMGFILE

echo "Done.  Disk image is called $DMGFILE."

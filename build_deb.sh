#!/bin/bash

set -x

export LANG="C"

PROG=soko4
EXE=soko4
ICON=$PROG.png

IS_DEB="NO"

NAME1=`uname`
if [ "Linux" == "${NAME1}" ]
then
    if [ -z "$DISTRO" -a -f /etc/debian_version ]; then
	if [ -f /etc/lsb-release ]; then
	    D1=`cat /etc/lsb-release|grep DISTRIB_ID|awk -F'=' '{print $2}'`
	    D2=`cat /etc/lsb-release|grep DISTRIB_RELEASE|awk -F'=' '{print $2}'`
	    DISTRO="$D1 $D2"
            REL_NUMBER=$D2
	else
	    REL_NUMBER=`cat /etc/debian_version`
            REL_NUMBER=`echo -n $REL_NUMBER | sed "s#/#_#"`
	    DISTRO="Debian $REL_NUMBER"
	fi
	IS_DEB="YES"
    fi
fi

if [ "$IS_DEB" != "YES" ]
then
    echo "It is not a deb-based distributive"
    exit 1
fi

case "$DISTRO" in
    Ubuntu\ *)            RELEASE="ubuntu_${REL_NUMBER}";;
    LinuxMint\ *)         RELEASE="mint_${REL_NUMBER}";;
    Debian\ *)            RELEASE="debian_${REL_NUMBER}";;
esac

if [ "FreeBSD" == "$NAME1" ]
then
    echo $NAME1
    DISTRO=`uname -a|awk -F' ' '{print $3}'`
    DISTRO="$NAME1 $DISTRO"
fi


#VERSION=`grep a.setApplicationVersion <../core/main.cpp|awk -F\" '{print $2}'|awk -F/ '{print $2}'`
#VERSION=`grep OT_VERSION <../shared/version.h|awk -F\" '{print $2}'`
VERSION="1.0.0"

if [ "ver" == "ver$VERSION" ]; then
    echo Unable to get the version \(sources changed?\)
    exit 1
fi

echo $VERSION

D=$PROG-$VERSION

rm -rf ${D}
rm -f ${PROG}_${VERSION}*
# ${D}/usr stuff
mkdir -p ${D}/debian
mkdir -p ${D}/usr/bin

if [ ! -e bin/$EXE ]; then
    echo Unable to stat bin/$EXE \(Is the OT compiled?\)
    exit 1
fi

cp bin/$EXE ${D}/usr/bin
strip ${D}/usr/bin/$EXE
mkdir -p ${D}/usr/share/pixmaps
mkdir -p ${D}/usr/share/doc/$PROG
cp pixmaps/hi128-app-ksokoban.png ${D}/usr/share/pixmaps/$ICON
cp deb/COPYING ${D}/usr/share/doc/$PROG
# copyright.tpl
m4  -DVERSION=$VERSION -DYEAR=`date +%Y` copyright.tpl >${D}/usr/share/doc/$PROG

NAME="Andrei Sartakov"
EMAIL=andrei.sartakov@gmail.com

ARCH=`dpkg --print-architecture`

m4 -DNAME=$NAME -DEMAIL=$EMAIL \
 -DARCH=$ARCH -DPACKAGENAME=$PROG control.tpl >control

m4 -DPROG=$PROG -DEXE=$EXE menu.tpl >menu

# ${D}/debian stuff

#odeskteam (1.3.2) unstable; urgency=low
#
#  * RC1 Release.
#  
# -- Andrei Sartakov <asartakov@odesk.com>  Sun, 14 Sep 2008 15:19:35 +0600
#   
#Local variables:
#mode: debian-changelog
#End:
echo "$PROG (${VERSION}) unstable; urgency=low" >changelog
echo "" >>changelog
# TODO: beta/minor/prod
echo "  * beta Release." >>changelog
echo "" >>changelog
DT1=`date --reference=changelog +%a`
DT2=`date --reference=changelog +%d`
DT3=`date --reference=changelog +%b`
DT4=`date --reference=changelog +%Y`
DT51=`date --reference=changelog +%H`
DT52=`date --reference=changelog +%M`
DT53=`date --reference=changelog +%S`
DT6=`date --reference=changelog +%z`
echo " -- $NAME $SURNAME <$EMAIL>  $DT1, $DT2 $DT3 $DT4 $DT51:$DT52:$DT53 $DT6" >>changelog
echo "" >>changelog
echo "Local variables:" >>changelog
echo "mode: debian-changelog" >>changelog
echo "End:" >>changelog

mv changelog ${D}/debian
mv control ${D}/debian

m4 -DPACKAGENAME=$PROG rules.tpl >rules
chmod +x rules
mv rules ${D}/debian
cp deb/compat ${D}/debian
cp deb/docs ${D}/debian
mkdir -p ${D}/debian/$PROG/usr/bin
mkdir -p ${D}/debian/$PROG/usr/share/doc/$PROG

mkdir -p ${D}/debian/$PROG/usr/share/$PROG/resources
mkdir -p ${D}/debian/$PROG/DEBIAN
cp deb/preinst ${D}/debian
cp deb/postinst ${D}/debian
cp deb/postrm ${D}/debian

mkdir ${D}/usr/share/menu
cp deb/menu ${D}/usr/share/menu/$PROG

mkdir ${D}/usr/share/applications
cat > ${D}/usr/share/applications/$PROG.desktop <<__EOT__
[Desktop Entry]
Encoding=UTF-8
Name=soko4
GenericName=Sokoban
Comment=A sokoban game
Exec=$EXE
Icon=$ICON
Terminal=false
Type=Application
Categories=Games;
StartupNotify=false
__EOT__


cd ./${D}
dpkg-buildpackage -rfakeroot
cd ..

mv ${PROG}_${VERSION}_${ARCH}.deb ${PROG}_${VERSION}_${RELEASE}_${ARCH}.deb
lintian -i ${PROG}_${VERSION}_${RELEASE}_${ARCH}.deb >err.txt 2>&1
cat err.txt

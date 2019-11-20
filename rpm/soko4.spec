Name:           soko4
Version:        1.0.0
Release:        1%{?dist}
Summary:        Sokoban is a type of transport puzzle

License:        GPLv3
URL:            https://bitbucket.org/asartakov/soko4.git
Source0:        soko4-1.0.0.tar.gz

BuildRequires:  qt-devel

%description
Sokoban is a type of transport puzzle, in which the player pushes boxes or crates around in a warehouse,
trying to get them to storage locations.


%prep
%setup -q -n %{name}-%{version}

%build
qmake-qt4
make


%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/{bin,share/soko4}
install -D -s -m755 bin/soko4                       $RPM_BUILD_ROOT/usr/bin/soko4
install -D -m644 levels/default.txt                 $RPM_BUILD_ROOT/usr/share/soko4/levels/default.txt
install -D -m644 pixmaps/hi128-app-ksokoban.png     $RPM_BUILD_ROOT/usr/share/soko4/pixmaps/hi128-app-ksokoban.png
install -D -m644 pixmaps/hi16-app-ksokoban.png      $RPM_BUILD_ROOT/usr/share/soko4/pixmaps/hi16-app-ksokoban.png
install -D -m644 pixmaps/hi22-app-ksokoban.png      $RPM_BUILD_ROOT/usr/share/soko4/pixmaps/hi22-app-ksokoban.png
install -D -m644 pixmaps/hi32-app-ksokoban.png      $RPM_BUILD_ROOT/usr/share/soko4/pixmaps/hi32-app-ksokoban.png
install -D -m644 pixmaps/hi48-app-ksokoban.png      $RPM_BUILD_ROOT/usr/share/soko4/pixmaps/hi48-app-ksokoban.png
install -D -m644 pixmaps/hi64-app-ksokoban.png      $RPM_BUILD_ROOT/usr/share/soko4/pixmaps/hi64-app-ksokoban.png
install -D -m644 themes/ksokoban/00000001.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000001.png
install -D -m644 themes/ksokoban/00000002.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000002.png
install -D -m644 themes/ksokoban/00000003.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000003.png
install -D -m644 themes/ksokoban/00000004.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000004.png
install -D -m644 themes/ksokoban/00000005.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000005.png
install -D -m644 themes/ksokoban/00000006.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000006.png
install -D -m644 themes/ksokoban/00000007.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000007.png
install -D -m644 themes/ksokoban/00000008.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000008.png
install -D -m644 themes/ksokoban/00000018.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000018.png
install -D -m644 themes/ksokoban/00000028.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000028.png
install -D -m644 themes/ksokoban/00000038.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000038.png
install -D -m644 themes/ksokoban/00000048.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000048.png
install -D -m644 themes/ksokoban/00000058.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000058.png
install -D -m644 themes/ksokoban/00000068.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000068.png
install -D -m644 themes/ksokoban/00000108.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000108.png
install -D -m644 themes/ksokoban/00000208.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000208.png
install -D -m644 themes/ksokoban/00000308.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000308.png
install -D -m644 themes/ksokoban/00000408.png       $RPM_BUILD_ROOT/usr/share/soko4/themes/ksokoban/00000408.png
install -D -m644 doc/COPYING                        $RPM_BUILD_ROOT/usr/share/soko4/doc/COPYING


%files
%license doc/COPYING
/usr/bin/soko4
/usr/share/soko4/levels/default.txt
/usr/share/soko4/pixmaps/hi128-app-ksokoban.png
/usr/share/soko4/pixmaps/hi16-app-ksokoban.png
/usr/share/soko4/pixmaps/hi22-app-ksokoban.png
/usr/share/soko4/pixmaps/hi32-app-ksokoban.png
/usr/share/soko4/pixmaps/hi48-app-ksokoban.png
/usr/share/soko4/pixmaps/hi64-app-ksokoban.png
/usr/share/soko4/themes/ksokoban/00000001.png
/usr/share/soko4/themes/ksokoban/00000002.png
/usr/share/soko4/themes/ksokoban/00000003.png
/usr/share/soko4/themes/ksokoban/00000004.png
/usr/share/soko4/themes/ksokoban/00000005.png
/usr/share/soko4/themes/ksokoban/00000006.png
/usr/share/soko4/themes/ksokoban/00000007.png
/usr/share/soko4/themes/ksokoban/00000008.png
/usr/share/soko4/themes/ksokoban/00000018.png
/usr/share/soko4/themes/ksokoban/00000028.png
/usr/share/soko4/themes/ksokoban/00000038.png
/usr/share/soko4/themes/ksokoban/00000048.png
/usr/share/soko4/themes/ksokoban/00000058.png
/usr/share/soko4/themes/ksokoban/00000068.png
/usr/share/soko4/themes/ksokoban/00000108.png
/usr/share/soko4/themes/ksokoban/00000208.png
/usr/share/soko4/themes/ksokoban/00000308.png
/usr/share/soko4/themes/ksokoban/00000408.png
/usr/share/soko4/doc/COPYING


%changelog
* Tue Nov 27 2018 Andrei R. Sartakov
- 

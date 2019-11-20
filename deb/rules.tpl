#!/usr/bin/make -f
# -*- makefile -*-
# Sample debian/rules that uses debhelper.
# This file was originally written by Joey Hess and Craig Small.
# As a special exception, when this file is copied by dh-make into a
# dh-make output file, you may use that output file without restriction.
# This special exception was added by Craig Small in version 0.37 of dh-make.

# Uncomment this to turn on verbose mode.
#export DH_VERBOSE=1

build:

clean:
	dh_testdir
	dh_testroot

	# Add here commands to clean up after the build process.

	#dh_clean 

DESTDIR=$(CURDIR)/debian/PACKAGENAME

install:
	dh_testdir
	dh_testroot
	dh_clean -k
	dh_installdirs

	# Add here commands to install the package into debian/PACKAGENAME.
	# $(MAKE) install DESTDIR=$(CURDIR)/debian/PACKAGENAME
	mkdir -p $(DESTDIR)
	cp -rv usr $(DESTDIR)


# Build architecture-independent files here.
binary-indep: install
# We have nothing to do by default.

# Build architecture-dependent files here.
binary-arch: install
	dh_testdir
	dh_testroot
	dh_installchangelogs 
	dh_installdocs
	dh_installexamples
#	dh_install
	dh_installmenu
#	dh_installdebconf
#	dh_installlogrotate
#	dh_installemacsen
#	dh_installpam
#	dh_installmime
#	dh_installinit
#	dh_installcron
#	dh_installinfo
	dh_installman
	dh_link
	dh_strip
	dh_compress
	dh_fixperms
#	dh_perl
#	dh_python
#	dh_makeshlibs
	dh_installdeb
	dh_shlibdeps
	dh_gencontrol
	dh_md5sums
	dh_builddeb

binary: binary-indep binary-arch
.PHONY: clean binary-indep binary-arch binary install configure

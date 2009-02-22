defsdir = $(datadir)/pygtk/2.0/defs
defs_DATA = gtkdatabox.defs

INCLUDES = \
	$(GTKDATABOX_CFLAGS) \
	$(GTK_CFLAGS) \
	$(PYGTK_CFLAGS) \
	$(PYTHON_INCLUDES)

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

pkgpyexec_LTLIBRARIES = gtkdatabox.la
gtkdatabox_la_LDFLAGS = -module -avoid-version -export-symbols-regex initgtkdatabox
gtkdatabox_la_LIBADD = $(GTKDATABOX_LIBS) $(GTK_LIBS) $(PYGTK_LIBS)
gtkdatabox_la_SOURCES = gtkdataboxmodule.c gtkdatabox.c gtkdatabox_plot.c util.c 
#nodist_gtkdatabox_la_SOURCES = gtkdatabox.c

CLEANFILES = gtkdatabox.c
EXTRA_DIST = gtkdatabox.override $(defs_DATA)

gtkdatabox.c: gtkdatabox.defs gtkdatabox.override
.defs.c:
	(cd $(srcdir)\
	 && pygtk-codegen-2.0 \
		--register $(PYGTK_DEFSDIR)/gtk-types.defs \
	    --register $(PYGTK_DEFSDIR)/gdk-base-types.defs \
		--override $*.override \
		--prefix py$* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c

update-defs:
	$(PYGTK_H2DEF) /usr/include/gtkdatabox.h > gtkdatabox-new.defs
	


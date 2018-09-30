TARGET:= toolset

SRCDIR:= lib/ src/
OBJDIR:= .objs/
OUTDIR:=
INSTALL_DIR:= /usr/local/bin
HEADERS:=
LIBS:= -lrt
INCLUDE:= -I.

CFLAGS:=
CXXFLAGS:=
LDFLAGS:=

include build.mak

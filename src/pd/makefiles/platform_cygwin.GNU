# platform_linux.GNU,v 4.26 1999/06/21 18:24:31 levine Exp

# According to Bryon G. Rigg <bgrigg@opus.bcbnet.com>, this file
# should allow PDL to be built on Linux.

debug = 1
optimize = 1

CC      = gcc
CXX     = g++
CFLAGS  += -W -Wall -Wpointer-arith -pipe
#CCFLAGS += $(CFLAGS) -fno-implicit-templates -Wno-deprecated -static
CCFLAGS += $(CFLAGS) -fno-implicit-templates -Wno-deprecated
DCFLAGS += -g
CDLD    = $(CC)
DLD     = $(CXX)
LD      = $(CXX)
#system command needed : ln -s /usr/lib/libcygwin.a /usr/lib/libdl.a
LIBS    += -ldl
OCFLAGS += -O3
PIC     = -fPIC
AR      = ar
ARFLAGS = rsuv
RANLIB  = @true
SOFLAGS = $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB  = @true

#### GNU gas has a string limit of 4096 characters.  On Alphas,
#### builds will fail due to running over that limit.  There are
#### at least two workarounds:
#### 1) Change the limit to 8192 characters and rebuild gas.  See
####    PDL-INSTALL.html for more information.
#### 2) Don't use -g when compiling those files.
#### If you're building on an Alpha and you haven't hacked and
#### rebuilt gas, you might need to uncomment the following.
#### ifeq ($(debug),1)
####   SUPPRESS_DASH_G = 1
#### endif # debug

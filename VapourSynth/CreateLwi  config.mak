CC = gcc
LD = gcc


### dll
# STRIP = strip
# CFLAGS = -Os -ffast-math -Wall -std=c99 -pedantic -I. -I. -D__USE_MINGW_ANSI_STDIO=1 -I/usr/local/x86/include -s -O3 -pipe -mtune=native -ffast-math -fexcess-precision=fast -fno-tree-vectorize -fno-strict-aliasing -fexcess-precision=fast -ID:/MSYS/local/x86/include  
# LDFLAGS = -L. -L/usr/local/x86/lib -L${exec_prefix}/lib -static -shared -Wl,--dll,--add-stdcall-alias

### exe  debug
# STRIP = 
# CFLAGS = -g -O0  -ffast-math -Wall -std=c99 -pedantic -I. -I. -D__USE_MINGW_ANSI_STDIO=1 -I/usr/local/x86/include -s -pipe -mtune=native -ffast-math -fexcess-precision=fast -fno-tree-vectorize -fno-strict-aliasing -fexcess-precision=fast -ID:/MSYS/local/x86/include  
# LDFLAGS = -L. -L/usr/local/x86/lib -L${exec_prefix}/lib -m32

### exe  release
 STRIP = strip
 CFLAGS = -s -O2  -ffast-math -Wall -std=c99 -pedantic -I. -I. -D__USE_MINGW_ANSI_STDIO=1 -I/usr/local/x86/include -s -pipe -mtune=native -ffast-math -fexcess-precision=fast -fno-tree-vectorize -fno-strict-aliasing -fexcess-precision=fast -ID:/MSYS/local/x86/include  
 LDFLAGS = -L. -L/usr/local/x86/lib -L${exec_prefix}/lib -m32



LIBS = -lwinmm -LD:/MSYS/local/x86/lib -llsmash -lavformat -lavcodec -lsecur32 -lbz2 -lz -lpsapi -ladvapi32 -lshell32 -lswresample -lswscale -lavutil -lm   
SRCDIR = .
DESTDIR = 
prefix = /usr/local/x86
exec_prefix = ${prefix}
libdir = ${exec_prefix}/lib
vsplugindir = ${libdir}/vapoursynth
###            CreateLwi.c
SRC_SOURCE =   CreateLwi.c    lsmashsource.c video_output.c libavsmash_source.c lwlibav_source.c              ../common/utils.c  ../common/qsv.c ../common/libavsmash.c                       ../common/libavsmash_video.c ../common/lwlibav_dec.c                            ../common/lwlibav_video.c ../common/lwlibav_audio.c                             ../common/lwindex.c ../common/video_output.c ../common/decode.c                 ../common/osdep.c
BASENAME=vslsmashsource
# SONAME=vslsmashsource.dll
SONAME=CreateLwi.exe
SONAME_LN=
SOSUFFIX=dll
SOFLAGS=-Wl,--enable-auto-image-base -Wl,--export-all-symbols

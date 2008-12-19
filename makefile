#######################################################################
#
# Instructions:
#
# make
#   Compiles all .c and .cpp files in the src directory to .o
#   files in the obj directory, and links them into an
#   executable named 'game' or 'game.exe' in the currect directory.
#
# make clean
#   Removes all .o files from the obj directory.
#
# make veryclean
#   Removes all .o files and the game executable.
#
# Optional parameters:
#
# STATICLINK=1
#   Compiles/removes a statically linked version of the game without
#   DLL dependencies. The static object files are put in obj/static
#   and the executable has '_static' appended to the name.
#
# NAME=game_name
#   Sets the name of the game executable. By default the game
#   executable is called 'game' or 'game.exe'.
#
# If you use add-on libraries, add them to the lines starting with
# 'LIBS='. Make sure you enter the libraries in both lines, for the
# normal and static version!
#
#######################################################################

CC = gcc
CXX = g++
LD = g++
# CFLAGS = -Iinclude -O3 -s -W -Wall
CFLAGS = -Iinclude -O3 -g -W -Wall

# Add-on libraries go here
ifdef STATICLINK
	LIBS =
else
	LIBS =
endif


ifndef NAME
	NAME = bedevil
endif

# ifndef WINDOWS
# ifdef MINGDIR
	WINDOWS = 1
# endif
# endif

ifdef WINDOWS
	RM = del /q
	CFLAGS += -D__GTHREAD_HIDE_WIN32API
	LFLAGS = -Wl,--subsystem,windows
	ifdef STATICLINK
		CFLAGS += -DSTATICLINK
		LIBS += -lalleg_s -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -lwinmm -ldsound
		OBJDIR = obj/static
		BIN = $(NAME)_static.exe
	else
		LIBS += -lalleg
		OBJDIR = obj
		BIN = $(NAME).exe
	endif
else
	RM = rm -f
	ifdef STATICLINK
		LIBS += `allegro-config --libs --static` -lXrender
		OBJDIR = obj/static
		BIN = $(NAME)_static
	else
		LIBS += `allegro-config --libs`
		OBJDIR = obj
		BIN = $(NAME)
	endif
endif

OBJ_CPP := $(addprefix $(OBJDIR)/, $(subst src/,,$(patsubst %.cpp,%.o,$(wildcard src/*.cpp))))
OBJ_C := $(addprefix $(OBJDIR)/, $(subst src/,,$(patsubst %.c,%.o,$(wildcard src/*.c))))

all: game

$(OBJDIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

game: $(OBJ_C) $(OBJ_CPP)
	$(LD) -o $(BIN) $(OBJ_C) $(OBJ_CPP) $(LIBS) $(LFLAGS)

clean:
ifdef WINDOWS
ifneq ($(OBJ_C),)
	-$(RM) $(subst /,\,$(OBJ_C))
endif
ifneq ($(OBJ_CPP),)
	-$(RM) $(subst /,\,$(OBJ_CPP))
endif
else
ifneq ($(OBJ_C),)
	-$(RM) $(OBJ_C)
endif
ifneq ($(OBJ_CPP),)
	-$(RM) $(OBJ_CPP)
endif
endif

veryclean: clean
	-$(RM) $(BIN)

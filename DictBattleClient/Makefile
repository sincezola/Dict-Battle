# Makefile for Dict Battle
# Written by:
#   Enzo Zamberlan <sincecontatodev@gmail.com>
# in January 2026

CC            = gcc
WINDRES       = windres
IMAGEMAGICK   = magick
GCC_TARGET    := $(shell $(CC) -dumpmachine)

ifeq ($(findstring mingw32, $(GCC_TARGET)), mingw32)
  _WIN32 = 1
endif

SRC_PATH      = src
INCLUDE_PATH  = include
OBJ_DIR       = obj
BIN_DIR       = bin
LOG_DIR       = log
INC_DIR       = -I. -I$(INCLUDE_PATH)
LIBS          = 

CCOPT         = -Wall -Wextra
ifdef _WIN32
  CCOPT += -D_WIN32
  LIBS  += -lws2_32
  LIBS  += -lwinmm
endif

ifdef LINUX
  CCOPT += -Wl,-rpath,/usr/lib64 -Wl,--enable-new-dtags -DLINUX
  LIBS  += -Wl,-rpath,/usr/lib64 -Wl,--enable-new-dtags -DLINUX
endif

DEBUG_ADD_FLAGS = -O2
ifdef DEBUG
  DEBUG_ADD_FLAGS = -g -ggdb
endif

DICT_EXEC = dictbattle

OBJS = \
	$(OBJ_DIR)/dict_battle.o \
	$(OBJ_DIR)/trace.o \
	$(OBJ_DIR)/sys_interface.o \
	$(OBJ_DIR)/terminal_utils.o \
	$(OBJ_DIR)/multiplayer.o \
	$(OBJ_DIR)/sockets.o \
	$(OBJ_DIR)/player.o \
	$(OBJ_DIR)/utils.o

all: clean directories $(BIN_DIR)/$(DICT_EXEC)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LOG_DIR)

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) $(LOG_DIR)

$(BIN_DIR)/$(DICT_EXEC): directories $(OBJS)
	$(CC) $(LDOPT) $(INC_DIR) -o $@ $(OBJS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_PATH)/%.c
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INC_DIR) $< -o $@
	
$(OBJ_DIR)/%.o: $(API_PATH)/%.c
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INC_DIR) $< -o $@

distclean: clean

.PHONY: all clean distclean directories

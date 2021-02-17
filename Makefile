ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
WIN_FOLDER=C:

OBJFILES = main
TARGET = multiplayer

ifeq ($(OS),Windows_NT)
	OBJFILES := $(addsuffix .c, $(OBJFILES)) WindowsClient.c
	PATH_ALLEGRO=$(WIN_FOLDER)\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
	ALLEGRO_INCLUDE = $(PATH_ALLEGRO)\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a -I $(PATH_ALLEGRO)\include
	WINDOWS_FLAGS = -lws2_32
	COMMAND = gcc $(addprefix src/, $(OBJFILES)) $(ALLEGRO_INCLUDE) -o $(TARGET).exe $(WINDOWS_FLAGS)
else
	OBJFILES := $(addsuffix .o, $(OBJFILES)) LinuxClient.o
	LINUX_FLAGS=-Wall -lallegro -lallegro_image -lallegro_primitives -lm
	COMMAND=gcc $(LINUX_FLAGS) -o $(TARGET) $(addprefix src/, $(OBJFILES))
endif

all: $(TARGET)

$(TARGET): $(addprefix src/, $(OBJFILES))
	$(COMMAND)

run:	$(TARGET)
	./$(TARGET)

clear: 
	rm -f  $(addprefix src/, $(OBJFILES)) $(TARGET) *~
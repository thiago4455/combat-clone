FLAGS = -Wall -lallegro -lallegro_image -lallegro_primitives -lm
OBJFILES = main.o Client.o
TARGET = multiplayer

all: $(TARGET)

$(TARGET): $(addprefix src/, $(OBJFILES))
	gcc $(FLAGS) -o $(TARGET) $(addprefix src/, $(OBJFILES))

run:	$(TARGET)
	./$(TARGET)
	rm -f $(OBJFILES)

clear: 
	rm -f  $(addprefix src/, $(OBJFILES)) $(TARGET) *~
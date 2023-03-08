OBJ = src/main.o

all: app

%.o: %.cpp
	g++ -c $< -o $@

app: $(OBJ)
	g++ $(OBJ) -o app -lsfml-graphics -lsfml-window -lsfml-system
	./app

clean:
	rm -f src/*.o%  
CPPFLAGS = -I/usr/include/irrlicht
CXXFLAGS = -Wall -Wextra -O2 -g -std=c++11 -Wno-unused-parameter

exec: main.o events.o player.o enemy.o camera.o
	g++ $^ -o $@ -lIrrlicht

main.o events.o player.o: events.hpp player.hpp enemy.hpp camera.hpp

clean:
	@rm -f exec *.o

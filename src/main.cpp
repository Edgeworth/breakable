#include "common.hpp"
#include "init.hpp"
#include "events.hpp"
#include "draw.hpp"
#include "breakable.hpp"

int main(int argc, char *argv[]) {
	win.Create(sf::VideoMode(800, 800), "Test");
	viewW = viewH = 25;

	init();
	initBreakable();
	Breakable::create("test.poly");
	while (win.IsOpened()) {
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		handleEvents();

		world.Step(1.0/60.0, 10, 10);
		breakApart();

		Breakable::draw();
		draw();
		win.Display();
	}

	deinitBreakable();
}

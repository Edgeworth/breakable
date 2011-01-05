#ifndef BREAKABLE_HPP_
#define BREAKABLE_HPP_
#include <utility>
#include "common.hpp"

class Breakable {
public:
	~Breakable();

	static Breakable* create(gpc_polygon* p);
	static Breakable* create(const std::string& filename);
	static void draw();

	b2Body *b;
	gpc_polygon poly;
	gpc_tristrip strip;

private:
	Breakable(gpc_polygon* p);
	Breakable(const std::string& filename);
	void init();
	void _draw();
};

void initBreakable();
void breakApart();
void deinitBreakable();

#endif

#ifndef COMMON_HPP_
#define COMMON_HPP_
#include "SFML/Graphics.hpp"
#include "Box2D/Box2D.h"
#include "gpc.hpp"

#define _V(v) ((v).x), ((v).y)

extern float viewW, viewH;
extern b2Vec2 centre;
extern b2World world;
extern sf::RenderWindow win;

void randPoly(float r, gpc_polygon* p,
							b2Vec2 centre = b2Vec2(0, 0),
							int n = 16, float d = 0.2);

void transform(gpc_polygon* poly, gpc_polygon* res, const b2Transform& tr);
void gpcCopy(gpc_polygon* src, gpc_polygon* dest);
void setView();

inline float area(b2Vec2 A, b2Vec2 B, b2Vec2 C) {
	b2Vec2 AB = B-A, AC = C-A;
	return (AB.x*AC.y - AC.x*AB.y)/2.0;
}

inline bool isCCW(b2Vec2 A, b2Vec2 B, b2Vec2 C) {
	float a = area(A, B, C);
	return a > 0;
}

inline float fRand(float min, float max) {
	return float(rand())/RAND_MAX * (max-min) + min;
}

inline b2Vec2 g2b(const gpc_vertex& g) {
	return b2Vec2(g.x, g.y);
}

inline b2Vec2 stb(b2Vec2 scr) {
	return b2Vec2((scr.x/win.GetWidth()-0.5)*viewW,
							-(scr.y/win.GetHeight()-0.5)*viewH)-centre;
}

inline b2Vec2 bts(b2Vec2 b2) {
	return b2Vec2((b2.x/viewW+0.5)*win.GetWidth(),
							-(b2.y/viewH+0.5)*win.GetHeight())+centre;
}

inline b2Vec2 stb(float scrX, float scrY) {
	return stb(b2Vec2(scrX, scrY));
}

inline b2Vec2 bts(float b2X, float b2Y) {
	return bts(b2Vec2(b2X, b2Y));
}

#endif

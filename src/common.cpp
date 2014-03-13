#include <cstring>
#include "common.hpp"

float viewW, viewH;
b2Vec2 centre(0.0, 0.0);
b2World world(b2Vec2(0.0, -9.8), true);
sf::RenderWindow win;

void setView() {
	glViewport(0, 0, win.GetWidth(), win.GetHeight());
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-viewW/2.0, viewW/2.0, -viewH/2.0, viewH/2.0, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void randPoly(float r, gpc_polygon* p, b2Vec2 centre, int n, float d) {
	gpc_vertex_list contour = {n, new gpc_vertex[n]};
	for (int i = 0; i < n; ++i) {
		float a = i/float(n)*2*b2_pi;
		contour.vertex[i].x = cos(a)*r+fRand(-d, d)+centre.x;
		contour.vertex[i].y = sin(a)*r+fRand(-d, d)+centre.y;
	}
	gpc_add_contour(p, &contour, false);
	delete[] contour.vertex;
}

void transform(gpc_polygon* poly, gpc_polygon* res, const b2Transform& tr) {
	gpcCopy(poly, res);
	for (int i = 0; i < poly->num_contours; ++i) {
		for (int j = 0; j < poly->contour[i].num_vertices; ++j) {
			b2Vec2 v(_V(poly->contour[i].vertex[j]));
			v = b2Mul(tr.R, v) + tr.position;
			res->contour[i].vertex[j].x = v.x;
			res->contour[i].vertex[j].y = v.y;
		}
	}
}

void gpcCopy(gpc_polygon* src, gpc_polygon* dest) {
	memset(dest, 0, sizeof(gpc_polygon));
	for (int i = 0; i < src->num_contours; ++i)
		gpc_add_contour(dest, &src->contour[i], src->hole[i]);
}

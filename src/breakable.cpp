#include <set>
#include <queue>
#include <algorithm>
#include "breakable.hpp"
#include "common.hpp"
#include "mouseJoint.hpp"
#include "b2Polygon.h"

struct BreakInfo {
	BreakInfo(Breakable* _b, b2Vec2 _p, b2Vec2 _n) : b(_b), p(_p), n(_n) {}
	Breakable* b;
	b2Vec2 p, n;
};

typedef std::set<Breakable*> BSet;
typedef BSet::iterator BSetItr;
BSet objs;
std::queue<BreakInfo> breaks;

class Listener : public b2ContactListener {
public:
	void PostSolve(b2Contact* c, const b2ContactImpulse *i) {
		if (i->normalImpulses[0] < 100) return;

		b2WorldManifold mani;
		c->GetWorldManifold(&mani);

		b2Body* a = c->GetFixtureA()->GetBody(), *b = c->GetFixtureB()->GetBody();

		if (a->GetUserData() != NULL && a->GetMass() > 1.0)
			breaks.push(BreakInfo((Breakable*)a->GetUserData(), mani.points[0], -mani.normal));
		if (b->GetUserData() != NULL && a->GetMass() > 1.0)
			breaks.push(BreakInfo((Breakable*)b->GetUserData(), mani.points[0], mani.normal));
	}
} listener;

void render(gpc_polygon* poly, float r, float g, float b) {
	glLineWidth(4);
	glColor3f(r, g, b);
	for (int i = 0; i < poly->num_contours; ++i) {
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < poly->contour[i].num_vertices; ++j)
			glVertex2f(_V(poly->contour[i].vertex[j]));
		glEnd();
	}
	glLineWidth(1);
}

void makeBodies(gpc_polygon* poly, b2Vec2 v) {
	for (int i = 0; i < poly->num_contours; ++i) {
		if (poly->hole[i]) continue;

		gpc_polygon sub = {};
		gpc_add_contour(&sub, &poly->contour[i], false);
		Breakable* b = Breakable::create(&sub);
		b->b->SetLinearVelocity(v);
		gpc_free_polygon(&sub);
	}
}

void breakApart() {
	while (!breaks.empty()) {
		Breakable* b = breaks.front().b;
		b2Vec2 p = breaks.front().p, n = breaks.front().n;
		breaks.pop();
		if (!objs.count(b)) {
			printf("Ignoring one break.\n");
			continue;
		}

		gpc_polygon xformed = {}, cutout = {}, inter = {}, diff = {};
		randPoly(2, &cutout, p);
		transform(&b->poly, &xformed, b->b->GetTransform());

		gpc_polygon_clip(GPC_INT, &xformed, &cutout, &inter);
		gpc_polygon_clip(GPC_DIFF, &xformed, &cutout, &diff);

		b2Vec2 vel = b->b->GetLinearVelocity();
		delete b;
		makeBodies(&inter, 4*n+vel);
		makeBodies(&diff, vel);

		gpc_free_polygon(&xformed);
		gpc_free_polygon(&cutout);
		gpc_free_polygon(&inter);
		gpc_free_polygon(&diff);
	}
}

void initBreakable() {
	world.SetContactListener(&listener);
}

void deinitBreakable() {
	for (BSetItr i = objs.begin(); i != objs.end(); ++i)
		delete *i;
}

void Breakable::draw() {
	for (BSetItr i = objs.begin(); i != objs.end(); ++i)
		(**i)._draw();
}


Breakable* Breakable::create(gpc_polygon* p) {
	Breakable* breakable = new Breakable(p);
	objs.insert(breakable);
	return breakable;
}

Breakable* Breakable::create(const std::string& filename) {
	Breakable* breakable = new Breakable(filename);
	objs.insert(breakable);
	return breakable;
}

Breakable::Breakable(gpc_polygon* p) {
	gpcCopy(p, &poly);
	gpc_polygon_to_tristrip(&poly, &strip);
	init();
}

Breakable::Breakable(const std::string& filename) {
	FILE* poly_file = fopen(filename.c_str(), "r");
	gpc_read_polygon(poly_file, true, &poly);
	gpc_polygon_to_tristrip(&poly, &strip);
	init();
}

Breakable::~Breakable() {
	gpc_free_polygon(&poly);
	gpc_free_tristrip(&strip);
	world.DestroyBody(b);
	objs.erase(this);
	if (mjBody && mjBody->GetUserData() == this) mj = NULL;
}

void Breakable::init() {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;
	b = world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	b2FixtureDef bodyFixtureDef;
	bodyFixtureDef.shape = &shape;
	bodyFixtureDef.friction = 0.3;
	bodyFixtureDef.restitution = 0.4;
	bodyFixtureDef.density = 1.0;

	for (int i = 0; i < strip.num_strips; ++i) {
		gpc_vertex* v = strip.strip[i].vertex;
		/*for (int j = 0; j < strip.strip[i].num_vertices-2; ++j) {
			b2Vec2 tri[3] = {g2b(v[j]), g2b(v[j+1]), g2b(v[j+2])};
			if (!isCCW(tri[0], tri[1], tri[2]))
				std::swap(tri[0], tri[2]);

			shape.Set(tri, 3);
			b2MassData mass;
			shape.ComputeMass(&mass, bodyFixtureDef.density);
			if (area(tri[0], tri[1], tri[2]) <= 0.0001 ||
				mass.I - mass.mass*b2Dot(mass.center, mass.center) <= 0.0) {
				printf("Rejecting triangle, would fail assert.\n");
				continue;
			}

			b2Fixture* f = b->CreateFixture(&bodyFixtureDef);
		}*/
		b2Triangle* tris = new b2Triangle[strip.strip[i].num_vertices-2];
		b2Polygon* polys = new b2Polygon[strip.strip[i].num_vertices-2];

		for (int j = 0; j < strip.strip[i].num_vertices-2; ++j) {
			b2Vec2 tri[3] = {g2b(v[j]), g2b(v[j+1]), g2b(v[j+2])};
			if (!isCCW(tri[0], tri[1], tri[2]))
				std::swap(tri[0], tri[2]);
			for (int k = 0; k < 3; ++k) {
				tris[j].x[k] = tri[k].x;
				tris[j].y[k] = tri[k].y;
			}
		}

		int amount = PolygonizeTriangles(tris, strip.strip[i].num_vertices-2, polys, strip.strip[i].num_vertices-2);

		for (int j = 0; j < amount; ++j) {
			if (!polys[j].IsUsable()) continue;
			b2Vec2* poly = polys[j].GetVertexVecs();
			shape.Set(poly, polys[j].nVertices);
			delete[] poly;

			b2MassData mass;
			shape.ComputeMass(&mass, bodyFixtureDef.density);
			if (mass.I - mass.mass*b2Dot(mass.center, mass.center) <= 0.0) {
				printf("Rejecting shape, would fail assert.\n");
				continue;
			}

			b2Fixture* f = b->CreateFixture(&bodyFixtureDef);
		}

		delete[] tris;
		delete[] polys;
	}
}

void Breakable::_draw() {
	glPushMatrix();
		glTranslatef(_V(b->GetPosition()), 0.0);
		glRotatef(b->GetAngle()/b2_pi*180.0, 0.0, 0.0, 1.0);
		glColor3f(0.6, 0.0, 0.0);
		for (int i = 0; i < strip.num_strips; ++i) {
			glVertexPointer(2, GL_DOUBLE, sizeof(gpc_vertex), strip.strip[i].vertex);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, strip.strip[i].num_vertices);
		}

		glColor3f(0.0, 0.0, 0.0);
		for (int i = 0; i < poly.num_contours; ++i) {
			glVertexPointer(2, GL_DOUBLE, sizeof(gpc_vertex), poly.contour[i].vertex);
			glDrawArrays(GL_LINE_LOOP, 0, poly.contour[i].num_vertices);
		}
	glPopMatrix();
}

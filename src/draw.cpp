#include <cmath>
#include <stdexcept>
#include "draw.hpp"

namespace {

const int CIRCLE_DIV = 16;

void drawLine(b2Vec2 a, b2Vec2 b) {
	glBegin(GL_LINES);
		glVertex2f(_V(a));
		glVertex2f(_V(b));
	glEnd();
}

void drawLineLoop(b2Vec2* v, int n) {
	glBegin(GL_LINE_LOOP);
		for (int i = 0; i < n; ++i)
			glVertex2f(_V(v[i]));
	glEnd();
}

void drawPolygon(b2Vec2* v, int n, bool outline = true) {
	glBegin(GL_POLYGON);
		for (int i = 0; i < n; ++i)
			glVertex2f(_V(v[i]));
	glEnd();
	if (!outline) return;

	glColor3f(0, 0, 0);
	drawLineLoop(v, n);
}

void drawCircle(float r) {
	b2Vec2 v[CIRCLE_DIV];
	for (int i = 0; i < CIRCLE_DIV; ++i) {
		float angle = (float(i)/CIRCLE_DIV)*2*b2_pi;
		v[i] = b2Vec2(cos(angle)*r, sin(angle)*r);
	}
	drawPolygon(v, CIRCLE_DIV, true);
}

}

void draw() {
	for (b2Body* body = world.GetBodyList(); body; body = body->GetNext()) {
		if (body->GetUserData() != NULL) continue;
		glPushMatrix();
		glTranslatef(_V(body->GetPosition()), 0.0);
		glRotatef(body->GetAngle()/b2_pi*180.0, 0.0, 0.0, 1.0);

		for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
			glColor3f(1.0, 0.6, 0.6);
			if (body->GetType() == b2_staticBody) glColor3f(0.6, 1.0, 0.6);
			if (body->GetType() == b2_kinematicBody) glColor3f(0.6, 0.6, 1.0);

			glPushMatrix();
				if (fixture->GetType() == b2Shape::e_polygon) {
					b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
					drawPolygon(poly->m_vertices, poly->m_vertexCount);
				}
				else if (fixture->GetType() == b2Shape::e_circle) {
					b2CircleShape* circle = (b2CircleShape*)fixture->GetShape();
					glTranslatef(_V(circle->m_p), 0.0);
					drawCircle(circle->m_radius);
				}
				else throw std::runtime_error("Unknown shape type.");
			glPopMatrix();
		}
		glPopMatrix();
	}
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(6);
	for (b2Joint* joint = world.GetJointList(); joint; joint = joint->GetNext()) {
		glBegin(GL_POINTS);
			glVertex2f(_V(joint->GetAnchorA()));
			glVertex2f(_V(joint->GetAnchorB()));
		glEnd();
		drawLine(joint->GetAnchorA(), joint->GetAnchorB());
	}
}

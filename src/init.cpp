#include "init.hpp"

void init() {
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0, -10.0);
	b2Body* ground = world.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50, 1, b2Vec2(0, 50), 0);
	ground->CreateFixture(&groundBox, 0.0);
	groundBox.SetAsBox(50, 1, b2Vec2(0, -50), 0);
	ground->CreateFixture(&groundBox, 0.0);
	groundBox.SetAsBox(1, 50, b2Vec2(50, 0), 0);
	ground->CreateFixture(&groundBox, 0.0);
	groundBox.SetAsBox(1, 50, b2Vec2(-50, 0), 0);
	ground->CreateFixture(&groundBox, 0.0);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;

	b2PolygonShape bodyBox;
	b2FixtureDef bodyFixtureDef;
	bodyFixtureDef.shape = &bodyBox;
	bodyFixtureDef.friction = 0.3;
	bodyFixtureDef.restitution = 0.4;
	bodyFixtureDef.density = 1.0;

	bodyDef.position.Set(-6, 0);
	b2Body* body0 = world.CreateBody(&bodyDef);
	bodyDef.position.Set(6, 0);
	b2Body* body1 = world.CreateBody(&bodyDef);
	bodyBox.SetAsBox(5, 2);
	body0->CreateFixture(&bodyFixtureDef);
	body1->CreateFixture(&bodyFixtureDef);

	srand(time(NULL));
	win.UseVerticalSync(true);
	setView();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnableClientState(GL_VERTEX_ARRAY);
	glClearColor(1, 1, 1, 1);
}

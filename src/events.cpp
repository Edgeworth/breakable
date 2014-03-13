#include "events.hpp"
#include "mouseJoint.hpp"

void fire(b2Vec2 pos) {
	b2Body* b;
	b2BodyDef def;
	def.type = b2_dynamicBody;
	def.position = pos;
	def.linearVelocity = b2Vec2(50, 0);
	def.bullet = true;
	b = world.CreateBody(&def);

	b2FixtureDef fdef;
	b2CircleShape cir;
	cir.m_radius = 0.2;

	fdef.shape = &cir;
	fdef.friction = 0.1;
	fdef.restitution = 0.4;
	fdef.density = 50.0;
	fdef.userData = def.userData;
	b->CreateFixture(&fdef);
}

void handleEvents() {
	sf::Event e;
	while (win.GetEvent(e)) {
		switch (e.Type) {
			case sf::Event::Closed:
				win.Close();
				break;

			case sf::Event::MouseWheelMoved: {
				float zoom = (e.MouseWheel.Delta < 0 ? 1.2 : 0.8);
				viewH *= zoom; viewW *= zoom;
				setView();
				break;
			}

			case sf::Event::MouseMoved:
				updateMJ(stb(e.MouseMove.X, e.MouseMove.Y));
				break;

			case sf::Event::MouseButtonPressed:
				if (e.MouseButton.Button == sf::Mouse::Left)
					createMJ(stb(e.MouseButton.X, e.MouseButton.Y));
				else fire(stb(e.MouseButton.X, e.MouseButton.Y));
				break;

			case sf::Event::MouseButtonReleased:
				if (e.MouseButton.Button == sf::Mouse::Left)
					destroyMJ();
				break;
		}
	}
	const sf::Input& inp = win.GetInput();
	if (inp.IsKeyDown(sf::Key::W)) centre += b2Vec2(0.0, -0.01*viewH);
	if (inp.IsKeyDown(sf::Key::S)) centre += b2Vec2(0.0, 0.01*viewH);
	if (inp.IsKeyDown(sf::Key::A)) centre += b2Vec2(0.01*viewW, 0.0);
	if (inp.IsKeyDown(sf::Key::D)) centre += b2Vec2(-0.01*viewW, 0.0);
	glTranslatef(_V(centre), -6.0);
}

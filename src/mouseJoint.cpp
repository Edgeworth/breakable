#include "mouseJoint.hpp"

b2MouseJoint* mj = NULL;
b2Body* mjBody = NULL;

class Query : public b2QueryCallback {
public:
	Query(b2Vec2 point) : body(NULL), p(point) {}
	virtual bool ReportFixture(b2Fixture* fixture) {
		if (fixture->GetBody()->GetType() != b2_dynamicBody) return true;
		if (!fixture->TestPoint(p)) return true;
		body = fixture->GetBody();
		return false;
	}

	b2Body* body;
	b2Vec2 p;
};

void createMJ(b2Vec2 pos) {
	static b2BodyDef unnecessaryDef;
	static b2Body* unnecessary = world.CreateBody(&unnecessaryDef);

	b2AABB aabb;
	aabb.lowerBound = pos-b2Vec2(0.01, 0.01);
	aabb.upperBound = pos+b2Vec2(0.01, 0.01);
	Query query(pos);
	world.QueryAABB(&query, aabb);

	if (query.body == NULL || mj) return;

	b2MouseJointDef mjDef;
	mjDef.bodyA = unnecessary;
	mjDef.bodyB = mjBody = query.body;
	mjDef.target = pos;
	mjDef.maxForce = 10000.0*query.body->GetMass();
	mj = (b2MouseJoint*)world.CreateJoint(&mjDef);
}

void updateMJ(b2Vec2 pos) {if (mj) mj->SetTarget(pos);}

void destroyMJ() {
	if (mj == NULL) return;
	world.DestroyJoint(mj);
	mj = NULL;
	mjBody = NULL;
}

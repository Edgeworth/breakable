#ifndef MOUSE_JOINT_HPP_
#define MOUSE_JOINT_HPP_
#include "common.hpp"

extern b2MouseJoint* mj;
extern b2Body* mjBody;

void createMJ(b2Vec2 pos);
void updateMJ(b2Vec2 pos);
void destroyMJ();


#endif

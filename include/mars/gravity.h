#pragma once
#include "al/LiveActor/LiveActor.h"
#include "al/area/AreaObj.h"
#include "sead/math/seadVector.h"
#include <sead/gfx/seadCamera.h>

namespace mars {

sead::Vector3f calcGravityDirection(sead::Vector3f* trans, al::AreaObj *areaObj);
void calcControlGravityDir(al::LiveActor* player, al::LiveActor* cap);
void calcActorGravity(al::LiveActor* actor);
};
#pragma once

#include <sead/math/seadQuat.h>
#include <sead/math/seadVector.h>
#include "al/LiveActor/LiveActor.h"

namespace al {

bool isNearZero(const sead::Vector2f&, float);
void verticalizeVec(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
bool tryNormalizeOrZero(sead::Vector3f*);
void normalize(sead::Vector3f*);
void turnVecToVecDegree(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, float);
void turnVecToVecRate(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, float);

void calcQuatFront(sead::Vector3f*, const sead::Quatf&);
void makeQuatFrontUp(sead::Quatf *, const sead::Vector3f &,
                     const sead::Vector3f &);

float calcAngleToTargetH(al::LiveActor const*,sead::Vector3<float> const&);
float calcAngleToTargetV(al::LiveActor const*,sead::Vector3<float> const&);
float calcAngleRadian(sead::Vector3<float> const&,sead::Vector3<float> const&);
float calcAngleDegree(sead::Vector3<float> const&,sead::Vector3<float> const&);
float calcAngleDegree(sead::Vector2<float> const&,sead::Vector2<float> const&);
bool tryCalcAngleDegree(float *,sead::Vector3<float> const&,sead::Vector3<float> const&);
float calcAngleOnPlaneRadian(sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&);
float calcAngleOnPlaneDegree(sead::Vector3f const&, sead::Vector3f const&, sead::Vector3f const&);
float calcAngleOnPlaneDegreeOrZero(sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&);
bool tryCalcAngleOnPlaneDegree(float *,sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&);
float calcAngleSignOnPlane(sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&);

}  // namespace al

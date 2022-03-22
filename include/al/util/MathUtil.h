#pragma once

#include <math/seadVector.h>
#include "math/seadQuat.h"

namespace al {
float normalize(float, float, float);
float normalize(signed int, signed int, signed int);

float easeIn(float);
float easeOut(float);
float easeInOut(float);

float modf(float, float);
int modi(int, int);

float sign(float);
inline int sign(int);

float squareIn(float);
float squareOut(float);

float powerIn(float, float);
float powerOut(float, float);

float lerpValue(float, float, float);

bool isNearZero(float, float);

bool isNearZero(sead::Vector3f const &, float);

template <typename T>
inline T clamp(T value, T min, T max) {
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

float calcSpringDumperForce(float unk1, float unk2, float unk3, float unk4);

void separateVectorHV(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f &,
                      const sead::Vector3f &);

void slerpQuat(sead::Quatf *,sead::Quatf const&,sead::Quatf const&,float);

bool tryNormalizeOrDirZ(sead::Vector3f *);
bool tryNormalizeOrDirZ(sead::Vector3f *, sead::Vector3f const&);
bool tryNormalizeOrZero(sead::Vector3f*, sead::Vector3f const&);
void rotateVectorDegreeX(sead::Vector3f*, float);
void rotateVectorDegreeY(sead::Vector3f*, float);
void rotateVectorDegreeZ(sead::Vector3f*, float);

};  // namespace al
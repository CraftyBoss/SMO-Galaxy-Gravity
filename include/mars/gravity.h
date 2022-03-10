#pragma once
#include "al/LiveActor/LiveActor.h"
#include <sead/gfx/seadCamera.h>

namespace mars {
    void calcGravityDirection(al::LiveActor*);
    sead::Matrix33f calcInverseMtx(const sead::Matrix33f&);
    sead::Vector3f calcMtxRot(sead::Matrix33f, sead::Vector3f *);
    sead::Matrix34f* calcRotMtx(sead::Matrix34f*, sead::Matrix33f);
    sead::Matrix33f QuatToMtx33(sead::Quatf*);
    void applyRotMatrix();
    void calcMatrixHook(sead::LookAtCamera*);
    bool isEnableVerticalAbsorber();
    float verticalAngleCtrl(float);
};
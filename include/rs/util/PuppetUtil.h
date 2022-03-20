#pragma once

#include "al/sensor/HitSensor.h"
#include "al/sensor/SensorMsg.h"
#include "game/Interfaces/IUsePlayerPuppet.h"

namespace rs {
IUsePlayerPuppet* startPuppet(al::HitSensor* target, al::HitSensor* source);
void startPuppetAction(IUsePlayerPuppet*, char const*);

void setPuppetVelocity(IUsePlayerPuppet*, sead::Vector3f const&);
sead::Vector3f* getPuppetTrans(IUsePlayerPuppet const*);

void setPuppetTrans(IUsePlayerPuppet*, sead::Vector3f const&);

void setPuppetQuat(IUsePlayerPuppet*, sead::Quatf const&);

void setPuppetFront(IUsePlayerPuppet*, sead::Vector3f const&);

void setPuppetAnimBlendWeight(IUsePlayerPuppet *,float,float,float,float,float,float);
void setPuppetAnimRate(IUsePlayerPuppet*, float);
void calcPuppetUp(sead::Vector3f *,IUsePlayerPuppet const*);
void setPuppetUp(IUsePlayerPuppet *,sead::Vector3f const&);

bool tryReceiveBindCancelMsgAndPuppetNull(IUsePlayerPuppet**, al::SensorMsg const*);
void endBindAndPuppetNull(IUsePlayerPuppet **);

void forcePutOnPuppetCap(IUsePlayerPuppet *);
void forcePutOnPuppetCapWithEffect(IUsePlayerPuppet *);

}
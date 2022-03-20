#pragma once

#include "al/sensor/HitSensor.h"
#include "prim/seadSafeString.h"

class IUsePlayerPuppet {
    public:
    virtual void start(al::HitSensor *,al::HitSensor *);
    virtual void end(void);
    virtual void cancel(void);
    virtual void setTrans(sead::Vector3<float> const&);
    virtual void setPose(sead::Quat<float> const&);
    virtual void setVelocity(sead::Vector3<float> const&);
    virtual void resetPosition(sead::Vector3<float> const&);
    virtual void getTrans(void);
    virtual void getVelocity(void);
    virtual void getGravity(void);
    virtual void calcFront(sead::Vector3<float> *);
    virtual void calcUp(sead::Vector3<float> *);
    virtual void startAction(sead::SafeString const&);
    virtual void isActionEnd(void);
    virtual void isActionPlaying(char const*);
    virtual void setAnimRate(float);
    virtual void getAnimFrameMax(void);
    virtual void hide(void);
    virtual void show(void);
    virtual void isHidden(void);
    virtual void hideSilhouette(void);
    virtual void showSilhouette(void);
    virtual void hideShadow(void);
    virtual void showShadow(void);
    virtual void requestDamage(void);
    virtual void clearRequestDamage(void);
    virtual void isRequestDamage(void);
    virtual void setBindEndOnGround(void);
    virtual void isBindEndOnGround(void);
    virtual void setBindEndJump(sead::Vector3<float> const&,int);
    virtual void isBindEndJump(void);
    virtual void validateCollisionCheck(void);
    virtual void invalidateCollisionCheck(void);
    virtual void isValidCollisionCheck(void);
    virtual void isCollidedGround(void);
    virtual void getCollidedGroundNormal(void);
    virtual void validateSensor(void);
    virtual void invalidateSensor(void);
};
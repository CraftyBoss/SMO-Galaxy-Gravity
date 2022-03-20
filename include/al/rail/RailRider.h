#pragma once

#include "math/seadVector.h"
namespace al {

class Rail;

class RailRider {
public:
    RailRider(al::Rail const*);
    void moveToRailStart(void);
    void move(void);
    void syncPosDir(void);
    void setCoord(float);
    void moveToRailPoint(int);
    void moveToRailEnd(void);
    void moveToBegin(void);
    void moveToGoal(void);
    void moveToNearestRail(sead::Vector3f const&);
    void reverse(void);
    void setMoveGoingStart(void);
    void setMoveGoingEnd(void);
    void setSpeed(float);
    void addSpeed(float);
    void scaleSpeed(float);

    bool isReachedGoal(void) const;
    bool isReachedRailEnd(void) const;
    bool isReachedRailStart(void) const;
    bool isReachedEdge(void) const;

    al::Rail *mRail; // 0x0
    sead::Vector3f mCurRailPos = sead::Vector3f::zero; // 0x8
    sead::Vector3f mCurRailDir = sead::Vector3f::zero; // 0x14
    float mRailLength; // 0x20
    bool mIsLoop = true;
    };

    class IUseRail
    {
    public:
        virtual al::RailRider* getRailRider() const = 0;
    };
};
#pragma once

#include "al/LiveActor/LiveActor.h"

class CapMessageEnableChecker {
public:
    CapMessageEnableChecker(al::LiveActor *,al::ActorInitInfo const&);
    void calcAimPos(sead::Vector3f *);
    void updateCheckEnableAppear(int);
    bool isClippedAimPoint(void) const;
    void checkInRadius(sead::Vector3f const&);
    void execute(void);

};
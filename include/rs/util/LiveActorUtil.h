#pragma once

#include "al/LiveActor/LiveActor.h"
#include "game/Player/HackCap/CapMessageEnableChecker.h"
#include "game/Player/HackCap/CapTargetInfo.h"

namespace rs
{
    void initItemByPlacementInfo(al::LiveActor *, al::ActorInitInfo const &, bool);
    float setShadowDropLength(al::LiveActor*, al::ActorInitInfo const&, char const*);
    CapTargetInfo* createCapTargetInfo(al::LiveActor*, char const*);
    void createCapMessageEnableChecker(CapMessageEnableChecker **,al::LiveActor *,al::ActorInitInfo const&);

} // namespace rs

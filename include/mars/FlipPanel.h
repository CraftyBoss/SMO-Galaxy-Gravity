#pragma once

#include "al/LiveActor/LiveActor.h"
#include "al/util/LiveActorUtil.h"
#include "al/util/NerveUtil.h"
#include <sead/container/seadPtrArray.h>

namespace mars {
    class FlipPanel : public al::LiveActor {
    public:
        FlipPanel(const char* name);
        void init(const al::ActorInitInfo&);
        void initAfterPlacement();
        bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source, al::HitSensor* target);

        void exeWait();
        void exePress();
        void exeEnd();
    private:
        bool isOn = false;

    
    };
    namespace {
        NERVE_HEADER(FlipPanel, Wait)
        NERVE_HEADER(FlipPanel, Flip)
    }

    class FlipPanelObserver : public al::LiveActor {
    public:
        FlipPanelObserver(const char* name);
        void init(const al::ActorInitInfo&);
        void initAfterPlacement();
        bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source, al::HitSensor* target);

    private:
        sead::PtrArray<FlipPanel> mPanels;
        bool isAllOn = false;

    };
}
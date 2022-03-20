#include "mars/FlipPanel.h"

namespace mars {
    FlipPanel::FlipPanel(const char* name) : al::LiveActor(name) {}

    void FlipPanel::init(const al::ActorInitInfo& info) {
        al::initMapPartsActor(this, info, nullptr);
        al::initNerve(this, &nrvFlipPanelWait, 0);
        makeActorAlive();
    }

    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source, al::HitSensor* target) {
        
    }

    namespace {
        NERVE_IMPL(FlipPanel, Wait)
        NERVE_IMPL(FlipPanel, Flip)
    }
}
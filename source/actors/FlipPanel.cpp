#include "actors/FlipPanel.h"
#include "logger.hpp"


FlipPanel::FlipPanel(const char* name) : al::LiveActor(name) {}

void FlipPanel::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "FlipPanel", 0);
    al::initNerve(this, &nrvFlipPanelWait, 0);
    al::startAction(this, "OffWait");
    al::hideMaterial(this, "FlipPanelEx");
    al::hideMaterial(this, "FlipPanelDone");
    makeActorAlive();
}

bool FlipPanel::receiveMsg(const al::SensorMsg* message, al::HitSensor* source, al::HitSensor* target) {
    if ((al::isMsgPlayerFloorTouch(message) || rs::isMsgCapHipDrop(message)) && !al::isNerve(this, &nrvFlipPanelEnd)) {
        mPressTimer = 7;
        if (!al::isNerve(this, &nrvFlipPanelPressed)) {
            al::startAction(this, "OnWait");
            al::setNerve(this, &nrvFlipPanelPressed);
        }
        return true;
    } 
    
    return false;
}

void FlipPanel::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "OffWait");
    }
}

void FlipPanel::exePressed() {
    if (al::isFirstStep(this)) {
        if (isOn) {
            isOn = !isOn;
            al::showMaterial(this, "FlipPanelQ");
            al::hideMaterial(this, "FlipPanelEx");
        } else {
            isOn = !isOn;
            al::hideMaterial(this, "FlipPanelQ");
            al::showMaterial(this, "FlipPanelEx");
        }
        al::startAction(this, "On");
    }
    mPressTimer -= 1;
    if (mPressTimer == 0) {
        al::startAction(this, "Off");
        al::setNerve(this, &nrvFlipPanelWait);
    }

}

void FlipPanel::exeEnd() {
    al::startAction(this, "OffWait");
    al::showMaterial(this, "FlipPanelDone");
    al::hideMaterial(this, "FlipPanelQ");
    al::hideMaterial(this, "FlipPanelEx");

}

namespace {
    NERVE_IMPL(FlipPanel, Wait)
    NERVE_IMPL(FlipPanel, Pressed)
    NERVE_IMPL(FlipPanel, End)
}

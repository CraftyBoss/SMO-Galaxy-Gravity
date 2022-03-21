#include "actors/FlipPanel.h"

FlipPanel::FlipPanel(const char* name) : al::LiveActor(name) {}

void FlipPanel::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "FlipPanel", 0);
    al::initNerve(this, &nrvFlipPanelWait, 0);
    al::startAction(this, "OffWait");
    //al::hideMaterial(this, "FlipPanelEx");
    //al::hideMaterial(this, "FlipPanelDone");
    makeActorAlive();
}

bool FlipPanel::receiveMsg(const al::SensorMsg* message, al::HitSensor* source, al::HitSensor* target) {
    if ((al::isMsgPlayerFloorTouch(message) || rs::isMsgCapHipDrop(message)) && 
        !al::isNerve(this, &nrvFlipPanelPressed) && !al::isNerve(this, &nrvFlipPanelEnd) && al::isNerve(this, &nrvFlipPanelWait) && !isOn ) 
    {
        al::setNerve(this, &nrvFlipPanelPressed);
        return true;
    } else if (!isOn && al::isNerve(this, &nrvFlipPanelWait)) {
        al::setNerve(this, &nrvFlipPanelPressed);
        return true;
    }
    
    return false;
}

void FlipPanel::exeWait() {
    if (isOn) {
        al::startAction(this, "OnWait");
    } else {
        al::startAction(this, "OffWait");
    }
}

void FlipPanel::exePressed() {
    if (isOn) {
        isOn = !isOn;
        al::startAction(this, "Off");
        //al::showMaterial(this, "FlipPanelQ");
        //al::hideMaterial(this, "FlipPanelEx");
    } else {
        isOn = !isOn;
        al::startAction(this, "On");
        //al::hideMaterial(this, "FlipPanelQ");
        //al::showMaterial(this, "FlipPanelEx");
    }
    al::setNerve(this, &nrvFlipPanelWait);
}

void FlipPanel::exeEnd() {
    al::startAction(this, "OnWait");
    //al::showMaterial(this, "FlipPanelDone");
    //al::hideMaterial(this, "FlipPanelQ");
    //al::hideMaterial(this, "FlipPanelEx");

}

namespace {
    NERVE_IMPL(FlipPanel, Wait)
    NERVE_IMPL(FlipPanel, Pressed)
    NERVE_IMPL(FlipPanel, End)
}

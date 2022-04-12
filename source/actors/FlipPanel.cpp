#include "actors/FlipPanel.h"
#include "logger.hpp"
#include "al/util/OtherUtil.h"


FlipPanel::FlipPanel(const char* name) : al::LiveActor(name) {}

void FlipPanel::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "FlipPanel", 0);
    al::initNerve(this, &nrvFlipPanelWait, 0);
    al::startAction(this, "OffWait");
    al::hideMaterial(this, "FlipPanelEx");
    al::hideMaterial(this, "FlipPanelDone");
    al::trySyncStageSwitchAppear(this);
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

void FlipPanel::exeAppear() {

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

bool FlipPanel::isGot() {
    return isOn;
}


namespace {
    NERVE_IMPL(FlipPanel, Appear);
    NERVE_IMPL(FlipPanel, Wait)
    NERVE_IMPL(FlipPanel, Pressed)
    NERVE_IMPL(FlipPanel, End)
}

FlipPanelObserver::FlipPanelObserver(const char* name) : al::LiveActor(name) {}

void FlipPanelObserver::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this,info);
    al::initStageSwitch(this,info);
    al::initExecutorWatchObj(this,info);
    mFlipPanelCount = al::calcLinkChildNum(info, mFlipPanelLink);
    int linkCount = mFlipPanelCount;
    mFlipPanels.allocBuffer(linkCount, nullptr);
    al::tryGetArg(&mDelayStep, info, "SwitchOnDelayStep");
    for (int i = 0; i < linkCount; i++) {
        auto flipPanel = new FlipPanel("FlipPanel");
        al::initLinksActor(flipPanel, info, mFlipPanelLink, i);
        mFlipPanels.pushBack(flipPanel);
    }
    al::initNerve(this, &nrvFlipPanelObserverWait, 0);
    if (al::trySyncStageSwitchAppear(this)) {
        for (int i = 0; i < linkCount; i++) {
            mFlipPanels[i]->makeActorDead();
        }
    }
}

void FlipPanelObserver::exeWait() {
    for (int i = 0; i < mFlipPanelCount; i++) {
        if (mFlipPanels[i]->isGot()) {
            mFlipPanelOnNum++;
        } else {
            mFlipPanelOnNum = 0;
            break;
        }
    }
    if (isAllOn()) {
        al::setNerve(this, &nrvFlipPanelObserverEnd);
    }
}

void FlipPanelObserver::exeEnd() {
    if (al::isGreaterEqualStep(this, mDelayStep)) {
        for (int i = 0; i < mFlipPanelCount; i++) {
            al::setNerve(mFlipPanels[i], &nrvFlipPanelEnd);
        }
        al::onStageSwitch(this, "SwitchActivateAllOn");
        al::startSe(this, "");
    }
}

bool FlipPanelObserver::isAllOn() {
    return mFlipPanelCount == mFlipPanelOnNum;
}

void FlipPanelObserver::appear() {
    al::LiveActor::appear();
    for (int i = 0; i < mFlipPanelCount; i++) {
       mFlipPanels[i]->appear();
       al::setNerve(mFlipPanels[i], &nrvFlipPanelAppear);
    }
}

namespace {
    NERVE_IMPL(FlipPanelObserver, Wait);
    NERVE_IMPL(FlipPanelObserver, End);
}
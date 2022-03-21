#pragma once

#include "al/LiveActor/LiveActor.h"
#include "al/util/LiveActorUtil.h"
#include "al/util/NerveUtil.h"
#include "rs/util/SensorUtil.h"
#include <sead/container/seadPtrArray.h>

class FlipPanel : public al::LiveActor {
public:
    FlipPanel(const char* name);
    void init(const al::ActorInitInfo&) override;
    //void initAfterPlacement() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source, al::HitSensor* target) override;

    void exeWait();
    void exePressed();
    void exeEnd();
private:
    bool isOn = false;

};
namespace {
    NERVE_HEADER(FlipPanel, Wait)
    NERVE_HEADER(FlipPanel, Pressed)
    NERVE_HEADER(FlipPanel, End)
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

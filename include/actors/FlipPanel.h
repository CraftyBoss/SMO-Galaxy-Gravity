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

    void exeAppear();
    void exeWait();
    void exePressed();
    void exeEnd();
    bool isGot();

private:
    bool isOn = false;
    int mPressTimer = 0;

};
namespace {
    NERVE_HEADER(FlipPanel, Appear)
    NERVE_HEADER(FlipPanel, Wait)
    NERVE_HEADER(FlipPanel, Pressed)
    NERVE_HEADER(FlipPanel, End)
}

class FlipPanelObserver : public al::LiveActor {
public:
    FlipPanelObserver(const char* name);
    void init(const al::ActorInitInfo&) override;

    void exeWait();
    void exeEnd();

    bool isAllOn();
    void appear() override;

private:
    constexpr static const char* mFlipPanelLink = "FlipPanelGroup";
    sead::PtrArray<FlipPanel> mFlipPanels;
    int mFlipPanelCount = 0;
    int mFlipPanelOnNum = 0;
    int mDelayStep = 10;

};
namespace {
    NERVE_HEADER(FlipPanelObserver, Wait);
    NERVE_HEADER(FlipPanelObserver, End);
}
#pragma once

#include "al/scene/SceneObjHolder.h"
#include "game/GameData/GameDataHolder.h"

namespace rs
{
    bool isHoldUiDecide(al::IUseSceneObjHolder const*);
    bool isHoldUiCancel(al::IUseSceneObjHolder const*);
    bool isHoldUiSelect(al::IUseSceneObjHolder const*);
    bool isHoldUiL(al::IUseSceneObjHolder const*);
    bool isHoldUiR(al::IUseSceneObjHolder const*);
    bool isHoldUiY(al::IUseSceneObjHolder const*);
    bool isHoldUiUp(al::IUseSceneObjHolder const*);
    bool isHoldUiDown(al::IUseSceneObjHolder const*);
    bool isHoldUiLeft(al::IUseSceneObjHolder const*);
    bool isHoldUiRight(al::IUseSceneObjHolder const*);

    bool isTriggerUiDecide(al::IUseSceneObjHolder const*);
    bool isTriggerUiDecide(GameDataHolder const*);
    bool isTriggerUiCancel(al::IUseSceneObjHolder const*);
    bool isTriggerUiSelect(al::IUseSceneObjHolder const*);
    bool isTriggerUiPause(al::IUseSceneObjHolder const*);
    bool isTriggerUiLeft(al::IUseSceneObjHolder const*);
    bool isTriggerUiRight(al::IUseSceneObjHolder const*);
    bool isTriggerUiUp(al::IUseSceneObjHolder const*);
    bool isTriggerUiDown(al::IUseSceneObjHolder const*);
    bool isTriggerUiR(al::IUseSceneObjHolder const*);
    bool isTriggerUiL(al::IUseSceneObjHolder const*);
    bool isTriggerUiZR(al::IUseSceneObjHolder const*);
    bool isTriggerUiZL(al::IUseSceneObjHolder const*);
    bool isTriggerUiX(al::IUseSceneObjHolder const*);
    bool isTriggerUiY(al::IUseSceneObjHolder const*);
    bool isTriggerUiAnyABXY(al::IUseSceneObjHolder const*);
    bool isTriggerUiRacePause(al::IUseSceneObjHolder const*);


} // namespace al

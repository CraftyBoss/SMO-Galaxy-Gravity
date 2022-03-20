#pragma once

#include "game/GameData/GameDataHolderAccessor.h"
#include "game/Player/HackCap/CapMessageEnableChecker.h"

namespace rs {
bool isSceneStatusInvalidSave(GameDataHolderAccessor holder);
bool tryCheckShowCapMsgCapCatapultLookFirst(al::IUseSceneObjHolder const*,CapMessageEnableChecker *);
}
#pragma once

#include "al/LiveActor/LiveActor.h"
#include "al/util/OtherUtil.h"
#include <math/seadVector.h>
#include "al/camera/CameraTicket.h"

class HackObjInfo;

struct PlayerCameraFunction {

static void calcCameraMoveInput(sead::Vector2f *,al::LiveActor const*);
static bool tryCalcCameraSubMoveInput(sead::Vector2f *,al::LiveActor const*);
static void checkNoCollisionForPlayerSubjectiveCamera(al::LiveActor const*,al::CameraTicket const*,bool);
static bool isInvalidatePlayerInput(al::LiveActor const*);
static bool isTriggerCameraReset(al::LiveActor const*);
static bool isTriggerCameraSubjective(al::LiveActor const*);
static bool isHoldCameraZoom(al::LiveActor const*);
static bool isHoldCameraSnapShotZoomIn(al::LiveActor const*);
static bool isHoldCameraSnapShotZoomOut(al::LiveActor const*);
static bool isHoldCameraSnapShotRollLeft(al::LiveActor const*);
static bool isHoldCameraSnapShotRollRight(al::LiveActor const*);
static bool isCameraInWater(al::LiveActor const*);
static bool isCameraClimbPole(al::LiveActor const*);
static bool isCameraGrabCeil(al::LiveActor const*);
static bool isCameraWallCatch(al::LiveActor const*);
static bool isCameraMoonGravity(al::LiveActor const*);
static bool isFailureCameraSubjective(al::LiveActor const*);
static void setCameraHackName(al::LiveActor *,HackObjInfo const*);
static void resetCameraHackName(al::LiveActor *);
static bool tryCalcPlayerRecoveryTarget(sead::Vector3f *,al::AreaObj const**,al::LiveActor const*);

};
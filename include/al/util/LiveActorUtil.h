#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>
#include "al/LiveActor/LiveActor.h"
#include "al/async/FunctorBase.h"
#include "al/collision/Collider.h"
#include "game/GameData/GameDataHolderBase.h"
#include "game/Player/PlayerActorHakoniwa.h"
#include "al/layout/LayoutActor.h"
#include "al/layout/LayoutInitInfo.h"

typedef unsigned int uint;

namespace al {

    struct Scene;
    struct ActorFactory;

    void tryInitFixedModelGpuBuffer(const LiveActor*);
    void offUpdateMovementEffectAudioCollisionSensor(const LiveActor*);
    void hideModel(LiveActor *);
    void showMaterial(LiveActor*, const char*);
    void hideMaterial(LiveActor*, const char*);
    void hideModelIfShow(const LiveActor*);
    void showModelIfHide(const LiveActor*);
    void setModelAlphaMask(const LiveActor*, float);
    void resetPosition(const LiveActor*);
    void onSyncClippingSubActor(LiveActor*, const LiveActor*);
    void onSyncHideSubActor(LiveActor*, const LiveActor*);
    void onSyncAlphaMaskSubActor(LiveActor*, const LiveActor*);
    void setMaterialProgrammable(LiveActor*);
    void startAction(LiveActor*, char const*);
    void startAction(IUseLayoutAction*, const char *, const char *);
    void startFreezeActionEnd(IUseLayoutAction *,char const*,char const*);
    void startHitReaction(const LiveActor*, char const*);
    void invalidateClipping(LiveActor *);
    void validateClipping(LiveActor *);
    void setNerveAtActionEnd(LiveActor*, const al::Nerve*);
    void updateMaterialCodeWater(LiveActor *);
    void updateMaterialCodeWater(LiveActor *, bool);
    void appearItem(const LiveActor *);
    void turnToTarget(LiveActor*, const sead::Vector3f&, float);
    void turnToTarget(LiveActor*, const al::LiveActor *, float);
    void onStageSwitch(IUseStageSwitch*, char const*);

    void expandClippingRadiusByShadowLength(LiveActor*, sead::Vector3f*, float);

    void initJointLocalXRotator(const LiveActor *,const float *,const char *);
    void initJointLocalYRotator(const LiveActor *,const float *,const char *);
    void initJointLocalZRotator(const LiveActor*, const float*, const char*);

    void initActorPoseTRSV(al::LiveActor *);
    void initActorPoseTRMSV(al::LiveActor *);
    void initActorPoseTRGMSV(al::LiveActor *);
    void initActorPoseTFSV(al::LiveActor *);
    void initActorPoseTFUSV(al::LiveActor *);
    void initActorPoseTFGSV(al::LiveActor *);
    void initActorPoseTQSV(al::LiveActor *);
    void initActorPoseTQGSV(al::LiveActor *);
    void initActorPoseTQGMSV(al::LiveActor *);
    void initActorPoseT(al::LiveActor *,sead::Vector3<float> const&);
    void initActorPoseTR(al::LiveActor *,sead::Vector3<float> const&,sead::Vector3<float> const&);

    void initLayoutPartsActor(LayoutActor*, LayoutActor*, const LayoutInitInfo&, char const*,
                              char const*);
    void initActor(LiveActor*, ActorInitInfo const&);
    void initCreateActorWithPlacementInfo(LiveActor*, const al::ActorInitInfo&);
    void initMapPartsActor(LiveActor *, const al::ActorInitInfo  &, const char *);
    void initActorWithArchiveName(LiveActor*, const al::ActorInitInfo&, const sead::SafeString&, const char*);
    void initJointControllerKeeper(const LiveActor*, int);
    void initJointGlobalQuatController(const LiveActor*, const sead::Quatf*, const char*);
    void initLinksActor(LiveActor*, ActorInitInfo const&, const char*, int);
    void initActorSceneInfo(al::LiveActor *,al::ActorInitInfo const&);
    void initStageSwitch(al::LiveActor *,al::ActorInitInfo const&);
    void initExecutorWatchObj(al::LiveActor*, al::ActorInitInfo const&);
    void initActorInitInfo(al::ActorInitInfo *, al::Scene const*,al::PlacementInfo const*,al::LayoutInitInfo const*,al::ActorFactory const*, struct SceneMsgCtrl *, GameDataHolderBase *);

    void appearBreakModelRandomRotateY(LiveActor*);

    void faceToTarget(al::LiveActor *,sead::Vector3<float> const&);
    void faceToTarget(al::LiveActor *,al::LiveActor const*);

    bool isNear(const LiveActor *, const LiveActor *, float);
    bool isClipped(const LiveActor*);
    bool isDead(const LiveActor*);
    bool isAlive(const LiveActor*);
    bool isHideModel(const LiveActor*);
    bool isEffectEmitting(const IUseEffectKeeper*, const char*);
    bool isActionEnd(const LiveActor*);
    bool isActionPlaying(const LiveActor*, const char *);
    bool isInvalidClipping(const LiveActor*);
    bool isInWater(const LiveActor *);
    bool isInWaterArea(const LiveActor *);
    bool isOnGround(const LiveActor *, unsigned int);
    bool isOnStageSwitch(IUseStageSwitch const *, const char *);
    bool isValidStageSwitch(IUseStageSwitch const *, const char *);
    bool isFallNextMove(const LiveActor *, const sead::Vector3f &, float, float);
    bool isInDeathArea(LiveActor *);
    bool isCollidedFloorCode(LiveActor *, const char *);
    bool isNoCollide(LiveActor const *);
    bool isNearPlayer(const LiveActor*, float);
    bool isFallOrDamageCodeNextMove(const LiveActor *,  const sead::Vector3f &, float, float);
    bool isFaceToTargetDegreeH(const LiveActor*, const sead::Vector3f &, const sead::Vector3f &, float);

    void tryListenStageSwitchAppear(al::LiveActor *);
    void tryListenStageSwitchKill(al::LiveActor *);
    
    bool tryOnSwitchDeadOn(IUseStageSwitch *);
    bool trySyncStageSwitchAppear(LiveActor *);
    PlayerActorHakoniwa* tryFindNearestPlayerActor(const LiveActor *);
    bool tryFindNearestPlayerPos(sead::Vector3f *, const LiveActor *);
    bool tryAddRippleMiddle(LiveActor*);
    bool tryStartActionIfNotPlaying(LiveActor*, const char*);
    bool tryAddDisplayOffset(LiveActor *,ActorInitInfo const&);

    sead::Vector3f &getTrans(const LiveActor*);
    sead::Vector3f* getTransPtr(LiveActor*);
    sead::Vector3f &getGravity(const LiveActor*);
    sead::Vector3f* getGravityPtr(const LiveActor*);
    sead::Vector3f &getFront(const LiveActor*);
    sead::Vector3f* getFrontPtr(LiveActor*);
    sead::Vector3f &getVelocity(const LiveActor*);
    sead::Vector3f* getVelocityPtr(LiveActor*);
    sead::Vector3f &getUp(const LiveActor*);
    Collider* getActorCollider(LiveActor*);

    sead::Matrix34f* getJointMtxPtr(const LiveActor*, const char*); //return type might be const

    sead::Quatf* getQuatPtr(LiveActor *);

    sead::Vector3f* getOnGroundNormal(const LiveActor *, uint);

    void scaleVelocity(LiveActor*, float);
    void scaleVelocityDirection(LiveActor*, sead::Vector3f const &, float);

    void setHitSensorPosPtr(LiveActor *,char const*,sead::Vector3f const*);
    void setTrans(LiveActor *, sead::Vector3f const &);
    void setVelocity(LiveActor*, sead::Vector3f const&);
    void setVelocity(LiveActor*, float, float, float);
    void setVelocityX(LiveActor*, float);
    void setVelocityY(LiveActor*, float);
    void setVelocityZ(LiveActor*, float);
    void setVelocityZero(LiveActor*);
    void setVelocityBlowAttackAndTurnToTarget(LiveActor *, sead::Vector3f const&,
                                                  float, float);
    void setActionFrameRate(LiveActor*, float);

    void addVelocityToGravityFittedGround(LiveActor*, float, unsigned int);
    void addVelocityToGravity(LiveActor*, float);
    void addVelocityToDirection(LiveActor*, sead::Vector3f const &, float);
    void addVelocity(LiveActor*, sead::Vector3f const &);
    void addVelocityX(LiveActor*, float);
    void addVelocityY(LiveActor*, float);
    void addVelocityZ(LiveActor*, float);

    void calcFrontDir(sead::Vector3f *, const LiveActor *);
    void calcQuat(sead::Quatf*, const LiveActor*);
    void calcJointFrontDir(sead::Vector3f*, const LiveActor*, const char*);
    void calcJointPos(sead::Vector3f*, const LiveActor*, const char*);
    int calcLinkChildNum(ActorInitInfo const&, char const*);

    void makeQuatUpFront(sead::Quatf *, sead::Vector3f const &, sead::Vector3f const &);

    void rotateQuatYDirDegree(LiveActor *, float);

    float* findActorParamF32(const LiveActor*, const char*);

    int* findActorParamS32(const LiveActor*, const char*);

    LiveActor* getSubActor(const LiveActor*, const char*); //NOTE: unknown return type

    bool listenStageSwitchOnAppear(IUseStageSwitch *, al::FunctorBase const &functor);
}

namespace rs {

sead::Vector3f* getPlayerPos(const al::LiveActor*);

}

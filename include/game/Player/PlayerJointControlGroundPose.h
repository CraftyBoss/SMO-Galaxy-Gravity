#pragma once

#include "al/LiveActor/LiveActor.h"
#include "sead/container/seadSafeArray.h"

class PlayerJointControlGroundPose {
public:
    PlayerJointControlGroundPose(al::LiveActor const*);
    void updateCenterTilt(float,sead::Vector3<float> const&);
    void updateCenterBalance(bool,bool,bool,sead::Vector3<float> const&,sead::Vector3<float> const&,bool);
    void update(float,float,float,float,bool);
    void resetTiltRate(void);
    void initCenterBalanceRate(void);
    const char *getCtrlTypeName(void);
    void calcTilt(int,sead::Matrix34<float> *);
    void calcSpineMtxTilt(sead::Matrix34<float> *);
    void calcSpineMtx(sead::Matrix34<float> *);
    void calcRootMtxTilt(sead::Matrix34<float> *);
    void calcRootMtx(sead::Matrix34<float> *);
    void calcLegTopMtxTilt(sead::Matrix34<float> *,bool);
    void calcLegBottomMtx(sead::Matrix34<float> *,sead::SafeArray<sead::Matrix34<float>,2> const&,sead::SafeArray<int,2> const&);
    void calcJointCallback(int,sead::Matrix34<float> *);
    void calcIK(int,sead::Matrix34<float> *);
    void calcHipMtx(sead::Matrix34<float> *);
    void calcHeadMtxTilt(sead::Matrix34<float> *);
    void calcGroundPoseUp(sead::Vector3<float> *);
    void calcCenterTilt(int,sead::Matrix34<float> *);
    void calcCenterBalanceBlendRate(void);
    void calcCenterBalance(int,sead::Matrix34<float> *);

};
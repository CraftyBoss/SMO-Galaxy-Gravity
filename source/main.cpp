#include "main.hpp"
#include "al/area/AreaObj.h"
#include "al/area/AreaObjGroup.h"
#include "al/camera/CameraDirector.h"
#include "al/camera/CameraPoser.h"
#include "al/util.hpp"
#include "al/util/InputUtil.h"
#include "al/util/LiveActorUtil.h"
#include "al/util/StringUtil.h"
#include "cameras/CameraPoserCustom.h"
#include "debugMenu.hpp"
#include "game/Player/HackCap.h"
#include "game/Player/PlayerJointControlGroundPose.h"
#include "mars/gravity.h"

static bool showMenu = true;

static bool isInGame = false;

int listCount = 0;

int curWarpPoint = 0;

void drawBackground(agl::DrawContext *context)
{

    sead::Vector3<float> p1; // top left
    p1.x = -1.0;
    p1.y = 0.3;
    p1.z = 0.0;
    sead::Vector3<float> p2; // top right
    p2.x = -0.2;
    p2.y = 0.3;
    p2.z = 0.0;
    sead::Vector3<float> p3; // bottom left
    p3.x = -1.0;
    p3.y = -1.0;
    p3.z = 0.0;
    sead::Vector3<float> p4; // bottom right
    p4.x = -0.2;
    p4.y = -1.0;
    p4.z = 0.0;

    sead::Color4f c;
    c.r = 0.1;
    c.g = 0.1;
    c.b = 0.1;
    c.a = 0.9;

    agl::utl::DevTools::beginDrawImm(context, sead::Matrix34<float>::ident, sead::Matrix44<float>::ident);
    agl::utl::DevTools::drawTriangleImm(context, p1, p2, p3, c);
    agl::utl::DevTools::drawTriangleImm(context, p3, p4, p2, c);
}

// ------------- Hooks -------------

void drawMainHook(HakoniwaSequence *curSequence, sead::Viewport *viewport, sead::DrawContext *drawContext)
{

    if (!showMenu)
    {
        al::executeDraw(curSequence->mLytKit, "２Ｄバック（メイン画面）");
        return;
    }

    int dispWidth = al::getLayoutDisplayWidth();
    int dispHeight = al::getLayoutDisplayHeight();

    gTextWriter->mViewport = viewport;

    gTextWriter->mColor = sead::Color4f(1.f, 1.f, 1.f, 0.8f);

    al::Scene *curScene = curSequence->curScene;

    if (curScene && isInGame) {

        drawBackground((agl::DrawContext *)drawContext);
        
        gTextWriter->beginDraw();

        gTextWriter->setCursorFromTopLeft(sead::Vector2f(10.f, (dispHeight / 3) + 30.f));

        gTextWriter->setScaleFromFontHeight(20.0f);

        al::CameraPoser *curPoser;

        al::CameraDirector *director = curScene->getCameraDirector();
        if (director) {
            al::CameraPoseUpdater *updater = director->getPoseUpdater(0);
            if (updater && updater->mTicket) {
                curPoser = updater->mTicket->mPoser;
            }
        } else {
            gTextWriter->printf("There is no director.\n");
        }

        al::PlayerHolder* pHolder = al::getScenePlayerHolder(curScene);

        PlayerActorHakoniwa* p1 = al::tryGetPlayerActor(pHolder, 0);

        sead::Vector3f* pTrans = al::getTransPtr(p1);
        sead::Vector3f* pVel = al::getVelocityPtr(p1);
        sead::Vector3f* pGrav = al::getGravityPtr(p1);
        float velH = al::calcSpeedH(p1);
        float velV = al::calcSpeedV(p1);
        
        gTextWriter->printf("Player Pos:\n%.3f   %.3f   %.3f\n", pTrans->x,pTrans->y,pTrans->z);
        gTextWriter->printf("Player Velocity:\n%.3f   %.3f   %.3f\nHorizontal: %.3f   Vertical: %.3f\n", pVel->x,pVel->y,pVel->z, velH, velV);
        gTextWriter->printf("Player Gravity:\n%.3f   %.3f   %.3f\n", pGrav->x,pGrav->y,pGrav->z);

        if (curPoser) {
            gTextWriter->printf("Camera Poser Name: %s\n", curPoser->getName());

            if (al::isEqualString(curPoser->getName(), "CameraPoserCustom")) {
                cc::CameraPoserCustom* poserCustom = (cc::CameraPoserCustom*)curPoser;
                gTextWriter->printf("Vertical Angle: %f\n", poserCustom->mAngleV);
                gTextWriter->printf("Horizontal Angle: %f\n", poserCustom->mAngleH);
                gTextWriter->printf("Prev Vertical Angle: %f\n", poserCustom->mPrevV);
                gTextWriter->printf("Prev Horizontal Angle: %f\n", poserCustom->mPrevH);
            }
        } else {
            gTextWriter->printf("There is no camera poser.\n");
        }

        isInGame = false;
    }

    gTextWriter->endDraw();

    al::executeDraw(curSequence->mLytKit, "２Ｄバック（メイン画面）");
}

void stageInitHook(al::ActorInitInfo *info, StageScene *curScene, al::PlacementInfo const *placement, al::LayoutInitInfo const *lytInfo, al::ActorFactory const *factory, al::SceneMsgCtrl *sceneMsgCtrl, al::GameDataHolderBase *dataHolder) {

    al::initActorInitInfo(info, curScene, placement, lytInfo, factory, sceneMsgCtrl,
                          dataHolder);
    
}

ulong threadInit()
{ // hook for initializing any threads we need
    __asm("STR X21, [X19,#0x208]");

    return 0x20;
}

bool hakoniwaSequenceHook(HakoniwaSequence* sequence) {

    bool isFirstStep = al::isFirstStep(sequence);

    if (isFirstStep) {
        return isFirstStep;
    }
    
    StageScene* stageScene = (StageScene*)sequence->curScene;

    al::PlayerHolder *pHolder = al::getScenePlayerHolder(stageScene);
    PlayerActorHakoniwa* p1 = al::tryGetPlayerActor(pHolder, 0);

    isInGame = true;

    if (p1) {
        mars::calcActorGravity(p1);
    }

    if (p1->mHackCap) {
        mars::calcActorGravity(p1->mHackCap);
    }

    if (al::isPadTriggerUp(-1)) // enables/disables debug menu
    {
         showMenu = !showMenu;
    }
    if (al::isPadTriggerLeft(-1)) {
        al::setVelocityZero(p1);
    }

    return isFirstStep;
}

void seadPrintHook(const char *fmt, ...) // hook for replacing sead::system::print with our custom logger
{
    va_list args;
    va_start(args, fmt);

    gLogger->LOG(fmt, args);

    va_end(args);
}
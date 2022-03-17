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

    sead::Vector2f startPos = sead::Vector2f(10.f, (dispHeight / 3) + 30.f);

    al::Scene *curScene = curSequence->curScene;

    if (curScene && isInGame) {

        gTextWriter->beginDraw();
        
        drawBackground((agl::DrawContext *)drawContext);

        gTextWriter->setCursorFromTopLeft(startPos);

        al::CameraPoser *curPoser;

        al::CameraDirector *director = curScene->getCameraDirector();
        if (director) {
            al::CameraPoseUpdater *updater = director->getPoseUpdater(0);
            if (updater && updater->mTicket) {
                curPoser = updater->mTicket->mPoser;
            }
        }

        if (curPoser) {
            gTextWriter->printf("Camera Poser Name: %s\n", curPoser->getName());

            if (al::isEqualString(curPoser->getName(), "CameraPoserCustom")) {
                cc::CameraPoserCustom* poserCustom = (cc::CameraPoserCustom*)curPoser;
                gTextWriter->printf("Verical Angle: %f\n", poserCustom->mAngleV);
                gTextWriter->printf("Horizontal Angle: %f\n", poserCustom->mAngleH);
            }
        }

        isInGame = false;
    }

    gTextWriter->endDraw();

    al::executeDraw(curSequence->mLytKit, "２Ｄバック（メイン画面）");
}

void stageInitHook(StageScene *initStageScene, al::SceneInitInfo *sceneInitInfo)
{
    __asm("MOV X19, X0");
    __asm("LDR X24, [X1, #0x18]");

    // place any code that needs to be ran during init here (creating actors for example)

    __asm("MOV X1, X24");
}

ulong threadInit()
{ // hook for initializing any threads we need
    __asm("STR X21, [X19,#0x208]");

    return 0x20;
}

void stageSceneHook(StageScene *stageScene)
{
    stageScene->stageSceneLayout->updatePlayGuideMenuText();

    al::PlayerHolder *pHolder = al::getScenePlayerHolder(stageScene);
    PlayerActorHakoniwa* p1 = al::tryGetPlayerActor(pHolder, 0);

    isInGame = true;

    if (p1) {
        mars::calcActorGravity(p1);
    }

    if (p1->mHackCap) {
        mars::calcActorGravity(p1->mHackCap);
    }

    // if (al::isPadTriggerUp(-1)) // enables/disables debug menu
    // {
    //     showMenu = !showMenu;
    // }
}

void seadPrintHook(const char *fmt, ...) // hook for replacing sead::system::print with our custom logger
{
    va_list args;
    va_start(args, fmt);

    gLogger->LOG(fmt, args);

    va_end(args);
}
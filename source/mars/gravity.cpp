#include "al/LiveActor/LiveActor.h"
#include "al/util.hpp"
#include <sead/math/seadMathBase.h>
#include <sead/math/seadMatrix.h>
#include <sead/math/seadVector.h>
#include <sead/gfx/seadCamera.h>
#include "debugMenu.hpp"
#include "mars/gravity.h"
#include "al/area/AreaObj.h"
#include "al/area/AreaObjGroup.h"
#include "al/area/AreaObjDirector.h"
#include "rs/util.hpp"
#include <cmath>
#include "al/camera/CameraAngleVerticalCtrl.h"

/*
 * GRAVITY TYPES
 * It's important to note that all of these shapes should use the area closest to their actual shape (AreaCube, AreaSphere, AreaCylinder etc).
 * For the shapes that use a cube, make sure the model name is AreaCubeCenter. For cone, it's recommended to use AreaCubeBase.
 * The base radius of these areas is 500 units. 
 */
enum GravityType : int {
    Spherical = 0, Cuboidal = 1, Parallel = 2, Cylindrical = 3, Torus = 4, Wedge = 5, Disk = 6, Cone = 7
};


void mars::calcGravityDirection(al::LiveActor* player) {
    sead::Vector3f* playerTrans = al::getTrans(player);
    al::AreaObjGroup* gravityAreaGroup = al::tryFindAreaObjGroup(player, "GravityArea");
    
    if (gravityAreaGroup) {
        al::AreaObj* gravityArea = gravityAreaGroup->getInVolumeAreaObj(*playerTrans);

        if (gravityArea && !rs::isPlayer2D(player)) {
            bool isInverted;
            int gravityType;
            al::tryGetAreaObjArg(&gravityType, gravityArea, "GravityType");
            al::tryGetAreaObjArg(&isInverted, gravityArea, "IsInverted");
            sead::Vector3f gravity;
            sead::Vector3f areaPos = {gravityArea->mAreaTR.m[0][3], gravityArea->mAreaTR.m[1][3], gravityArea->mAreaTR.m[2][3]};
            sead::Matrix33f mtx;
            sead::Matrix33CalcCommon<float>::copy(mtx, gravityArea->mAreaTR);
            sead::Vector3f dist = {areaPos.x - playerTrans->x, areaPos.y - playerTrans->y, areaPos.z - playerTrans->z};
            if (gravityType == Spherical || gravityType == Parallel || gravityType == Cylindrical || gravityType == Wedge) {
                sead::Matrix33CalcCommon<float>::inverse(mtx, mtx);
                sead::Vector3CalcCommon<float>::mul(dist, mtx, dist);

            } else if (gravityType == Cuboidal || gravityType == Torus || gravityType == Disk || gravityType == Cone) {
                sead::Vector3f scale = gravityArea->mAreaShape->mScale;
                mtx = {
                    mtx.m[0][0]*scale.x, mtx.m[0][1]*scale.x, mtx.m[0][2]*scale.x,
                    mtx.m[1][0]*scale.y, mtx.m[1][1]*scale.y, mtx.m[1][2]*scale.y,
                    mtx.m[2][0]*scale.z, mtx.m[2][1]*scale.z, mtx.m[2][2]*scale.z
                };
                sead::Matrix33CalcCommon<float>::inverse(mtx, mtx);
                sead::Vector3CalcCommon<float>::mul(dist, mtx, dist);
            }
            
            switch (gravityType) {
                case Spherical: {
                    // points towards position of the area
                    gravity = dist;
                    break;
                }
                case Cuboidal: {                
                    float radius = 250;
                    // gravity points towards nearest point on a cubes surface; if inside, pushes you out.
                    if (abs(dist.x) < radius && abs(dist.y) < radius && abs(dist.z) < radius) {
                        gravity = {-dist.x, -dist.y, -dist.z};
                    } else {
                        sead::Vector3f closestPointInBox = {al::clamp(dist.x,-radius,radius),  al::clamp(dist.y,-radius,radius), al::clamp(dist.z,-radius,radius)};
                        gravity = {dist.x - closestPointInBox.x, dist.y - closestPointInBox.y, dist.z - closestPointInBox.z};
                    }
                    break;
                }
                case Parallel: {
                    // rotate the cube to the direction wanted. default is down
                    gravity = {0.0, -1.0, 0.0};
                    break;
                }
                case Cylindrical: {
                    gravity = {dist.x, 0.0, dist.z};
                    break;
                }
                case Torus: {
                    // base radius is 250; scaling the area will also scale the radius.
                    // radius is measured from center of the torus to center of the circle of the torus, if that makes sense.
                    float R = 250;
                    float gravityPoint = sqrtf(R*R/(dist.x*dist.x + dist.z*dist.z));
                    gravity = {dist.x*(1-gravityPoint), dist.y, dist.z*(1-gravityPoint)};
                    break;
                }
                case Wedge: {
                    sead::Vector3f scale = gravityArea->mAreaShape->mScale;
                    float radius = 500;
                    // pushes gravity in -y +z direction from +y -z edge
                    gravity = {0, -(dist.y + radius*scale.y), radius*scale.z - dist.z};
                    break;
                }
                case Disk: {
                    // base radius is 250; scaling the area will also scale the radius.
                    float R = 250;
                    if (sqrtf(dist.x*dist.x+dist.z*dist.z) < R) {
                        gravity = {0, dist.y, 0};
                    } else {
                        float gravityPoint = sqrtf(R*R/(dist.x*dist.x + dist.z*dist.z));
                        gravity = {dist.x*(1-gravityPoint), dist.y, dist.z*(1-gravityPoint)};
                    }
                    break;

                }
                case Cone: {
                    // recommended to use AreaCubeBase for this one
                    float r = 250;
                    float h = 250;
                    float distXZ = sqrtf(dist.x*dist.x + dist.z*dist.z);
                    if (dist.y > 0) {
                        return;
                    } else if (abs(dist.y) > h && distXZ <= abs(r/h*(h+dist.y))) {
                        gravity = {dist.x, dist.y + h, dist.z};
                    } else {
                        gravity = {h*dist.x/distXZ, -r, h*dist.z/distXZ};
                    }
                    break;
                }
                
            }
            if (isInverted) {
                gravity = {-gravity.x, -gravity.y, -gravity.z};
            }
            sead::Matrix33CalcCommon<float>::inverse(mtx, mtx);
            sead::Vector3CalcCommon<float>::mul(gravity,mtx,gravity);
            al::normalize(&gravity);
            al::setGravity(player, gravity);
        } else if (!gravityArea && !rs::isPlayer2D(player)) { // sticks to collision normals when not in 2D
            PlayerActorHakoniwa* playerActor = al::getPlayerActor(player, 0);
            PlayerColliderHakoniwa* playerCollision = playerActor->mPlayerCollider;
            sead::Vector3f gravity = {0,0,0};
            if (rs::calcOnGroundNormalOrGravityDir(&gravity, playerActor, playerCollision)) {
                gravity = {-gravity.x, -gravity.y, -gravity.z};
                al::normalize(&gravity);
                al::setGravity(player, gravity);
            }
        }
    } else if (!rs::isPlayer2D(player)) { // sticks to collision normals when not in 2D
        PlayerActorHakoniwa* playerActor = al::getPlayerActor(player, 0);
        PlayerColliderHakoniwa* playerCollision = playerActor->mPlayerCollider;
        sead::Vector3f gravity = {0,0,0};
        if (rs::calcOnGroundNormalOrGravityDir(&gravity, playerActor, playerCollision)) {
            gravity = {-gravity.x, -gravity.y, -gravity.z};
            al::normalize(&gravity);
            al::setGravity(player, gravity);
        }
    }

    
}


void initHackCapHook(al::LiveActor *cappy) {
    al::initActorPoseTQGSV(cappy);
}
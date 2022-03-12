#include "al/LiveActor/LiveActor.h"
#include "al/util.hpp"
#include <sead/math/seadMathBase.h>
#include <sead/math/seadMatrix.h>
#include <sead/math/seadVector.h>
#include <sead/gfx/seadCamera.h>
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
            sead::Vector3f gravity = {0,0,0};
            sead::Vector3f scale = gravityArea->mAreaShape->mScale;
            sead::Matrix34f mtx = gravityArea->mAreaTR;
            sead::Vector3f areaPos =  {mtx.m[0][3], mtx.m[1][3],mtx.m[2][3]};
            sead::Matrix33f areaMtxScaled = {
                mtx.m[0][0]*scale.x, mtx.m[0][1]*scale.x, mtx.m[0][2]*scale.x, 
                mtx.m[1][0]*scale.y, mtx.m[1][1]*scale.y, mtx.m[1][2]*scale.y,
                mtx.m[2][0]*scale.z, mtx.m[2][1]*scale.z, mtx.m[2][2]*scale.z};
            sead::Matrix33f areaMtx = {
                mtx.m[0][0], mtx.m[0][1], mtx.m[0][2], 
                mtx.m[1][0], mtx.m[1][1], mtx.m[1][2],
                mtx.m[2][0], mtx.m[2][1], mtx.m[2][2]};
            sead::Matrix33f inverseMtx = calcInverseMtx(areaMtx);
            sead::Vector3f dist = {areaPos.x - playerTrans->x, areaPos.y - playerTrans->y, areaPos.z - playerTrans->z};
            dist = calcMtxRot(inverseMtx, &dist);

            switch (gravityType) {
                case Spherical: {
                    // points towards position of the area
                    gravity = dist;
                    break;
                }
                case Cuboidal: {
                    sead::Vector3f dist = {areaPos.x - playerTrans->x, areaPos.y - playerTrans->y, areaPos.z - playerTrans->z};
                    sead::Matrix33f inverseMtx = calcInverseMtx(areaMtxScaled);
                    dist = calcMtxRot(inverseMtx, &dist);
                    float radius = 250;
                    // gravity points towards nearest point on a cubes surface; if inside, pushes you out.
                    if (abs(dist.x) < radius && abs(dist.y) < radius && abs(dist.z) < radius) {
                        gravity = {-dist.x, -dist.y, -dist.z};
                    } else {
                        sead::Vector3f closestPointInBox = {al::clamp(dist.x,-radius,radius),  al::clamp(dist.y,-radius,radius), al::clamp(dist.z,-radius,radius)};
                        gravity = {dist.x - closestPointInBox.x, dist.y - closestPointInBox.y, dist.z - closestPointInBox.z};
                    }
                    gravity = calcMtxRot(areaMtxScaled, &gravity);
                    if (isInverted) {
                        gravity = {-gravity.x, -gravity.y, -gravity.z};
                    }
                    al::normalize(&gravity);
                    al::setGravity(player, gravity);
                    return;
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
                    sead::Vector3f dist = {areaPos.x - playerTrans->x, areaPos.y - playerTrans->y, areaPos.z - playerTrans->z};
                    sead::Matrix33f inverseMtx = calcInverseMtx(areaMtxScaled);
                    dist = calcMtxRot(inverseMtx, &dist);
                    float gravityPoint = sqrt(R*R/(dist.x*dist.x + dist.z*dist.z));
                    gravity = {dist.x*(1-gravityPoint), dist.y, dist.z*(1-gravityPoint)};
                    gravity = calcMtxRot(areaMtxScaled, &gravity);
                    if (isInverted) {
                        gravity = {-gravity.x, -gravity.y, -gravity.z};
                    }
                    float magnitude = sqrt(gravity.x*gravity.x + gravity.y*gravity.y + gravity.z*gravity.z);
                    gravity = {gravity.x/magnitude, gravity.y/magnitude, gravity.z/magnitude};
                    al::normalize(&gravity);
                    al::setGravity(player, gravity);
                    return;
                }
                case Wedge: {
                    float radius = 500;
                    // pushes gravity in -y +z direction from +y -z edge
                    gravity = {0, -dist.y - radius*scale.y, dist.z - radius*scale.z};
                    break;
                }
                case Disk: {
                    // base radius is 250; scaling the area will also scale the radius.
                    float R = 250;
                    sead::Vector3f dist = {areaPos.x - playerTrans->x, areaPos.y - playerTrans->y, areaPos.z - playerTrans->z};
                    sead::Matrix33f inverseMtx = calcInverseMtx(areaMtxScaled);
                    dist = calcMtxRot(inverseMtx, &dist);
                    if (sqrt(dist.x*dist.x+dist.z*dist.z) < R) {
                        gravity = {0, dist.y, 0};
                    } else {
                        float gravityPoint = sqrt(R*R/(dist.x*dist.x + dist.z*dist.z));
                        gravity = {dist.x*(1-gravityPoint), dist.y, dist.z*(1-gravityPoint)};
                    }
                    gravity = calcMtxRot(areaMtxScaled, &gravity);
                    if (isInverted) {
                        gravity = {-gravity.x, -gravity.y, -gravity.z};
                    }
                    float magnitude = sqrt(gravity.x*gravity.x + gravity.y*gravity.y + gravity.z*gravity.z);
                    gravity = {gravity.x/magnitude, gravity.y/magnitude, gravity.z/magnitude};
                    al::normalize(&gravity);
                    al::setGravity(player, gravity);
                    return;

                }
                case Cone: {
                    // recommended to use AreaCubeBase for this one
                    float r = 250;
                    float h = 250;
                    sead::Vector3f dist = {areaPos.x - playerTrans->x, areaPos.y - playerTrans->y, areaPos.z - playerTrans->z};
                    sead::Matrix33f inverseMtx = calcInverseMtx(areaMtxScaled);
                    dist = calcMtxRot(inverseMtx, &dist);
                    float distXZ = sqrt(dist.x*dist.x + dist.z*dist.z);
                    if (dist.y > 0) {
                        return;
                    } else if (abs(dist.y) > h && distXZ <= abs(r/h*(h+dist.y))) {
                        gravity = {dist.x, dist.y + h, dist.z};
                    } else {
                        gravity = {h*dist.x/distXZ, -r, h*dist.z/distXZ};
                    }
                    gravity = calcMtxRot(areaMtxScaled, &gravity);
                    if (isInverted) {
                        gravity = {-gravity.x, -gravity.y, -gravity.z};
                    }
                    float magnitude = sqrt(gravity.x*gravity.x + gravity.y*gravity.y + gravity.z*gravity.z);
                    gravity = {gravity.x/magnitude, gravity.y/magnitude, gravity.z/magnitude};
                    al::normalize(&gravity);
                    al::setGravity(player, gravity);
                    return;
                }
                
            }
            if (isInverted) {
                gravity = {-gravity.x, -gravity.y, -gravity.z};
            }
            gravity = calcMtxRot(areaMtx, &gravity);
            al::normalize(&gravity);
            al::setGravity(player, gravity);
            sead::Vector3f invertedGravity = {-gravity.x, -gravity.y, -gravity.z};
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

sead::Matrix33f mars::calcInverseMtx(const sead::Matrix33f& matrix) {
    sead::Matrix33f inverse;

    inverse.m[0][0] = matrix.m[1][1]*matrix.m[2][2] - matrix.m[1][2]*matrix.m[2][1];
    inverse.m[0][1] = -(matrix.m[0][1]*matrix.m[2][2] - matrix.m[0][2]*matrix.m[2][1]);
    inverse.m[0][2] = matrix.m[0][1]*matrix.m[1][2] - matrix.m[0][2]*matrix.m[1][1];
    inverse.m[1][0] = -(matrix.m[1][0]*matrix.m[2][2] - matrix.m[1][2]*matrix.m[2][0]);
    inverse.m[1][1] = matrix.m[0][0]*matrix.m[2][2] - matrix.m[0][2]*matrix.m[2][0];
    inverse.m[1][2] = -(matrix.m[0][0]*matrix.m[1][2] - matrix.m[0][2]*matrix.m[1][0]);
    inverse.m[2][0] = matrix.m[1][0]*matrix.m[2][1] - matrix.m[1][1]*matrix.m[2][0];
    inverse.m[2][1] = -(matrix.m[0][0]*matrix.m[2][1] - matrix.m[0][1]*matrix.m[2][0]);
    inverse.m[2][2] = matrix.m[0][0]*matrix.m[1][1] - matrix.m[0][1]*matrix.m[1][0];

    float det = matrix.m[0][0]*matrix.m[1][1]*matrix.m[2][2] + matrix.m[1][0]*matrix.m[2][1]*matrix.m[0][2] + matrix.m[2][0]*matrix.m[0][1]*matrix.m[1][2] -
        (matrix.m[0][2]*matrix.m[1][1]*matrix.m[2][0] + matrix.m[2][1]*matrix.m[1][2]*matrix.m[0][0] + matrix.m[2][2]*matrix.m[1][0]*matrix.m[0][1]);
    for (int i = 0; i < 12;) {
        inverse.a[i] /= det;
        i++;
    } 
    
    return inverse;
}

sead::Vector3f mars::calcMtxRot(sead::Matrix33f matrix, sead::Vector3f * pos) {
    sead::Vector3f newPos;
    newPos.x = pos->x*matrix.m[0][0] + pos->y*matrix.m[0][1] + pos->z*matrix.m[0][2];
    newPos.y = pos->x*matrix.m[1][0] + pos->y*matrix.m[1][1] + pos->z*matrix.m[1][2];
    newPos.z = pos->x*matrix.m[2][0] + pos->y*matrix.m[2][1] + pos->z*matrix.m[2][2];
    return newPos;
}

sead::Matrix34f* mars::calcRotMtx(sead::Matrix34f* matrix, sead::Matrix33f rot) {
    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 3; column++) {
            matrix->m[row][column] = matrix->m[0][column]*rot.m[row][0] + matrix->m[1][column]*rot.m[row][1] + matrix->m[2][column]*rot.m[row][2];
        }
    }
    return matrix;
}

void initHackCapHook(al::LiveActor *cappy) {
    al::initActorPoseTQGSV(cappy);
}
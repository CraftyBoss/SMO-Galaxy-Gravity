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

static sead::Vector3f storedUpDir = {0,0,0};
static bool isVertAbsorber = false;
// static sead::Matrix34f playerMtx;
// static sead::Vector3f playerPos = {0,0,0};
static bool isGravity = false;

/*
 * GRAVITY TYPES
 * It's important to note that all of these shapes should use the area closest to their actual shape (AreaCube, AreaSphere, AreaCylinder etc).
 * For the shapes that use a cube, make sure the model name is AreaCubeCenter. For cone, it's recommended to use AreaCubeBase.
 * The base radius of these areas is 500 units. 
 */
enum GravityType : int {
    Spherical = 0, Cuboidal = 1, Parallel = 2, Cylindrical = 3, Torus = 4, Wedge = 5, Disk = 6, Cone = 7
};

enum CubeSide : int {
    faceY = 0, faceX = 1, faceZ = 2, 
    edgeXpZp = 3, edgeXnZp = 4, edgeXpZn = 5, edgeXnZn = 6, 
    edgeXpYp = 7, edgeXnYp = 8, edgeXpYn = 9, edgeXnYn = 10,
    edgeZpYp = 11, edgeZnYp = 12, edgeZpYn = 13, edgeZnYn = 14,
    cornerXpYpZp = 15, cornerXpYpZn = 16, cornerXpYnZp = 17, cornerXpYnZn = 18, 
    cornerXnYpZp = 19, cornerXnYpZn = 20, cornerXnYnZp = 21, cornerXnYnAn = 22
};

void mars::calcGravityDirection(al::LiveActor* player) {
    sead::Vector3f* playerTrans = al::getTrans(player);
    // ::playerPos = *playerTrans;
    // ::playerMtx = player->mPoseKeeper->getMtx();

    al::AreaObjGroup* gravityAreaGroup = al::tryFindAreaObjGroup(player, "GravityArea");
    
    if (gravityAreaGroup) {
        al::AreaObj* gravityArea = gravityAreaGroup->getInVolumeAreaObj(*playerTrans);
    
        if (gravityArea && !rs::isPlayer2D(player)) {
            ::isGravity = true;
            bool isInverted;
            int gravityType;
            al::tryGetAreaObjArg(&gravityType, gravityArea, "GravityType");
            al::tryGetAreaObjArg(&isInverted, gravityArea, "IsInverted");
            sead::Vector3f gravity = {0,0,0};
            sead::Vector3f scale = gravityArea->mAreaShape->mScale;
            sead::Vector3f areaPos =  {gravityArea->mAreaTR.m[0][3], gravityArea->mAreaTR.m[1][3],gravityArea->mAreaTR.m[2][3]};
            sead::Matrix33f areaMtxScaled = {
                gravityArea->mAreaTR.m[0][0]*scale.x, gravityArea->mAreaTR.m[0][1]*scale.x, gravityArea->mAreaTR.m[0][2]*scale.x, 
                gravityArea->mAreaTR.m[1][0]*scale.y, gravityArea->mAreaTR.m[1][1]*scale.y, gravityArea->mAreaTR.m[1][2]*scale.y,
                gravityArea->mAreaTR.m[2][0]*scale.z, gravityArea->mAreaTR.m[2][1]*scale.z, gravityArea->mAreaTR.m[2][2]*scale.z};
            sead::Matrix33f areaMtx = {
                gravityArea->mAreaTR.m[0][0], gravityArea->mAreaTR.m[0][1], gravityArea->mAreaTR.m[0][2], 
                gravityArea->mAreaTR.m[1][0], gravityArea->mAreaTR.m[1][1], gravityArea->mAreaTR.m[1][2],
                gravityArea->mAreaTR.m[2][0], gravityArea->mAreaTR.m[2][1], gravityArea->mAreaTR.m[2][2]};
            sead::Matrix33f inverseMtx = calcInverseMtx(areaMtx);
            sead::Vector3f dist = {areaPos.x - playerTrans->x, areaPos.y - playerTrans->y, areaPos.z - playerTrans->z};
            dist = calcMtxRot(inverseMtx, &dist);

            switch (gravityType) {
                case Spherical: {
                    // points towards position of the area
                    gravity = dist;
                    isVertAbsorber = false;
                    break;
                }
                case Cuboidal: {
                    // need to rewrite to smooth out corners and edges by defining a base face length for the cube (probably 250u)
                    isVertAbsorber = false;
                    sead::Vector3f dist = {areaPos.x - playerTrans->x, areaPos.y - playerTrans->y, areaPos.z - playerTrans->z};
                    sead::Matrix33f inverseMtx = calcInverseMtx(areaMtxScaled);
                    dist = calcMtxRot(inverseMtx, &dist);
                    float radius = 250;
                    int cubeSide;
                    float distX = abs(dist.x);
                    float distY = abs(dist.y);
                    float distZ = abs(dist.z);
                    if (distY >= distX && distY >= distZ) {
                        if (distX < radius && distZ < radius) {
                            gravity = {0, dist.y, 0}; // y face
                        } else if (distX < radius && distZ > radius) {
                            gravity = {0, -radius*dist.y/distY + dist.y, -radius*dist.z/distZ + dist.z}; // edge 1
                        } else if (distX > radius && distZ < radius) {
                            gravity = {-radius*dist.x/distX + dist.x, -radius*dist.y/distY + dist.y, 0}; // edge 2
                        } else if (distX > radius && distZ > radius) {
                            gravity = {-radius*dist.x/distX + dist.x, -radius*dist.y/distY + dist.y, -radius*dist.z/distZ + dist.z}; // corner
                        }
                    } else if (distX >= distY && distX >=distZ) {
                        if (distY < radius && distZ < radius) {
                            gravity = {dist.x, 0, 0};
                        } else if (distY <= radius && distZ >= radius) {
                            gravity = {-radius*dist.x/distX + dist.x, 0, -radius*dist.z/distZ + dist.z}; // edge 1
                        } else if (distY >= radius && distZ <= radius) {
                            gravity = {-radius*dist.x/distX + dist.x, -radius*dist.y/distY + dist.y, 0}; // edge 2
                        } else if (distY >= radius && distZ >= radius) {
                            gravity = {-radius*dist.x/distX + dist.x, -radius*dist.y/distY + dist.y, -radius*dist.z/distZ + dist.z}; // corner
                        }
                    } else if (distZ >= distY && distZ >= distX) {
                        if (distY < radius && distX < radius) {
                            gravity = {0, 0, dist.z};
                        } else if (distY <= radius && distX >= radius) {
                            gravity = {-radius*dist.x/distX + dist.x, 0, -radius*dist.z/distZ + dist.z}; // edge 1
                        } else if (distY >= radius && distX <= radius) {
                            gravity = {0, -radius*dist.y/distY + dist.y, -radius*dist.z/distZ + dist.z}; // edge 2
                        } else if (distY >= radius && distZ >= radius) {
                            gravity = {-radius*dist.x/distX + dist.x, -radius*dist.y/distY + dist.y, -radius*dist.z/distZ + dist.z}; // corner
                        }
                    }
                    gravity = calcMtxRot(areaMtxScaled, &gravity);
                    if (isInverted) {
                        gravity = {-gravity.x, -gravity.y, -gravity.z};
                    }
                    al::normalize(&gravity);
                    al::setGravity(player, gravity);
                    sead::Vector3f invertedGravity = {-gravity.x, -gravity.y, -gravity.z};
                    storedUpDir = invertedGravity;
                    return;
                }
                case Parallel: {
                    // rotate the cube to the direction wanted. default is down
                    isVertAbsorber = false;
                    gravity = {0.0, -1.0, 0.0};
                    break;
                }
                case Cylindrical: {
                    isVertAbsorber = false;
                    gravity = {dist.x, 0.0, dist.z};
                    break;
                }
                case Torus: {
                    // ngl not even i know how this one works and i figured out the math myself
                    // it's like, a cylindrical sphere, bro
                    isVertAbsorber = false;
                    // base radius is 250; scaling the area will also scale the radius.
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
                    sead::Vector3f invertedGravity = {-gravity.x, -gravity.y, -gravity.z};
                    storedUpDir = invertedGravity;
                    return;
                }
                case Wedge: {
                    isVertAbsorber = false;
                    if (dist.y > 0 && dist.z < 0) {
                        gravity = {0, -dist.y, -dist.z};
                        break;
                    }
                    gravity = *al::getGravity(player);
                    sead::Vector3f invertedGravity = {-gravity.x, -gravity.y, -gravity.z};
                    storedUpDir = invertedGravity;
                    return;
                }
                case Disk: {
                    isVertAbsorber = false;
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
                    sead::Vector3f invertedGravity = {-gravity.x, -gravity.y, -gravity.z};
                    storedUpDir = invertedGravity;
                    return;

                }
                case Cone: {
                    isVertAbsorber = false;
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
                    sead::Vector3f invertedGravity = {-gravity.x, -gravity.y, -gravity.z};
                    storedUpDir = invertedGravity;
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
            storedUpDir = invertedGravity;
        } else if (!gravityArea && !rs::isPlayer2D(player)) {
            ::isGravity = false;
            isVertAbsorber = false;
            PlayerActorHakoniwa* playerActor = al::getPlayerActor(player, 0);
            PlayerColliderHakoniwa* playerCollision = playerActor->mPlayerCollider;
            sead::Vector3f gravity = {0,0,0};
            if (rs::calcOnGroundNormalOrGravityDir(&gravity, playerActor, playerCollision)) {
                storedUpDir = {0,1.0,0.0};
                gravity = {-gravity.x, -gravity.y, -gravity.z};
                al::normalize(&gravity);
                al::setGravity(player, gravity);
            }
        }
    } else if (!rs::isPlayer2D(player)) {
        ::isGravity = false;
        isVertAbsorber = false;
        PlayerActorHakoniwa* playerActor = al::getPlayerActor(player, 0);
        PlayerColliderHakoniwa* playerCollision = playerActor->mPlayerCollider;
        sead::Vector3f gravity = {0,0,0};
        if (rs::calcOnGroundNormalOrGravityDir(&gravity, playerActor, playerCollision)) {
            storedUpDir = {0,1.0,0.0};
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

sead::Matrix33f mars::QuatToMtx33(sead::Quatf* quat) {
    sead::Matrix33f mtx;
    mtx.m[0][0] = 1-2*quat->y*quat->y - 2*quat->z*quat->z;
    mtx.m[0][1] = 2*quat->x*quat->y - 2*quat->z*quat->w;
    mtx.m[0][2] = 2*quat->x*quat->z + 2*quat->y*quat->w;
    mtx.m[1][0] = 2*quat->x*quat->y + 2*quat->z*quat->w;
    mtx.m[1][1] = 1-2*quat->x*quat->x - 2*quat->z*quat->z;
    mtx.m[1][2] = 2*quat->y*quat->z - 2*quat->x*quat->w;
    mtx.m[2][0] = 2*quat->x*quat->z - 2*quat->y*quat->w;
    mtx.m[2][1] = 2*quat->y*quat->z + 2*quat->x*quat->w;
    mtx.m[2][2] = 1-2*quat->x*quat->x - 2*quat->y*quat->y;
    /*
    for (int column = 0; column < 3; column++) {
        for (int row = 0; row < 3; row++) {
            mtx.m[row][column] /= sqrt(mtx.m[0][column]*mtx.m[0][column] + mtx.m[1][column]*mtx.m[1][column] + mtx.m[2][column]*mtx.m[2][column]);
        }
    }*/
    return mtx;
}

void initHackCapHook(al::LiveActor *cappy) {
    al::initActorPoseTQGSV(cappy);
}
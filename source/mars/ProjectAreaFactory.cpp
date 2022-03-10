#include "al/factory/ProjectAreaFactory.h"
#include "al/factory/AreaObjFactoryEntries100.h"

ProjectAreaFactory::ProjectAreaFactory() : AreaObjFactory("エリア生成") {
    this->actorTable = areaEntries;
    this->factoryCount = sizeof(areaEntries)/sizeof(areaEntries[0]);
    this->mGraphicsArea = &_graphicsAreaUnk;
};
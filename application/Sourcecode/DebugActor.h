#pragma once
#include "IActor.h"
class DebugActor :
    public IActor
{
private:
    Sphere col_;
public:
    DebugActor();
    void PreUpdate();
    void CollisionUpdate();
    void PostUpdate();

    void Draw();
public:
    Object3d* GetDisplayObj() { return displayObj_.get(); }
    Object3d* GetObj() { return obj_.get(); }
};


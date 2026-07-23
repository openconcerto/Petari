#pragma once

#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"

class MultiEventCamera {
public:
    MultiEventCamera();

    bool isEnd() const;
    void setUp(const char*, const ActorCameraInfo*, s32);
    void setEndCameraTypeSoon();
    void setEndCameraTypeAtLanding();
    void start(s32);
    void start(const CameraTargetArg&, s32);
    void update();
    void nextForce();
    void endForce();
    void endForceSoon();
    void endForceAtLanding();
    void changeTargetPlayer();
    bool isActive(s32) const;
    void declareEventCamera(s32) NO_INLINE;
    void startCamera();
    void endCamera(s32);
    void changeTarget(const CameraTargetArg&);

    const char* mName;  // 0x0
    s32 mCameraCount;   // 0x4
    s32 mCurrentCameraIndex;
    s32 mUpdateCounter;
    s32 mCurrentCameraFrameCount;
    s32 mCameraType;                     // 0x14
    const ActorCameraInfo* mCameraInfo;  // 0x18
    CameraTargetArg mTarget;             // 0x1C
};

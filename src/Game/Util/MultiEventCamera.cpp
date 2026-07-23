#include "Game/Util/MultiEventCamera.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include <cstdio>

#define CAMERA_FORMAT "%s:%03d:%02d番目"
#define TYPE_SOON 0
#define TYPE_AT_LANDING 1

MultiEventCamera::MultiEventCamera()
    : mName(nullptr), mCameraCount(0), mCurrentCameraIndex(-1), mUpdateCounter(0), mCurrentCameraFrameCount(0), mCameraType(TYPE_SOON),
      mCameraInfo(nullptr), mTarget() {
}

bool MultiEventCamera::isEnd() const {
    return mCurrentCameraIndex < 0;
}

void MultiEventCamera::setUp(const char* pName, const ActorCameraInfo* pCameraInfo, s32 cameraCount) {
    mName = pName;
    mCameraInfo = pCameraInfo;
    mCameraCount = cameraCount;

    for (int i = 0; i < mCameraCount; i++) {
        declareEventCamera(i);
    }
}

void MultiEventCamera::setEndCameraTypeSoon() {
    mCameraType = TYPE_SOON;
}

void MultiEventCamera::setEndCameraTypeAtLanding() {
    mCameraType = TYPE_AT_LANDING;
}

void MultiEventCamera::start(s32 index) {
    CameraTargetArg target;
    start(target, index);
}

void MultiEventCamera::start(const CameraTargetArg& rTarget, s32 index) {
    if (mCameraCount <= 0) {
        return;
    }

    mUpdateCounter = 0;
    mCurrentCameraIndex = index;
    mTarget = rTarget;
    startCamera();
}

void MultiEventCamera::update() {
    if (isEnd()) {
        return;
    }

    if (mCurrentCameraFrameCount <= mUpdateCounter && mCurrentCameraIndex < mCameraCount - 1) {
        mUpdateCounter = 0;
        mCurrentCameraIndex++;
        startCamera();
    }

    mUpdateCounter++;
}

void MultiEventCamera::nextForce() {
    if (isEnd()) {
        return;
    }

    if (mCurrentCameraIndex < mCameraCount - 1) {
        mUpdateCounter = 0;
        mCurrentCameraIndex++;
        startCamera();
    } else {
        endCamera(mCameraType);
    }
}

void MultiEventCamera::endForce() {
    if (mCurrentCameraIndex >= 0) {
        endCamera(mCameraType);
    }
}

void MultiEventCamera::endForceSoon() {
    if (mCurrentCameraIndex >= 0) {
        endCamera(TYPE_SOON);
    }
}

void MultiEventCamera::endForceAtLanding() {
    if (mCurrentCameraIndex >= 0) {
        endCamera(1);
    }
}

void MultiEventCamera::changeTargetPlayer() {
    CameraTargetArg target;
    MR::setCameraTargetToPlayer(&target);
    changeTarget(target);
}

bool MultiEventCamera::isActive(s32 index) const {
    char cameraName[256];
    snprintf(cameraName, sizeof(cameraName), CAMERA_FORMAT, mName, mCameraInfo->mCameraSetID, index);
    return MR::isEventCameraActive(mCameraInfo, cameraName);
}

void MultiEventCamera::declareEventCamera(s32 index) {
    char cameraName[256];
    snprintf(cameraName, sizeof(cameraName), CAMERA_FORMAT, mName, mCameraInfo->mCameraSetID, index);
    MR::declareEventCamera(mCameraInfo, cameraName);
}

void MultiEventCamera::startCamera() {
    char cameraName[256];
    snprintf(cameraName, sizeof(cameraName), CAMERA_FORMAT, mName, mCameraInfo->mCameraSetID, mCurrentCameraIndex);
    MR::startEventCamera(mCameraInfo, cameraName, mTarget, -1);
    mCurrentCameraFrameCount = MR::getEventCameraFrames(mCameraInfo, cameraName);
}

void MultiEventCamera::endCamera(s32 type) {
    char cameraName[256];
    snprintf(cameraName, sizeof(cameraName), CAMERA_FORMAT, mName, mCameraInfo->mCameraSetID, mCurrentCameraIndex);

    switch (type) {
    case TYPE_SOON:
        MR::endEventCamera(mCameraInfo, cameraName, true, -1);
        break;
    case TYPE_AT_LANDING:
        MR::endEventCameraAtLanding(mCameraInfo, cameraName, -1);
        break;
    }

    mCurrentCameraIndex = -1;
}

void MultiEventCamera::changeTarget(const CameraTargetArg& rTarget) {
    char cameraName[256];
    snprintf(cameraName, sizeof(cameraName), CAMERA_FORMAT, mName, mCameraInfo->mCameraSetID, mCurrentCameraIndex);
    MR::changeEventCameraTarget(mCameraInfo, cameraName, rTarget);
}

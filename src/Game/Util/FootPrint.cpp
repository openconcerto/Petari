#include "Game/Util/FootPrint.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>

// to match original
static inline f32 getZero() {
    return 0.0f;
}

FootPrint::FootPrint(const char* pName, s32 printNum, s32 drawOrder) : NameObj(pName) {
    getZero();
    initMember(printNum, drawOrder);
}

FootPrint::FootPrint(const char* pName, s32 printNum) : NameObj(pName) {
    initMember(printNum, 23);
}

void FootPrint::initMember(s32 printNum, s32 drawOrder) {
    mTexture = nullptr;
    mPrints = nullptr;
    mMaxNum = 0;
    mNum = 0;
    mIndex = 0;
    mSideOffset = 20.0f;
    mWidth = 20.0f;
    mLength = 20.0f;
    mMinDistance = 80.0f;
    mLastAddPos.z = getZero();
    mLastAddPos.y = getZero();
    mLastAddPos.x = getZero();

    mPrints = new FootPrintInfo[printNum];
    mMaxNum = printNum;

    for (s32 i = 0; i < mMaxNum; i++) {
        mPrints[i].mIsValid = false;
    }

    MR::connectToScene(this, 34, -1, -1, drawOrder);
}

FootPrintInfo::FootPrintInfo() {
}

void FootPrint::setTexture(ResTIMG* pImg) {
    MR::CurrentHeapRestorer restorer(MR::getSceneHeapGDDR3());
    mTexture = new JUTTexture(pImg, 0);
}

void FootPrint::movement() {
    if (mIsClearing) {
        for (s32 i = 0; i < mNum; i++) {
            if (mPrints[i].mIsValid) {
                mPrints[i].mLife--;

                if (mPrints[i].mLife <= 0) {
                    mPrints[i].mIsValid = false;
                }
            }
        }
    } else {
        for (s32 i = mIndex; i < mIndex + 10; i++) {
            s32 idx = i;

            if (i >= mMaxNum) {
                idx = i - mMaxNum;
            }

            if (mPrints[idx].mIsValid) {
                mPrints[idx].mLife--;

                if (mPrints[idx].mLife <= 0) {
                    mPrints[idx].mIsValid = false;
                }
            }
        }
    }
}

bool FootPrint::addPrint(const TVec3f& rPosition, const TVec3f& rDirection, const TVec3f& rNormal, bool mirror) {
    mIsClearing = false;

    if (mNum > 0 && rPosition.distance(mLastAddPos) < mMinDistance) {
        return false;
    }

    mLastAddPos = rPosition;
    mPrints[mIndex].mPosition = rPosition;
    mPrints[mIndex].mDirection = rDirection;
    mPrints[mIndex].mNormal = rNormal;
    mPrints[mIndex].mLife = 60;
    mPrints[mIndex].mIsValid = true;
    mPrints[mIndex].mMirror = mirror;
    mIndex++;
    mNum++;

    if (mIndex >= mMaxNum) {
        mIndex -= mMaxNum;
    }

    if (mNum > mMaxNum) {
        mNum = mMaxNum;
    }

    return true;
}

void FootPrint::draw() const {
    if (mNum <= 0) {
        return;
    }

    TDDraw::setup(2, 1, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    mTexture->load(GX_TEXMAP0);

    Color8 color(0, 0, 0, 0xFF);

    for (s32 i = 0; i < mNum; i++) {
        if (mPrints[i].mIsValid) {
            f32 alpha = MR::normalize(mPrints[i].mLife, 0.0f, 60.0f);
            color.a = 255.0f * alpha;
            GXSetTevColor(GX_TEVREG0, color);

            TVec3f side;
            PSVECCrossProduct(&mPrints[i].mDirection, &mPrints[i].mNormal, &side);

            TVec3f pos;

            if (i & 1) {
                pos = mPrints[i].mPosition + side * mSideOffset;
            } else {
                pos = mPrints[i].mPosition - side * mSideOffset;
            }

            pos += mPrints[i].mNormal * 5.0f;
            TDDraw::drawTexture3D(pos, mPrints[i].mNormal, mPrints[i].mDirection, mLength, mWidth, nullptr, mPrints[i].mMirror, false);
        }
    }

    TDDraw::close();
}

void FootPrint::clear() {
    mIsClearing = true;
}

void FootPrint::clearForce() {
    mNum = 0;
    mIndex = 0;
}

TVec3f* FootPrint::getPrintPos(u32 idx) const {
    return &mPrints[idx % mMaxNum].mPosition;
}

void FootPrint::invalidate(u32 idx) {
    mPrints[idx % mMaxNum].mIsValid = false;
}

bool FootPrint::isValid(u32 idx) const {
    return mPrints[idx % mMaxNum].mIsValid;
}

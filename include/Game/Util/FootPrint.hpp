#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class ResTIMG;
class JUTTexture;

class FootPrintInfo {
public:
    FootPrintInfo();

    /* 0x00 */ TVec3f mPosition;
    /* 0x0C */ TVec3f mDirection;
    /* 0x18 */ TVec3f mNormal;
    /* 0x24 */ s32 mLife;
    /* 0x28 */ u8 mIsValid;
    /* 0x29 */ u8 mMirror;
};

class FootPrint : public NameObj {
public:
    FootPrint(const char* pName, s32 printNum);
    FootPrint(const char* pName, s32 printNum, s32 drawOrder);

    virtual void movement();
    virtual void draw() const;

    void initMember(s32 printNum, s32 drawOrder);
    void setTexture(ResTIMG* pImg);
    bool addPrint(const TVec3f& rPosition, const TVec3f& rDirection, const TVec3f& rNormal, bool mirror);
    void clear();
    void clearForce();
    TVec3f* getPrintPos(u32 idx) const;
    void invalidate(u32 idx);
    bool isValid(u32 idx) const;

    /* 0x0C */ JUTTexture* mTexture;
    /* 0x10 */ FootPrintInfo* mPrints;
    /* 0x14 */ TVec3f mLastAddPos;
    /* 0x20 */ s32 mMaxNum;
    /* 0x24 */ s32 mNum;
    /* 0x28 */ s32 mIndex;
    /* 0x2C */ f32 mSideOffset;
    /* 0x30 */ f32 mWidth;
    /* 0x34 */ f32 mLength;
    /* 0x38 */ f32 mMinDistance;
    /* 0x3C */ u8 mIsClearing;
};

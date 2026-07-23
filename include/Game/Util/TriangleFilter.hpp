#pragma once

class Triangle;

typedef bool (*TriangleFunc)(const Triangle*);

class TriangleFilterBase {
public:
    virtual bool isInvalidTriangle(const Triangle*) const = 0;
};

class TriangleFilterFunc : public TriangleFilterBase {
public:
    TriangleFilterFunc(TriangleFunc func) {
        mFunction = func;
    }

    virtual bool isInvalidTriangle(const Triangle*) const;

    TriangleFunc mFunction; // 0x4
};

namespace MR {
    TriangleFilterFunc* createTriangleFilterFunc(TriangleFunc);
};  // namespace MR

class TriangleFilterDangerCode : public TriangleFilterBase {
public:
    virtual bool isInvalidTriangle(const Triangle*) const;
};

template < typename T >
class TriangleFilterDelegator : public TriangleFilterBase {
public:
    typedef bool (T::*DelegateFilter)(const Triangle*);
    TriangleFilterDelegator(T* parent, DelegateFilter filter) : mParent(parent), mFunc(filter) {
    }

    virtual bool isInvalidTriangle(const Triangle*) const;

    static inline TriangleFilterDelegator< T >* allocateDelegator(T* parent, DelegateFilter filter) {
        return new TriangleFilterDelegator< T >(parent, filter);
    }

    T* mParent;            // 0x4
    DelegateFilter mFunc;  // 0x10
};

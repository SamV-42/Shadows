#ifndef AABB_HPP
#define AABB_HPP

#include <cmath>
#include "Point.hpp"

#include <iostream>

class AABB {
public:
  AABB(double centerX, double centerY, double centerZ,
    double halfLengthX, double halfLengthY, double halfLengthZ) :
    mCenterX(centerX), mCenterY(centerY), mCenterZ(centerZ),
    mHalfLengthX(halfLengthX), mHalfLengthY(halfLengthY), mHalfLengthZ( halfLengthZ)
  {  }

  AABB(Point center, double halfLengthX, double halfLengthY, double halfLengthZ) :
    AABB(center.x, center.y, center.z, halfLengthX, halfLengthY, halfLengthZ) { }

  AABB(Point A, Point B) :
    AABB( (A.x + B.x)/2, (A.y + B.y)/2, (A.z + B.z)/2,
      std::abs(A.x - B.x), std::abs(A.y - B.y), std::abs(A.z - B.z) ) { }

  inline double getCenterX() const { return mCenterX; }
  inline double getCenterY() const { return mCenterY; }
  inline double getCenterZ() const { return mCenterZ; }
  inline Point getCenterPoint() const { return Point(mCenterX, mCenterY, mCenterZ); }

  inline double getMinX() const { return mCenterX - mHalfLengthX; }
  inline double getMinY() const { return mCenterY - mHalfLengthY; }
  inline double getMinZ() const { return mCenterZ - mHalfLengthZ; }

  inline double getMaxX() const { return mCenterX + mHalfLengthX; }
  inline double getMaxY() const { return mCenterY + mHalfLengthY; }
  inline double getMaxZ() const { return mCenterZ + mHalfLengthZ; }

  inline double getHalfLengthX() const { return mHalfLengthX; }
  inline double getHalfLengthY() const { return mHalfLengthY; }
  inline double getHalfLengthZ() const { return mHalfLengthZ; }

  inline double getLengthX() const { return 2 * mHalfLengthX; }
  inline double getLengthY() const { return 2 * mHalfLengthY; }
  inline double getLengthZ() const { return 2 * mHalfLengthZ; }

  inline void setCenterX(double centerX) { mCenterX = centerX; }
  inline void setCenterY(double centerY) { mCenterY = centerY; }
  inline void setCenterZ(double centerZ) { mCenterZ = centerZ; }
  inline void setCenterPoint(Point & center) { mCenterX = center.x; mCenterY = center.y; mCenterZ = center.z; }

  inline void setHalfLengthX(double halfLengthX) { mHalfLengthX = halfLengthX; }
  inline void setHalfLengthY(double halfLengthY) { mHalfLengthY = halfLengthY; }
  inline void setHalfLengthZ(double halfLengthZ) { mHalfLengthZ = halfLengthZ; }

  const bool intersect(const AABB& A) {
    return intersectAABB(A, *this);
  }

  const bool intersect(const Point& P) {
    return intersectPoint(P, *this);
  }

  AABB translate(double x, double y, double z) {
    return AABB(mCenterX + x, mCenterY + y, mCenterZ + z, mHalfLengthX, mHalfLengthY, mHalfLengthZ);
  } //compiler, please optimize this away...

  static const bool intersectAABB(const AABB& A, const AABB& B) {
    return  ( A.getMaxX() >= B.getMinX() && A.getMinX() <= B.getMaxX() )
      &&    ( A.getMaxY() >= B.getMinY() && A.getMinY() <= B.getMaxY() )
      &&    ( A.getMaxZ() >= B.getMinZ() && A.getMinZ() <= B.getMaxZ() );
  }

  static const bool intersectPoint(const Point& P, const AABB& A) {
    return  ( P.x >= A.getMinX() && P.x <= A.getMaxX() )
      &&    ( P.y >= A.getMinY() && P.y <= A.getMaxY() )
      &&    ( P.z >= A.getMinZ() && P.z <= A.getMaxZ() );
  }

  static const bool intersectPoint(const AABB& A, const Point& P) {
    return intersectPoint(P, A);
  }

  void testingPrint() {
    std::cout << mCenterX << " " << mCenterY << " " << mCenterZ << " " << mHalfLengthX << " " << mHalfLengthY << " " << mHalfLengthZ;
  }

private:
  double mCenterX, mCenterY, mCenterZ, mHalfLengthX, mHalfLengthY, mHalfLengthZ;

};

#endif

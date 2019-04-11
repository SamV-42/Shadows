#ifndef POINT_HPP
#define POINT_HPP

class Point {
public:
  Point() : Point(0, 0, 0) {}
  Point(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

  double x, y, z;
  //all public, just because this specific class will get used so frequently
};
#endif

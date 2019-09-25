
#include "AABB.hpp"
#include "Octree.h"

#include <iostream>

int main() {

  AABB levelBoundary = AABB(0,0,0,20,20,20);
  Octree* tester = new Octree(nullptr, levelBoundary, 5, 3);

  std::shared_ptr<AABB> boxes[] = {
    std::make_shared<AABB> (2,0.8,2,1,1,1),
    std::make_shared<AABB> (2.5,0.8,2,1,1,1),
    std::make_shared<AABB> (3.25,0.8,2,1,1,1),
    std::make_shared<AABB> (4.25,0.8,2,1,1,1),
    std::make_shared<AABB> (-5,0.8,2,1,1,1),
    std::make_shared<AABB> (-8,0.8,2,1,1,1),
    std::make_shared<AABB> (2,-5,2,1,1,1),
    std::make_shared<AABB> (2,-8,2,1,1,1),
    std::make_shared<AABB> (0,0,0,1,1,1) };

    for(int i = 0; i < 9; ++i) {
      tester->addMember(boxes[i]);
    }

    //Octree::level = 1;
    //tester->testingPrint();
    AABB jdskf = AABB(0,4,0,2,2.5,2);
    std::shared_ptr<AABB> x = tester->checkCollision(jdskf) ;
    if(x != nullptr) {
      x->testingPrint();
    }
    std::cout << (x == nullptr ? "Null" : "" ) << std::endl;
};

#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <memory>

#include "AABB.hpp"

class Octree {
//specific to AABBs -- this could maybe be made generic, but the req of center + widths is kind of specific
public:
  Octree(Octree* parent, AABB& boundary, int maxPopulation, int maxDepth, int currDepth = 0);
  ~Octree();

  void addMember(std::shared_ptr<AABB> aabb);

  std::shared_ptr<AABB> checkCollision(AABB & aabb);  //compare to nullptr for true/false

  static int level;
  void testingPrint(int nodeIndex = -1);

private:
  bool isLeaf;
  Octree* mNodes[8];
  Octree* mParent;
  std::vector<std::shared_ptr<AABB> > mSplittableMembers;
  std::vector<std::shared_ptr<AABB> > mChildrenMembers;

  AABB mBoundary;

  int mMaxPopulation;
  int mMaxDepth;
  int mCurrDepth;

  void addMember_private(std::shared_ptr<AABB> aabb);
  void updateTree();
};

#endif

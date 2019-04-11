#include "Octree.h"

#include <iostream>
int Octree::level = 1;
void Octree::testingPrint(int nodeIndex) {
  if(mChildrenMembers.size() == 0 && mSplittableMembers.size() == 0 && isLeaf) {
    return;
  }
  if(nodeIndex != -1) {
    std::cout << "\n#:" << nodeIndex << std::endl;
  }
  mBoundary.testingPrint(); std::cout << std::endl;
  std::cout << "Item Lvl|" << level << std::endl;
  for(int i = 0; i < mChildrenMembers.size(); ++i) {
    (mChildrenMembers[i])->testingPrint();
    std::cout << std::endl;
  }
  level *= 10;
  if(isLeaf) {
    for(int i = 0; i < mSplittableMembers.size(); ++i) {
      (mSplittableMembers[i])->testingPrint();
      std::cout << std::endl;
    }
    std::cout << "Leaf" << std::endl;
  } else {
    for(int i = 0; i < 8; ++i) {
      mNodes[i]->testingPrint(i);
    }
  }
  level /= 10;
}

void Octree::addMember_private(std::shared_ptr<AABB> aabbPtr) {
  if(aabbPtr->intersect(mBoundary.getCenterPoint())) {
    mChildrenMembers.push_back(aabbPtr);
  } else {  //This check to approximate if something overlaps so many octants that it's not worth adding to children
    mSplittableMembers.push_back(aabbPtr);
  }
}

void Octree::addMember(std::shared_ptr<AABB> aabb) {  //since it calls update, try to preconstruct any octrees
  addMember_private(aabb);
  updateTree();
}

Octree::Octree(Octree* parent, AABB & boundary, int maxPopulation, int maxDepth, int currDepth)
  : isLeaf(true), mParent(parent), mBoundary(boundary), mMaxPopulation(maxPopulation), mMaxDepth(maxDepth), mCurrDepth(currDepth) {
  //prereq: assume each member is already at least partially within the current node
  mSplittableMembers.reserve(maxPopulation + 2);  //2 extra, just in case of addmember_private without updateTree
}

void Octree::updateTree() { //Hey! This is probably going to be really slow, don't call it mid-gameplay!
  if(mSplittableMembers.size() > mMaxPopulation && mCurrDepth < mMaxDepth) {
    isLeaf = false;
    int index = -1;
    for(int i = 1; i >= -1; i -= 2) {
      for(int j = 1; j >= -1; j -= 2) {
        for(int k = 1; k >= -1; k -= 2) {
          ++index;
          AABB newBoundary = AABB(mBoundary.getCenterPoint(),
            mBoundary.getHalfLengthX() / 2, mBoundary.getHalfLengthY() / 2, mBoundary.getHalfLengthZ() / 2);
          newBoundary = newBoundary.translate(
            mBoundary.getHalfLengthX() / 2 * i, mBoundary.getHalfLengthY() / 2 * j, mBoundary.getHalfLengthZ() / 2 * k);

          mNodes[index] = new Octree(this, newBoundary, mMaxPopulation, mMaxDepth, mCurrDepth + 1);

          for(auto & aabbPtr : mSplittableMembers) {
            if(aabbPtr->intersect(newBoundary)) {
              mNodes[index]->addMember_private(aabbPtr);
            }
          }
          mNodes[index]->updateTree();
        }
      }
    }

  }
}

Octree::~Octree() {
  //fwiw since I haven't really used destructors: they should automatically handle deleting each member
  if(!isLeaf) {
    for(int i = 0; i < 8; ++i) {
      delete mNodes[i];
    }
  }
}


std::shared_ptr<AABB> Octree::checkCollision(AABB & aabb) {
  for(std::shared_ptr<AABB> aabbPtr : mChildrenMembers) {
    if(aabbPtr->intersect(aabb)) {
      return aabbPtr;
    }
  }
  if(isLeaf) {
    for(std::shared_ptr<AABB> aabbPtr : mSplittableMembers) {
      if(aabbPtr->intersect(aabb)) {
        return aabbPtr;
      }
    }
  } else {
    /*for(int i = 0; i < 8; ++i) {  //replace this, we can do it more efficiently
      if(mNodes[i]->mBoundary.intersect(aabb)) {
        std::shared_ptr<AABB> result = mNodes[i]->checkCollision(aabb);
        if(result != nullptr) {
          return result;
        }
      }
    }*/
    unsigned char enabled = 0xFF;
    unsigned char left = aabb.getMinX() > mBoundary.getCenterX() ? 0x55 : 0xFF; // 0101 0101 - turn off 1357
    unsigned char right = aabb.getMaxX() < mBoundary.getCenterX() ? 0xAA : 0xFF; // 1010 1010 - turn off 0246
    unsigned char top = aabb.getMaxY() < mBoundary.getCenterY() ? 0xCC : 0xFF; // 1100 1100  - turn off 0145
    unsigned char bottom = aabb.getMinY() > mBoundary.getCenterY() ? 0x33 : 0xFF; // 0011 0011 - turn off 2367
    unsigned char front = aabb.getMinZ() > mBoundary.getCenterZ() ? 0xF0 : 0xFF; // 1111 0000  - turn off 0123
    unsigned char back = aabb.getMaxZ() < mBoundary.getCenterZ() ? 0x0F : 0xFF; // 0000 1111 - turn off 4567
    enabled = enabled & left & right & top & bottom & front & back;

    std::cout << "HI|" << std::endl << std::hex << (int)enabled << std::endl;

    for(int i = 0; i < 8; ++i) {
      if(enabled & (0x01 << i) != 0x00) {
        std::shared_ptr<AABB> result = mNodes[i]->checkCollision(aabb);
        if(result != nullptr) {
          return result;
        }
      }
    }
  }
  return nullptr;
}

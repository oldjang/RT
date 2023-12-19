cmake_minimum_required(VERSION 3.10)
project(Sigmod16)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(Sigmod16 preMain.cpp FileIO/LogGen_IO.cpp FileIO/QueryGen_IO.cpp FileIO/ResultFile_IO.cpp GlobalVars.cpp BFNaive.cpp Query.cpp QueryBase.cpp ToolFunctions.cpp IntervalTree/IntervalTree.cpp IntervalTree/IntervalTreeLog.cpp IntervalTree/IntervalTreeNode.cpp Logarithmic/AbsStructure.cpp Logarithmic/LogarithmicFramework.cpp MyHeap/MinHeap.cpp MyHeap/MaxHeap.cpp MyHeap/MyHeap.cpp MyHeap/MyHeap.h MyRandom/MyRandom.cpp MyVector/MyVectorInt.cpp Rtree/Rectangle.cpp Rtree/Rtree.cpp Rtree/RtreeNode.cpp Rtree/Zorder.cpp SegmentTree/SegmentTree.cpp SegmentTree/SegmentTreeLog.cpp SegmentTree/TreeNode.cpp SegmentTreePure/SegmentTreePure.cpp SegmentTreePure/SegmentTreePureLog.cpp SegmentTreePure/TreeNodePure.cpp main.cpp SkipListNode.hpp Range.hpp BucketElement.hpp Buckets.hpp global.hpp DataMaker.hpp HighDimSkipList.cpp HighDimSkipList.h HighDimSkipListNode.cpp HighDimSkipListNode.h)

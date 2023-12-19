//
// Created by Mengxu Jiang on 19/12/2023.
//

#include "HighDimSkipListNode.h"
#include "HighDimSkipList.h"

HighDimSkipListNode *HighDimSkipListNode::nextNode(void *SL) {
    HighDimSkipList *S = ( HighDimSkipList *)SL;
    return SL->sl;
}

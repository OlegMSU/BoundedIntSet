#ifndef BOUNDEDINTSET
#define BOUNDEDINTSET



#pragma once

template <class NumType = unsigned, class SizeType = unsigned, class Allocator = std::allocator<unsigned>>
class BoundedIntSet
{
public:
    Allocator SetAllocator;
    SizeType max_size;
    SizeType size;
    NumType* set_ptr;
    BoundedIntSet(SizeType maxsize);
    BoundedIntSet(SizeType maxsize, NumType single_element);
    void Unite(BoundedIntSet<NumType, SizeType, Allocator>* second_set);
    bool UnionCheck(BoundedIntSet<NumType, SizeType, Allocator>* second_set);
    bool Add(NumType new_element, bool checked);
    void Print();
    SizeType Find(NumType num);
    ~BoundedIntSet();
};

#define BOUNDEDINTSET_FUNCTIONS
#include "Source.cpp"

#endif

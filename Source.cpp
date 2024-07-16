#ifndef BOUNDEDINTSET_FUNCTIONS
#include "Source.h"




template <class NumType, class SizeType, class Allocator>
BoundedIntSet<NumType, SizeType, Allocator>::BoundedIntSet(SizeType maxsize)
{
	max_size = maxsize;
	size = 0;
	set_ptr = SetAllocator.allocate(maxsize);
}

template <class NumType, class SizeType, class Allocator>
BoundedIntSet<NumType, SizeType, Allocator>::BoundedIntSet(SizeType maxsize, NumType single_element)
{
	max_size = maxsize;
	size = 0;
	set_ptr = SetAllocator.allocate(maxsize);
	this->Add(single_element, 1);
}

template <class NumType, class SizeType, class Allocator>
void BoundedIntSet<NumType, SizeType, Allocator>::Unite(BoundedIntSet<NumType, SizeType, Allocator>* second_set)
{
	SizeType index = 0;
	SizeType n = this->size;
	SizeType m = second_set->size;
	SizeType i = 0, j = 0;
	while ((i < n) && (j < m))
	{
		if (this->set_ptr[i] == second_set->set_ptr[j])
		{
			i++;
			j++;
		}
		else {
			if (this->set_ptr[i] < second_set->set_ptr[j]) {
				i++;
			}
			else {
				this->Add(second_set->set_ptr[j], 1);
				j++;
			}
		}
	}
	while (j < m) {
		this->Add(second_set->set_ptr[j], 1);
		j++;
	}
}

template <class NumType, class SizeType, class Allocator>
bool BoundedIntSet<NumType, SizeType, Allocator>::UnionCheck(BoundedIntSet<NumType, SizeType, Allocator>* second_set)
{

	SizeType cnt = 0;
	SizeType n = this->size;
	SizeType m = second_set->size;
	SizeType i = 0, j = 0;
	while ((i < n) && (j < m))
	{
		if (this->set_ptr[i] == second_set->set_ptr[j])
		{
			cnt++;
			i++;
			j++;
		}
		else {
			if (this->set_ptr[i] < second_set->set_ptr[j]) {
				cnt++;
				i++;
			}
			else {
				cnt++;
				j++;
			}
		}
	}
	while (i < n) {
		cnt++;
		i++;
	}
	while (j < m) {
		cnt++;
		j++;
	}
	if ((cnt <= this->max_size) && (cnt > this->size)) return true;
	else return false;
}

template <class NumType, class SizeType, class Allocator>
bool BoundedIntSet<NumType, SizeType, Allocator>::Add(NumType new_element, bool checked)
{
	if ((this->size + 1 <= this->max_size) && (checked || (this->Find(new_element) == this->max_size)))
	{
		SizeType i = 0;
		while ((this->set_ptr[i] < new_element) && (i < this->size)) i++;
		SizeType j = this->size;
		while ((j > i))
		{
			this->set_ptr[j] = this->set_ptr[j - 1];
			j--;
		}
		this->set_ptr[i] = new_element;
		this->size++;
	}
	else return false;
}

template <class NumType, class SizeType, class Allocator>
void BoundedIntSet<NumType, SizeType, Allocator>::Print()
{
	for (auto i = 0; i < size; i++)
	{
		std::cout << this->set_ptr[i] << ' ';
	}
	std::cout << '\n';
}

template <class NumType, class SizeType, class Allocator>
SizeType BoundedIntSet<NumType, SizeType, Allocator>::Find(NumType num)
{
	if (this->size == 0) return this->max_size;
	bool result = false;
	SizeType l = 0;
	SizeType r = this->size - 1;
	SizeType mid = (l + r) / 2;
	while ((l <= r) && (result != true) && (mid != 0)) {
		mid = (l + r) / 2;
		if (this->set_ptr[mid] == num) result = true;
		if (this->set_ptr[mid] > num) r = mid - 1;
		else l = mid + 1;
	}
	if (result) return mid;
	else return this->max_size;
}

template <class NumType, class SizeType, class Allocator>
BoundedIntSet<NumType, SizeType, Allocator>::~BoundedIntSet()
{
	SetAllocator.deallocate(set_ptr, max_size);
}

#endif

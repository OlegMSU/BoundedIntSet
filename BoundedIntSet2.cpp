#include <iostream>
#include <unordered_set>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <chrono>
#include <vector>


class BoundedIntSet
{
public:
    unsigned max_size;
    unsigned* set_ptr;
    unsigned size;
    BoundedIntSet(unsigned maxsize)
    {
        max_size = maxsize;
        size = 0;
        set_ptr = (unsigned*)malloc(maxsize * sizeof(unsigned));
    }
    BoundedIntSet(unsigned maxsize, unsigned single_element)
    {
        max_size = maxsize;
        size = 0;
        set_ptr = (unsigned*)malloc(maxsize * sizeof(unsigned));
        this->Add(single_element);
    }
    void Unite(BoundedIntSet* second_set)
    {
        unsigned index = 0; 
        unsigned n = this->size;
        unsigned m = second_set->size;
        unsigned i = 0, j = 0;
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
                    this->Add(second_set->set_ptr[j]);
                    j++;
                }
            }
        }
        while (j < m) {
            this->Add(second_set->set_ptr[j]);
            j++;
        }
    }
    bool UnionCheck(BoundedIntSet* second_set)
    {
        
        unsigned cnt = 0;
        unsigned n = this->size;
        unsigned m = second_set->size;
        unsigned i = 0,  j = 0;
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
        if ((cnt <= this->max_size)&&(cnt > this->size)) return true;
        else return false;
    }
    bool Add(unsigned new_element)
    {
        if ((this->size + 1 <= this->max_size)&&(this->Find(new_element) == this->max_size))
        {
            unsigned i = 0;
            while ((this->set_ptr[i] < new_element) && (i < this->size)) i++;
            unsigned j = this->size;
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

    void Print()
    {
        for (auto i = 0; i < size; i++)
        {
            std::cout << set_ptr[i] << ' ';
        }
        std::cout << '\n';
    }
    unsigned Find(unsigned num)
    {
        if (this->size == 0) return this->max_size;
        bool result = false;
        int l = 0; 
        int r = this->size - 1; 
        int mid;
        while ((l <= r) && (result != true)) {
            mid = (l + r) / 2; 
            if (this->set_ptr[mid] == num) result = true; 
            if (this->set_ptr[mid] > num) r = mid - 1; 
            else l = mid + 1;
        }
        if (result) return mid;
        else return this->max_size;
    }
    ~BoundedIntSet()
    {
        if (set_ptr != NULL) free(set_ptr);
    }
};


int main()
{
    srand(time(0));

    const unsigned amount = 100;
    const unsigned cycles = 100;
    const unsigned k = 20;




    BoundedIntSet* arr[amount];
    std::unordered_set<unsigned>* arr2[amount];
    for (auto i = 0; i < amount; i++)
    {
        unsigned r = rand();
        arr[i] = new BoundedIntSet(k, r);
        arr2[i] = new std::unordered_set<unsigned>{ r };
    }


    auto sum1 = 0;
    auto sum2 = 0;
    auto len = 0;
    for (auto i = 0; i < cycles; i++)
    {
        unsigned i1 = rand() % amount;
        unsigned i2 = rand() % amount;
        auto start_time = std::chrono::steady_clock::now();
        if (arr[i1]->UnionCheck(arr[i2])) {
            auto end_time = std::chrono::steady_clock::now();
            auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
            std::cout << elapsed_ns.count() << " ns ";
            start_time = std::chrono::steady_clock::now();
            arr[i1]->Unite(arr[i2]);
            end_time = std::chrono::steady_clock::now();
            elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
            sum1 += elapsed_ns.count();
            std::cout << elapsed_ns.count() << " ns ";
            start_time = std::chrono::steady_clock::now();
            arr2[i1]->merge(*arr2[i2]);
            end_time = std::chrono::steady_clock::now();
            elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
            sum2 += elapsed_ns.count();
            std::cout << elapsed_ns.count() << " ns \n";
        }
    }


   

    std::cout << sum1 << " ns\n";
    std::cout << sum2 << " ns\n";

    return 0;
}

#include <iostream>
#include <unordered_set>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <chrono>

const unsigned PRIME = 3;

unsigned Hash_fun(unsigned num)
{
    return num % 97;
}

class BoundedIntSet
{
public:
    struct Set_Elem
    {
        unsigned data;
        bool free;
        struct Set_Elem* next;
    };
    unsigned MAXSIZE;
    struct Set_Elem* set_ptr;
    unsigned set_size;
    struct Set_Elem* buf_ptr;
    BoundedIntSet(unsigned maxsize)
    {
        this->MAXSIZE = maxsize;
        set_ptr = (struct Set_Elem*)malloc((MAXSIZE - 1 + PRIME) * sizeof(Set_Elem));
        buf_ptr = set_ptr + PRIME;
        set_size = 0;
        for (auto i = 0; i < (MAXSIZE - 1 + PRIME); i++)
        {
            set_ptr[i].free = 0;
            set_ptr[i].next = NULL;
        }
    }
    BoundedIntSet(unsigned maxsize, unsigned single_element)
    {
        this->MAXSIZE = maxsize;
        set_ptr = (struct Set_Elem*)malloc((MAXSIZE - 1 + PRIME) * sizeof(Set_Elem));
        buf_ptr = set_ptr + PRIME;
        set_size = 0;
        for (auto i = 0; i < (MAXSIZE - 1 + PRIME); i++)
        {
            set_ptr[i].free = 0;
            set_ptr[i].next = NULL;
        }
        this->Add(single_element);
    }
    void Unite(BoundedIntSet* set)
    {
        struct Set_Elem* head;
        for (auto i = 0; i < PRIME; i++)
        {
            head = (set->set_ptr) + i;
            if (head->free)
            {
                if (!this->Find(head->data))
                {
                    this->Add(head->data);
                }
                while (head->next != NULL)
                {
                    head = head->next;
                    if (!this->Find(head->data)) this->Add(head->data);
                }

            }
        }
    }
    bool UnionCheck(BoundedIntSet* second_set)
    {
        unsigned cnt = this->set_size;
        struct Set_Elem* head;
        if (this->MAXSIZE != second_set->MAXSIZE) return false;
        for (auto i = 0; i < PRIME; i++)
        {
            head = (second_set->set_ptr) + i;
            if (head->free)
            {
                if (!this->Find(head->data)) cnt++;
                while (head->next != NULL)
                {
                    head = head->next;
                    if (!this->Find(head->data)) cnt++;
                }

            }
        }
        if (cnt <= MAXSIZE) return true;
        else return false;
    }
    unsigned Add(unsigned new_element)
    {
        if (this->set_size == MAXSIZE) return 0;
        unsigned index = Hash_fun(new_element) % PRIME;
        struct Set_Elem* head = (this->set_ptr) + index;
        if (head->free == 0)
        {
            head->free = 1;
            head->data = new_element;
            this->set_size++;
            return 1;
        }
        else if (head->data == new_element)
        {
            return 0;
        }
        else
        {
            while (head->next != NULL)
            {
                if (head->data == new_element) break;
                head = head->next;
            }
            if (head->data != new_element)
            {
                head->next = buf_ptr;
                head = head->next;
                head->data = new_element;
                head->free = 1;
                buf_ptr += 1;
                this->set_size++;
                return 1;
            }
            return 0;
        }
    }

    void Print()
    {
        for (auto i = 0; i < PRIME; i++)
        {
            struct Set_Elem* head = (this->set_ptr) + i;
            if (head->free)
            {
                std::cout << head->data << ' ';
                while (head->next != NULL)
                {
                    head = head->next;
                    std::cout << head->data << ' ';
                }

            }
        }
        std::cout << '\n';
    }
    bool Find(unsigned num)
    {
        struct Set_Elem* head = this->set_ptr + Hash_fun(num) % PRIME;
        while ((head->data != num) && (head->next != NULL)) head = head->next;
        if (head->data == num) return true;
        else return false;

    }
    ~BoundedIntSet()
    {
        if (set_ptr != NULL) free(set_ptr);
    }
};


int main()
{
    srand(time(0));

    const unsigned amount = 20000;
    const unsigned cycles = 10000;
    const unsigned k = 8;




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

    for (auto i = 0; i < cycles; i++)
    {
        unsigned i1 = rand() % amount;
        unsigned i2 = rand() % amount;
        if (arr[i1]->UnionCheck(arr[i2])) {
            auto start_time = std::chrono::steady_clock::now();
            arr[i1]->Unite(arr[i2]);
            auto end_time = std::chrono::steady_clock::now();
            auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
            sum1 += elapsed_ns.count();
            std::cout << elapsed_ns.count() << " ns ";
            start_time = std::chrono::steady_clock::now();
            arr2[i1]->insert(arr2[i2]->begin(), arr2[i2]->end());
            end_time = std::chrono::steady_clock::now();
            elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
            std::cout << elapsed_ns.count() << " ns\n";
            sum2 += elapsed_ns.count();
        }
    }


   

    std::cout << sum1 << " ns\n";
    std::cout << sum2 << " ns\n";


}

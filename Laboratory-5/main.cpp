#ifndef __MMU__H__
#define __MMU__H__
#define SIZE 65536
#include <iostream>
#include <vector>

typedef short byte_t;

typedef int address_t;

typedef struct hole
{
    address_t adr;
    int sz;
} hole_t;

typedef struct
{
    byte_t mem[SIZE];
    std::vector<hole_t> holes; // holes list
} mem_t;

// dynamically allocates a mem_t structure and initializes its content
mem_t initMem()
{
    mem_t mem1; // we create a mem_t object

    hole_t initHole;
    initHole.adr = 0;
    initHole.sz = SIZE;
    std::vector<hole_t> hole;
    hole.push_back(initHole); // we initialize our hole vector (there is only one hole at the beginning of the code, with a size of 65536)
    mem1.holes = hole;

    return mem1;
}

// allocates space in bytes (byte_t) using First-Fit, Best-Fit or Worst-Fit
address_t myAlloc(mem_t *mp, int sz)
{
    address_t address = 0;

    for (int i = 0; i < mp->holes.size(); ++i)
    { // we check every potential holes
        if (mp->holes[i].sz >= sz)
        { // we check if there is enough space in the hole in order to allocate new memory

            address = mp->holes[i].adr; // we keep in memory the address where something has been allocated

            mp->holes[i].sz -= sz;  // we adjust hole's size after allocation
            mp->holes[i].adr += sz; // we adjust the first address of the hole after allocation
            if (mp->holes[i].sz == 0)
            {
                mp->holes.erase(mp->holes.begin() + i); // if the allocation fill a hole, we remove it from the vector
            }
            i = mp->holes.size(); // we stop the iteration : we've successfuly allocated memory
            return address;       // we return the address where we have started the allocation
        }
    }
    return -1;
}

// release memory that has already been allocated previously
void myFree(mem_t *mp, address_t p, int sz)
{
    for (int i = 0; i < mp->holes.size(); ++i)
    {
        if (mp->holes[i].adr == p + sz && mp->holes[i - 1].adr + mp->holes[i - 1].sz == p)
        {                                            // if we both have a hole behind and after the space we want to free
            mp->holes[i].adr = mp->holes[i - 1].adr; // we modify hole "i" before erasing hole "i - 1"
            mp->holes[i].sz += mp->holes[i - 1].sz;
            mp->holes.erase(mp->holes.begin() + i - 1);
        }
        else if (mp->holes[i].adr == p + sz)
        {                           // if the memory we want to free is located "under" a hole
            mp->holes[i].adr -= sz; // we adjust hole "i"
            mp->holes[i].sz += sz;
        }
        else if (mp->holes[i - 1].adr + mp->holes[i - 1].sz == p)
        { // if the memory we want to free is located on top of hole i - 1
            mp->holes[i].adr -= mp->holes[i - 1].sz;
            mp->holes[i].sz += mp->holes[i - 1].sz;
            mp->holes.erase(mp->holes.begin() + i - 1);
        }
        else
        { // if we're surrounded by 2 spaces allocated
            hole new_Hole;
            new_Hole.adr = p;
            new_Hole.sz = sz;
            if (mp->holes[i].adr > p && mp->holes[i - 1].adr < p)
            {
                mp->holes.insert(mp->holes.begin() + i, new_Hole);
            }
        }
    }
}

// assign a value to a byte
void myWrite(mem_t *mp, address_t p, byte_t val)
{
    mp->mem[p] = val;
}

// read memory from a byte
byte_t myRead(mem_t *mp, address_t p)
{
    return mp->mem[p];
}
#endif

int main()
{
    mem_t tempMem = initMem();
    mem_t *mem = &tempMem; // we put it in a pointer so that we can modifiy it in the functions

    std::cout << "--------- myAlloc ---------" << std::endl;
    address_t adr1 = myAlloc(mem, 5); // we call myAlloc, giving in parameters a space of 5
    std::cout << "Hole 0 Address : " << mem->holes[0].adr << std::endl;
    address_t adr2 = myAlloc(mem, 10);
    std::cout << "Hole 0 Address : " << mem->holes[0].adr << std::endl;
    address_t adr3 = myAlloc(mem, 100);
    std::cout << "Hole 0 Address : " << mem->holes[0].adr << std::endl;

    std::cout << "--------- myFree ---------" << std::endl;

    myFree(mem, adr2, 10); // we want to free from adr2 a space of 10. We will see that 2 holes are created and we will check their adresses
    std::cout << "Hole 0 Address : " << mem->holes[0].adr << std::endl;
    std::cout << "Hole 1 Address : " << mem->holes[1].adr << std::endl;

    myFree(mem, adr1, 5); // we want to free from adr1 a space of 5. We will see that we still have 2 holes and we will check their adresses
    std::cout << "Hole 0 Address : " << mem->holes[0].adr << std::endl;
    std::cout << "Hole 1 Address : " << mem->holes[1].adr << std::endl;

    myWrite(mem, adr3, 543); // write on the 1st byte myWrite(mem, adr3+9, 34); // write on the 10th byte

    std::cout << "--------- myRead ---------" << std::endl;
    byte_t val1 = myRead(mem, adr3);
    byte_t val2 = myRead(mem, adr3 + 9);
    std::cout << "Val1 value : " << val1 << std::endl;
    std::cout << "Val2 value : " << val2 << std::endl;
}
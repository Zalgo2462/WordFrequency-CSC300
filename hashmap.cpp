#include "hashmap.h"
#include <cmath>

template<typename keyT, typename valueT>
void HashMap<keyT, valueT>::resize(int newSize)
{

}

template<typename keyT, typename valueT>
bool HashMap<keyT, valueT>::isPrime(int prime) const
{
    if (prime <= 1)
        return false;
    if (prime == 2)
        return true;
    if (prime % 2 == 0)
        return false;
    int sqrted = sqrt(prime);
    for (int i = 3; i <=sqrted; i += 2)
    {
        if (prime % i == 0)
            return false;
    }
    return true;
}

template<typename keyT, typename valueT>
int HashMap<keyT, valueT>::getNextPrime(int start) const
{
    while(!isPrime(start))
    {
        start++;
    }
    return start;
}

template<typename keyT, typename valueT>
int inline HashMap<keyT, valueT>::probeSequence(int i) const
{

}

template<typename keyT, typename valueT>
HashMap<keyT, valueT>::HashMap(int initCapacity)
{

}

template<typename keyT, typename valueT>
HashMap<keyT, valueT>::HashMap(HashMap<keyT, valueT> & copy)
{

}

template<typename keyT, typename valueT>
HashMap<keyT, valueT>::~HashMap()
{

}

template<typename keyT, typename valueT>
valueT & HashMap<keyT, valueT>::operator[](keyT key) const
{

}

template<typename keyT, typename valueT>
void HashMap<keyT, valueT>::insert(keyT key, valueT value)
{

}

template<typename keyT, typename valueT>
bool HashMap<keyT, valueT>::remove(keyT key)
{

}

template<typename keyT, typename valueT>
bool HashMap<keyT, valueT>::contains(keyT key, valueT value) const
{

}

template<typename keyT, typename valueT>
typename HashMap<keyT, valueT>::MapEntry * HashMap<keyT, valueT>::getEntries() const
{

}

template<typename keyT, typename valueT>
int HashMap<keyT, valueT>::size() const
{

}

template<typename keyT, typename valueT>
int HashMap<keyT, valueT>::capacity() const
{

}

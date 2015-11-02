#ifndef _HASHMAP_H_
#define _HASHMAP_H_
#include <cmath>
#include <stdexcept>

template<typename keyT, typename valueT>
class HashMap
{
public:
    struct MapEntry
    {
        keyT key;
        valueT value;
        bool deleted;
    };
private:
    MapEntry * table;
    int currentSize;
    int maxSize;
    keyT empty;
    void resize(int newSize);
    bool isPrime(int prime) const;
    int getNextPrime(int start) const;
    int inline probeSequence(int i) const;
    int (*hash)(keyT key, int maxSize);

public:
    HashMap(int initCapacity, keyT emptyValue, int (*hash) (keyT key, int maxSize));
    HashMap(HashMap<keyT, valueT> & copy);
    ~HashMap();

    valueT & operator[](keyT key) const;
    void insert(keyT key, valueT value);
    bool remove(keyT key);
    bool contains(keyT key) const;
    MapEntry * getEntries() const;
    int size() const;
    int capacity() const;
};

template<typename keyT, typename valueT>
void HashMap<keyT, valueT>::resize(int newSize)
{
    HashMap<keyT, valueT>::MapEntry * oldTable = table;
    int oldSize = maxSize;

    table = new HashMap<keyT, valueT>::MapEntry[newSize];
    maxSize = newSize;

    for (int i = 0; i < oldSize; i++)
    {
        if(!oldTable[i].deleted && oldTable[i].key != empty)
        {
            insert(oldTable[i].key, oldTable[i].value);
        }
    }
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
    return i * i;
}

template<typename keyT, typename valueT>
HashMap<keyT, valueT>::HashMap(int initMaxSize, keyT emptyValue, int (*hasher) (keyT, int))
{
    empty = emptyValue;
    currentSize = 0;
    maxSize = getNextPrime(initMaxSize);
    table = new HashMap<keyT, valueT>::MapEntry[maxSize];
    hash = hasher;
}

template<typename keyT, typename valueT>
HashMap<keyT, valueT>::HashMap(HashMap<keyT, valueT> & copy)
{
    empty = copy.empty;
    currentSize = copy.currentSize;
    maxSize = copy.maxSize;
    hash = copy.hash;
    table = new HashMap<keyT, valueT>::MapEntry[maxSize];
    for (int i = 0; i < copy.maxSize; i++)
    {
        table[i] = copy.table[i];
    }
}

template<typename keyT, typename valueT>
HashMap<keyT, valueT>::~HashMap()
{
    delete[] table;
}

template<typename keyT, typename valueT>
valueT & HashMap<keyT, valueT>::operator[](keyT key) const
{
    int h = hash(key, maxSize);
    int i = 0;
    int index = (h + probeSequence(i)) % maxSize;
    for (;
        table[index].key != key && table[index].key != empty;
        index = (h + probeSequence(++i)) % maxSize)
    { }
    if (table[index].key == empty)
    {
        throw std::invalid_argument("Key not found in hashmap");
    }
    return table[index].value;
}

template<typename keyT, typename valueT>
void HashMap<keyT, valueT>::insert(keyT key, valueT value)
{
    if (currentSize >= 0.75 * maxSize)
    {
            resize(getNextPrime(2 * maxSize));
    }
    int h = hash(key, maxSize);
    int i = 0;
    int index = (h + probeSequence(i)) % maxSize;
    //Don't have to handle full case since we are rehashing
    for (;
        table[index].key != empty && !table[index].deleted;
        index = (h + probeSequence(++i)) % maxSize)
    { }
    table[index].key = key;
    table[index].value = value;
    table[index].deleted = false;
    currentSize++;
}

template<typename keyT, typename valueT>
bool HashMap<keyT, valueT>::remove(keyT key)
{
    int hash = hash(key, maxSize);
    int i = 0;
    int index = (hash + probeSequence(i)) % maxSize;

    for (;
        table[index].key != key && table[index].key != empty;
        index = (hash + probeSequence(++i)) % maxSize)
    { }

    if (table[index].key == empty)
    {
        return false;
    }

    currentSize--;
    table[index].deleted = true;
    return true;
}

template<typename keyT, typename valueT>
bool HashMap<keyT, valueT>::contains(keyT key) const
{
    int h = hash(key, maxSize);
    int i = 0;
    int index = (h + probeSequence(i)) % maxSize;

    for (;
        table[index].key != key && table[index].key != empty;
        index = (h + probeSequence(++i)) % maxSize)
    { }

    if (table[index].key == empty)
    {
        return false;
    }

    return true;
}

template<typename keyT, typename valueT>
typename HashMap<keyT, valueT>::MapEntry * HashMap<keyT, valueT>::getEntries() const
{
    return table;
}

template<typename keyT, typename valueT>
int HashMap<keyT, valueT>::size() const
{
    return currentSize;
}

template<typename keyT, typename valueT>
int HashMap<keyT, valueT>::capacity() const
{
    return maxSize;
}
#endif

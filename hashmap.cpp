#ifndef _HASHMAP_H_
#define _HASHMAP_H_
#include <cmath>
#include <stdexcept>
#include <iostream>

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
    unsigned int (*hash)(keyT key);

public:
    HashMap(int initCapacity, keyT emptyValue, unsigned int (* hasher) (keyT key));
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
    std::cerr << "Resizing hashmap from " << maxSize << " to " << newSize << std::endl;
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
    delete[] oldTable;
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
HashMap<keyT, valueT>::HashMap(int initMaxSize, keyT emptyValue, unsigned int (*hasher) (keyT))
{
    hash = hasher;
    empty = emptyValue;
    currentSize = 0;
    maxSize = getNextPrime(initMaxSize);
    table = new HashMap<keyT, valueT>::MapEntry[maxSize];
}

template<typename keyT, typename valueT>
HashMap<keyT, valueT>::HashMap(HashMap<keyT, valueT> & copy)
{
    hash = copy.hash;
    empty = copy.empty;
    currentSize = copy.currentSize;
    maxSize = copy.maxSize;
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
    int h = hash(key) % maxSize;
    std::cerr << "[Dereference] Hashing " << key << " to " << h << std::endl;

    int i = 0;
    int index = (h + probeSequence(i)) % maxSize;
    for (;
        table[index].key != key && table[index].key != empty;
        index = (h + probeSequence(++i)) % maxSize)
    {
        std::cerr << "Did not find " << key << " at: " << index << std::endl;
    }
    if (table[index].deleted)
    {
        throw std::invalid_argument("Key has been deleted");
    }
    if (table[index].key == empty)
    {
        throw std::invalid_argument("Key not found in hashmap");
    }
    std::cerr << "Found " << key << " at: " << index << std::endl;
    return table[index].value;
}

template<typename keyT, typename valueT>
void HashMap<keyT, valueT>::insert(keyT key, valueT value)
{
    if (currentSize >= 0.75 * maxSize)
    {
        int newSize = getNextPrime(2 * maxSize);
        resize(newSize);
    }
    int h = hash(key) % maxSize;
    std::cerr << "[Insert] Hashing " << key << " to " << h << std::endl;

    int i = 0;
    int index = (h + probeSequence(i)) % maxSize;
    //Don't have to handle full case since we are rehashing
    for (;
        table[index].key != empty && !table[index].deleted;
        index = (h + probeSequence(++i)) % maxSize)
    {
        std::cerr << "Failed insertion at: " << index << std::endl;
    }
    std::cerr << "Inserted " << key << " in " << index << std::endl;
    table[index].key = key;
    table[index].value = value;
    table[index].deleted = false;
    currentSize++;
}

template<typename keyT, typename valueT>
bool HashMap<keyT, valueT>::remove(keyT key)
{
    int h = hash(key) % maxSize;
    std::cerr << "[Remove] Hashing " << key << " to " << h << std::endl;

    int i = 0;
    int index = (h + probeSequence(i)) % maxSize;

    for (;
        table[index].key != key && table[index].key != empty;
        index = (h + probeSequence(++i)) % maxSize)
    {
        std::cerr << "Did not find " << key << " at: " << index << std::endl;
    }

    if (table[index].key == empty)
    {
        return false;
    }
    currentSize--;
    table[index].deleted = true;
    std::cerr << "Removed " << key << " at: " << index << std::endl;
    return true;
}

template<typename keyT, typename valueT>
bool HashMap<keyT, valueT>::contains(keyT key) const
{
    int h = hash(key) % maxSize;
    std::cerr << "[Contains] Hashing " << key << " to " << h << std::endl;

    int i = 0;
    int index = (h + probeSequence(i)) % maxSize;

    for (;
        table[index].key != key && table[index].key != empty;
        index = (h + probeSequence(++i)) % maxSize)
    {
        std::cerr << "Did not find " << key << " at: " << index << std::endl;
    }
    if (table[index].deleted)
    {
        return false;
    }
    if (table[index].key == empty)
    {
        return false;
    }
    std::cerr << "Found " << key << " at: " << index << std::endl;
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

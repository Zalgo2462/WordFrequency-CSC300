#ifndef _HASHMAP_H_
#define _HASHMAP_H_

template<typename keyT, typename valueT>
class HashMap
{
private:
    void resize(int newSize);
    bool isPrime(int prime) const;
    int getNextPrime(int start) const;
    int inline probeSequence(int i) const;
public:
    struct MapEntry
    {
        keyT key;
        valueT value;
    };
    HashMap(int initCapacity);
    HashMap(HashMap<keyT, valueT> & copy);
    ~HashMap();

    valueT & operator[](keyT key) const;
    void insert(keyT key, valueT value);
    bool remove(keyT key);
    bool contains(keyT key, valueT value) const;
    MapEntry * getEntries() const;
    int size() const;
    int capacity() const;
};
#endif

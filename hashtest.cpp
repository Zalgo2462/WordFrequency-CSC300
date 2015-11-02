#include <iostream>
#include "hashmap.cpp"

using namespace std;

//FNV-1a hash
int stringHash(string s, int maxCapacity)
{
    unsigned int h = 2166136261;
    for (unsigned char c : s)
    {
        h = h * 16777619;
        h = h ^ c;
    }
    return h % maxCapacity;
}

int main()
{
    HashMap<string, int> map(1000, "", stringHash);
    string in;
    int num;
    while (cin >> in && cin >> num)
    {
        if (map.contains(in))
        {
            map[in] += num;
        }
        else
        {
            map.insert(in, num);
        }
        cout << in << ": " << map[in] << endl;
    }
}

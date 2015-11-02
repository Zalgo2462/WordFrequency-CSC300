#include <iostream>
#include "hashmap.cpp"

using namespace std;

//FNV-1a hash
//http://www.isthe.com/chongo/tech/comp/fnv/#FNV-1a
unsigned int stringHash(string s)
{
    unsigned int h = 2166136261;
    for (unsigned char c : s)
    {
        h = h ^ c;
        h = h * 16777619;
    }
    return h;
}

unsigned int badHash(string s)
{
    return 0;
}

int main()
{
    HashMap<string, int> map(1000, "", stringHash);
    string in;
    int num;
    while (cin >> in)
    {
        if (in == "delete")
        {
            cin >> in;
            map.remove(in);
        }
        else
        {
            cin >> num;
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
}

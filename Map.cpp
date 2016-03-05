
#include "Map.h"


Map::Map()
{
    num = -1;
};

bool Map::empty() const
{
    return size()== 0;
};

int Map::size() const
{
    return num+1;
};

bool Map::insert(const KeyType& key, const ValueType& value)
{
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full).
    
    if (size() >= DEFAULT_MAX_ITEMS)
        return false;
    
    if (contains(key))
            return false;
    
    num++;
    data[num].key = key;
    data[num].value = value;
    return true;

};

bool Map::update(const KeyType& key, const ValueType& value)
{
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.
    
    if(empty())
        return false;
    
    for (int i = 0; i < size(); i++)
    {
        if (data[i].key == key)
        {
            data[i].value = value;
            return true;
        }
    }
    
    return false;
    
};

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that the key is not already in the map and the map has a fixed
// capacity and is full).

    if (update(key, value))
        return true;
    else
        return insert(key, value);
    
};
    
bool Map::erase(const KeyType& key)
{
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.
    
    if(empty())
        return false;
    
    if (size() == 1 && data[0].key == key)
    {
        num--;
        return true;
    }
    
    
    for (int i = 0; i < size(); i++)
    {
        if (data[i].key == key)
        {
            if (i == size()-1)
            {
                num--;
                return true;
            }
            else
            {
                data[i].key = data[num].key;
                data[i].value = data[num].value;
                num--;
                return true;
            }
        }
    }
    
    return false;
    
};

bool Map::contains(const KeyType& key) const
{
// Return true if key is equal to a key currently in the map, otherwise
// false.
    
    if(empty())
        return false;
    
    for (int i = 0; i < size(); i++)
    {
        if (data[i].key == key)
            return true;
    }
    
    return false;
    
};

bool Map::get(const KeyType& key, ValueType& value) const
{
// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.
    
    if(empty())
        return false;
 
    for (int i = 0; i < size(); i++)
    {
        if (data[i].key == key)
        {
            value = data[i].value;
            return true;
        }
    }
    
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)
    
    if (i >= 0 && i < size())
    {
        key = data[i].key;
        value = data[i].value;
        return true;
    }
    else
        return false;
};

void Map::swap(Map& other)
{
// Exchange the contents of this map with the other one.

    Map temp = other;
    other = *this;
    *this = temp;
    
};
 


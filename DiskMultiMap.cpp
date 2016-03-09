//
//  DiskMultiMap.cpp
//  Project 4
//
//  Created by ShenYan on 3/7/16.
//  Copyright © 2016 ShenYan. All rights reserved.
//

#include "DiskMultiMap.h"
#include <functional>

//////////////////////////
//      Iterator        //
//////////////////////////

DiskMultiMap::Iterator::Iterator()
{
    m_valid = false;
}

DiskMultiMap::Iterator::Iterator(std::string& file, BinaryFile::Offset startLoc, BinaryFile::Offset bucketLoc)
{
    m_valid = true;
    cur = startLoc;
    m_file = file;
    m_bucketLoc = bucketLoc;
}

bool DiskMultiMap::Iterator::isValid() const
{
    return m_valid;
}

DiskMultiMap::Iterator& DiskMultiMap::Iterator::operator++()
{
        //open the file
    BinaryFile iterBF;
    iterBF.openExisting(m_file);
    
        //find the Association that is pointed to, and find the offset of the next association
    Association currentAsso;
    iterBF.read(currentAsso, cur);
    BinaryFile::Offset next = currentAsso.next;
    
        //if it points back to the bucket, then set m_valid to false
    if(next == m_bucketLoc)
    {
        m_valid = false;
    }
    else    //else, set iterator to the next association
    {
        cur = next;
    }
    
    //iterBF.close();
    
    return *this;
}

MultiMapTuple DiskMultiMap::Iterator::operator*()
{
    string k, v, c;
    
    BinaryFile iterBF;
    iterBF.openExisting(m_file);
    
    Bucket b;
    iterBF.read(b, m_bucketLoc);
    k = b.key;
    
    Association currentAsso;
    iterBF.read(currentAsso, cur);
    v = currentAsso.value;
    c = currentAsso.context;
    
    MultiMapTuple tuple;
    tuple.key = k;
    tuple.value = v;
    tuple.context = c;
    
    return tuple;
}

//////////////////////////////
//      DiskMultiMap        //
//////////////////////////////
DiskMultiMap::DiskMultiMap()
{}

DiskMultiMap::~DiskMultiMap()
{
    bf.close();
}

bool DiskMultiMap::createNew(const std::string& filename, unsigned int numBuckets)
{
    close();
    
    bool success = bf.createNew(filename);
    if(!success)
        return false;
    
    totalBuckets = numBuckets;
    m_fileName = filename;
    
        //initialize hash table
    DiskMultiMap::Header h;
    h.numOfBuckets = numBuckets;
    h.headToDeleted = 0;            //no deleted node
    bf.write(h, 0);
    
    for(int i = 0; i < numBuckets; i++)
    {
        DiskMultiMap::Bucket b;
        strcpy(b.key, "");
        b.used = false;
        b.head = bf.fileLength();   //points to itself
        
        bf.write(b, bf.fileLength());
    }
    
    return true;
}


bool DiskMultiMap::openExisting(const std::string& filename)
{
    close();
    
    bool success  = bf.openExisting(filename);
    return success;
}

void DiskMultiMap::close()
{
    if(bf.isOpen())
        close();
}


bool DiskMultiMap::insert(const std::string& key, const std::string& value, const std::string& context)
{
    if(key.length() > 120 || value.length() > 120 || context.length() > 120)
        return false;
    
        //create the association
    Association a;
        strcpy(a.value, value.c_str());
        strcpy(a.context, context.c_str());
    
        //find the slot to insert new association
    BinaryFile::Offset availOffset;
    
        // need to find recently deleted slots
    Header h;
    bf.read(h, 0);
    if(h.headToDeleted == 0)            //no deleted head
        availOffset = bf.fileLength();
    else
    {
        availOffset = h.headToDeleted;      //NEED MODIFICATION
        
        //read the deleted node
        Association reuse;
        bf.read(reuse, h.headToDeleted);
        h.headToDeleted = reuse.next;
        
    }
        //write association
    //bf.write(a, availOffset);

        //find hash value for the bucket
    unsigned int bucketLoc = findHashValue(key);
    
        //modify bucket
    Bucket b;
    bf.read(b, bucketLoc);
    
    if(!b.used)
    {
        strcpy(b.key, key.c_str());
        b.used = true;
        b.head = availOffset;
        
        a.next = bucketLoc;                            //not sure if it works, points to the bucket
    }
    else
    {
        a.next = b.head;
        b.head = availOffset;
    }
    
    //write association
    bf.write(b, bucketLoc);
    bf.write(a, availOffset);
    return true;
        
}


DiskMultiMap::Iterator DiskMultiMap::search(const std::string& key)
{
    Bucket b;
    BinaryFile::Offset bucketNum = findHashValue(key);
    bf.read(b, bucketNum);
    
    if(b.used)
    {
        BinaryFile::Offset firstAsso = b.head;
        DiskMultiMap::Iterator it(m_fileName, firstAsso, findHashValue(key));
        return it;
    }
    else
    {
        DiskMultiMap::Iterator it;
        return it;
    }
}


BinaryFile::Offset DiskMultiMap::findHashValue(const std::string& key)
{
    std::hash<std::string> str_hash;
    unsigned long hashValue = str_hash(key);
    unsigned int bucketNum = sizeof(Header) + hashValue%totalBuckets*SIZE_OF_BUCKETS;
    return bucketNum;
}

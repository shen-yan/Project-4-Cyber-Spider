#ifndef DISKMULTIMAP_H_
#define DISKMULTIMAP_H_

#include <string>
#include "MultiMapTuple.h"
#include "BinaryFile.h"

class BinaryFile;

class DiskMultiMap
{
public:
    
    class Iterator
    {
    public:
        Iterator();
        // You may add additional constructors
        Iterator(std::string& file, BinaryFile::Offset startLoc, BinaryFile::Offset bucketLoc);
        
        bool isValid() const;
        Iterator& operator++();
        MultiMapTuple operator*();
        
    private:
        // Your private member declarations will go here
        bool m_valid;
        BinaryFile::Offset cur;
        BinaryFile::Offset m_bucketLoc;
        string m_file;
        
    };
    
    DiskMultiMap();
    ~DiskMultiMap();
    bool createNew(const std::string& filename, unsigned int numBuckets);
    bool openExisting(const std::string& filename);
    void close();
    bool insert(const std::string& key, const std::string& value, const std::string& context);
    Iterator search(const std::string& key);
    int erase(const std::string& key, const std::string& value, const std::string& context);
    
private:
    // Your private member declarations will go here
    BinaryFile bf;
    string m_fileName;
    int totalBuckets;
    //const int NUM_OF_BUCKETS;
    
    const int SIZE_OF_HEADER = sizeof(Header);
    const int SIZE_OF_BUCKETS = sizeof(Bucket);
    //const int SIZE_OF_ASSO;
    
    struct Header
    {
        int numOfBuckets;
        BinaryFile::Offset headToDeleted;
    };
    
    struct Bucket
    {
        char key[120+1];
        bool used;
        BinaryFile::Offset head;
    };
    
    struct Association
    {
        char value[120+1];
        char context[120+1];
        BinaryFile::Offset next;
    };
    
    BinaryFile::Offset findHashValue(const std::string& key);

};

#endif // DISKMULTIMAP_H_
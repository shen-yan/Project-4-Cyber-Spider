#include "IntelWeb.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <stack>
using namespace std;


IntelWeb::IntelWeb()
{
    
}

IntelWeb::~IntelWeb()
{
    m_disk.close();
}

bool IntelWeb::createNew(const std::string& filePrefix, unsigned int maxDataItems)
{
    close();
    
    m_fileName = filePrefix;
    int numOfBuckets = maxDataItems/0.75;
    bool success = m_disk.createNew(filePrefix, numOfBuckets);
    bool success2 = m_disk2.createNew(filePrefix+"-2", numOfBuckets);
    
    if(success && success2)
        return true;
    else
    {
        close();
        return false;
    }
}

bool IntelWeb::openExisting(const std::string& filePrefix)
{
    close();
    
    bool success = m_disk.openExisting(m_fileName);
    bool success2 = m_disk2.openExisting(m_fileName+"-2");
    
    if(success && success2)
        return true;
    else
    {
        close();
        return false;
    }

}

void IntelWeb::close()
{
    m_disk.close();
    m_disk2.close();
}

bool IntelWeb::ingest(const std::string& telemetryFile)
{
    ifstream inf("telemetryFile");
    
    if(!inf)
        return false;
    
    string line;
    while(getline(inf, line))
    {
        istringstream iss(line);
        string context;
        string from;
        string to;
        
        if(!(iss >> context >> to >> from))
        {
            cout << "Ignoring badly-formatted input line: " << line << endl;
            continue;
        }
        
        m_disk.insert(from, to, context);
        m_disk2.insert(to, from, context);
    }
    
    return true;
}

unsigned int IntelWeb::crawl(const std::vector<std::string>& indicators,
                   unsigned int minPrevalenceToBeGood,
                   std::vector<std::string>& badEntitiesFound,
                   std::vector<InteractionTuple>& interactions
                   )
{
    int count = 0;
    badEntitiesFound.clear();
    interactions.clear();
    
    stack<string> badStack;         //bad entities to check
    set<string> badEntitiesSet;
    set<InteractionTuple> interSet;
    
    int p = 100;      //NEED MODIFICATION
    
        //put all the bad entities in indicator to badStack to check later
    vector<string>::const_iterator indic;
    for(indic = indicators.begin(); indic != indicators.end(); indic++)
        badStack.push(*indic);

    while(!badStack.empty())
    {
        string key = badStack.top();
        badStack.pop();
        
        DiskMultiMap::Iterator it;
        it = m_disk.search(key);
        
        if(it.isValid())
            badEntitiesSet.insert( (*it).key);      //put it to set
        
            //find bad entity
        while(it.isValid() && (*it).key == key)
        {
            InteractionTuple m;                     //insert the connection
            m.from = (*it).key;
            m.to = (*it).value;
            m.context = (*it).context;
            interSet.insert(m);
            
            //find p for (*it).value
            if( p < minPrevalenceToBeGood && badEntitiesSet.find((*it).value) != badEntitiesSet.end())
                badStack.push( (*it).value);            //put the exe/web associated to malicious
            
            ++it;
        }
        
        
        it = m_disk2.search(key);
        
        if(it.isValid())
            badEntitiesSet.insert( (*it).key);      //put it to set
        while(it.isValid() && (*it).key == key)
        {
            InteractionTuple m;                     //insert the connection
            m.to = (*it).key;
            m.from = (*it).value;
            m.context = (*it).context;
            interSet.insert(m);
            
            //find p for (*it).value
            if( p < minPrevalenceToBeGood && badEntitiesSet.find((*it).value) != badEntitiesSet.end())
                badStack.push( (*it).value);            //put the exe/web associated to malicious
            
            ++it;
        }
        
    }
    
    set<string>::iterator itt;
    itt = badEntitiesSet.begin();
    while(itt != badEntitiesSet.end())
    {
        badEntitiesFound.push_back(*itt);
        count++;
    }
    
    set<InteractionTuple>::iterator itt2;
    itt2 = interSet.begin();
    while(itt2 != interSet.end())
        interactions.push_back(*itt2);
    
    return count;
}

//////////////////////////
//      Auxiliary       //
//////////////////////////
bool operator<(const InteractionTuple & a, const InteractionTuple& b)
{
    if(a.context < b.context)
        return true;
    
    if(a.context == b.context && a.from < b.from)
        return true;
    
    if(a.context == b.context && a.from == b.from && a.to < b.to)
        return true;
    
    return false;
}

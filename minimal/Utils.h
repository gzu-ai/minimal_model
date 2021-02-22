//

#ifndef MINISAT_UTILS_H
#define MINISAT_UTILS_H

#endif //MINISAT_UTILS_H

#include <vector>
#include "SpaceDefine.h"

template <class T>
inline bool isSub(T &source,SOlVER_NAMESPACE::Clause &clause,int limit){
    int sourceIndex=0;
    int clauseIndex=0;
    if (source.size()<clause.size()){
        return false;
    }
    while ( sourceIndex<source.size()&&clauseIndex<clause.size()){
        int atom=SOlVER_NAMESPACE::var((clause)[clauseIndex]);
        if(source[sourceIndex]>=limit){
            break;
        }
        if(atom==source[sourceIndex]){
            ++clauseIndex;
            ++sourceIndex;
            continue;
        }
        if(atom>source[sourceIndex]){
            ++sourceIndex;
        } else{
            return false;
        }
    }
    return  clauseIndex == clause.size() ;
}
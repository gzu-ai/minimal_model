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
/**
 *
 * @param clauses
 * @param ca
 * @param model
 * @return
 */
inline bool mr(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &clauses,SOlVER_NAMESPACE::ClauseAllocator &ca, SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> &model){
    int clauseSize=clauses.size();
    int clauseIndex=0;
    int clauseLastIndex=0;
    for (; clauseLastIndex < clauseSize; ++clauseLastIndex) {
        SOlVER_NAMESPACE::CRef c = clauses[clauseLastIndex];
        clauses[clauseIndex] = c;
        SOlVER_NAMESPACE::Clause &clause = ca[c];
        int atomLastIndex = 0;
        int atomIndex = 0;
        int atomSize = clause.size();
        bool del = false;
        for (; atomLastIndex < atomSize; ++atomLastIndex) {
            auto d = (clause)[atomLastIndex];
            (clause)[atomIndex] = d;
            bool sign = SOlVER_NAMESPACE::sign(d);
            int value = SOlVER_NAMESPACE::var(d);

            if (sign && model[value] == SOlVER_NAMESPACE::l_False) {

                del = true;
                break;
            }
            if (model[value] == SOlVER_NAMESPACE::l_True) {
                ++atomIndex;
            }

        }
        if (del|| atomIndex==0) {
            ca.free(c);
            continue;
        }
        for (; atomIndex < atomSize; ++atomIndex) {
            clause.pop();
        }
        ++clauseIndex;

    }
    for (; clauseIndex < clauseSize;++clauseIndex) {
        clauses.pop();
    }
    return true;
}

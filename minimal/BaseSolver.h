

#ifndef MINISAT_BASESOLVER_H
#define MINISAT_BASESOLVER_H

#include "SpaceDefine.h"

namespace Minimal {
    class BaseSolver {
    public:
        void readCNF(const char *path, bool strictp, int verbosity);

        virtual bool solve() = 0;

         SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> &getModel() {
            return model;
        };

        inline int nVars() const { return _nVars +1;}

        virtual void addClause(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> &lits){
            SOlVER_NAMESPACE::sort(lits);
            SOlVER_NAMESPACE::Lit& lit=  lits.last();
            int value=SOlVER_NAMESPACE::var(lit);
            _nVars=_nVars>value?_nVars:value;
            SOlVER_NAMESPACE::CRef crf = ca.alloc(lits);
            clauses.push(crf);
         }
         template <class  Solver>
        void addClauseToSolver(Solver *S){
             for (int k = 0; k < _nVars; ++k) {
                 S->newVar();
             }
             SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> lits;
             for (int i = 0; i < clauses.size(); ++i) {
                 lits.clear();
                 SOlVER_NAMESPACE::CRef  crf= clauses[i];
                 SOlVER_NAMESPACE::Clause & clause=  ca[crf];
                 for (int j = 0; j < clause.size(); ++j) {
                     lits.push(clause[j]);
                 }
                 if (lits.size()>0){
                     S->addClause(lits);
                 }
             }
         }
        virtual void printStats() { _printStats(); }

    protected:
        void _printStats();
        SOlVER_NAMESPACE::ClauseAllocator ca;
        SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> model;
        SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> clauses;
    private:
        int _nVars=0;

    };

}
#endif //MINISAT_BASESOLVER_H

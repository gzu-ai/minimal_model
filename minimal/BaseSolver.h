

#ifndef MINISAT_BASESOLVER_H
#define MINISAT_BASESOLVER_H

#include "minisat//core//Solver.h"

class BaseSolver {
public:
    BaseSolver(const char *path, bool strictp,int verbosity);
    virtual bool solve() =0;
    inline int   nVars         ()      const   { return solver.nVars(); }

    virtual Minisat::vec<Minisat::lbool>&  getModel()  {
        return solver.model;
    };

    virtual void printStats() {_printStats();}

protected:
    void _printStats();
    Minisat::Solver solver;

};


#endif //MINISAT_BASESOLVER_H

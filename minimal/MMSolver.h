

#ifndef MINISAT_MMSOLVER_H
#define MINISAT_MMSOLVER_H

#include "minisat/core/Solver.h"
#include "BaseSolver.h"
class MMSolver : public BaseSolver{

public:
    bool solve();
    MMSolver(const char *path, bool strictp,int verbosity):BaseSolver(path,strictp,verbosity){}
    void printStats();
    Minisat::vec<Minisat::lbool>&  getModel(){
        return model;
    }
private:
    int compute_model_count=0;
    bool result = false;
    Minisat::vec<Minisat::lbool> model;
};


#endif //MINISAT_MMSOLVER_H

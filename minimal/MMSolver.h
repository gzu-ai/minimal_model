

#ifndef MINISAT_MMSOLVER_H
#define MINISAT_MMSOLVER_H

#include "minisat/core/Solver.h"
#include "BaseSolver.h"
class MMSolver : public BaseSolver{

public:
    bool solve();
    void printStats();
    Minisat::vec<Minisat::lbool>&  getModel(){
        return model;
    }
    void addClause(Minisat::vec<Minisat::Lit>&lits){}
private:
    int compute_model_count=0;
    bool result = false;
    Minisat::vec<Minisat::lbool> model;
};


#endif //MINISAT_MMSOLVER_H

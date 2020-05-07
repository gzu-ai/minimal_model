
#include "MMSolver.h"
#include "Utils.h"
#include <zlib.h>
#include <minisat/core/Dimacs.h>
using namespace Minimal;

bool MMSolver::solve() {
    SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> litsT;
    if(compute_model_count>0){
        return result;
    }
    compute_model_count=1;
    solver->eliminate(true);
    while (solver->solve()){
        result =true;
        litsT.clear();
        model.clear();
//        auto * tempSolver=newSolver();
        for (int i = 0; i < solver->nVars(); i++) {
            SOlVER_NAMESPACE::lbool value=solver->model[i];
            model.push(value);
            if (value == SOlVER_NAMESPACE::l_True) {
                litsT.push(~SOlVER_NAMESPACE::mkLit(i));
            }
        }
        int  nvar=solver->nVars();
        delete solver;
        solver = newSolver(nvar);
        mr(this->clauses,ca,model);
        copyToSolver();
        solver->addClause(litsT);
        ++compute_model_count;
    }
    return result;
}

void MMSolver::printStats() {
    BaseSolver::_printStats();
    printf("ComputeModelCount     : %d \n", compute_model_count);
}

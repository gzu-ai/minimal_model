
#include "MMSolver.h"
#include <zlib.h>
#include <minisat/core/Dimacs.h>
using namespace Minimal;

bool MMSolver::solve() {
    SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> litsT;
    if(compute_model_count>0){
        return result;
    }
    compute_model_count=1;
    SOlVER_NAMESPACE::SimpSolver *solver= new  SOlVER_NAMESPACE::SimpSolver();
    addClauseToSolver(solver);
    solver->eliminate(true);
    while (solver->solve()){
        result =true;
        SOlVER_NAMESPACE::SimpSolver *temp= new  SOlVER_NAMESPACE::SimpSolver();
        addClauseToSolver(temp);
        litsT.clear();
        model.clear();
        for (int i = 0; i < solver->nVars(); i++) {
            SOlVER_NAMESPACE::lbool value=solver->model[i];
            model.push(value);
            if (value == SOlVER_NAMESPACE::l_True) {
                litsT.push(~SOlVER_NAMESPACE::mkLit(i));
            } else if(value==SOlVER_NAMESPACE::l_False){
                temp->addClause(~SOlVER_NAMESPACE::mkLit(i));
            }
        }
        temp->addClause(litsT);
        delete solver;
        solver=temp;
        ++compute_model_count;
        solver->eliminate(true);
    }
    delete solver;
    return result;
}

void MMSolver::printStats() {
    BaseSolver::_printStats();
    printf("ComputeModelCount     : %d \n", compute_model_count);
}

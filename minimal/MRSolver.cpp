

#include <minisat/simp/SimpSolver.h>
#include "MRSolver.h"

using namespace Minimal;

bool MRSolver::solve() {
    if (compute_model_count>1){
        return result;
    }
    compute_model_count=1;
    SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> litsT;
    solver.simplify();
    while (solver.solve(false, true)){
        result =true;
        litsT.clear();
        model.clear();
        for (int i = 0; i < solver.nVars(); i++) {
            SOlVER_NAMESPACE::lbool value =solver.model[i];
            model.push(value);
            if (value == SOlVER_NAMESPACE::l_True) {
                litsT.push(~SOlVER_NAMESPACE::mkLit(i));
            } else if (value == SOlVER_NAMESPACE::l_False) {
                solver.addClause(~SOlVER_NAMESPACE::mkLit(i));
            }
        }
        if (this->check()){
            break;
        }
        ++compute_model_count;
        solver.addClause(litsT);
        solver.simplify();
    }
    return result;
}

bool MRSolver::check() {
    ++check_model_count;
    SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> model;
    int modeSize=0;
    for (int j = 0; j < solver.nVars(); ++j) {
        SOlVER_NAMESPACE::lbool& value = solver.model[j];
        model.push(value);
        if (value==SOlVER_NAMESPACE::l_True){
            ++modeSize;
        }
    }
    int limit =solver.nVars();
    mr(this->clauses,model);
    SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> clauses;
    copyToClauses(this->clauses,clauses);
    StronglyConnectedGraph graph;
    createGraph(clauses,graph);
    int node= 0;
    SOlVER_NAMESPACE::vec<int> S;
    SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef > ts;
    node = graph.getEmptyInDegree();
    while (node>-1){
        if (node>=limit){
            graph.remove(node);
            node = graph.getEmptyInDegree();
            continue;
        }
        computeS(graph.getComponent(node),S);
        computeTS(clauses,S,ts);
        if (!compute(ts,S)){
            break;
        }
        //M=M -S
        for (int i = 0; i < S.size(); ++i) {
            int key = S[i];
            --modeSize;
            model[key] = SOlVER_NAMESPACE::l_False;

        }
        reduce(clauses,S);
        graph.remove(node);
        node = graph.getEmptyInDegree();
    }

    for (int l = 0; l < clauses.size(); ++l) {
        ca.free(clauses[l]);
    }
    return modeSize==0;
}


bool MRSolver::mr(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &clauses,SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> &model) {
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

bool MRSolver::createGraph(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef>& clauses,StronglyConnectedGraph &graph) {
    Graph gr;
    int base= solver.nVars();
    for (int i = 0; i < clauses.size(); ++i) {
        int key=base+i;
        SOlVER_NAMESPACE::Clause * clause= ca.lea(clauses[i]);
        for (int j = 0; j < clause->size(); ++j) {
             SOlVER_NAMESPACE::Lit& lit= clause->operator[](j);
             int value=SOlVER_NAMESPACE::var(lit);
             if (SOlVER_NAMESPACE::sign(lit)){
                 gr.addPoint(value,key);
             } else{
                 gr.addPoint(key,value);
             }
        }
    }
    gr.done();
    graph.compute(&gr);
    graph.done();
    return true;
}

void MRSolver::copyToClauses(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &source,
        SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &dist) {
    SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> list;
    for (int i = 0; i < source.size(); ++i) {
         SOlVER_NAMESPACE::Clause &clause= ca[source[i]];
         list.clear();
        for (int j = 0; j < clause.size(); ++j) {
            list.push(clause[j]);
        }
         SOlVER_NAMESPACE::CRef  c = ca.alloc(list);
         dist.push(c);
    }
}

bool MRSolver::compute(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &ts, SOlVER_NAMESPACE::vec<int> &S) {
    if (ts.size() == 0) {
        return S.size() == 0;
    }
    if (S.size() == 1) {
        for (int i = 0; i < ts.size(); ++i) {
            SOlVER_NAMESPACE::Clause *clause = ca.lea(ts[i]);
            for (int j = 0; j < clause->size(); ++j) {
                if (SOlVER_NAMESPACE::sign((*clause)[j])) {
                    return false;
                }
            }
        }
        return true;
    }
    SOlVER_NAMESPACE::SimpSolver solver;
    SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::Lit> lits;
    for (int i = 0; i < this->solver.nVars(); i++) {
        solver.newVar();
    }

    for (int i = 0; i < ts.size(); ++i) {
        lits.clear();
        SOlVER_NAMESPACE::Clause &clause = ca[ts[i]];
        if (clause.size() == 0)continue;
        for (int j = 0; j < clause.size(); ++j) {
            SOlVER_NAMESPACE::Lit lit = clause[j];
            lits.push(lit);
        }
        solver.addClause(lits);
    }
    lits.clear();
    for (int k = 0; k < S.size(); ++k) {
        lits.push(~SOlVER_NAMESPACE::mkLit(S[k]));
    }
    solver.addClause(lits);
    solver.eliminate(true);
    ++compute_mini_model_count;
    return !solver.solve();
}


void MRSolver::clear(SOlVER_NAMESPACE::Clause &clause, SOlVER_NAMESPACE::vec<int> &S) {
    int clauseSize=clause.size();
    int clauseIndex=0;
    int clauseLastIndex=0;
    int sIndex=0;
    int sSize=S.size();
    int s;

    while (sIndex<sSize&&clauseLastIndex<clauseSize){
        SOlVER_NAMESPACE::Lit lit=clause[clauseLastIndex];
        clause[clauseIndex]=lit;
        s=S[sIndex];
        int var= SOlVER_NAMESPACE::var(lit);
        if (var==s) {
            // 删除
            ++clauseLastIndex;
            ++sIndex;
            continue;
        }
        if(var>s){
            ++sIndex;
        } else{
            ++clauseLastIndex;
            ++clauseIndex;
        }
    }
    for (; clauseLastIndex <clauseSize ; ++clauseIndex,++clauseLastIndex) {
        (clause)[clauseIndex]= (clause)[clauseLastIndex];
    }
    for ( ;clauseIndex <clauseSize ; ++clauseIndex) {
        clause.pop();
    }
}

void MRSolver::reduce(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &clauses, SOlVER_NAMESPACE::vec<int> &S) {
    int clauseSize = clauses.size();
    int clauseIndex=0;
    int clauseLastIndex=0;
    for (; clauseLastIndex < clauseSize;++clauseLastIndex) {
        SOlVER_NAMESPACE::CRef crf = clauses[clauseLastIndex];
        clauses[clauseIndex]=crf;
        SOlVER_NAMESPACE::Clause &clause = ca[crf];
        if (isIntersection(S, clause)) {
            // 删除
            ca.free(crf);
            continue;
        }
        clear(clause,S);
        if (clause.size()==0){
            ca.free(crf);
            continue;
        }
        ++clauseIndex;
    }
    for(;clauseIndex<clauseSize;++clauseIndex){
        clauses.pop();
    }

}

void MRSolver::computeTS(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &clauses, SOlVER_NAMESPACE::vec<int> &source, SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::CRef> &ts) {
    ts.clear();
    int  limit=solver.nVars();
    for (int i = 0; i < clauses.size(); ++i) {
        SOlVER_NAMESPACE::CRef  crf=clauses[i];
        if(isSub(source,ca[crf],limit)){
            ts.push(crf);
        }
    }
}

void MRSolver::printStats() {
    BaseSolver::_printStats();
    printf("ComputeModelCount     : %d \n", compute_model_count);
    printf("CheckModelCount       : %d \n", check_model_count);
    printf("ComputeMIniModelCount : %d \n", compute_mini_model_count);
}

int MRSolver::copyModel(SOlVER_NAMESPACE::vec<SOlVER_NAMESPACE::lbool> &mode) {
    return 0;
}



#include "Graph.h"
#include <algorithm>

using namespace Minimal;
void Graph::addPoint(int from, int to) {
    auto it= content.find(from);
    if(it==content.end()){
        std::unordered_set<int > set;
        content.insert(std::pair<int ,std::unordered_set<int> >(from,set));
        it= content.find(from);
     }
    maxNode=from>maxNode?from:maxNode;
    maxNode=to>maxNode?to:maxNode;

    auto toIt= content.find(to);
    if (toIt==content.end()){
        content.insert(std::pair<int ,std::unordered_set<int> >(to,std::unordered_set<int >()));
    }
    if(from==to){
        return;
    }
    auto result=  it->second.insert(to);
    if (!result.second){
        return;
    }
    auto indegreeIt= indegree.find(from);
    if(indegreeIt==indegree.end()){
        indegree.insert(std::pair<int, int>(from,0));
    }
    auto indegreeItTo= indegree.find(to);
    if (indegreeItTo==indegree.end()){
        indegree.insert(std::pair<int, int>(to,1));
    } else{
        ++indegree[to];
    }
}

bool Graph::remove(int point) {
    auto indegreeItTo= indegree.find(point);
    if (indegreeItTo==indegree.end()){
        return false;
    }
    for (auto & it: content[point]){
        if(--indegree[it]==0){
            emptyInDegree.insert(it);
        }
    }
    int degree= indegreeItTo->second;
    emptyInDegree.erase(point);
    indegree.erase(point);
    content.erase(point);
    if (degree==0){
        return true;
    }
    for (auto &  it:content) {
        auto  itt = it.second.begin();
        while (itt!=it.second.end()){
            if (*itt==0){
                itt= it.second.erase(itt);
            } else{
                ++itt;
            }
        }
    }
    return true;
}

void Graph::done() {
    for (auto & it : indegree) {
        if (it.second==0){
            emptyInDegree.insert(it.first);
        }
    }
}

Graph *Graph::reverse() {
    Graph * graph=new Graph();
    for (auto &  it:content) {
        auto  itt = it.second.begin();
        while (itt!=it.second.end()){
            graph->addPoint(*itt,it.first);
            ++itt;
        }
    }
    return graph;
}

void Graph::print() {
    puts("graph");

    for (auto item :content){
        printf("%d  -> ",item.first);
        for (auto it:item.second) {
            printf(" %d",it);
        }
        putchar('\n');
    }
}


bool StronglyConnectedGraph::remove(int point) {
    if (Graph::remove(point)){
        weights.erase(point);
        return true;
    }
    return false;
}



void StronglyConnectedGraph::compute(Graph *graph) {
    Graph *reverse= graph->reverse();
    bool* visited=(bool*)malloc(sizeof(bool)*(graph->maxNode+1));
    memset(visited,0, sizeof(bool)*(graph->maxNode+1));
    Minisat::IntMap<int,int > w;
    std::vector<int> list;
    loop_dfs(reverse,visited,&list);
    memset(visited,0, sizeof(bool)*(graph->maxNode+1));
    for (int i = list.size()-1; i > -1; --i) {
        if (visited[list[i]]){
            continue;
        }
        std::vector<int> vec;
        dfs(graph,list[i],&vec,visited);
        vec.push_back(list[i]);
        std::sort(vec.begin(),vec.end());
        weights[vec[0]]=vec;
        for (auto it:vec){
            w.insert(it,vec[0]);
        }
    }

    for (auto &  it:graph->content) {
        auto  itt = it.second.begin();
        int from=w[it.first];
        while (itt!=it.second.end()){
            int to=w[*itt];
            addPoint(from,to);
            ++itt;
        }
        if (it.second.empty()){
            addPoint(from,from);
        }
    }
    delete  reverse;
    free(visited);

}

void StronglyConnectedGraph::loop_dfs(Graph *graph,bool *visited, std::vector<int> *list) {
    for (auto &  it:graph->content) {
        if (!visited[(it.first)]){
            dfs(graph,it.first,list,visited);
            list->push_back(it.first);
        }
    }
}

void StronglyConnectedGraph::dfs(Graph *graph, int node, std::vector<int> *list, bool *visited) {
    visited[node]= true;
    auto it= graph->content.find(node);
    if (it==graph->content.end()){
        return;
    }
    for(auto itt:it->second){
        if (visited[itt]){
            continue;
        }
        dfs(graph,itt,list,visited);
        list->push_back(itt);
    }
}

void StronglyConnectedGraph::done() {
    Graph::done();
}

int StronglyConnectedGraph::getEmptyInDegree() {
    return Graph::getEmptyInDegree();
}

void StronglyConnectedGraph::print() {
    Graph::print();
    puts("SCC");
    for (auto it:weights){
        printf("%d  : ",it.first);
        for(auto item:it.second){
            printf(" %d",item);
        }
        putchar('\n');
    }
    putchar('\n');
}

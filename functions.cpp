/* Gryllos Prokopis 2013-2014
 * Implementation of dynamic incremental algorithm for minimal length paths
 * [AIMN91] as described in the Journal of Algorithms 12 (1991), pp.615-638.
 * DistanceMap member functions add, decrease, length, minpath.
 */

#include "aimn91.h"
// add edge (x,y) with edge weight w
void DistanceMap::add(vertex_desc i, vertex_desc j, int w){
    edge_tree e = boost::add_edge(i,j,g).first; //add new edge to the graph
    boost::put(boost::edge_weight, g, e, w); //assign weight to the new edge 
    anc_i = ANC[i];
    vertex_tree x = boost::any_cast<vertex_tree>(anc_i[boost::graph_bundle].root);
    updateForwardBackward(x,i,j,DESC[j],w); // update desc and anc trees
    return;
}
// decrease edge weight of existing edge (i,j)
void DistanceMap::decrease(vertex_desc i, vertex_desc j, int w){
    int existing_weight = boost::get(boost::edge_weight, g, std::make_pair(i,j));
    if (existing_weight > w){
        boost::put(boost::edge_weight, g, e, w); //assign weight to the new edge
        Tree anc_i = ANC[i];
        vertex_tree x = boost::any_cast<vertex_tree>(anc_i[boost::graph_bundle].root);
        updateForwardBackward(x,i,j,DESC[j],w); // update desc and anc trees
    }
    return;
}
// retutn length of minpath from vertex x to vertex y
int DistanceMap::length(vertex_desc x, vertex_desc y){
    int length = d[std::make_pair(x,y)];
    return length;
}
// retutn minpath from vertex x to vertex y
path DistanceMap::minpath(vertex_desc x, vertex_desc y){
    if (F[std::make_pair(x,y)]==NULL)
        return NULL;
    else{
        Tree desc_x = DESC[x];
        vertex_tree root = F[std::make_pair(x,x)];
        vertex_tree it = F[std::make_pair(x,y)];
        path minpath;
        do{
            vertex_desc it_in_g = desc_x[it].v_in_g;
            vertex_tree it_p = boost::any_cast<vertex_tree>(desc_x[it].p_in_t);
            edge_desc e = boost::edge(desc_x[it_p].v_in_g,it_in_g,g).first;
            minpath.push(e);
            it = it_p;
        }while(it_p!=root);
    }
    return minpath;
}

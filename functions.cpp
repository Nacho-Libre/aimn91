/* Gryllos Prokopis 2013-2014
 * Implementation of dynamic incremental algorithm for minimal length paths
 * [AIMN91] as described in the Journal of Algorithms 12 (1991), pp.615-638.
 * DistanceMap member functions add, decrease, length, minpath.
 */

#include "aimn91.h"
// add edge (i,j) with edge weight w
void DistanceMap::test(Graph& g, v_desc i,v_desc h){
   // Tree anc_i,anc_h;
   // Tree anc_h;
    std::cout<<boost::any_cast<v_desc>(g[i].ANC[B[std::make_pair(i,i)]].v_g)<<std::endl;
    std::cout<<boost::any_cast<v_desc>(g[i].ANC[F[std::make_pair(h,h)]].v_g)<<std::endl;
   // v_desc h;
//  int count=0;
//  std::pair<vertex_it, vertex_it> vi4;
//  count = 0;
//  for (vi4=boost::vertices(g); vi4.first != vi4.second; ++vi4.first){
//      std::cout<<anc_h[F[std::make_pair(h,h)]].v_g<<std::endl;
//      if(count == 6){
//          //h = *vi4.first;
//            Tree anc_h = ANC[h];
//      }
//      count++;
//  }
//
//  //vi4=boost::vertices(g);
//std::pair<tree_vertex_it, tree_vertex_it> vi2;
//std::pair<vertex_it, vertex_it> vi3;
//for (vi3=boost::vertices(g); vi3.first != vi3.second; ++vi3.first){
//    Tree test2 = DESC[*vi3.first];
//    std::cout<<"root "<<test2[F[std::make_pair(*vi3.first,*vi3.first)]].v_g<<std::endl;
//    for (vi2=boost::vertices(test2); vi2.first != vi2.second; ++vi2.first){
//        std::cout<<*vi2.first<<" "<<test2[*vi2.first].v_g<<std::endl;
//    }
//}
////std::pair<vertex_it, vertex_it> vi2;
////for (vi2=boost::vertices(g); vi2.first != vi2.second; ++vi2.first){
////    Tree test = ANC[*vi2.first];
////    vertex_tree root = boost::any_cast<vertex_tree>(test[boost::graph_bundle].root);
////    std::cout<<*vi2.first<<" "<<test[root].v_g<<std::endl;
////}
    return;
}
void DistanceMap::add(v_desc i, v_desc j, int w, Graph& g){
    vertex_tree x = B[std::make_pair(i,i)];
    vertex_tree root = F[std::make_pair(j,j)];
    edge_desc e = boost::add_edge(i,j,g).first; //add new edge to the graph
    boost::put(boost::edge_weight, g, e, w); //assign weight to the new edge 
    UpdateForwardBackward(x,i,j,g[j].DESC,root,w,g); // update desc and anc trees
    return;
}
// decrease edge weight of existing edge (i,j)
void DistanceMap::decrease(v_desc i, v_desc j, int w, Graph& g){
    edge_desc e = boost::edge(i,j,g).first;
    int existing_weight = boost::get(boost::edge_weight, g, e);
    if (existing_weight > w){
        boost::put(boost::edge_weight, g, e, w); //assign weight to the new edge
        vertex_tree x = B[std::make_pair(i,i)];
        vertex_tree root = F[std::make_pair(j,j)];
        UpdateForwardBackward(x,i,j,g[i].DESC,root,w,g); // update desc and anc trees
    }
    return;
}
// return length of minpath from vertex x to vertex y
int DistanceMap::length(v_desc x, v_desc y){
    int length = d[std::make_pair(x,y)];
    return length;
}
// return minpath from vertex x to vertex y
path DistanceMap::minpath(v_desc x, v_desc y, Graph& g){
    path minpath;
    std::pair<v_desc, v_desc> edge;
    if (F[std::make_pair(x,y)]==NULL)
        return minpath;
    else{
        //Tree desc_x = DESC[x];
        vertex_tree root = F[std::make_pair(x,x)];
        vertex_tree it = F[std::make_pair(x,y)];
        vertex_tree it_p = boost::any_cast<vertex_tree>(g[x].DESC[it].p_in_t);
        do{
            v_desc it_in_g = boost::any_cast<v_desc>(g[x].DESC[it].v_g);
            edge = std::make_pair(boost::any_cast<v_desc>(g[x].DESC[it_p].v_g), it_in_g);
            minpath.push(edge);
            it = it_p;
            it_p = boost::any_cast<vertex_tree>(g[x].DESC[it].p_in_t);
        }while(it_p!=root);
    }
    return minpath;
}

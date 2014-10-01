/* Gryllos Prokopis 2013-2014
 * Implementation of dynamic incremental algorithm for minimal length paths
 * [AIMN91] as described in the Journal of Algorithms 12 (1991), pp.615-638.
 */

#include "aimn91.h"

using namespace boost;
using namespace std;

void DistanceMap::UpdateForwardBackward(vertex_tree x_in_tree, v_desc i,
    v_desc j, Tree& desc_j, vertex_tree root, int w, Graph& g)
{
    vertex_tree y_in_tree;
    pair<tree_vertex_it, tree_vertex_it> vi_tree;
    queue<vertex_tree> Q;
    Tree N;
    pair<out_edges_it,out_edges_it> out_ei;
    v_desc x = any_cast<v_desc>(g[i].ANC[x_in_tree].v_g);
    Q.push(root);
    while(!Q.empty())
    {
        y_in_tree = Q.front();
        v_desc y = any_cast<v_desc>(desc_j[y_in_tree].v_g);
        Q.pop();
        /* distance from vertex x to vertex i. */
        int dist_x_i = d[make_pair(x,i)];
        /* distance from vertex x to vertex y. */
        int dist_x_y = d[make_pair(x,y)];
        /* distance from vertex j to vertex y. */
        int dist_j_y = d[make_pair(j,y)];
        
        /* if total distance from vertex x to vertex y is shortent after (i,j) 
           edge was added. */
        if (dist_x_i + w + dist_j_y < dist_x_y){
            /**************************************/
            /* update forward minimal path trees. */
            /**************************************/
            if (y == j){
                /* check if vertex already exist in Desc_x. */
                if (F[make_pair(x,j)]==0){
                /* add vertex_for_j in desc_x as a children of i and connect Graph
                   vertex with corrilating descendants tree vertex. */
                    vertex_tree vertex_for_j = add_vertex(g[x].DESC);
                    g[x].DESC[vertex_for_j].v_g = j;
                    g[x].DESC[vertex_for_j].p_in_t = F[make_pair(x,i)];
                    add_edge(F[make_pair(x,i)],vertex_for_j,g[x].DESC).first;
                    F[make_pair(x,j)] = vertex_for_j;
                }
                else{
                    std::cout<<"bb"<<std::endl;
                    /* removing old edge to j and adding the correct one. */
                    remove_edge(any_cast<vertex_tree>(g[x].DESC[F[make_pair(x,j)]].p_in_t),
                        F[make_pair(x,j)],g[x].DESC);
                    g[x].DESC[F[make_pair(x,j)]].p_in_t = F[make_pair(x,i)];
                    add_edge(F[make_pair(x,i)],F[make_pair(x,j)],g[x].DESC).first;
                }

                /* create the pruned copy of desc_j. */
                vertex_tree j_in_N = add_vertex(N);
                N[j_in_N].v_g = j;
                N[j_in_N].p_in_t = j;
                root = j_in_N;
            }
            else{
                vertex_tree p_y = any_cast<vertex_tree>(desc_j[y_in_tree].p_in_t);
                if (F[make_pair(x,y)]==0){
                    /* add vertex_for_y in desc_x with the same parent it has in
                    pruned desc_j. */
                    vertex_tree vertex_for_y = add_vertex(g[x].DESC);
                    g[x].DESC[vertex_for_y].v_g = y;
                    g[x].DESC[vertex_for_y].p_in_t = F[make_pair(x,any_cast<v_desc>(desc_j[p_y].v_g))];
                    add_edge(F[make_pair(x,any_cast<v_desc>(g[x].DESC[p_y].v_g))],
                        vertex_for_y,g[x].DESC).first;
                    F[make_pair(x,y)] = vertex_for_y;
                }
                else{
                    remove_edge(any_cast<vertex_tree>(g[x].DESC[F[make_pair(x,y)]].p_in_t),
                        F[make_pair(x,y)],g[x].DESC);
                    g[x].DESC[F[make_pair(x,y)]].p_in_t = F[make_pair(x,any_cast<v_desc>(g[x].DESC[p_y].v_g))];
                    add_edge(F[make_pair(x,any_cast<v_desc>(g[x].DESC[p_y].v_g))],
                        F[make_pair(x,y)],g[x].DESC).first;
                }
                /* fill the pruned copy of desc_j. */
                vertex_tree y_in_N = add_vertex(N);
                /* look for vertex y parent in N. */
                for (vi_tree=vertices(N); vi_tree.first != vi_tree.second; ++vi_tree.first){
                    if (any_cast<v_desc>(N[*vi_tree.first].v_g) == any_cast<v_desc>(desc_j[p_y].v_g))
                        break;
                }
                N[y_in_N].v_g = y;
                N[y_in_N].p_in_t = *vi_tree.first;
                add_edge(*vi_tree.first,y_in_N,g[x].DESC).first;
            }
            /***************************************/
            /* update backward minimal path trees. */
            /***************************************/
            if (x == i){
                if (B[make_pair(y,i)]==0){
                    vertex_tree vertex_for_i = add_vertex(g[y].ANC);
                    g[y].ANC[vertex_for_i].v_g = i;
                    g[y].ANC[vertex_for_i].p_in_t = B[make_pair(y,j)];
                    add_edge(B[make_pair(y,j)],vertex_for_i,g[y].ANC).first;
                    B[make_pair(y,i)] = vertex_for_i;
                }
                else{
                    remove_edge(any_cast<vertex_tree>(g[y].ANC[B[make_pair(y,i)]].p_in_t),
                        B[make_pair(y,i)],g[y].ANC);
                    g[y].ANC[B[make_pair(y,i)]].p_in_t = B[make_pair(y,j)];
                    add_edge(B[make_pair(y,j)],B[make_pair(y,i)],g[y].ANC).first;
                }
            }
            else{
                vertex_tree p_x = any_cast<vertex_tree>(g[i].ANC[x_in_tree].p_in_t);
                if (B[make_pair(y,x)]==0){
                    vertex_tree vertex_for_x = add_vertex(g[y].ANC);
                    g[y].ANC[vertex_for_x].v_g = x;
                    g[y].ANC[vertex_for_x].p_in_t = B[make_pair(y,any_cast<v_desc>(g[i].ANC[p_x].v_g))];
                    add_edge(B[make_pair(y,any_cast<v_desc>(g[i].ANC[p_x].v_g))],
                        vertex_for_x,g[i].ANC).first;
                    B[make_pair(y,x)] = vertex_for_x;
                }
                else{
                    remove_edge(any_cast<vertex_tree>(g[y].ANC[B[make_pair(y,x)]].p_in_t),
                        B[make_pair(y,x)],g[y].ANC);
                    g[y].ANC[B[make_pair(y,x)]].p_in_t = B[make_pair(y,any_cast<v_desc>(g[i].ANC[p_x].v_g))];
                    add_edge(B[make_pair(y,any_cast<v_desc>(g[i].ANC[p_x].v_g))],
                        B[make_pair(y,x)],g[y].ANC).first;
                }
            }
            /* updating distance matrix values. */
            d[make_pair(x,y)] = d[make_pair(x,i)]+ w +d[make_pair(j,y)];
            /* iterating over out edges of y and adding y's children in Q. */
            for (out_ei=out_edges(y_in_tree,desc_j); out_ei.first != out_ei.second; ++out_ei.first){
                Q.push(target(*out_ei.first,desc_j));
            }
        }
    }
    /* wow so much recursiveness. */
    if (num_vertices(N)!=0){
        for (out_ei=out_edges(x_in_tree,g[i].ANC); out_ei.first != out_ei.second; ++out_ei.first){
            UpdateForwardBackward(target(*out_ei.first,g[i].ANC),i,j,N,root,w,g);
        }
    }
}

/**********************************/
/* Data structure initialization. */
/**********************************/
DistanceMap::DistanceMap(Graph& g)
{
    pair<vertex_it, vertex_it> vi, vi_i, vi_j;
    /* initialize Distance matrix d with max int values in order to simulate 
       infinity as a value */
    for (vi_i=vertices(g); vi_i.first != vi_i.second; ++vi_i.first)
        for (vi_j=vertices(g); vi_j.first != vi_j.second; ++vi_j.first){
            if (*vi_i.first==*vi_j.first)
                d[make_pair(*vi_i.first,*vi_j.first)] = 0;
            else
                d[make_pair(*vi_i.first,*vi_j.first)] = numeric_limits<int>::max();
            F[make_pair(*vi_i.first,*vi_j.first)] = 0;
            B[make_pair(*vi_i.first,*vi_j.first)] = 0;
        }
    for (vi=vertices(g); vi.first != vi.second; ++vi.first){
        /* create descedants tree for vertex pointed by vi.first. */
        vertex_tree r_d = add_vertex(g[*vi.first].DESC);
        /* connect Graph vertex with corrilating descendants tree vertex. */
        g[*vi.first].DESC[r_d].v_g = *vi.first;
        g[*vi.first].DESC[r_d].p_in_t = r_d;
        g[*vi.first].DESC[graph_bundle].root = r_d;
        F[make_pair(*vi.first,*vi.first)] = r_d;
    }
    for (vi=vertices(g); vi.first != vi.second; ++vi.first){
        /* create ancestors tree for vertex pointed by vi.first. */
        vertex_tree r_a = add_vertex(g[*vi.first].ANC);
        /* connect Graph vertex with corrilating ancestors tree vertex. */
        g[*vi.first].ANC[r_a].v_g = *vi.first;
        g[*vi.first].ANC[r_a].p_in_t = r_a;
        g[*vi.first].ANC[graph_bundle].root = r_a;
        B[make_pair(*vi.first,*vi.first)] = r_a;
    }
}


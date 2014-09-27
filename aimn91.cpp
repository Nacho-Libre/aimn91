/* Gryllos Prokopis 2013-2014
 * Implementation of dynamic incremental algorithm for minimal length paths
 * [AIMN91] as described in the Journal of Algorithms 12 (1991), pp.615-638.
 */

#include "aimn91.h"

DistanceMap d;
/* using boost pointer map we associate each vertex with a set of descendants
   organised as a tree of forward minimal paths (DESC[x]) rooted at x and a set
   of ancestors organized as a tree of backward minimal paths (ANC[x]) rooted at
   x and Initialize each tree with its root vertex. */
boost::ptr_map<vertex_desc, Tree> DESC,ANC;

/* We are going to use this type to store pointers to the ANC and DESC trees
   vertices. */
std::map<std::pair<vertex_desc,vertex_desc>, vertex_tree> F,B;

void DistanceMap::UpdateForwardBackward(vertex_tree x_in_tree, vertex_desc i,
    vertex_desc j, Tree& pruned_desc_j, int w)
{
    vertex_tree y_in_tree,w;
    std::pair<tree_vertex_it, tree_vertex_it> vi_tree;
    std::queue<vertex_tree> Q;
    Tree N;
    std::pair<out_edges_it,out_edges_it> out_ei;
    Tree anc_i = ANC[i];
    vertex_desc x = anc_i[x_in_tree].v_in_g;
    Tree desc_x = DESC[x];

    Q.push(boost::any_cast<vertex_tree>(pruned_desc_j[boost::graph_bundle].root));
    while(!Q.empty())
    {
        y_in_tree = Q.front();
        vertex_desc y = pruned_desc_j[y_in_tree].v_in_g;
        Tree anc_y = ANC[y];
        Q.pop();
        /* distance from vertex x to vertex i. */
        int dist_x_i = d[std::make_pair(x,i)];
        /* distance from vertex x to vertex y. */
        int dist_x_y = d[std::make_pair(x,y)];
        /* distance from vertex j to vertex y. */
        int dist_j_y = d[std::make_pair(j,y)];
        
        /* if total distance from vertex x to vertex y is shortent after (i,j) 
           edge was added. */
        if (dist_x_i + w + dist_j_y < dist_x_y){
            /**************************************/
            /* update forward minimal path trees. */
            /**************************************/
            if (y == j){
                /* check if vertex already exist in Desc_x. */
                if (F[std::make_pair(x,j)]==NULL){
                /* add vertex_for_j in desc_x as a children of i and connect Graph
                   vertex with corrilating descendants tree vertex. */
                    vertex_tree vertex_for_j = boost::add_vertex(desc_x);
                    desc_x[vertex_for_j].v_in_g = j;
                    desc_x[vertex_for_j].p_in_t = F[std::make_pair(x,i)];
                    edge_tree e1 = boost::add_edge(F[std::make_pair(x,i)],vertex_for_j,desc_x).first;
                    F[std::make_pair(x,j)] = vertex_for_j;
                }
                else{
                    /* removing old edge to j and adding the correct one. */
                    boost::remove_edge(boost::any_cast<vertex_tree>(desc_x[F[std::make_pair(x,j)]].p_in_t),
                        F[std::make_pair(x,j)],desc_x);
                    desc_x[F[std::make_pair(x,j)]].p_in_t = F[std::make_pair(x,i)];
                    edge_tree e1 = boost::add_edge(F[std::make_pair(x,i)],
                        F[std::make_pair(x,j)],desc_x).first;
                }

                /* create the pruned copy of desc_j. */
                vertex_tree j_in_N = boost::add_vertex(N);
                N[j_in_N].v_in_g = j;
                N[j_in_N].p_in_t = j;
                N[boost::graph_bundle].root = j_in_N;
            }
            else{
                vertex_tree parent_of_y = boost::any_cast<vertex_tree>(pruned_desc_j[y_in_tree].p_in_t);
                if (F[std::make_pair(x,y)]==NULL){
                    /* add vertex_for_y in desc_x with the same parent it has in
                    pruned desc_j. */
                    vertex_tree vertex_for_y = boost::add_vertex(desc_x);
                    desc_x[vertex_for_y].v_in_g = y;
                    desc_x[vertex_for_y].p_in_t = F[std::make_pair(x,pruned_desc_j[parent_of_y].v_in_g)];
                    edge_tree e1 = boost::add_edge(F[std::make_pair(x,desc_x[parent_of_y].v_in_g)],
                        vertex_for_y,desc_x).first;
                    F[std::make_pair(x,y)] = vertex_for_y;
                }
                else{
                    boost::remove_edge(boost::any_cast<vertex_tree>(desc_x[F[std::make_pair(x,y)]].p_in_t),
                        F[std::make_pair(x,y)],desc_x);
                    desc_x[F[std::make_pair(x,y)]].p_in_t = F[std::make_pair(x,desc_x[parent_of_y].v_in_g)];
                    edge_tree e1 = boost::add_edge(F[std::make_pair(x,desc_x[parent_of_y].v_in_g)],
                        F[std::make_pair(x,y)],desc_x).first;
                }
                /* fill the pruned copy of desc_j. */
                vertex_tree y_in_N = boost::add_vertex(N);
                /* look for vertex y parent in N. */
                for (vi_tree=vertices(N); vi_tree.first != vi_tree.second; ++vi_tree.first){
                    if (N[*vi_tree.first].v_in_g == pruned_desc_j[parent_of_y].v_in_g)
                        break;
                }
                N[y_in_N].v_in_g = y;
                N[y_in_N].p_in_t = *vi_tree.first;
                edge_tree e2 = boost::add_edge(*vi_tree.first,y_in_N,desc_x).first;
            }
            /***************************************/
            /* update backward minimal path trees. */
            /***************************************/
            if (x == i){
                if (B[std::make_pair(y,i)]==NULL){
                    vertex_tree vertex_for_i = boost::add_vertex(anc_y);
                    anc_y[vertex_for_i].v_in_g = i;
                    anc_y[vertex_for_i].p_in_t = j;
                    edge_tree e2 = boost::add_edge(B[std::make_pair(y,j)],vertex_for_i,anc_y).first;
                    B[std::make_pair(y,i)] = vertex_for_i;
                }
                else{
                    boost::remove_edge(boost::any_cast<vertex_tree>(anc_y[B[std::make_pair(y,i)]].p_in_t),
                        B[std::make_pair(y,i)],anc_y);
                    anc_y[B[std::make_pair(y,i)]].p_in_t = B[std::make_pair(y,j)];
                    edge_tree e2 = boost::add_edge(B[std::make_pair(y,j)],
                        B[std::make_pair(y,i)],anc_y).first;
                }
            }
            else{
                vertex_tree parent_of_x = boost::any_cast<vertex_tree>(anc_i[x_in_tree].p_in_t);
                if (B[std::make_pair(y,x)]==NULL){
                    vertex_tree vertex_for_x = boost::add_vertex(anc_y);
                    anc_y[vertex_for_x].v_in_g = x;
                    anc_y[vertex_for_x].p_in_t = B[std::make_pair(y,anc_i[parent_of_x].v_in_g)];
                    edge_tree e2 = boost::add_edge(B[std::make_pair(y,anc_i[parent_of_x].v_in_g)],
                        vertex_for_x,anc_y).first;
                    B[std::make_pair(y,x)] = vertex_for_x;
                }
                else{
                    boost::remove_edge(boost::any_cast<vertex_tree>(anc_y[B[std::make_pair(y,x)]].p_in_t),
                        B[std::make_pair(y,x)],anc_y);
                    anc_y[B[std::make_pair(y,x)]].p_in_t = B[std::make_pair(y,anc_i[parent_of_x].v_in_g)];
                    edge_tree e2 = boost::add_edge(B[std::make_pair(y,anc_i[parent_of_x].v_in_g)],
                        B[std::make_pair(y,x)],anc_y).first;
                }
            }
            /* updating distance matrix values. */
            d[std::make_pair(x,y)] = d[std::make_pair(x,i)]+ w +d[std::make_pair(j,y)];
            /* iterating over out edges of y and adding y's children in Q. */
            for (out_ei=out_edges(y_in_tree,pruned_desc_j); out_ei.first != out_ei.second; ++out_ei.first){
                Q.push(target(*out_ei.first,pruned_desc_j));
            }
        }
    }
    /* wow so much recursiveness. */
    if (boost::num_vertices(N)!=0){
        for (out_ei=out_edges(x_in_tree,anc_i); out_ei.first != out_ei.second; ++out_ei.first){
            UpdateForwardBackward(target(*out_ei.first,anc_i),i,j,N,w);
        }
    }
}

/**********************************/
/* Data structure initialization. */
/**********************************/
void DistanceMap::init_DistanceMap(Graph& g)
{
    std::pair<vertex_it, vertex_it> vi;
    int N = boost::num_vertices(g);
    /* initialize Distance matrix d with max int values in order to simulate 
       infinity as a value */
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++){
            if (i==j)
                d[std::make_pair(i,j)] = 0;
            else
                d[std::make_pair(i,j)] = std::numeric_limits<int>::max();
            F[std::make_pair(i,j)] = NULL;
            B[std::make_pair(i,j)] = NULL;
        }
    for (vi=boost::vertices(g); vi.first != vi.second; ++vi.first){
        /* create descedants tree for vertex pointed by vi.first. */
        Tree desc;
        vertex_tree root_of_desc = boost::add_vertex(desc);
        /* connect Graph vertex with corrilating descendants tree vertex. */
        desc[root_of_desc].v_in_g = *vi.first;
        desc[root_of_desc].p_in_t = root_of_desc;
        desc[boost::graph_bundle].root = root_of_desc;
        DESC[*vi.first] = desc;
        F[std::make_pair(*vi.first,*vi.first)] = root_of_desc;
        /* create ancestors tree for vertex pointed by vi.first. */
        Tree anc;
        vertex_tree root_of_anc = boost::add_vertex(anc);
        /* connect Graph vertex with corrilating ancestors tree vertex. */
        anc[root_of_anc].v_in_g = *vi.first;
        anc[root_of_anc].p_in_t = root_of_anc;
        anc[boost::graph_bundle].root = root_of_anc;
        ANC[*vi.first] = anc;
        B[std::make_pair(*vi.first,*vi.first)] = root_of_anc;
    }
}


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
std::map<std::pair<vertex_desc,vertex_desc>, vertex_tree> FORWARD,BACKWARD;

void UpdateForwardBackward(vertex_tree x_in_tree, vertex_desc i, vertex_desc j, Tree& pruned_desc_j)
{
    vertex_tree y_in_tree,w;
    std::pair<tree_vertex_it, tree_vertex_it> vi_tree;
    std::queue<vertex_tree> Q;
    Tree N;
    std::pair<out_edges_it,out_edges_it> out_ei;
    Tree anc_i = ANC[i];
    vertex_desc x = anc_i[x_in_tree].vertex_in_graph;
    Tree desc_x = DESC[x];

    Q.push(boost::any_cast<vertex_tree>(pruned_desc_j[boost::graph_bundle].root));
    while(!Q.empty())
    {
        y_in_tree = Q.front();
        vertex_desc y = pruned_desc_j[y_in_tree].vertex_in_graph;
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
        if (dist_x_i + 1 + dist_j_y < dist_x_y){
            /**************************************/
            /* update forward minimal path trees. */
            /**************************************/
            if (y == j){
                /* check if vertex already exist in Desc_x. */
                if (FORWARD[std::make_pair(x,j)]==NULL){
                /* add vertex_for_j in desc_x as a children of i and connect Graph
                   vertex with corrilating descendants tree vertex. */
                    vertex_tree vertex_for_j = boost::add_vertex(desc_x);
                    desc_x[vertex_for_j].vertex_in_graph = j;
                    desc_x[vertex_for_j].parent_in_tree = FORWARD[std::make_pair(x,i)];
                    edge_tree e1 = boost::add_edge(FORWARD[std::make_pair(x,i)],vertex_for_j,desc_x).first;
                    FORWARD[std::make_pair(x,j)] = vertex_for_j;
                }
                else{
                    /* removing old edge to j and adding the correct one. */
                    boost::remove_edge(boost::any_cast<vertex_tree>(desc_x[FORWARD[std::make_pair(x,j)]].parent_in_tree),
                        FORWARD[std::make_pair(x,j)],desc_x);
                    desc_x[FORWARD[std::make_pair(x,j)]].parent_in_tree = FORWARD[std::make_pair(x,i)]; 
                    edge_tree e1 = boost::add_edge(FORWARD[std::make_pair(x,i)],FORWARD[std::make_pair(x,j)],desc_x).first;
                }

                /* create the pruned copy of desc_j. */
                vertex_tree j_in_N = boost::add_vertex(N);
                N[j_in_N].vertex_in_graph = j;
                N[j_in_N].parent_in_tree = j;
                N[boost::graph_bundle].root = j_in_N;
            }
            else{
                vertex_tree parent_of_y = boost::any_cast<vertex_tree>(pruned_desc_j[y_in_tree].parent_in_tree);
                if (FORWARD[std::make_pair(x,y)]==NULL){
                    /* add vertex_for_y in desc_x with the same parent it has in
                    pruned desc_j. */
                    vertex_tree vertex_for_y = boost::add_vertex(desc_x);
                    desc_x[vertex_for_y].vertex_in_graph = y;
                    desc_x[vertex_for_y].parent_in_tree = FORWARD[std::make_pair(x,pruned_desc_j[parent_of_y].vertex_in_graph)];
                    edge_tree e1 = boost::add_edge(FORWARD[std::make_pair(x,desc_x[parent_of_y].vertex_in_graph)],vertex_for_y,desc_x).first;
                    FORWARD[std::make_pair(x,y)] = vertex_for_y;
                }
                else{
                    boost::remove_edge(boost::any_cast<vertex_tree>(desc_x[FORWARD[std::make_pair(x,y)]].parent_in_tree),
                        FORWARD[std::make_pair(x,y)],desc_x);
                    desc_x[FORWARD[std::make_pair(x,y)]].parent_in_tree = FORWARD[std::make_pair(x,desc_x[parent_of_y].vertex_in_graph)]; 
                    edge_tree e1 = boost::add_edge(FORWARD[std::make_pair(x,desc_x[parent_of_y].vertex_in_graph)],
                        FORWARD[std::make_pair(x,y)],desc_x).first;
                }
                /* fill the pruned copy of desc_j. */
                vertex_tree y_in_N = boost::add_vertex(N);
                /* look for vertex y parent in N. */
                for (vi_tree=vertices(N); vi_tree.first != vi_tree.second; ++vi_tree.first){
                    if (N[*vi_tree.first].vertex_in_graph == pruned_desc_j[parent_of_y].vertex_in_graph)
                        break;
                }
                N[y_in_N].vertex_in_graph = y;
                N[y_in_N].parent_in_tree = *vi_tree.first;
                edge_tree e2 = boost::add_edge(*vi_tree.first,y_in_N,desc_x).first;
            }
            /***************************************/
            /* update backward minimal path trees. */
            /***************************************/
            if (x == i){
                if (BACKWARD[std::make_pair(y,i)]==NULL){
                    vertex_tree vertex_for_i = boost::add_vertex(anc_y);
                    anc_y[vertex_for_i].vertex_in_graph = i;
                    anc_y[vertex_for_i].parent_in_tree = j;
                    edge_tree e2 = boost::add_edge(BACKWARD[std::make_pair(y,j)],vertex_for_i,anc_y).first;
                    BACKWARD[std::make_pair(y,i)] = vertex_for_i;
                }
                else{
                    boost::remove_edge(boost::any_cast<vertex_tree>(anc_y[BACKWARD[std::make_pair(y,i)]].parent_in_tree),
                        BACKWARD[std::make_pair(y,i)],anc_y);
                    anc_y[BACKWARD[std::make_pair(y,i)]].parent_in_tree = BACKWARD[std::make_pair(y,j)];
                    edge_tree e2 = boost::add_edge(BACKWARD[std::make_pair(y,j)],BACKWARD[std::make_pair(y,i)],anc_y).first;
                }
            }
            else{
                vertex_tree parent_of_x = boost::any_cast<vertex_tree>(anc_i[x_in_tree].parent_in_tree);
                if (BACKWARD[std::make_pair(y,x)]==NULL){
                    vertex_tree vertex_for_x = boost::add_vertex(anc_y);
                    anc_y[vertex_for_x].vertex_in_graph = x;
                    anc_y[vertex_for_x].parent_in_tree = BACKWARD[std::make_pair(y,anc_i[parent_of_x].vertex_in_graph)];
                    edge_tree e2 = boost::add_edge(BACKWARD[std::make_pair(y,anc_i[parent_of_x].vertex_in_graph)],vertex_for_x,anc_y).first;
                    BACKWARD[std::make_pair(y,x)] = vertex_for_x;
                }
                else{
                    boost::remove_edge(boost::any_cast<vertex_tree>(anc_y[BACKWARD[std::make_pair(y,x)]].parent_in_tree),
                        BACKWARD[std::make_pair(y,x)],anc_y);
                    anc_y[BACKWARD[std::make_pair(y,x)]].parent_in_tree = BACKWARD[std::make_pair(y,anc_i[parent_of_x].vertex_in_graph)]; 
                    edge_tree e2 = boost::add_edge(BACKWARD[std::make_pair(y,anc_i[parent_of_x].vertex_in_graph)],
                        BACKWARD[std::make_pair(y,x)],anc_y).first;
                }
            }
            /* updating distance matrix values. */
            d[std::make_pair(x,y)] = d[std::make_pair(x,i)]+1+d[std::make_pair(j,y)];
            /* iterating over out edges of y and adding y's children in Q. */
            for (out_ei=out_edges(y_in_tree,pruned_desc_j); out_ei.first != out_ei.second; ++out_ei.first){
                Q.push(target(*out_ei.first,pruned_desc_j));
            }
        }
    }
    /* wow so much recursiveness. */
    if (boost::num_vertices(N)!=0){
        for (out_ei=out_edges(x_in_tree,anc_i); out_ei.first != out_ei.second; ++out_ei.first){
            UpdateForwardBackward(target(*out_ei.first,anc_i),i,j,N);
        }
    }
}

/**********************************/
/* Data structure initialization. */
/**********************************/
void aimn91(Graph& g, DistanceMap& D)
{
    std::pair<vertex_it, vertex_it> vi;
    int N = boost::num_vertices(g);
    /* initialize Distance matrix d with max int values in order to simulate 
       infinity as a value */
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++){
            if (i==j) d[std::make_pair(i,j)] = 0;
            else d[std::make_pair(i,j)] = std::numeric_limits<int>::max();
            FORWARD[std::make_pair(i,j)] = NULL;
            BACKWARD[std::make_pair(i,j)] = NULL;
        }
    for (vi=boost::vertices(g); vi.first != vi.second; ++vi.first){
        /* create descedants tree for vertex pointed by vi.first. */
        Tree desc;
        vertex_tree root_of_desc = boost::add_vertex(desc);
        /* connect Graph vertex with corrilating descendants tree vertex. */
        desc[root_of_desc].vertex_in_graph = *vi.first;
        desc[root_of_desc].parent_in_tree = root_of_desc;
        desc[boost::graph_bundle].root = root_of_desc;
        DESC[*vi.first] = desc;
        FORWARD[std::make_pair(*vi.first,*vi.first)] = root_of_desc;
        /* create ancestors tree for vertex pointed by vi.first. */
        Tree anc;
        vertex_tree root_of_anc = boost::add_vertex(anc);
        /* connect Graph vertex with corrilating ancestors tree vertex. */
        anc[root_of_anc].vertex_in_graph = *vi.first;
        anc[root_of_anc].parent_in_tree = root_of_anc;
        anc[boost::graph_bundle].root = root_of_anc;
        ANC[*vi.first] = anc;
        BACKWARD[std::make_pair(*vi.first,*vi.first)] = root_of_anc;
    }
}


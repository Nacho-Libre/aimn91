/* Gryllos Prokopis 2013-2014
 * Implementation of dynamic incremental algorithm for minimal length paths
 * [AIMN91] as described in the Journal of Algorithms 12 (1991), pp.615-638.
 * header file.
 */

#include <boost/graph/adjacency_list.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/assign/ptr_map_inserter.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/foreach.hpp>
#include <boost/any.hpp>
#include <stdexcept>
#include <limits>
#include <stack>
#include <chrono>

// tree graph info
struct tree_graph_info {
        boost::any root;
};
// custom vertex for tree graphs
struct tree_vertex_info{
    // corrilating vertex in graph
    boost::any v_g;
    // pointer to parent vertex
    boost::any p_in_t;
};

// the tree graph type for descendants and ancestor trees
typedef boost::adjacency_list<boost::listS, boost::listS, boost::directedS,
        tree_vertex_info, boost::no_property, tree_graph_info> Tree;

struct vertex_info{
        Tree ANC;
        Tree DESC;
};

// Graph type definition
typedef boost::adjacency_list < boost::listS,boost::listS,boost::directedS,
        vertex_info,
        boost::property < boost::edge_weight_t, int > > Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor v_desc;
typedef boost::graph_traits<Graph>::vertex_iterator vertex_it;
typedef boost::graph_traits<Graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<Graph>::edge_iterator e_it;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

// Graph type to test with other algorithms
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
        boost::no_property,
        boost::property< boost::edge_weight_t, int> > Graph_test;

typedef boost::graph_traits<Graph_test>::edge_descriptor edge_desc_t;
typedef boost::graph_traits<Graph_test>::vertex_iterator vertex_it_t;

typedef boost::graph_traits<Tree>::vertex_descriptor vertex_tree;
typedef boost::graph_traits<Tree>::edge_descriptor edge_tree;
typedef boost::graph_traits<Tree>::out_edge_iterator out_edges_it;
typedef boost::graph_traits<Tree>::vertex_iterator tree_vertex_it;

/* Distance matrix is implemented using a pair (x,y) of vertex descriptors as 
   index to an std::map that returns an integer value which represents the
   shortest path distance between from vertex x to vertex y. */
typedef std::map<std::pair<v_desc,v_desc>, int> Map;
typedef std::stack<edge_desc> path;

class DistanceMap{
        public:
            DistanceMap(Graph& G);
            void add_e(v_desc x, v_desc y, int w);
            void decrease_w(v_desc x, v_desc y, int w);
            int length(v_desc x, v_desc y);
            path minpath(v_desc x, v_desc y);
        private:
            Map d;
            Graph& g;
            /* We are going to use this type to store pointers to the ANC and DESC trees
               vertices. */
            std::map<std::pair<v_desc,v_desc>, vertex_tree> F,B;
            void UpdateForwardBackward(vertex_tree x_in_tree, v_desc i,
                v_desc j, Tree& desc_j, vertex_tree root, int w);
};

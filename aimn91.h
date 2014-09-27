/* Gryllos Prokopis 2013-2014
 * Implementation of dynamic incremental algorithm for minimal length paths
 * [AIMN91] as described in the Journal of Algorithms 12 (1991), pp.615-638.
 * header file.
 */

#include <boost/graph/adjacency_list.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/any.hpp>
#include <limits>
#include <queue>

// Graph type definition
typedef boost::adjacency_list < boost::vecS,boost::vecS,boost::undirectedS,
        boost::no_property,
        boost::property < boost::edge_weight_t, int > > Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<Graph>::vertex_iterator vertex_it;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

// tree graph info
struct graph_info { 
        boost::any root;
};
// custom vertex for tree graphs
struct tree_vertex_info{
    // corrilating vertex in graph
    vertex_desc v_in_g;
    // pointer to parent vertex
    boost::any p_in_t;
};
// the tree graph type for descendants and ancestor trees
typedef boost::adjacency_list<boost::listS, boost::listS, boost::directedS,
        tree_vertex_info, boost::no_property, graph_info> Tree;

typedef boost::graph_traits<Tree>::vertex_descriptor vertex_tree;
typedef boost::graph_traits<Tree>::edge_descriptor edge_tree;
typedef boost::graph_traits<Tree>::out_edge_iterator out_edges_it;
typedef boost::graph_traits<Tree>::vertex_iterator tree_vertex_it;

/* Distance matrix is implemented using a pair (x,y) of vertex descriptors as 
   index to an std::map that returns an integer value which represents the
   shortest path distance between from vertex x to vertex y. */
typedef std::map<std::pair<vertex_desc,vertex_desc>, int> Map;
typedef std::stack<vertex_desc> path;

class DistanceMap{
        public:
            void init_DistanceMap(Graph& g);
            void add(vertex_desc x, vertex_desc y, int w);
            void decrease(vertex_desc x, vertex_desc y, int w);
            int length(vertex_desc x, vertex_desc y);
            path minpath(vertex_desc x, vertex_desc y);
            void UpdateForwardBackward(vertex_tree x_in_tree, vertex_desc i,
                vertex_desc j, Tree& pruned_desc_j, int w);
        private:
            Map d;
            /* using boost pointer map we associate each vertex with a set of descendants
               organised as a tree of forward minimal paths (DESC[x]) rooted at x and a set
               of ancestors organized as a tree of backward minimal paths (ANC[x]) rooted at
               x and Initialize each tree with its root vertex. */
            boost::ptr_map<vertex_desc, Tree> DESC,ANC;
            /* We are going to use this type to store pointers to the ANC and DESC trees
               vertices. */
            std::map<std::pair<vertex_desc,vertex_desc>, vertex_tree> F,B;
}

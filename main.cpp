/* Gryllos Prokopis 2013-2014
 * Implementation of dynamic incremental algorithm for minimal length paths
 * [AIMN91] as described in the Journal of Algorithms 12 (1991), pp.615-638.
 * main file for testing data structure.
 */
#include "aimn91.h"

int main(){
    /* create a sample graph with no edges and test data structure */
    Graph g;
    v_desc v,z;
    for(int i=0;i<10;i++){
        v = boost::add_vertex(g);
        z = boost::add_vertex(g);
    }
    DistanceMap d(g);

    std::cout<<"i = "<<v<<", j = "<<z<<std::endl;
    d.add(v,z,6,g);
    d.add(z,v,6,g);
    std::cout<<num_edges(g)<<std::endl;

    return 0;
}

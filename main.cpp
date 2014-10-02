/* Gryllos Prokopis 2013-2014
 * Implementation of dynamic incremental algorithm for minimal length paths
 * [AIMN91] as described in the Journal of Algorithms 12 (1991), pp.615-638.
 * main file for testing data structure.
 */
#include "aimn91.h"

int main(){
    /* create a sample graph with no edges and test data structure */
    Graph g;
    v_desc v,z,k,r,f;
    v = boost::add_vertex(g);
    z = boost::add_vertex(g);
    k = boost::add_vertex(g);
    r = boost::add_vertex(g);
    f = boost::add_vertex(g);
    DistanceMap d(g);
    edge_desc e;

    std::cout<<"i = "<<v<<", j = "<<z<<std::endl;
    d.add_e(v,z,6,g);
    std::cout<<"i = "<<z<<", j = "<<k<<std::endl;
    d.add_e(z,k,2,g);
    std::cout<<"i = "<<z<<", j = "<<r<<std::endl;
    d.add_e(z,r,7,g);
    std::cout<<"i = "<<f<<", j = "<<v<<std::endl;
    d.add_e(f,v,5,g);
    path minpath = d.minpath(f,r,g);
    while(!minpath.empty()){
        e = minpath.top();
        std::cout<<boost::source(e,g)<<" "<<boost::target(e,g)<<std::endl;
        minpath.pop();
    }
    d.add_e(v,r,3,g);
    minpath = d.minpath(f,r,g);
    while(!minpath.empty()){
        e = minpath.top();
        std::cout<<boost::source(e,g)<<" "<<boost::target(e,g)<<std::endl;
        minpath.pop();
    }

    return 0;
}

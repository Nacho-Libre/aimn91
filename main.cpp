/* Gryllos Prokopis 2013-2014
 * Implementation of dynamic incremental algorithm for minimal length paths
 * [AIMN91] as described in the Journal of Algorithms 12 (1991), pp.615-638.
 * main file for testing data structure.
 */
#include "aimn91.h"

#define NUMBER_OF_VERTICES 1000
#define RANDOMNESS 20
#define RANGE 30

int main(){
    int graph_size [] = {100,500,1000};
    // auxiliary time variables
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> el_s;
    /* randomness out of thin air generator */
    boost::random::mt19937 gen;
    boost::random::uniform_int_distribution<> dist_1(1, RANDOMNESS);
    boost::random::uniform_int_distribution<> dist_2(1, RANGE);

    for(unsigned int k=0; k<sizeof(graph_size)/sizeof(graph_size[0]); k++){
        /* create a pseudo-random DAG */
        Graph g;
        /* create a second graph with different type for testing */
        Graph_test g_test;
        std::pair<vertex_it,vertex_it> it_out,it_in;
        std::pair<vertex_it_t,vertex_it_t> it_out_t,it_in_t;
        for(int i=0;i<graph_size[k];i++){
            boost::add_vertex(g);
            boost::add_vertex(g_test);
        }
        /* for each vertex in the graph with propability 1/RANDOMNESS create edge
           to another vertex with lower index number and assign random weight in
           range [1..RANGE] */
        gen.seed(std::time(NULL));
        for (it_out=boost::vertices(g); it_out.first != it_out.second; ++it_out.first)
            for (it_in=boost::vertices(g); it_in.first != it_in.second; ++it_in.first){
                if(*it_out.first<*it_in.first){
                    if(dist_1(gen)==1){
                        edge_desc e = boost::add_edge(*it_out.first,*it_in.first,g).first;
                        int weight = dist_2(gen)+1;
                        boost::put(boost::edge_weight, g, e, weight); //assign weight to the new edge
                    }
                }
        }
        std::cout<<"\n\n\t**********[testing upon graph initialization]**********"<<std::endl;
        std::cout<<"\tNumber of edges in DistanceMap Graph "<<boost::num_edges(g)<<std::endl;
        // timing aimn91 on graph g
        start = std::chrono::system_clock::now();
        DistanceMap d(g);
        end = std::chrono::system_clock::now();
        el_s = end - start;
        std::cout<<"\t\t>>>[AIMN91] elapsed time->"<<el_s.count()<<"s \n";
        /* fill test graph for johnson and floy warshal algorithms */
        gen.seed(std::time(NULL));
        for (it_out_t=boost::vertices(g_test); it_out_t.first != it_out_t.second; ++it_out_t.first)
            for (it_in_t=boost::vertices(g_test); it_in_t.first != it_in_t.second; ++it_in_t.first){
                if(*it_out_t.first<*it_in_t.first){
                    if(dist_1(gen)==1){
                        edge_desc_t e = boost::add_edge(*it_out_t.first,*it_in_t.first,g_test).first;
                        int weight = dist_2(gen)+1;
                        boost::put(boost::edge_weight, g_test, e, weight); //assign weight to the new edge
                    }
                }
             }
        std::cout<<"\tNumber of edges in Testing Graph "<<boost::num_edges(g_test)<<std::endl;
        int D[NUMBER_OF_VERTICES][NUMBER_OF_VERTICES];
        // timing johnson_all_pairs_shortest_paths on g_test
        start = std::chrono::system_clock::now();
        boost::johnson_all_pairs_shortest_paths(g_test, D);
        end = std::chrono::system_clock::now();
        el_s = end - start;
        std::cout<<"\t\t>>>[johnson_all_pairs_shortest_paths] elapsed time->"<<el_s.count()<<"s \n";

        // timing floyd_warshall_all_pairs_shortest_paths on g_test
        start = std::chrono::system_clock::now();
        boost::floyd_warshall_all_pairs_shortest_paths(g_test, D);
        end = std::chrono::system_clock::now();
        el_s = end - start;
        std::cout<<"\t\t>>>[floyd_warshall_all_pairs_shortest_paths] elapsed time->"<<el_s.count()<<"s \n";

        std::cout<<"\n\t**************[testing upon edge addition]**************"<<std::endl;

        int count=0;
        v_desc v_1,v_2;
        gen.seed(std::time(NULL));
        // first we need to select some random vertices from each graph
        for (it_out=boost::vertices(g); it_out.first != it_out.second; ++it_out.first){
            int rand = dist_1(gen);
            if(rand==10){
                v_1 = *it_out.first;
                count++;
                if(dist_1(gen)==2 && count>=2) break;
            }
            if(rand==7){
                v_2 = *it_out.first;
                count++;
                if(dist_1(gen)==2 && count>=2) break;
            }
        }
        if(!count>=2) std::cout<<"WARNING! an edge was not selected"<<std::endl;
        // calculating elapsed time for edge addition and distance map update
        int weight = dist_2(gen);
        start = std::chrono::system_clock::now();
        d.add_e(v_1,v_2,weight);
        end = std::chrono::system_clock::now();
        el_s = end - start;
        std::cout<<"\t\t>>>[AIMN91] edge addition and update elapsed time->"<<el_s.count()<<"s \n";
    }
    return 0;
}

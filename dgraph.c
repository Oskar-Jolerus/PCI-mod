 /****************************************************************
  * Course:     Datastrukturer och algoritmer
  * Assignment: Obligatorisk uppgift 5
  * Created by: Carl-Johan Möller and Björn Ahlman
  * Data ID:    tfy14cmr and tfy14ban
  * Cas ID:     cama0152 and bjnahn04
  * Date:       31/5/2017 
  ****************************************************************/

#include "dgraph.h"

//****************************************************************
/* Function that creates an array representing the graph. The first dimension 
 * represents all nodes in the graph and the second dimension represents the 
 * neigbours for each node.
 *
 * In:      - number_of_nodes - Int representing the number of nodes in the 
 *              file containing the nodes.
 *
 * Returns: A pointer to the graph.
 */
graph *graph_empty(int number_of_nodes){
    graph *theGraph = malloc(sizeof(graph));
    theGraph->array_graph = array_create(2,0,0,number_of_nodes,number_of_nodes);
    theGraph->index = 0;
    return theGraph;
}

//****************************************************************
/* Function that inserts a node and an empty list for its neighbours in the 
 * graph
 *
 * In:      - *theGraph- Pointer to the graph
 *          - v - Char representing the node to be inserted
 *
 * Returns:
 */
void graph_insertNode(graph *theGraph,char v[]){
    array_setValue(theGraph->array_graph,v,theGraph->index,0);
    array_setValue(theGraph->array_graph,dlist_empty(),theGraph->index,1);
    theGraph->index++;
}

//****************************************************************
/* Function that inserts an edge between two nodes in the graph. 
 * graph
 *
 * In:      - *theGraph- Pointer to the graph
 *          - node_from - Char representing the start node
 *          - node_to - Char representing the neigbour node to the start node
 *
 * Returns:
 */
void graph_insertEdge(graph *theGraph,char node_from[],char node_to[]){
    /* For all nodes in the graph, check if the current node is the same as the
     * start node. If true insert the neighbour node in the neighbours list.*/
    for (int i = 0; i < theGraph->index; i++){
        dlist *current_list = (dlist*)array_inspectValue(
            theGraph->array_graph,i,1);
        dlist_position p = dlist_first(current_list);
        if (strcmp((char *) array_inspectValue(
            theGraph->array_graph,i,0),node_from) == 0){
            if (dlist_isEmpty(current_list)){
                dlist_insert(current_list,p,node_to);
                return;
            }
            /* Go through all neighbours and check if the neighbour node is 
             * present. If not, insert it in the neighbour list.*/
            while (!dlist_isEnd(current_list,p)) {
                if (strcmp((char*)dlist_inspect(current_list,p), node_to) == 0){
                    return;
                }
                p = dlist_next(current_list,p);
            }
            dlist_insert(current_list,p,node_to);
            return;
        }
    }
}

//****************************************************************
/* Function that looks up the neighbours of a specific node.
 * graph
 *
 * In:      - *theGraph- Pointer to the graph
 *          - v- Char representing the node for which the neighbours are looked 
 *              for
 *
 * Returns: The list of neigbours corresponding to the input node if the input
 *          node is in the graph. Else NULL
 */
dlist *graph_getNeighbours(graph *theGraph,char v[]){
    for (int i = 0;i < theGraph->index;i++){
        if (strcmp((char*)array_inspectValue(
            theGraph->array_graph,i,0),v) == 0){
            return (dlist*)array_inspectValue(theGraph->array_graph,i,1);
        }
    }
    return NULL;
}

//****************************************************************
/* Function that checks if the graph contains a specific node.
 * graph
 *
 * In:      - *theGraph- Pointer to the graph
 *          - v- char representing the node which is checked if it is in the 
 *              graph
 *
 * Returns: True if the graph contains the node. Else false.
 */
bool graph_containsNode(graph *theGraph,char v[]){
    for (int i = 0;i < theGraph->index;i++){
        if (strcmp((char*)array_inspectValue(
            theGraph->array_graph,i,0),v) == 0){
            return true;
        }
    }
    return false;
}

//****************************************************************
/* Function that frees up the memory allocated by the graph.
 * graph
 *
 * In:      - *theGraph- Pointer to the graph
 *
 * Returns: 
 */
void graph_freeGraph(graph *theGraph){
    for (int i = 0;i < theGraph->index;i++){
        dlist_free(array_inspectValue(theGraph->array_graph,i,1));
    }
    array_free(theGraph->array_graph);
    free(theGraph);
}
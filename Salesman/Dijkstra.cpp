#include "stdafx.h"
#include "CGraph.h"


// =============================================================================
// Dijkstra ====================================================================
// =============================================================================

void CGraph::Dijkstra(CVertex *pStart)
{

	list<CVertex*> unvisited;

	for (CVertex v : m_Vertices) {
		v.m_DijkstraDistance = numeric_limits<double>::max();
		v.m_DijkstraVisit = false;
		unvisited.push_front(&v);	
	}


	pStart->m_DijkstraDistance = 0;

	CVertex* v = pStart;
	
	while (!unvisited.empty()) {
		CVertex* u = unvisited.front();
		for (CVertex* a:u->m_Neighbords) {
			double dist = (&v->m_Point - &u->m_Point);
			if (dist < a->m_DijkstraDistance) {
				a->m_DijkstraDistance = dist;
				a->m_pDijkstraPrevious = v;
			}
		}
		unvisited.pop_front();
	}



}

// =============================================================================
// DijkstraQueue ===============================================================
// =============================================================================

void CGraph::DijkstraQueue(CVertex *pStart)
{
	pStart->m_DijkstraDistance = 0;


	for (CVertex &v : m_Vertices){
		v.m_DijkstraDistance = numeric_limits<double>::max();
	}

	pStart->m_DijkstraDistance = 0;
	

}

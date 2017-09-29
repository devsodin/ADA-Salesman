#include "stdafx.h"
#include "CGraph.h"


// =============================================================================
// Dijkstra ====================================================================
// =============================================================================


double CalcDistances(CVertex source, CVertex destination) {
	double dist = hypot(destination.m_Point.m_X - source.m_Point.m_X, destination.m_Point.m_Y - source.m_Point.m_Y);
	return dist;
}

void CGraph::Dijkstra(CVertex *pStart)
{
	for (CVertex &v : m_Vertices) {
		v.m_DijkstraDistance = numeric_limits<double>::max();
		v.m_DijkstraVisit = false;
	}
	
	CVertex* selected = pStart;
	selected->m_DijkstraDistance = 0;
	
	while (!selected->m_DijkstraVisit) {
		selected->m_DijkstraVisit = true;
		
		for (CVertex* c : selected->m_Neighbords) {
			double dist = (c->m_Point - selected->m_Point).Module();
			if (dist < c->m_DijkstraDistance) {
				c->m_DijkstraDistance = dist;
				c->m_pDijkstraPrevious = selected;
			}
		}
		
		for (CVertex &c :m_Vertices) {
			if (!(c.m_DijkstraVisit))
				selected = &c;
		}
	}


}



// =============================================================================
// DijkstraQueue ===============================================================
// =============================================================================

void CGraph::DijkstraQueue(CVertex *pStart)
{
	for (CVertex &v : m_Vertices) {
		v.m_DijkstraDistance = numeric_limits<double>::max();
		v.m_DijkstraVisit = false;
	}

	CVertex* selected = pStart;
	selected->m_DijkstraDistance = 0;


	

}

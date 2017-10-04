#include "stdafx.h"
#include "CGraph.h"


// =============================================================================
// Dijkstra ====================================================================
// =============================================================================
void CGraph::Dijkstra(CVertex *pStart)
{
	// Set all vertex to infinite distance(unknown) and not visited
	for (CVertex &v : m_Vertices) {
		v.m_DijkstraDistance = numeric_limits<double>::max();
		v.m_DijkstraVisit = false;
	}
	printf("set");
	// Set source vertex to dist 0
	CVertex* selected = pStart;
	selected->m_DijkstraDistance = 0;
	//selected->m_DijkstraVisit = false;
	printf("first");

	while (true) {
		printf("loop_start");
		for (CVertex* x : selected->m_Neighbords) {
			double dist = selected->m_DijkstraDistance + (x->m_Point - selected->m_Point).Module();
			if (dist < x->m_DijkstraDistance) {
				x->m_DijkstraDistance = dist;
				x->m_pDijkstraPrevious = selected;
			}
		}
		printf("neighs");
		selected->m_DijkstraVisit = true;

		CVertex* tmp = NULL;
		for (CVertex &a : m_Vertices) {
			if (!a.m_DijkstraVisit && (tmp == NULL|| a.m_DijkstraDistance < tmp->m_DijkstraDistance)) {
				tmp = &a;
				//mindist = a.m_DijkstraDistance;
			}

		}

		if (tmp == NULL) {
			break;
		}
		selected = tmp;
	}
}
//=============================================================================
//DijkstraQueue ===============================================================
//=============================================================================


void CGraph::DijkstraQueue(CVertex *pStart)
{	// Set all vertex to infinite distance(unknown) and not visited
	for (CVertex &v : m_Vertices) {
		v.m_DijkstraDistance = numeric_limits<double>::max();
		v.m_DijkstraVisit = false;
	}
	
	
	CVertex s = *pStart;
	s.m_DijkstraVisit = true;
	s.m_DijkstraDistance = 0;

	auto comparepq = [](CVertex z, CVertex x) { return z.m_DijkstraDistance < x.m_DijkstraDistance; };
	priority_queue < CVertex, vector<CVertex>, decltype(comparepq) > pq(comparepq);

	pq.push(s);

	while (!pq.empty()) {
		CVertex selected = pq.top();
		pq.pop();
		for (CVertex* u : selected.m_Neighbords) {
			double dist = selected.m_DijkstraDistance + (u->m_Point - selected.m_Point).Module();
			if (dist < u->m_DijkstraDistance) {
				u->m_DijkstraDistance = dist;
			}
			if (!u->m_DijkstraVisit) {
				u->m_DijkstraVisit = true;
				pq.push(*u);
			}
		}
	}
}

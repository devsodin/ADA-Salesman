#include "stdafx.h"
#include "CGraph.h"

// SalesmanTrackGreedy =========================================================

CTrack CGraph::SalesmanTrackGreedy(CVisits &visits) 
{
	CTrack track(this);
	list<CVertex*> unvisited = visits.m_Vertices;
	for (CVertex* v : visits.m_Vertices) {
		v->m_VertexToVisit = true;
		unvisited.push_back(v);
	}

	CVertex* selected = visits.m_Vertices.front();
	track.AddFirst(selected);
	unvisited.remove(selected);

	do {
		if(selected->m_VertexToVisit){
			selected->m_VertexToVisit = false;
			visits.m_pGraph->Dijkstra(selected);
			double mindist = numeric_limits<double>::max();
			for (CVertex* neigh : selected->m_Neighbords) {
				if (neigh != visits.m_Vertices.back() && neigh->m_DijkstraDistance < mindist) {
					if (neigh->m_VertexToVisit) {
						selected = neigh;
					}
				}
			}
			unvisited.remove(selected);
			
		}
		track.AddLast(selected);
		

	} while (unvisited.size() > 1);
	track.AddLast(visits.m_Vertices.back());


	return track;
}

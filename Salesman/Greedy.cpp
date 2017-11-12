#include "stdafx.h"
#include "CGraph.h"

// SalesmanTrackGreedy =========================================================
void reconstruct(CVertex* next, CTrack* track) {
	if (track->m_Vertices.back() == next->m_pDijkstraPrevious) {
		track->AddLast(next);
		return;
	}
	else {
		reconstruct(next->m_pDijkstraPrevious, track);
		track->AddLast(next);
	}
}

CTrack CGraph::SalesmanTrackGreedy(CVisits &visits)
{
	CTrack track(this);
	double minDist;

	CVertex* selected = visits.m_Vertices.front();
	track.AddFirst(selected);
	visits.m_Vertices.pop_front();

	CVertex* last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();

	CGraph::Dijkstra(selected);
	//Por cada nodo a visitrar
	while (!visits.m_Vertices.empty()) {
		//Seleccionar la proxima visita mas cercana
		minDist = numeric_limits<double>::max();
		for (CVertex* x : visits.m_Vertices) {
			if (minDist > x->m_DijkstraDistance) {
				minDist = x->m_DijkstraDistance;
				selected = x;
			}
		}
		//quitar de la lista y hacer camino del nodo anterior al nuevo
		visits.m_Vertices.remove(selected);
		reconstruct(selected, &track);
		CGraph::Dijkstra(selected);
	}
	//reconstruir camino hasta el ultimo nodo
	reconstruct(last, &track);
	return track;

}
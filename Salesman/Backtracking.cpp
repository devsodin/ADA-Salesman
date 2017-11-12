#include "stdafx.h"
#include "CGraph.h"
#include <set>

void pure_bt(CVisits &visits, CTrack track, set<CVertex*>a_to_b, CTrack &bestTrack, double &bestDist, CVertex* lastVisit) {

	//Caso base
	if (track.Length() > bestDist)
		return;

	CVisits aux_visits = visits;
	CTrack aux_track = track;
	set<CVertex*> aux_a_to_b = a_to_b;

	if (visits.m_Vertices.empty()) {
		bestTrack = track;
		bestDist = bestTrack.Length();
		return;
	}
	else {
		CVertex* selected = track.m_Vertices.back();
		bool visit = false;

		for (CVertex* n : selected->m_Neighbords) {
			if (!a_to_b.count(n)) {
				for (CVertex* temp : visits.m_Vertices) {
					if (temp == n)
						visit = true;
				}
				track.AddLast(n);
				a_to_b.insert(n);
				if (track.Length() < bestDist && visit) {
					a_to_b.clear();
					a_to_b.insert(n);
					visits.m_Vertices.remove(n);
					pure_bt(visits, track, a_to_b, bestTrack, bestDist, lastVisit);
				}
				else {
					pure_bt(visits, track, a_to_b, bestTrack, bestDist, lastVisit);
				}

				visits = aux_visits;
				track = aux_track;
				a_to_b = aux_a_to_b;
			}
		}
		return;
	}
}

CTrack CGraph::SalesmanTrackBacktracking(CVisits &visits)
{
	CTrack track(this);
	set<CVertex*> a_to_b;

	double bestdist = numeric_limits<double>::max();
	CTrack bestTrack(this);

	CVertex* first = visits.m_Vertices.front();
	visits.m_Vertices.pop_front();
	track.AddFirst(first);
	a_to_b.insert(first);

	CVertex* last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();

	pure_bt(visits, track, a_to_b, bestTrack, bestdist, last);

	CVertex *last_previous = bestTrack.m_Vertices.back();
	CGraph::Dijkstra(last_previous);
	CVertex* path = last;
	CTrack aux;
	while (last_previous != path) {
		aux.AddFirst(path);
		path = path->m_pDijkstraPrevious;
	}

	bestTrack.Append(aux);
	return bestTrack;
}

// SalesmanTrackBacktrackingGreedy ============================================
void greedy_bt(list<int> &best, list<int> &visited, list<int> &toVisit, vector<double> &matrix, double &bestCost, double partialCost, int &nVertex, vector<double> &minCost) {

	if (toVisit.size() == 0) {
		int d = matrix[visited.back() * nVertex + nVertex - 1];
		if (partialCost + d < bestCost) {
			best = visited;
			bestCost = partialCost + d;
		}
	}
	//Not a leaf (regular case)
	else {
		double h = 0;
		for (int he : toVisit) {
			h += minCost[he];
		}
		h += minCost[nVertex - 1];
		if (partialCost + h - 1 <= bestCost) {
			int actual = visited.back();
			int distance;
			list<int> aux_toVisit = toVisit;
			for (int i : aux_toVisit) {
				distance = matrix[actual * nVertex + i];
				//Still viable?
				if (partialCost + distance < bestCost) {
					//Delete other nodes and add path
					toVisit.remove(i);
					visited.push_back(i);
					//next level call
					greedy_bt(best, visited, toVisit, matrix, bestCost, partialCost + distance, nVertex, minCost);
					//turn back (remove path and add other nodes)
					visited.remove(i);
					toVisit.push_front(i);
				}
			}
		}
	}
}

CTrack CGraph::SalesmanTrackBacktrackingGreedy(CVisits &visits)
{

	CTrack bestTrack(this);
	CTrack partial_track(this);

	list<int> visited;
	visited.push_front(0);
	list<int> best;
	list<int> toVisit;


	//Set bestCost to inf
	double bestCost = numeric_limits<double>::max();

	//Matrix to store costs and association pos-Vertex
	vector<double> cost_Matrix;
	int n_vertex = visits.m_Vertices.size();
	cost_Matrix.resize(n_vertex * n_vertex);
	vector<CVertex*> v{ begin(visits.m_Vertices), end(visits.m_Vertices) };

	vector<double> minCost;
	minCost.resize(n_vertex);

	//Calculate distances
	for (int i = 0; i < n_vertex; i++) {
		CGraph::Dijkstra(v[i]);
		for (int j = 0; j < n_vertex; j++) {
			cost_Matrix[i * n_vertex + j] = (v[j]->m_DijkstraDistance);
		}
	}

	double min;
	for (int j = 0; j < n_vertex; j++) {
		min = cost_Matrix[0 + j];
		for (int i = 0; i < n_vertex; i++) {
			if (cost_Matrix[i * n_vertex + j] < min && i != j) {
				min = cost_Matrix[i*n_vertex + j];
			}
			minCost[j] = min;
		}
	}

	CVertex* first = visits.m_Vertices.front();
	visits.m_Vertices.pop_front();
	CVertex* last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();


	int count = 1;
	for (CVertex* pCount : visits.m_Vertices) {
		toVisit.push_back(count);
		count++;
	}

	//Call the backtracking function
	greedy_bt(best, visited, toVisit, cost_Matrix, bestCost, 0, n_vertex, minCost);

	best.push_back(n_vertex - 1);

	//Path creation
	int Start_point = best.front();
	CVertex* Start_vertex = v[Start_point];

	int last_point;
	CVertex* last_vertex;

	best.pop_front();
	bestTrack.AddFirst(first);


	for (int n : best) {
		partial_track.Clear();
		last_point = n;
		last_vertex = v[last_point];
		CGraph::Dijkstra(v[Start_point]);

		while (Start_vertex != last_vertex) {
			partial_track.AddFirst(last_vertex);
			last_vertex = last_vertex->m_pDijkstraPrevious;
		}

		Start_point = last_point;
		Start_vertex = v[Start_point];
		bestTrack.Append(partial_track);

	}
	return bestTrack;
}

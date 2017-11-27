#include "stdafx.h"
#include "CGraph.h"

class bnb_Node {
public:
	bnb_Node(list<int> tovisit, double h, double cost, list<int> sol);
	double heuristic;
	double cost;

	list<int> m_tovisit;
	list<int> sol;

};
bnb_Node::bnb_Node(list<int> tovisit, double h, double pcost, list<int> solution) {
	cost = pcost;
	heuristic = h;

	sol = solution;
	m_tovisit = tovisit;
}


struct comparator {
	bool operator()(bnb_Node const *i1, bnb_Node const *i2) {
		return i1->heuristic < i2->heuristic;
	}
};

// SalesmanTrackBranchAndBound1 ===================================================

CTrack CGraph::SalesmanTrackBranchAndBound1(CVisits &visits)
{
	int n_visits = visits.m_Vertices.size();
	priority_queue<bnb_Node*, vector<bnb_Node*>, comparator> priorityQueue;
	double mincost = numeric_limits<double>::max();
	list<int> visited, toVisit, solution;

	vector<double> dijkstraMatrix;
	dijkstraMatrix.resize(n_visits*n_visits);
	vector<CVertex*> v{ begin(visits.m_Vertices), end(visits.m_Vertices) };

	for (int start = 0; start < n_visits; start++) {
		CGraph::Dijkstra(v[start]);
		for (int end = 0; end < n_visits; end++)
			dijkstraMatrix[start*n_visits + end] = (v[end]->m_DijkstraDistance);
	}

	visited.push_front(0);
	CVertex *first = visits.m_Vertices.front();
	CVertex *last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();
	visits.m_Vertices.pop_front();

	//Excepcion para grafos de 1 y 2 aristas
	if (n_visits <= 2) {
		for (int nv = 0; nv < n_visits; nv++)
			solution.push_back(nv);
		goto exit_bnb1;
	}

	int count = 1;
	for (CVertex *pCount : visits.m_Vertices) {
		toVisit.push_back(count);
		count++;
	}

	bnb_Node *selected = new bnb_Node(toVisit, 0, 0, visited);
	bnb_Node *next;
	priorityQueue.push(selected);

	int actual;
	while (!priorityQueue.empty()) {
		selected = priorityQueue.top();
		priorityQueue.pop();

		if (selected->heuristic < mincost) {
			actual = selected->sol.back();
			for (int i : selected->m_tovisit) {
				if (selected->cost + dijkstraMatrix[actual*n_visits + i] < mincost) {
					next = new bnb_Node(selected->m_tovisit, 0, selected->cost, selected->sol);
					next->cost += dijkstraMatrix[actual*n_visits + i];
					next->heuristic = next->cost;
					next->m_tovisit.remove(i);
					next->sol.push_back(i);

					if (next->m_tovisit.empty()) {
						next->cost += dijkstraMatrix[(next->sol.back()*n_visits) + n_visits - 1];
						if (next->cost < mincost) {
							next->sol.push_back(n_visits - 1);
							solution = next->sol;
							mincost = next->cost;
						}
						else
							delete next;
					}
					else {
						if (next->heuristic < mincost)
							priorityQueue.push(next);
						else
							delete next;
					}
				}
			}
		}
		delete selected;
	}
exit_bnb1:
	CTrack bestTrack(this);
	CTrack auxTrack(this);
	int start_index = solution.front();
	CVertex* start = v[start_index];
	int end_index;
	CVertex* end;
	solution.pop_front();
	bestTrack.AddFirst(first);
	for (int j : solution) {
		auxTrack.Clear();
		end_index = j;
		end = v[end_index];
		Dijkstra(v[start_index]);
		while (start != end) {
			auxTrack.AddFirst(end);
			end = end->m_pDijkstraPrevious;
		}
		start_index = end_index;
		start = v[start_index];
		bestTrack.Append(auxTrack);

	}
	return bestTrack;
}

// SalesmanTrackBranchAndBound2 ===================================================

CTrack CGraph::SalesmanTrackBranchAndBound2(CVisits &visits)
{
	double bestCost = numeric_limits<double>::max();
	double checkCost = bestCost;
	priority_queue<bnb_Node*, vector<bnb_Node*>, comparator> priorityQueue;
	list<int> visited, solution, toVisit;

	int n_visits = visits.m_Vertices.size();
	vector<double> dijkstraMatrix;
	dijkstraMatrix.resize(n_visits*n_visits);
	vector<CVertex*> v{ begin(visits.m_Vertices), end(visits.m_Vertices) };


	for (int start = 0; start < n_visits; start++) {
		CGraph::Dijkstra(v[start]);
		for (int end = 0; end < n_visits; end++)
			dijkstraMatrix[start*n_visits + end] = (v[end]->m_DijkstraDistance);
	}

	vector<double> minCost;
	minCost.resize(n_visits);
	double min;
	for (int end = 0; end < n_visits; end++) {
		min = dijkstraMatrix[0 + end];
		for (int start = 0; start < n_visits; start++) {
			if (dijkstraMatrix[start*n_visits + end] < min && start != end) {
				min = dijkstraMatrix[start*n_visits + end];
			}
			minCost[end] = min;
		}
	}


	visited.push_front(0);
	CVertex *first = visits.m_Vertices.front();
	CVertex *last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();
	visits.m_Vertices.pop_front();

	//Excepcion grafos de 1 o 2 aristas
	if (n_visits <= 2) {
		for (int nv = 0; nv < n_visits; nv++)
			solution.push_back(nv);
		goto exit_bnb2;
	}

	int count = 1;
	for (CVertex *pCount : visits.m_Vertices) {
		toVisit.push_back(count);
		count++;
	}

	double h = 0;
	for (int he : toVisit) {
		h += minCost[he];
	}
	h += minCost[n_visits - 1];
	int actual;
	int lastVertex = n_visits - 1;
	double selected_h, newCost;

	bnb_Node *selected = new bnb_Node(toVisit, h, 0, visited);
	bnb_Node *next;
	priorityQueue.push(selected);
	while (!priorityQueue.empty()) {
		selected = priorityQueue.top();
		priorityQueue.pop();
		selected_h = selected->heuristic;
		if (selected_h < bestCost) {
			actual = selected->sol.back();
			for (int i : selected->m_tovisit) {

				newCost = selected->cost + dijkstraMatrix[actual*n_visits + i];
				h = selected_h - minCost[i] + dijkstraMatrix[actual*n_visits + i];
				if (h < bestCost) {
					next = new bnb_Node(selected->m_tovisit, h, newCost, selected->sol);
					next->m_tovisit.remove(i);
					next->sol.push_back(i);
					if (next->m_tovisit.empty()) {
						next->cost += dijkstraMatrix[(next->sol.back()*n_visits) + lastVertex];
						if (next->cost < bestCost) {
							next->sol.push_back(lastVertex);
							solution = next->sol;
							bestCost = next->cost;
						}
						else
							delete next;
					}
					else
						priorityQueue.push(next);
				}
			}
		}
		else
			goto exit_bnb2;

		delete selected;
	}

exit_bnb2:
	CTrack bestTrack(this);
	CTrack auxTrack(this);
	int start_index = solution.front();
	CVertex* start = v[start_index];
	int end_index;
	CVertex* end;
	solution.pop_front();
	bestTrack.AddFirst(first);
	for (int j : solution) {
		auxTrack.Clear();
		end_index = j;
		end = v[end_index];
		Dijkstra(v[start_index]);
		while (start != end) {
			auxTrack.AddFirst(end);
			end = end->m_pDijkstraPrevious;
		}
		start_index = end_index;
		start = v[start_index];
		bestTrack.Append(auxTrack);

	}
	return bestTrack;

}

// SalesmanTrackBranchAndBound3 ===================================================

CTrack CGraph::SalesmanTrackBranchAndBound3(CVisits &visits)
{
	double bestCost = numeric_limits<double>::max();
	double checkCost = bestCost;
	priority_queue<bnb_Node*, vector<bnb_Node*>, comparator> priorityQueue;
	list<int> visited, solution, toVisit;

	int n_visits = visits.m_Vertices.size();
	vector<double> dijkstraMatrix;
	dijkstraMatrix.resize(n_visits*n_visits);
	vector<CVertex*> v{ begin(visits.m_Vertices), end(visits.m_Vertices) };


	for (int start = 0; start < n_visits; start++) {
		CGraph::Dijkstra(v[start]);
		for (int end = 0; end < n_visits; end++)
			dijkstraMatrix[start*n_visits + end] = (v[end]->m_DijkstraDistance);
	}

	vector<double> minCost;
	minCost.resize(n_visits);
	double min;
	for (int end = 0; end < n_visits; end++) {
		min = dijkstraMatrix[0 + end];
		for (int start = 0; start < n_visits; start++) {
			if (dijkstraMatrix[start*n_visits + end] < min && start != end) {
				min = dijkstraMatrix[start*n_visits + end];
			}
			minCost[end] = min;
		}
	}


	visited.push_front(0);
	CVertex *first = visits.m_Vertices.front();
	CVertex *last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();
	visits.m_Vertices.pop_front();

	//Excepcion grafos de 1 o 2 aristas
	if (n_visits <= 2) {
		for (int nv = 0; nv < n_visits; nv++)
			solution.push_back(nv);
		goto exit_bnb2;
	}

	int count = 1;
	for (CVertex *pCount : visits.m_Vertices) {
		toVisit.push_back(count);
		count++;
	}

	double h = 0;
	for (int he : toVisit) {
		h += minCost[he];
	}
	h += minCost[n_visits - 1];
	int actual;
	int lastVertex = n_visits - 1;
	double selected_h, newCost;

	bnb_Node *selected = new bnb_Node(toVisit, h, 0, visited);
	bnb_Node *next;
	priorityQueue.push(selected);
	while (!priorityQueue.empty()) {
		selected = priorityQueue.top();
		priorityQueue.pop();
		selected_h = selected->heuristic;
		if (selected_h < bestCost) {
			actual = selected->sol.back();
			for (int i : selected->m_tovisit) {

				newCost = selected->cost + dijkstraMatrix[actual*n_visits + i];
				h = selected_h - minCost[i] + dijkstraMatrix[actual*n_visits + i];
				if (h < bestCost) {
					next = new bnb_Node(selected->m_tovisit, h, newCost, selected->sol);
					next->m_tovisit.remove(i);
					next->sol.push_back(i);
					if (next->m_tovisit.empty()) {
						next->cost += dijkstraMatrix[(next->sol.back()*n_visits) + lastVertex];
						if (next->cost < bestCost) {
							next->sol.push_back(lastVertex);
							solution = next->sol;
							bestCost = next->cost;
						}
						else
							delete next;
					}
					else
						priorityQueue.push(next);
				}
			}
		}
		else
			goto exit_bnb2;

		delete selected;
	}

exit_bnb2:
	CTrack bestTrack(this);
	CTrack auxTrack(this);
	int start_index = solution.front();
	CVertex* start = v[start_index];
	int end_index;
	CVertex* end;
	solution.pop_front();
	bestTrack.AddFirst(first);
	for (int j : solution) {
		auxTrack.Clear();
		end_index = j;
		end = v[end_index];
		Dijkstra(v[start_index]);
		while (start != end) {
			auxTrack.AddFirst(end);
			end = end->m_pDijkstraPrevious;
		}
		start_index = end_index;
		start = v[start_index];
		bestTrack.Append(auxTrack);

	}
	return bestTrack;

}
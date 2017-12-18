#include "stdafx.h"
#include "CGraph.h"



class bnb1_Node {
public:
	bnb1_Node(vector<int> tovisit, double h, double cost, vector<int> sol);
	vector<int> __tovisit;
	double __h;
	double __cost;
	vector<int> __sol;

};
bnb1_Node::bnb1_Node(vector<int> tovisit, double h, double cost, vector<int> sol) {
	__tovisit = tovisit;
	__cost = cost;
	__h = h;
	__sol = sol;
}

//Extra param
class bnb2_Node {
public:
	bnb2_Node(vector<int> tovisit, double h, double cost, vector<int> sol, double cota);
	vector<int> __tovisit, __sol;
	double __h,__cost, __cota;

};
bnb2_Node::bnb2_Node(vector<int> tovisit, double h, double cost, vector<int> sol, double cota) {
	__tovisit = tovisit;
	
	__cost = cost;
	__sol = sol;

	__h = h;
	__cota = cota;
}
struct comparator_h1 {
	bool operator()(bnb1_Node const* n1, bnb1_Node const* n2) {
		return (n1->__h > n2->__h);
	}
};

struct comparator_h2 {
	bool operator()(bnb1_Node const* n1, bnb1_Node const* n2) {
		return (n1->__h < n2->__h);
	}
};

struct comparator_h3 {
	bool operator()(bnb2_Node const* n1, bnb2_Node const* n2) {
		return (n1->__h > n2->__h);
	}
};

// SalesmanTrackBranchAndBound1 ===================================================

CTrack CGraph::SalesmanTrackBranchAndBound1(CVisits &visits)
{
	int n_vertices = visits.m_Vertices.size();
	priority_queue<bnb1_Node*, vector<bnb1_Node*>, comparator_h1> priorityQueue;
	double min_cost = numeric_limits<double>::max();
	vector<int> visited, solution, toVisit;
	visited.push_back(0);


	vector<double> dijkstraMatrix;
	dijkstraMatrix.resize(n_vertices*n_vertices);
	vector<CVertex*> vertices{ begin(visits.m_Vertices), end(visits.m_Vertices) };
	for (int start = 0; start < n_vertices; start++) {
		CGraph::Dijkstra(vertices[start]);
		for (int last = 0; last < n_vertices; last++) {
			dijkstraMatrix[start * n_vertices + last] = (vertices[last]->m_DijkstraDistance);
		}
	}

	CVertex* first = visits.m_Vertices.front();
	visits.m_Vertices.pop_front();
	CVertex* last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();

	bool control = true;
	if (n_vertices <= 2) {
		control = false;
		for (int nv = 0; nv < n_vertices; nv++)
			solution.push_back(nv);
	}

	if (control) {
		int count = 1;
		for (CVertex *pCount : visits.m_Vertices) {
			toVisit.push_back(count);
			count++;
		}

		bnb1_Node* selected = new bnb1_Node(toVisit, 0, 0, visited);
		priorityQueue.push(selected);
		bnb1_Node* next;

		int selected_index, ival, iv;
		while (!priorityQueue.empty()) {
			selected = priorityQueue.top();
			priorityQueue.pop();
			if (selected->__h < min_cost) {
				selected_index = selected->__sol.back();
				for (iv = 0; iv < selected->__tovisit.size(); iv++) {
					ival = selected->__tovisit[iv];
					if (selected->__cost + dijkstraMatrix[selected_index * n_vertices + ival] < min_cost) {
						next = new bnb1_Node(selected->__tovisit, 0, selected->__cost, selected->__sol);
						next->__cost += dijkstraMatrix[selected_index * n_vertices + ival];
						next->__h = next->__cost;
						next->__tovisit.erase(next->__tovisit.begin() + iv);
						next->__sol.push_back(ival);

						if (next->__tovisit.empty()) {
							next->__cost += dijkstraMatrix[(next->__sol.back() * n_vertices) + n_vertices - 1];
							if (next->__cost < min_cost) {
								next->__sol.push_back(n_vertices - 1);
								solution = next->__sol;
								min_cost = next->__cost;
							}
							else
								delete next;
						}
						else {
							if (next->__h < min_cost)
								priorityQueue.push(next);
							else
								delete next;
						}
					}
				}
			}
			delete selected;
		}
	}

	CTrack Track(this);
	CTrack auxTrack(this);
	int pStart = solution.front();
	CVertex* vStart = vertices[pStart];
	int pLast;
	CVertex* vLast;
	solution[0];
	Track.AddFirst(first);
	for (int n : solution) {
		auxTrack.Clear();
		pLast = n;
		vLast = vertices[pLast];
		Dijkstra(vertices[pStart]);
		while (vStart != vLast) {
			auxTrack.AddFirst(vLast);
			vLast = vLast->m_pDijkstraPrevious;
		}
		pStart = pLast;
		vStart = vertices[pStart];
		Track.Append(auxTrack);

	}
	return Track;
}

// SalesmanTrackBranchAndBound2 ===================================================

CTrack CGraph::SalesmanTrackBranchAndBound2(CVisits &visits)
{

	double min_cost = numeric_limits<double>::max();
	priority_queue<bnb1_Node*, vector<bnb1_Node*>, comparator_h2> priorityQueue;
	vector<int> visited, solution, toVisit;
	int n_vertices = visits.m_Vertices.size();

	vector<double> dijkstraMatrix;
	dijkstraMatrix.resize(n_vertices*n_vertices);

	vector<CVertex*> vertices{ begin(visits.m_Vertices), end(visits.m_Vertices) };

	for (int start = 0; start < n_vertices; start++) {
		CGraph::Dijkstra(vertices[start]);
		for (int last = 0; last < n_vertices; last++) {
			dijkstraMatrix[start * n_vertices + last] = (vertices[last]->m_DijkstraDistance);
		}
	}

	vector<double> minCost;
	minCost.resize(n_vertices);

	double min_value;
	for (int last = 0; last < n_vertices; last++) {
		min_value = dijkstraMatrix[0 + last];
		for (int start = 0; start < n_vertices; start++) {
			if (dijkstraMatrix[start * n_vertices + last] < min_value && start != last) {
				min_value = dijkstraMatrix[start*n_vertices + last];
			}
			minCost[last] = min_value;
		}
	}

	visited.push_back(0);

	CVertex* first = visits.m_Vertices.front();
	visits.m_Vertices.pop_front();
	CVertex* last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();

	bool control = true;
	if (n_vertices <= 2) {
		control = false;
		for (int v = 0; v < n_vertices; v++)
			solution.push_back(v);

	}
	if (control) {
		int count = 1;
		for (CVertex *pCount : visits.m_Vertices) {
			toVisit.push_back(count);
			count++;
		}


		double h = 0;
		for (int heuristic : toVisit) {
			h += minCost[heuristic];
		}
		h += minCost[n_vertices - 1];
		int index_selected;
		int lastVertex = n_vertices - 1;
		double selected_heuristic, next_cost;

		bnb1_Node* selected = new bnb1_Node(toVisit, h, 0, visited);
		bnb1_Node* next;
		int ival, iv;
		priorityQueue.push(selected);
		while (!priorityQueue.empty()) {
			selected = priorityQueue.top();
			priorityQueue.pop();
			selected_heuristic = selected->__h;
			if (selected_heuristic < min_cost) {
				index_selected = selected->__sol.back();
				for (iv = 0; iv < selected->__tovisit.size(); iv++) {
					ival = selected->__tovisit[iv];
					next_cost = selected->__cost + dijkstraMatrix[index_selected*n_vertices + ival];
					h = selected_heuristic - minCost[ival] + dijkstraMatrix[index_selected*n_vertices + ival];
					if (h < min_cost) {
						next = new bnb1_Node(selected->__tovisit, h, next_cost, selected->__sol);
						next->__tovisit.erase(next->__tovisit.begin() + iv);
						next->__sol.push_back(ival);
						if (next->__tovisit.empty()) {

							next->__cost += dijkstraMatrix[(next->__sol.back()*n_vertices) + lastVertex];
							if (next->__cost < min_cost) {
								next->__sol.push_back(lastVertex);

								solution = next->__sol;
								min_cost = next->__cost;
							}
							else {
								delete next;
							}
						}
						else
							priorityQueue.push(next);
					}
				}
			}
			delete selected;
		}
	}


	CTrack Track(this);
	CTrack auxTrack(this);
	int pStart = solution.front();
	CVertex* vStart = vertices[pStart];
	int pLast;
	CVertex* vLast;
	solution[0];
	Track.AddFirst(first);
	for (int n : solution) {
		auxTrack.Clear();
		pLast = n;
		vLast = vertices[pLast];
		Dijkstra(vertices[pStart]);
		while (vStart != vLast) {
			auxTrack.AddFirst(vLast);
			vLast = vLast->m_pDijkstraPrevious;
		}
		pStart = pLast;
		vStart = vertices[pStart];
		Track.Append(auxTrack);

	}
	return Track;
}

// SalesmanTrackBranchAndBound3 ===================================================

CTrack CGraph::SalesmanTrackBranchAndBound3(CVisits &visits)
{
	double min_cost = numeric_limits<double>::max();
	priority_queue<bnb2_Node*, vector<bnb2_Node*>, comparator_h3> priorityQueue;
	vector<int> visited, solution, toVisit;
	int n_vertices = visits.m_Vertices.size();
	vector<double> dijkstraMatrix;
	dijkstraMatrix.resize(n_vertices*n_vertices);
	vector<CVertex*> vertices{ begin(visits.m_Vertices), end(visits.m_Vertices) };

	for (int start = 0; start < n_vertices; start++) {
		CGraph::Dijkstra(vertices[start]);
		for (int last = 0; last < n_vertices; last++) {
			dijkstraMatrix[start * n_vertices + last] = (vertices[last]->m_DijkstraDistance);
		}
	}

	vector<double> minCost;
	minCost.resize(n_vertices);
	double min;
	for (int last = 0; last < n_vertices; last++) {
		min = dijkstraMatrix[0 + last];
		for (int start = 0; start < n_vertices; start++) {
			if (dijkstraMatrix[start * n_vertices + last] < min && start != last) {
				min = dijkstraMatrix[start * n_vertices + last];
			}
			minCost[last] = min;
		}
	}

	visited.push_back(0);

	CVertex* first = visits.m_Vertices.front();
	visits.m_Vertices.pop_front();
	CVertex* last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();

	bool control = true;
	if (n_vertices <= 2) {
		control = false;
		for (int v = 0; v < n_vertices; v++)
			solution.push_back(v);
	}

	if (control) {
		int count = 1;
		for (CVertex *pCount : visits.m_Vertices) {
			toVisit.push_back(count);
			count++;
		}

		double h = 0;
		for (int heuristic : toVisit) {
			h += minCost[heuristic];
		}
		h += minCost[n_vertices - 1];
		int index_selected;
		int last_vertex = n_vertices - 1;
		double last_cost = minCost[last_vertex];
		double selected_heuristic, next_cost;
		bnb2_Node *selected = new bnb2_Node(toVisit, h, 0, visited, h);
		bnb2_Node *next;
		priorityQueue.push(selected);
		int index_value;
		while (!priorityQueue.empty()) {
			selected = priorityQueue.top();
			priorityQueue.pop();
			selected_heuristic = selected->__cota;
			if (selected_heuristic < min_cost) {
				index_selected = selected->__sol.back();
				for (int i = 0; i < selected->__tovisit.size(); i++) {
					index_value = selected->__tovisit[i];
					next_cost = selected->__cost + dijkstraMatrix[index_selected*n_vertices + index_value];
					h = selected_heuristic - minCost[index_value] + dijkstraMatrix[index_selected * n_vertices + index_value];
					if (h < min_cost) {
						next = new bnb2_Node(selected->__tovisit, h, next_cost, selected->__sol, h);
						next->__tovisit.erase(next->__tovisit.begin() + i);
						next->__sol.push_back(index_value);
						next->__h = (next->__h) * (next->__tovisit.size());
						if (next->__tovisit.empty()) {
							next->__cost += dijkstraMatrix[(next->__sol.back()*n_vertices) + last_vertex];
							if (next->__cost < min_cost) {
								next->__sol.push_back(last_vertex);
								solution = next->__sol;
								min_cost = next->__cost;
							}
							else {
								delete next;
							}
						}
						else {
							priorityQueue.push(next);
						}
					}
				}
			}

			delete selected;
		}
	}
	CTrack Track(this);
	CTrack auxTrack(this);
	int pStart = solution.front();
	CVertex* vStart = vertices[pStart];
	int pLast;
	CVertex* vLast;
	solution[0];
	Track.AddFirst(first);
	for (int n : solution) {
		auxTrack.Clear();
		pLast = n;
		vLast = vertices[pLast];
		Dijkstra(vertices[pStart]);
		while (vStart != vLast) {
			auxTrack.AddFirst(vLast);
			vLast = vLast->m_pDijkstraPrevious;
		}
		pStart = pLast;
		vStart = vertices[pStart];
		Track.Append(auxTrack);

	}
	return Track;
}



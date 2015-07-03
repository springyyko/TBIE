#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#define EPSILON 0.00001

using namespace std;

class Node {
private:
	int id;
	int in_degree;
	int out_degree;
	int onpath;
	double toInf;
	double fromInf;
	vector<int> in_neighbor;
	vector<int> out_neighbor;

	/*	
	save obj to file without get_method()

	friend ostream& operator<<(ostream& os, Node& node) {
		os << node.id << "\t" << node.in_degree << "\t" << node.out_degree << endl;
		
		for (vector<int>::iterator iter = node.neighbor_out.begin(); iter != node.neighbor_out.end(); iter++) {
			os << *iter << "\t";
		}
		os << endl;
		for (vector<int>::iterator iter = node.neighbor_in.begin(); iter != node.neighbor_in.end(); iter++) {
			os << *iter << "\t";
		}
		os << endl;
		return os;
	}
	*/
public:
	Node();
	Node(int source, int target, bool flag);
	int get_id();
	int get_indegree();
	int get_outdegree();
	int get_onpath();

	double get_toinf();
	double get_fromInf();

	vector<int> get_in_neighbor();
	vector<int> get_out_neighbor();

	void set_id(int _id);
	void set_onpath(int _onpath);
	void set_toInf(double _toInf);
	void set_fromInf(double _toInf);

	void add_outneighbor(int neighbor);
	void add_inneighbor(int neighbor);
	void writeTofile(ofstream *out);
	void readFromfile(ifstream *in);

	void print_neighbor();
};
/* constructor for each node*/
Node::Node() {
	id = 0;
	onpath = 0;
	toInf = 0.0;
	fromInf = 0.0;
	in_degree = 0;
	out_degree = 0;
}
Node::Node(int source, int target, bool flag) {
	/* for source node */
	if (flag == false) {
		id = source;
		out_neighbor.push_back(target);
		out_degree += 1;
	}
	/* for target node */
	else {
		id = target;
		out_neighbor.push_back(source);
		in_degree += 1;
	}
	onpath = 0;
	toInf = 0.0;
	fromInf = 0.0;
	in_degree = 0;
	out_degree = 0;
}

/* getter */
int Node::get_id() {
	return id;
}
int Node::get_indegree() {
	return in_degree;
}
int Node::get_outdegree() {
	return out_degree;
}
int Node::get_onpath() {
	return onpath;
}

double Node::get_toinf() {
	return toInf;
}
double Node::get_fromInf() {
	return fromInf;
}

vector<int> Node::get_in_neighbor(){
	return in_neighbor;
}
vector<int> Node::get_out_neighbor(){
	return out_neighbor;
}


/* setter */
void Node::set_id(int _id) {
	id = _id;
}
void Node::set_onpath(int _onpath) {
	onpath = _onpath;
}
void Node::set_toInf(double _toInf) {
	toInf = _toInf;
}
void Node::set_fromInf(double _fromInf) {
	fromInf = _fromInf;
}

void Node::add_outneighbor(int neighbor) {
	out_neighbor.push_back(neighbor);
	out_degree += 1;
}
void Node::add_inneighbor(int neighbor) {
	in_neighbor.push_back(neighbor);
	in_degree += 1;
}

/* read/write a object from/to */
void Node::writeTofile(ofstream *out) {
	out->write((char*) this, sizeof(this));
}
void Node::readFromfile(ifstream *in) {
	in->read((char*) this, sizeof(this));
}
/* for debuggin */
void Node::print_neighbor() {
	cout << "Node id: " << id << endl;
	cout << "on Path ?: " << onpath << endl;
	cout << "to influence: " << toInf << endl;
	cout << "from influence: " << fromInf << endl;
	cout << "out-neighbor: " << out_neighbor.size() << endl;
	//for (vector<int>::iterator iter = out_neighbor.begin(); iter != out_neighbor.end(); iter++) {
	//	cout << *iter << "\t";
	//}
	cout << endl;
	cout << "in-neighbor: " << in_neighbor.size() << endl;
	//for (vector<int>::iterator iter = in_neighbor.begin(); iter != in_neighbor.end(); iter++) {
	//	cout << *iter << "\t";
	//}
	cout << endl;
}
/* function define */
void preprocessing(string filename, Node node[]);
void updateCELFQueue(multimap< double, int> CELF, Node node[]);
void updateFrominf(Node node[], int seed, double thresh, double current_path);
double TargetBasedInfluence(Node node[], int i, double thresh, double current_path);
double SourceBasedInfluence(Node node[], int i, double thresh, double current_path);


/* main function */
int main() {
	ofstream fout;
	fout.open("Seedset_target.txt");
	int select_way = 0;
	if (select_way == 0){
		cout << "Target node base " << endl;
	}
	else{
		cout << "Source node base " << endl;
	}
	int seed_size = 20;
	double prev_max = 0.0;
	double prev_max_tmp = 0.0;
	int *seed = new int[seed_size];
	double path_threshold = (1.0/160.0);

	Node* node = new Node[281904];	//	There are 281,903 nodes 2312497 edges in stanford data.
	preprocessing("web-Stanford.txt", node);

	/* data structure for CELF Queue, sorted by toInfluence */
	multimap< double, int > Nodes;
	multimap< double, int >::reverse_iterator Iter_node;
	typedef pair< double, int > NodePair;

	/* 
	select first node and initiate CELF Queue
	Node[] index from 1 to 281,903 	
	*/
	for (int i = 1; i < 281904; i++){
		/* if the index has no a real node */
		if ((i % 28000) == 0){
			cout << (i / 28000) * 10 << "% loading.." << endl;
		}
		if (node[i].get_id() == 0){
			continue;
		}
		else{
			node[i].set_toInf(TargetBasedInfluence(node, i, path_threshold, 1.0));
			Nodes.insert(NodePair(node[i].get_toinf(), i));
			//char tmp;
			//cout << "for viewing the intermediate values" << endl;
			//cin >> tmp;
		}
	}
	/* select first seed and delete CELF Queue */
	seed[0] = Nodes.rbegin()->second;
	node[seed[0]].set_onpath(1);

	Nodes.erase(--Nodes.rbegin().base());
	cout << "1st seed selected" << endl;
	cout << "============================" << endl;
	prev_max = node[seed[0]].get_toinf();
	node[seed[0]].print_neighbor();

	fout << "Node id: " << seed[0] << endl;
	fout << "indegree: " << node[seed[0]].get_indegree() << endl;
	fout << "outdegree: " << node[seed[0]].get_outdegree() << endl;


	/* select 2~n seeds */
	if (select_way == 0){
		/* Target node bases influence estimation */

		/* update frominf by first seed */
		updateFrominf(node, seed[0], path_threshold, 1);
		node[seed[0]].set_onpath(1);

		for (int i = 1; i < seed_size; i++){
			cout << i + 1 << "th seed selected " << endl;
			cout << "============================" << endl;
			/* start top of CELF Queue */
			double marginal_gain = 0.0;
			Node max_marginal;
			for (Iter_node = Nodes.rbegin(); Iter_node != Nodes.rend(); ++Iter_node){
				/* CELF Algorithm */
				if (marginal_gain >= Iter_node->first){
					break;
				}
				/* gain from a new node */
				int candidata_id = Iter_node->second;
				double incByNewNode = TargetBasedInfluence(node, candidata_id, path_threshold, 1.0);

				/* recomputation of seeds influence because of a new node */
				node[candidata_id].set_onpath(1);
				double seedInf_modified = 0.0;
				for (int j = 0; j < i; j++){
					seedInf_modified += TargetBasedInfluence(node, seed[j], path_threshold, 1.0);
					node[seed[j]].set_onpath(1);
				}

				node[candidata_id].set_onpath(0);
				/* compute and update marginal gain of a new node */
				marginal_gain = (seedInf_modified + incByNewNode) - prev_max;
				node[candidata_id].set_toInf(marginal_gain);

				if (max_marginal.get_toinf() < marginal_gain){
					max_marginal = node[candidata_id];
					prev_max_tmp = (seedInf_modified + incByNewNode);
				}
			}
			prev_max = prev_max_tmp;
			/* select a next seed */
			seed[i] = max_marginal.get_id();
			node[seed[i]].set_onpath(1);
			node[seed[i]].print_neighbor();

			//char tmp;
			//cout << "for viewing the intermediate values" << endl;
			//cin >> tmp;

			cout << "CELF Queue update" << endl;

			/* update CELF Queue, 1-scan CELF Queue */
			vector<int> tmp_update;
			for (multimap< double, int >::iterator Iter = Nodes.begin(); Iter != Nodes.end();){
				multimap< double, int >::iterator erase_iter = Iter++;
				/* if key and toinf are not same because key was recomputed */
				double new_inf = node[erase_iter->second].get_toinf();
				double diff = fabs(erase_iter->first - new_inf);
				if (diff > EPSILON){
					tmp_update.push_back(erase_iter->second);
					Nodes.erase(erase_iter);
				}
			}

			for (int i = 0; i < tmp_update.size(); i++){
				Nodes.insert(NodePair(node[tmp_update[i]].get_toinf(), node[tmp_update[i]].get_id()));
			} 

			seed[i] = Nodes.rbegin()->second;
			Nodes.erase(--Nodes.rbegin().base());

			cout << "Current seed set: ";
			for (int j = 0; j <= i; j++){
				cout << seed[j] << ", ";

			}
			fout << "Node id: " << seed[i] << endl;
			fout << "indegree: " << node[seed[i]].get_indegree() << endl;
			fout << "outdegree: " << node[seed[i]].get_outdegree() << endl;
		}
	}
	else{
		/* Source node bases influence estimation */
		for (int i = 1; i < seed_size; i++){
			cout << i + 1 << "th seed selected " << endl;
			cout << "============================" << endl;
			/* start top of CELF Queue */
			double marginal_gain = 0.0;
			Node max_marginal;
			for (Iter_node = Nodes.rbegin(); Iter_node != Nodes.rend(); ++Iter_node){
				/* CELF Algorithm */
				if (marginal_gain >= Iter_node->first){
					break;
				}
				/* gain from a new node */
				int candidata_id = Iter_node->second;
				double incByNewNode = SourceBasedInfluence(node, candidata_id, path_threshold, 1.0);

				/* recomputation of seeds influence because of a new node */
				node[candidata_id].set_onpath(1);
				double seedInf_modified = 0.0;
				for (int j = 0; j < i; j++){
					seedInf_modified += SourceBasedInfluence(node, seed[j], path_threshold, 1.0);
					node[seed[j]].set_onpath(1);
				}

				node[candidata_id].set_onpath(0);
				/* compute and update marginal gain of a new node */
				marginal_gain = (seedInf_modified + incByNewNode) - prev_max;
				node[candidata_id].set_toInf(marginal_gain);

				if (max_marginal.get_toinf() < marginal_gain){
					max_marginal = node[candidata_id];
					prev_max_tmp = (seedInf_modified + incByNewNode);
				}
			}
			prev_max = prev_max_tmp;
			/* select a next seed */
			seed[i] = max_marginal.get_id();
			node[seed[i]].set_onpath(1);
			node[seed[i]].print_neighbor();

			//char tmp;
			//cout << "for viewing the intermediate values" << endl;
			//cin >> tmp;

			cout << "CELF Queue update" << endl;

			/* update CELF Queue, 1-scan CELF Queue */
			vector<int> tmp_update;
			for (multimap< double, int >::iterator Iter = Nodes.begin(); Iter != Nodes.end();){
				multimap< double, int >::iterator erase_iter = Iter++;
				/* if key and toinf are not same because key was recomputed */
				double new_inf = node[erase_iter->second].get_toinf();
				double diff = fabs(erase_iter->first - new_inf);
				if (diff > EPSILON){
					tmp_update.push_back(erase_iter->second);
					Nodes.erase(erase_iter);
				}
			}

			for (int i = 0; i < tmp_update.size(); i++){
				Nodes.insert(NodePair(node[tmp_update[i]].get_toinf(), node[tmp_update[i]].get_id()));
			}

			seed[i] = Nodes.rbegin()->second;
			Nodes.erase(--Nodes.rbegin().base());

			cout << "Current seed set: ";
			for (int j = 0; j <= i; j++){
				cout << seed[j] << ", ";

			}
			fout << "Node id: " << seed[i] << endl;
			fout << "indegree: " << node[seed[i]].get_indegree() << endl;
			fout << "outdegree: " << node[seed[i]].get_outdegree() << endl;
		}
	}



	fout.close();
	delete[] seed;
	delete[] node;
	return 0;
}

/* computing the influence of a node based on target nodes (proposed)*/
double TargetBasedInfluence(Node node[], int i, double thresh, double current_path){
	int outdegree = node[i].get_outdegree();
	
	/* this node is on path */

	node[i].set_onpath(1);
	
	/* if there is no out-neighbor, stop path making */
	if (outdegree == 0){
		/* this node leaves path */
		node[i].set_onpath(0);
		return 1.0;
	}
	/* computing influence of children recursively */
	else{
		double inf = 1.0;	// influence itself
		vector<int> out_neighbor = node[i].get_out_neighbor();
		for (vector<int>::iterator iter = out_neighbor.begin(); iter != out_neighbor.end(); iter++){
			/* if target node is not on the path */
			if (node[*iter].get_onpath() == 0){
				double weight_child = (1.0 / node[*iter].get_indegree());
				/* compare threshold and current path weight */
				if (thresh <= (weight_child*current_path)){
					double inf_child = TargetBasedInfluence(node, *iter, thresh, weight_child*current_path);
					inf += weight_child*inf_child;
				}
				else{
					continue;
				}
			}
		}
		/* this node leaves path */
		node[i].set_onpath(0);
		//cout << node[i].get_id() << " influence: " << inf << endl;
		//cout << endl;
		return inf; 
	}
}
/* computing the influence of a node based on source nodes (existing) */
double SourceBasedInfluence(Node node[], int i, double thresh, double current_path){
	int outdegree = node[i].get_outdegree();

	/* this node is on path */

	node[i].set_onpath(1);

	/* if there is no out-neighbor, stop path making */
	if (outdegree == 0){
		/* this node leaves path */
		node[i].set_onpath(0);
		return 1.0;
	}
	/* computing influence of children recursively */
	else{
		double inf = 1.0;	// influence itself
		vector<int> out_neighbor = node[i].get_out_neighbor();
		for (vector<int>::iterator iter = out_neighbor.begin(); iter != out_neighbor.end(); iter++){
			/* if target node is not on the path */
			if (node[*iter].get_onpath() == 0){
				double weight_child = (1.0 / node[*iter].get_indegree());
				/* compare threshold and current path weight */
				if (thresh <= (weight_child*current_path)){
					double inf_child = SourceBasedInfluence(node, *iter, thresh, weight_child*current_path);
					inf += weight_child*inf_child;
				}
				else{
					continue;
				}
			}
		}
		/* this node leaves path */
		node[i].set_onpath(0);
		//cout << node[i].get_id() << " influence: " << inf << endl;
		//cout << endl;
		return inf;
	}
}

/* preprocessing the origin data*/
void preprocessing(string filename, Node node[]) {
	ifstream fin;
	fin.open(filename);
	string line;

	/* except header*/
	fin.seekg(169);

	int i = 0;
	while (getline(fin, line)) {
		int tmp = 0;
		char tmp_strTochar[30];
		/* string line to char array for strtok_s()*/
		strcpy_s(tmp_strTochar, 30, line.c_str());

		/*separate source and target by using strtok_s()*/
		char* target = NULL;
		char* source = strtok_s(tmp_strTochar, "\t", &target);

		/* assign values*/
		int idx_source = atoi(source);
		int idx_target = atoi(target);

		/* if source node not initialized */
		if (node[idx_source].get_id() == 0) {
			node[idx_source].set_id(idx_source);
			node[idx_source].add_outneighbor(idx_target);
		}
		else {
			node[idx_source].add_outneighbor(idx_target);
		}
		/* if target node not initialized */
		if (node[idx_target].get_id() == 0) {
			node[idx_target].set_id(idx_target);
			node[idx_target].add_inneighbor(idx_source);
		}
		else {
			node[idx_target].add_inneighbor(idx_source);
		}

		/* check current % */
		i++;
		if ((i % 460000) == 0) {
			cout << (i / 230000) * 10 << " % read" << endl;
			//break;
		}
	}
	cout << "Input file read successfully" << endl;
	fin.close();
}
void updateCELFQueue(multimap< double, int> Nodes, Node node[]){
	vector<int> tmp_update;
	typedef pair< double, int > NodePair;
	for (multimap< double, int >::iterator Iter = Nodes.begin(); Iter != Nodes.end();){
		multimap< double, int >::iterator erase_iter = Iter++;
		/* if key and toinf are not same because key was recomputed */
		double new_inf = node[erase_iter->second].get_toinf();
		double diff = fabs(erase_iter->first - new_inf);
		if (diff > EPSILON){
			tmp_update.push_back(erase_iter->second);
			Nodes.erase(erase_iter);
		}
	}

	for (int i = 0; i < tmp_update.size(); i++){
		Nodes.insert(NodePair(node[tmp_update[i]].get_toinf(), node[tmp_update[i]].get_id()));
	}
}
void updateFrominf(Node node[], int seed, double thresh, double current_path){
	int i = seed;
	int outdegree = node[i].get_outdegree();

	/* this node is on path */
	node[i].set_onpath(1);

	/* if there is no out-neighbor, stop this process */
	if (outdegree == 0){
		/* this node leaves path */
		node[i].set_onpath(0);
	}
	/* updating from Influence recursively */
	else{
		double inf = 1.0;	// influence itself
		vector<int> out_neighbor = node[i].get_out_neighbor();
		for (vector<int>::iterator iter = out_neighbor.begin(); iter != out_neighbor.end(); iter++){
			/* if target node is not on the path */
			if (node[*iter].get_onpath() == 0){
				double weight_child = (1.0 / node[*iter].get_indegree());
				/* compare threshold and current path weight */
				if (thresh <= (weight_child*current_path)){
					double inf_child = SourceBasedInfluence(node, *iter, thresh, weight_child*current_path);
					inf += weight_child*inf_child;
				}
				else{
					continue;
				}
			}
		}
		/* this node leaves path */
		node[i].set_onpath(0);
		//cout << node[i].get_id() << " influence: " << inf << endl;
		//cout << endl;
	}
}
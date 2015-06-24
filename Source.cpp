#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
void preprocessing(string filename);

class Node {
private:
	int id;
	float toInf;
	float fromInf;
	vector<int> neighbor_in;
	vector<int> neighbor_out;
public:
	Node();
	Node(int source, int target, bool flag);
	int get_id();
	void set_id(int _id);
	void add_outneighbor(int neighbor);
	void add_inneighbor(int neighbor);
	void print_neighbor();
};
/* constructor for each node*/
Node::Node() {
	id = 0;
	toInf = 0.0;
	fromInf = 0.0;
}
Node::Node(int source, int target, bool flag) {
	/* for source node */
	if (flag == true) {
		id = source;
		neighbor_out.push_back(target);

	}
	/* for target node */
	else {
		id = target;
		neighbor_in.push_back(source);

	}
	toInf = 0.0;
	fromInf = 0.0;	
}
int Node::get_id() {
	return id;
}
void Node::set_id(int _id) {
	id = _id;
}
void Node::add_outneighbor(int neighbor) {
	neighbor_out.push_back(neighbor);
}
void Node::add_inneighbor(int neighbor) {
	neighbor_in.push_back(neighbor);
}
/* for debuggin */
void Node::print_neighbor() {
	cout << "Node id: " << id << endl;
	cout << "out-neighbor: " << neighbor_out.size() << endl;
	for (vector<int>::iterator iter = neighbor_out.begin(); iter != neighbor_out.end(); iter++) {
		cout << *iter << "\t";
	}
	cout << endl;
	cout << "in-neighbor: " << neighbor_in.size() << endl;
	for (vector<int>::iterator iter = neighbor_in.begin(); iter != neighbor_in.end(); iter++) {
		cout << *iter << "\t";
	}
	cout << endl;
}

int main() {
	preprocessing("web-Stanford.txt");
	return 0;
}
/* preprocessing the origin data*/
void preprocessing(string filename) {
	ifstream fin;
	fin.open(filename);
	string line;

	Node *node = new Node[281904];	//	There are 281,903 nodes 2312497 edges in stanford data.

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

		i++;
		if ((i%10000) == 0) {
			cout << i <<" edges read" << endl;
		}
	}
	delete[] node;
	fin.close();
}

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
Node* preprocessing(string filename);

class Node {
private:
	int id;
	int in_degree;
	int out_degree;
	float toInf;
	float fromInf;
	vector<int> neighbor_in;
	vector<int> neighbor_out;

	/*	save obj to file without get_method() */
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

public:
	Node();
	Node(int source, int target, bool flag);
	int get_id();
	int get_indegree();
	int get_outdegree();
	//vector<int> get_neighbor_();
	//vector<int> get_neighbor_out();

	void set_id(int _id);

	void add_outneighbor(int neighbor);
	void add_inneighbor(int neighbor);
	void writeTofile(ofstream *out);
	void readFromfile(ifstream *in);

	void print_neighbor();
};
/* constructor for each node*/
Node::Node() {
	id = 0;
	toInf = 0.0;
	fromInf = 0.0;
	in_degree = 0;
	out_degree = 0;
}
Node::Node(int source, int target, bool flag) {
	/* for source node */
	if (flag == true) {
		id = source;
		neighbor_out.push_back(target);
		out_degree += 1;
	}
	/* for target node */
	else {
		id = target;
		neighbor_in.push_back(source);
		in_degree += 1;
	}
	toInf = 0.0;
	fromInf = 0.0;
	in_degree = 0;
	out_degree = 0;
}
int Node::get_id() {
	return id;
}
int Node::get_indegree() {
	return in_degree;
}
int Node::get_outdegree() {
	return out_degree;
}
void Node::set_id(int _id) {
	id = _id;
}
void Node::add_outneighbor(int neighbor) {
	neighbor_out.push_back(neighbor);
	out_degree += 1;
}
void Node::add_inneighbor(int neighbor) {
	neighbor_in.push_back(neighbor);
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
	Node* node = preprocessing("web-Stanford.txt");
	
	return 0;
}
/* preprocessing the origin data*/
Node* preprocessing(string filename) {
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

		/* check current % */
		i++;
		if ((i%230000) == 0) {
			cout << (i/230000)*10 <<" % read" << endl;
		}
	}
	fin.close();
	return node;
}
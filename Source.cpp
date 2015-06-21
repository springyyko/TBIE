#include <iostream>
#include <fstream>
#include <string>

using namespace std;
void preprocessing(string filename);

class Node{
private:
	int id;
	int outdegree;
	float toInf;
	float fromInf;
	int *neighbor_id = new int [];
public:
	Node();
	Node(int source, int target);
};
Node::Node(){
	id = 0;
	outdegree = 0;
	toInf = 0.0;
	fromInf = 0.0;
}
Node::Node(int source, int target){
	id = source;
	outdegree = 1;
	toInf = 0.0;
	fromInf = 0.0;
	neighbor_id[0] = target;
}
int main(){
	preprocessing("web-Stanford.txt");
	
	return 0;
}
/* preprocessing the origin data*/
void preprocessing(string filename){
	ifstream fin;
	fin.open(filename);
	string line;

	/* except header*/
	fin.seekg(169);

	int i = 0;
	while (getline(fin, line)){
		Node *node[281903] = { NULL };	//	There are 281,903 nodes in stanford data.
		cout << "asdf" << endl;
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

		node[tmp] = new Node(idx_source, idx_target);



		i++;
		if (i == 1000){
			break;
		}
	}
	fin.close();
}
#include <iostream>
#include <conio.h>
#include <string.h>
#include <fstream>
#include <list>
#include <vector>
#include <stack>
#include <string>
#include <set>
#include <queue>
#include <stdlib.h>
#include <time.h>

//clockwise=1
//anticlockwise=0

using namespace std;


class Gnode {
private:
	int node[6][3][3];			//holds the cube entries
	list<Gnode> adjacencylist;	//used to store successors of each state of cube
	int depthLevel;
	string rotation;			//rotation applied to get to the state

	Gnode* parent;
	int f, g, h;			//f for storing the value of g+h; g being the UCS and h being the heuristic value

public:
	Gnode() {}
	Gnode(int temp[6][3][3])
	{
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					node[i][j][k] = temp[i][j][k];
				}
			}
		}
		parent = 0;
	}
	~Gnode() {
		if (parent != 0)
			parent = 0;
	}

	void setDepthLevel(int level)
	{
		this->depthLevel = level;
	}

	int getDepth()
	{
		return this->depthLevel;
	}

	void setRotation(string temp) {
		this->rotation = temp;
	}

	string getRotation()
	{
		return this->rotation;
	}

	void addChild(Gnode temp) {
		this->adjacencylist.push_back(temp);
	}

	void printAdjacencyList()
	{
		int i = 1;
		list<Gnode>::iterator it;
		for (it = this->adjacencylist.begin(); it != this->adjacencylist.end(); it++)
		{
			Gnode temp = it->node;
			cout << "Child " << i << endl;
			temp.printNode();
			i++;
		}
	}

	static Gnode clone(Gnode src)				//returns a copy of passed parameter
	{
		return Gnode(src);
	}

	int getFValue() const
	{
		return this->f;
	}

	void setParent(Gnode parent)
	{
		this->parent = new Gnode(parent);
	}

	void printNode() {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					cout << this->node[i][j][k] << " ";
				}
				cout << endl;
			}
			cout << endl;
		}
	}

	static bool compareNode(Gnode temp, Gnode target)
	{
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					if (temp.node[i][j][k] != target.node[i][j][k])
						return false;
				}
			}
		}
		return true;
	}

	static void runDFS(Gnode start, int depth, Gnode target)		//This function is called from int main() to run IDS
	{
		bool temp = false;
		int numOfNodes = 0;
		stack<string> moves;										//stores moves taken to get target/goal state
		if (depth >= 0)
		{
			time_t my_time = time(NULL);
			printf("Start Time: %s", ctime(&my_time));

			for (int i = 0; i <= depth; i++)
			{
				cout << "Depth " << i << "\n";
				temp = utilDFS(start, i, target, moves, numOfNodes);	//recursively calls the DFS for each level of depth
				if (temp == true)
				{
					cout << "Nodes traversed: " << numOfNodes << endl << endl;
					cout << "Goal found.. :)\n\n";

					time_t my_time = time(NULL);
					printf("End Time: %s", ctime(&my_time));

					int total_moves = moves.size();
					for (int k = 0; k < total_moves; k++)			//printing moves taken to solve the cube
					{
						cout << moves.top();
						moves.pop();
					}
					return;
				}
				cout << "Nodes traversed: " << numOfNodes << endl << endl;
				numOfNodes = 0;
			}
		}
	}

	static bool utilDFS(Gnode start, int depth, Gnode target, stack<string>& moves, int& num)
	{
		if (compareNode(start, target))
		{
			num++;
			return true;
		}
		if (depth == 0)
		{
			return false;
		}
		int j = 1;

		if (depth > 0) {
			if (start.adjacencylist.size() == 0) {
				Gnode successor[12];

				for (int i = 0; i < 12; i++)
				{
					successor[i] = clone(start);
				}

				callRotation(successor, 12);

				for (int i = 0; i < 12; i++) {
					start.addChild(successor[i]);
				}
			}

			for (auto i = start.adjacencylist.begin(); i != start.adjacencylist.end(); i++)
			{
				Gnode temp = *i;
				num++;
				bool found = utilDFS(temp, depth - 1, target, moves, num);
				if (found == true)
				{
					moves.push(temp.rotation);
					return true;
				}
				j++;
			}

		}
		return false;
	}

	static void callRotation(Gnode* arr, int n)				//applies all 12 rotations to current state of cube
	{
		int j = 0;
		for (int i = 0; i < n / 2; i++)
		{
			arr[i].rotateNode(j, 1);
			arr[i].rotation = "Move face ";
			arr[i].rotation.append(to_string(j));
			arr[i].rotation.append(" Clockwise\n");
			j++;
		}
		j = 0;
		for (int i = n / 2; i < n; i++)
		{
			arr[i].rotateNode(j, 0);
			arr[i].rotation = "Move face ";
			arr[i].rotation.append(to_string(j));
			arr[i].rotation.append(" AntiClockwise\n");
			j++;
		}
	}

	void rotateNode(int face, int direction)				//takes a face and type of rotations to rotate the face
	{	
		rotateCube(this->node, face, direction);
	}

	void rotateCube(int arr[6][3][3], int face, int move)						//applies rotations to 3D array holding cube entries
	{
		int i = 0, j = 0, z = 0;
		int a1[3], a2[3], a3[3], a4[4], store[2], tran[3][3], k1 = 0, k2 = 0;

		if (move == 1)
		{
			if (face == 0)
			{
				for (i = 0; i < 3; i++) {
					a1[i] = arr[1][2][i];
					a2[i] = arr[4][i][0];
					a3[i] = arr[3][0][i];
					a4[i] = arr[5][i][2];
				}

				for (i = 0; i < 3; i++) {
					arr[4][i][0] = a1[i];
					arr[3][0][i] = a2[i];
					arr[5][i][2] = a3[i];
					arr[1][2][i] = a4[i];
				}
			}

			if (face == 1)
			{
				for (i = 0; i < 3; i++) {
					a1[i] = arr[2][2][i];
					a2[i] = arr[4][i][0];
					a3[i] = arr[0][0][i];
					a4[i] = arr[5][i][2];
				}

				for (i = 0; i < 3; i++) {
					arr[4][i][0] = a1[i];
					arr[0][0][i] = a2[i];
					arr[5][i][2] = a3[i];
					arr[2][2][i] = a4[i];
				}
			}

			if (face == 2)
			{
				for (i = 0; i < 3; i++) {
					a1[i] = arr[3][2][i];
					a2[i] = arr[4][i][0];
					a3[i] = arr[1][0][i];
					a4[i] = arr[5][i][2];
				}

				for (i = 0; i < 3; i++) {
					arr[4][i][0] = a1[i];
					arr[1][0][i] = a2[i];
					arr[5][i][2] = a3[i];
					arr[3][2][i] = a4[i];
				}
			}

			if (face == 3)
			{
				for (i = 0; i < 3; i++) {
					a1[i] = arr[0][2][i];
					a2[i] = arr[4][i][0];
					a3[i] = arr[2][0][i];
					a4[i] = arr[5][i][2];
				}

				for (i = 0; i < 3; i++) {
					arr[4][i][0] = a1[i];
					arr[2][0][i] = a2[i];
					arr[5][i][2] = a3[i];
					arr[0][2][i] = a4[i];
				}
			}

			if (face == 4)
			{
				for (i = 0; i < 3; i++) {
					a1[i] = arr[1][i][2];
					a2[i] = arr[2][i][2];
					a3[i] = arr[3][i][2];
					a4[i] = arr[0][i][2];
				}

				for (i = 0; i < 3; i++) {
					arr[2][i][2] = a1[i];
					arr[3][i][2] = a2[i];
					arr[0][i][2] = a3[i];
					arr[1][i][2] = a4[i];
				}
			}

			if (face == 5)
			{
				for (i = 0; i < 3; i++) {
					a1[i] = arr[1][i][0];
					a2[i] = arr[0][i][0];
					a3[i] = arr[3][i][0];
					a4[i] = arr[2][i][0];
				}

				for (i = 0; i < 3; i++) {
					arr[0][i][0] = a1[i];
					arr[3][i][0] = a2[i];
					arr[2][i][0] = a3[i];
					arr[1][i][0] = a4[i];
				}
			}

			for (i = 0; i < 3; i++) {
				for (j = 0; j < 3; j++) {
					tran[j][i] = arr[face][i][j];
				}
			}

			k1 = 2, k2 = 0;
			for (j = 0; j < 3; j++) {
				store[0] = tran[j][k2];
				store[1] = tran[j][k1];
				tran[j][k2] = store[1];
				tran[j][k1] = store[0];
			}
			for (i = 0; i < 3; i++) {
				for (j = 0; j < 3; j++) {
					arr[face][i][j] = tran[i][j];
				}
			}


		}
		//-----------------------------------------------------------------------------------------------------------------
		if (move == 0)
		{
			if (face == 0)
			{
				for (i = 0; i < 3; i++) {
					a1[i] = arr[1][2][i];
					a2[i] = arr[4][i][0];
					a3[i] = arr[3][0][i];
					a4[i] = arr[5][i][2];
				}
				for (i = 0; i < 3; i++) {
					arr[4][i][0] = a3[i];
					arr[3][0][i] = a4[i];
					arr[5][i][2] = a1[i];
					arr[1][2][i] = a2[i];
				}
			}

			if (face == 1)
			{
				for (i = 0; i < 3; i++) {
					a1[i] = arr[2][2][i];
					a2[i] = arr[4][i][0];
					a3[i] = arr[0][0][i];
					a4[i] = arr[5][i][2];
				}
				for (i = 0; i < 3; i++) {
					arr[4][i][0] = a3[i];
					arr[0][0][i] = a4[i];
					arr[5][i][2] = a1[i];
					arr[2][2][i] = a2[i];
				}
			}

			if (face == 2)
			{
				for (i = 0; i < 3; i++) {
					a1[i] = arr[3][2][i];
					a2[i] = arr[4][i][0];
					a3[i] = arr[1][0][i];
					a4[i] = arr[5][i][2];
				}
				for (i = 0; i < 3; i++) {
					arr[4][i][0] = a3[i];
					arr[1][0][i] = a4[i];
					arr[5][i][2] = a1[i];
					arr[3][2][i] = a2[i];
				}
			}

			if (face == 3)
			{
				for (i = 0; i < 3; i++) {
					a1[i] = arr[0][2][i];
					a2[i] = arr[4][i][0];
					a3[i] = arr[2][0][i];
					a4[i] = arr[5][i][2];
				}
				for (i = 0; i < 3; i++) {
					arr[4][i][0] = a3[i];
					arr[2][0][i] = a4[i];
					arr[5][i][2] = a1[i];
					arr[0][2][i] = a2[i];
				}
			}

			if (face == 4)
			{
				for (i = 0; i < 3; i++) {
					a1[i] = arr[1][i][2];
					a2[i] = arr[2][i][2];
					a3[i] = arr[3][i][2];
					a4[i] = arr[0][i][2];
				}
				for (i = 0; i < 3; i++) {
					arr[2][i][2] = a3[i];
					arr[3][i][2] = a4[i];
					arr[0][i][2] = a1[i];
					arr[1][i][2] = a2[i];
				}
			}

			if (face == 5)
			{
				for (i = 0; i < 3; i++) {
					a1[i] = arr[1][i][0];
					a2[i] = arr[0][i][0];
					a3[i] = arr[3][i][0];
					a4[i] = arr[2][i][0];
				}
				for (i = 0; i < 3; i++) {
					arr[0][i][0] = a3[i];
					arr[3][i][0] = a4[i];
					arr[2][i][0] = a1[i];
					arr[1][i][0] = a2[i];
				}
			}
			//swap elements to rotate 90 degree
			k1 = 2, k2 = 0;
			for (j = 0; j < 3; j++) {
				swap(arr[face][j][0], arr[face][j][2]);
			}

			for (i = 0; i < 3; i++) {
				for (j = 0; j < 3; j++) {
					tran[j][i] = arr[face][i][j];
				}
			}
			for (i = 0; i < 3; i++) {
				for (j = 0; j < 3; j++) {
					arr[face][i][j] = tran[i][j];
				}
			}
		}
	}

	////////////////////////////////////////////////////////////////////////   A* from here downwards

	static void runAStarPQ(Gnode start, Gnode target)
	{
		stack<Gnode> closed;
		priority_queue<Gnode> open;

		bool foundDest = false;
		int j = 0;

		start.f = 0;
		start.g = 0;
		start.h = 0;

		time_t my_time = time(NULL);
		printf("Start Time A*: %s", ctime(&my_time));

		open.push(start);
		Gnode q;

		while (!open.empty())
		{
			q = open.top();
			open.pop();
			j++;

			if (compareNode(q, target))
			{
				cout << "Goal Found\n";
				q.printNode();
				time_t my_time = time(NULL);
				printf("End Time A*: %s\n", ctime(&my_time));
				closed.push(q);

				cout << "Nodes traversed: " << j << endl << endl;

				cout << q.rotation << endl;								//Printing moves from the stack taken to solve cube.
				Gnode* curr = q.parent;									//This follows bottom up approach.
				while (curr != 0) {										//Starting from goal and going to the top/initial state.
					cout << curr->rotation << endl;
					curr = curr->parent;
				}
				break;
			}

			//if (q.adjacencylist.size() == 0)
			{
				Gnode child[12];
				for (int i = 0; i < 12; i++)
				{
					child[i] = clone(q);
					child[i].setParent(q);
				}

				callRotation(child, 12);

				bool temp[6][3][3];
				clear_bool(temp);

				for (int i = 0; i < 12; i++) {
					child[i].g = q.g + 1;											//setting g(n)
					child[i].h = Heuristic(child[i].node, temp, target.node);		//setting h(n)
					child[i].f = child[i].g + child[i].h;
					bool found = findMatch(child[i], open);							//checks if a node of same cube state and smaller f value already exists in the 'open' queue
					if (found == true)
					{
						open.push(child[i]);										//push child to 'open' queue
					}
				}
			}
			closed.push(q);															//pushing visited nodes in the 'closed' stack
		}
	}

	static void clear_bool(bool v[6][3][3])											//sets all bool values to true
	{
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					v[i][j][k] = true;
				}
			}
		}

	}

	static int Return_Num(int phase, int i)					//supplementary function to check faces of cube while computing heuristic
	{
		int st = 0;
		if (phase == 0)
		{
			if (i == 0) {
				st = 0;
			}
			else if (i == 1 || i == 3 || i == 4 || i == 5) {
				st = 1;
			}
			else {
				st = 2;
			}

		}

		else if (phase == 1)
		{
			if (i == 1) {
				st = 0;
			}
			else if (i == 2 || i == 0 || i == 4 || i == 5) {
				st = 1;
			}
			else {
				st = 2;
			}
		}

		else if (phase == 2)
		{
			if (i == 2) {
				st = 0;
			}
			else if (i == 1 || i == 3 || i == 4 || i == 5) {
				st = 1;
			}
			else {
				st = 2;
			}
		}

		else if (phase == 3)
		{
			if (i == 3) {
				st = 0;
			}
			else if (i == 0 || i == 2 || i == 4 || i == 5) {
				st = 1;
			}
			else {
				st = 2;
			}
		}

		else if (phase == 4)
		{
			if (i == 4) {
				st = 0;
			}
			else if (i == 0 || i == 1 || i == 2 || i == 3) {
				st = 1;
			}
			else {
				st = 2;
			}
		}

		else if (phase == 5)
		{
			if (i == 5) {
				st = 0;
			}
			else if (i == 0 || i == 1 || i == 2 || i == 3) {
				st = 1;
			}
			else {
				st = 2;
			}
		}

		return st;
	}

	//supplementary function to check faces of cube while computing heuristic
	static int Calculate(int arr[6][3][3], int goal[6][3][3], bool vis[6][3][3], int temp, int r, int c)
	{
		int i = 0, j = 0, k = 0, temp1 = 0;
		int sum = 0, st = 0;
		bool flag1 = true, flag2 = true, flag3 = true;

		temp1 = temp;
		for (i = 0; i < 6 && flag1 == true; i++)
		{
			for (j = 0; j < 3 && flag2 == true; j++)
			{
				for (k = 0; k < 3 && flag3 == true; k++)
				{
					//vis[temp1][r][c] = false;
					if (arr[temp1][j][k] == goal[temp][r][c] && vis[temp1][j][k] != false)
					{
						st = Return_Num(temp, temp1);
						sum += abs(r - j) + abs(c - k) + st;
						vis[temp1][j][k] = false;
						flag1 = false;
						flag2 = false;
						flag3 = false;
					}

				}
			}
			temp1 = (temp1 + 1) % 6;

		}

		return sum;

	}

	static int Heuristic(int arr[6][3][3], bool vis[6][3][3], int goal[6][3][3])		//Heuristic function
	{
		int phase = 0;
		clear_bool(vis);
		int r = 0, c = 0, sum = 0, i = 0, j = 0, k = 0;
		double h = 0;
		bool flag1 = true, flag2 = true, flag3 = true;
		int st = 0;
		int temp = 0, temp1 = 0;

		for (phase = 0; phase < 6; phase++)  //loop for each phase to calculate
		{
			for (r = 0; r < 3; r++)
			{
				for (c = 0; c < 3; c++)
				{
					if (arr[temp][r][c] != goal[temp][r][c])
					{
						sum += Calculate(arr, goal, vis, temp, r, c);
					}
					else
					{
						sum += 0;
					}

				}
			}

			temp++;
		}
		//cout << "Inline: " << h / 8 << endl;
		sum /= 12;
		int h1 = sum;
		//sum = 0;
		//clear_bool(vis);
		//for (phase = 0; phase < 6; phase++)  //loop for each phase to calculate
		//{
		//	for (r = 0; r < 3; r++)
		//	{
		//		for (c = 0; c < 3; c++)
		//		{
		//			for (i = 0; i < 6 && flag1 == true; i++)
		//			{
		//				for (j = 0; j < 3 && flag2 == true; j++)
		//				{
		//					for (k = 0; k < 3 && flag3 == true; k++)
		//					{
		//						if ((r + c) == 1 || (r + c) == 3)
		//						{
		//							vis[phase][1][1] = false;  //to avoid check itself in same phase
		//							if (arr[phase][1][1] != arr[phase][r][c])
		//							{
		//								if (arr[phase][1][1] == arr[i][j][k] && vis[i][j][k] != false)
		//								{
		//									st = Return_Num(phase, i);
		//									sum += abs(r - j) + abs(c - k) + st;
		//									vis[i][j][k] = false;
		//									flag1 = false;
		//									flag2 = false;
		//									flag3 = false;
		//								}
		//							}
		//						}
		//					}
		//				}
		//			}
		//			flag1 = true;
		//			flag2 = true;
		//			flag3 = true;
		//		}
		//	}
		//	clear_bool(vis);
		//}
		////cout << "Inline: " << h / 8 << endl;
		//sum /= 4;
		//int h2 = sum;
		/*if (h2>h1)
			return h1;*/
		return h1;
	}


	static bool findMatch(Gnode src, priority_queue<Gnode> ls)				//checks if a node of same cube state and smaller f value already exists in the 'open' queue
	{
		for (int i = 0; i < ls.size(); i++)
		{
			Gnode temp = ls.top();
			ls.pop();
			if (compareNode(src, temp) == true)
			{
				if (src.f > temp.f)
				{
					return false;
				}
			}
		}
		return true;
	}
};

bool operator<(const Gnode& p1, const Gnode& p2)					//operator overloaded for priority queue to sort on basis of f-value
{
	return p1.getFValue() > p2.getFValue();
}

void readFile(int init[6][3][3], int final[6][3][3])
{
	ifstream fin;
	fin.open("file.txt");

	if (fin.is_open()) {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					fin >> init[i][j][k];
					//cout << init[i][j][k];
				}
			}
		}
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					fin >> final[i][j][k];
					//cout << final[i][j][k];
				}
			}
		}
	}
	fin.close();
}

void printCube(int arr[6][3][3]) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				cout << arr[i][j][k] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

void rotateCubee(int arr[6][3][3], int face, int move)
{
	int i = 0, j = 0, z = 0;
	int a1[3], a2[3], a3[3], a4[4], store[2], tran[3][3], k1 = 0, k2 = 0;

	if (move == 1)
	{
		if (face == 0)
		{
			for (i = 0; i < 3; i++) {
				a1[i] = arr[1][2][i];
				a2[i] = arr[4][i][0];
				a3[i] = arr[3][0][i];
				a4[i] = arr[5][i][2];
			}

			for (i = 0; i < 3; i++) {
				arr[4][i][0] = a1[i];
				arr[3][0][i] = a2[i];
				arr[5][i][2] = a3[i];
				arr[1][2][i] = a4[i];
			}

		}

		if (face == 1)
		{
			for (i = 0; i < 3; i++) {
				a1[i] = arr[2][2][i];
				a2[i] = arr[4][i][0];
				a3[i] = arr[0][0][i];
				a4[i] = arr[5][i][2];
			}

			for (i = 0; i < 3; i++) {
				arr[4][i][0] = a1[i];
				arr[0][0][i] = a2[i];
				arr[5][i][2] = a3[i];
				arr[2][2][i] = a4[i];
			}
		}

		if (face == 2)
		{
			for (i = 0; i < 3; i++) {
				a1[i] = arr[3][2][i];
				a2[i] = arr[4][i][0];
				a3[i] = arr[1][0][i];
				a4[i] = arr[5][i][2];
			}

			for (i = 0; i < 3; i++) {
				arr[4][i][0] = a1[i];
				arr[1][0][i] = a2[i];
				arr[5][i][2] = a3[i];
				arr[3][2][i] = a4[i];
			}

		}

		if (face == 3)
		{
			for (i = 0; i < 3; i++) {
				a1[i] = arr[0][2][i];
				a2[i] = arr[4][i][0];
				a3[i] = arr[2][0][i];
				a4[i] = arr[5][i][2];
			}

			for (i = 0; i < 3; i++) {
				arr[4][i][0] = a1[i];
				arr[2][0][i] = a2[i];
				arr[5][i][2] = a3[i];
				arr[0][2][i] = a4[i];
			}

		}

		if (face == 4)
		{
			for (i = 0; i < 3; i++) {
				a1[i] = arr[1][i][2];
				a2[i] = arr[2][i][2];
				a3[i] = arr[3][i][2];
				a4[i] = arr[0][i][2];
			}

			for (i = 0; i < 3; i++) {
				arr[2][i][2] = a1[i];
				arr[3][i][2] = a2[i];
				arr[0][i][2] = a3[i];
				arr[1][i][2] = a4[i];
			}
		}

		if (face == 5)
		{
			for (i = 0; i < 3; i++) {
				a1[i] = arr[1][i][0];
				a2[i] = arr[0][i][0];
				a3[i] = arr[3][i][0];
				a4[i] = arr[2][i][0];
			}

			for (i = 0; i < 3; i++) {
				arr[0][i][0] = a1[i];
				arr[3][i][0] = a2[i];
				arr[2][i][0] = a3[i];
				arr[1][i][0] = a4[i];
			}
		}

		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				tran[j][i] = arr[face][i][j];
			}
		}

		k1 = 2, k2 = 0;
		for (j = 0; j < 3; j++) {
			store[0] = tran[j][k2];
			store[1] = tran[j][k1];
			tran[j][k2] = store[1];
			tran[j][k1] = store[0];
		}
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				arr[face][i][j] = tran[i][j];
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------------
	if (move == 0)
	{
		if (face == 0)
		{
			for (i = 0; i < 3; i++) {
				a1[i] = arr[1][2][i];
				a2[i] = arr[4][i][0];
				a3[i] = arr[3][0][i];
				a4[i] = arr[5][i][2];
			}
			for (i = 0; i < 3; i++) {
				arr[4][i][0] = a3[i];
				arr[3][0][i] = a4[i];
				arr[5][i][2] = a1[i];
				arr[1][2][i] = a2[i];
			}

		}

		if (face == 1)
		{
			for (i = 0; i < 3; i++) {
				a1[i] = arr[2][2][i];
				a2[i] = arr[4][i][0];
				a3[i] = arr[0][0][i];
				a4[i] = arr[5][i][2];
			}
			for (i = 0; i < 3; i++) {
				arr[4][i][0] = a3[i];
				arr[0][0][i] = a4[i];
				arr[5][i][2] = a1[i];
				arr[2][2][i] = a2[i];
			}
		}

		if (face == 2)
		{
			for (i = 0; i < 3; i++) {
				a1[i] = arr[3][2][i];
				a2[i] = arr[4][i][0];
				a3[i] = arr[1][0][i];
				a4[i] = arr[5][i][2];
			}
			for (i = 0; i < 3; i++) {
				arr[4][i][0] = a3[i];
				arr[1][0][i] = a4[i];
				arr[5][i][2] = a1[i];
				arr[3][2][i] = a2[i];
			}
		}

		if (face == 3)
		{
			for (i = 0; i < 3; i++) {
				a1[i] = arr[0][2][i];
				a2[i] = arr[4][i][0];
				a3[i] = arr[2][0][i];
				a4[i] = arr[5][i][2];
			}
			for (i = 0; i < 3; i++) {
				arr[4][i][0] = a3[i];
				arr[2][0][i] = a4[i];
				arr[5][i][2] = a1[i];
				arr[0][2][i] = a2[i];
			}
		}

		if (face == 4)
		{
			for (i = 0; i < 3; i++) {
				a1[i] = arr[1][i][2];
				a2[i] = arr[2][i][2];
				a3[i] = arr[3][i][2];
				a4[i] = arr[0][i][2];
			}
			for (i = 0; i < 3; i++) {
				arr[2][i][2] = a3[i];
				arr[3][i][2] = a4[i];
				arr[0][i][2] = a1[i];
				arr[1][i][2] = a2[i];
			}
		}

		if (face == 5)
		{
			for (i = 0; i < 3; i++) {
				a1[i] = arr[1][i][0];
				a2[i] = arr[0][i][0];
				a3[i] = arr[3][i][0];
				a4[i] = arr[2][i][0];
			}
			for (i = 0; i < 3; i++) {
				arr[0][i][0] = a3[i];
				arr[3][i][0] = a4[i];
				arr[2][i][0] = a1[i];
				arr[1][i][0] = a2[i];
			}
		}
		//swap elements to rotate 90 degree
		k1 = 2, k2 = 0;
		for (j = 0; j < 3; j++) {
			swap(arr[face][j][0], arr[face][j][2]);
		}

		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				tran[j][i] = arr[face][i][j];
			}
		}
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				arr[face][i][j] = tran[i][j];
			}
		}
	}
}

int main() {

	int init[6][3][3], final[6][3][3];
	readFile(init, final);

	bool temp[6][3][3];

	//rotateCubee(init, 3, 1);
	//rotateCubee(init, 1, 1);
	//rotateCubee(init, 5, 0);
	//rotateCubee(init, 2, 0);
	//rotateCubee(init, 4, 0);
	//rotateCubee(init, 1, 0);
	//rotateCubee(init, 0, 1);
	//rotateCubee(init, 4, 1);
	//rotateCubee(init, 1, 0);

	Gnode start(init);
	Gnode target(final);

	cout << "Init state:\n";
	start.printNode();
	start.setDepthLevel(0);
	cout << "Goal state:\n";
	target.printNode();

	Gnode::runDFS(start, 15, target);						//Calling IDS

	cout << "\n\nIDS exited..\n\n";

	Gnode::runAStarPQ(start, target);						//Calling A*
	cout << "A* exited..\n";

	_getch();

}
struct Node{
	char **cell;
	int f;
	int g;
	Node *parent;
	int action;
};

int N;							// Width of Puzzle

int heuristic1(Node *p); 		// Manhattan
int heuristic2(Node *p); 		// Manhattan + Linear Conflict
int heuristic3(Node *p); 		// Tiles out of row and column
int heuristic4(Node *p); 		// Pythagorean - not admissable
int heuristic5(Node *p);		// N-MaxSwap - Gaschnig's Heuristic
int inRow(int position, int row);
int inColumn(int position, int column);
int mapIndex(int row, int column);

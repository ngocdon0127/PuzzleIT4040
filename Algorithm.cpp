#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

list<Node*> listNode;			// Fringe
set<Node*> setNode;				// Store all used node. Used to free memory
Node *node;						// Start State
Node *goal;						// Store Goal State
int deep = 0;					// Solution 'S Deep
float numOfNode = 1;				// No. of node used to search
int cutoff = 0;					// Use in IDA*
int newcutoff = 0;				// Use in IDA*
int timeAStar = 30;				// If A* run too long, terminate it
int timeIDAStar = 60;			// If IDA* run too long, terminate it
char choice;					// Heuristic Flag

int check(Node *p);		 		// Check if p is goal state or not
int checkSolvable(Node *p);		// Check state if it is solvable or not
int treeSearch(void);			// A* Search
int treeSearch1(void);			// IDA* Search
Node* pick(void);				// Choose one node to expand from fringe. Use in A*
Node* pick1(void);				// Choose one node to expand from fringe. Use in IDA*
int blank_x(Node *p);			// return x coordinates of blank tile
int blank_y(Node *p);			// return y coordinates of blank tile
int calculate(Node *p);
int copy(Node *a, Node *b);		// copy data from b to a
int freeNode(Node *p);			// Free memory allocated for node p

int treeSearch(void){
	Timer ti;
	Node *p;
	Node *p1;
	int x;
	int y;
//	int keyp1;
	while (!listNode.empty()){
		y = ti.getElapsedTime();
		if (y > 30){
			deep = -1;
			return 0;
		}
		p = pick();
		if (check(p)){
//			result(p);
			goal = p;
			deep = p->g;
			return 0;
		}
		x = blank_x(p);
		y = blank_y(p);
		// blank up
		if ((x > 0) && (p->action != DOWN)){
			p1 = newNode();
			setNode.insert(p1);
			copy(p1, p);
			p1->cell[x - 1][y] = 0;
			p1->cell[x][y] = p->cell[x-1][y];
			p1->g = p->g + 1;
			calculate(p1);
			p1->parent = p;
			p1->action = UP;
//			if (!exist(p1)){
				listNode.push_back(p1);
//				mark(p1);
				numOfNode++;
//			}
		}
		
		// blank down
		if ((x < N - 1) && (p->action != UP)){
			p1 = newNode();
			setNode.insert(p1);
			copy(p1, p);
			p1->cell[x + 1][y] = 0;
			p1->cell[x][y] = p->cell[x + 1][y];
			p1->g = p->g + 1;
			calculate(p1);
			p1->parent = p;
			p1->action = DOWN;
//			if (!exist(p1)){
				listNode.push_back(p1);
//				mark(p1);
				numOfNode++;
//			}
		}
		
		// blank left
		if ((y > 0) && (p->action != RIGHT)){
			p1 = newNode();
			setNode.insert(p1);
			copy(p1, p);
			p1->cell[x][y - 1] = 0;
			p1->cell[x][y] = p->cell[x][y - 1];
			p1->g = p->g + 1;
			calculate(p1);
			p1->parent = p;
			p1->action = LEFT;
//			if (!exist(p1)){
				listNode.push_back(p1);
//				mark(p1);
				numOfNode++;
//			}
		}
		
		// blank right
		if ((y < N - 1) && (p->action != LEFT)){
			p1 = newNode();
			setNode.insert(p1);
			copy(p1, p);
			p1->cell[x][y + 1] = 0;
			p1->cell[x][y] = p->cell[x][y + 1];
			p1->g = p->g + 1;
			calculate(p1);
			p1->parent = p;
			p1->action = RIGHT;
//			if (!exist(p1)){
				listNode.push_back(p1);
//				mark(p1);
				numOfNode++;
//			}
		}
		//system("cls");
//		printf("%i:%i blank [%i] [%i]\n", numOfNode, listNode.size(), x, y);
	}
	return 0;
}

int treeSearch1(void){
	Timer ti;
	Node *p;
	Node *p1;
	int x;
	int y;
	int c = 0;
	cutoff = node->f;
//	int keyp1;
	while (1){
		if (listNode.empty()){
			set<Node*>::iterator it = setNode.begin();
			//puts("before clean");
			//getch();
			Node *nodeToFree = NULL;
			for(; it != setNode.end(); it++){
				if (*it == node)
					continue;
				nodeToFree = *it;
				freeNode(nodeToFree);
			}
			//puts("after clean");
			//getch();
			listNode.clear();
			listNode.push_back(node);
			setNode.clear();
			setNode.insert(node);
			cutoff = newcutoff;
			c = 0;
		}
		while (!listNode.empty()){
			y = ti.getElapsedTime();
			if (y > timeIDAStar){
				deep = -1;
				return 0;
			}
			p = pick1();
			if (check(p)){
//				result(p);
				goal = p;
				deep = p->g;
				return 0;
			}
			x = blank_x(p);
			y = blank_y(p);
			
			// blank up
			if ((x > 0) && (p->action != DOWN)){
				p1 = newNode();
				numOfNode++;
				setNode.insert(p1);
//				puts("before");
				copy(p1, p);
//				puts("after");
				p1->cell[x - 1][y] = 0;
				p1->cell[x][y] = p->cell[x-1][y];
				p1->g = p->g + 1;
				calculate(p1);
				p1->parent = p;
				p1->action = UP;
				int newcost = p1->f;
				if (newcost <= cutoff){
					listNode.push_front(p1);
//					mark(p1);
				}
				else{
					if (c == 0){
						newcutoff = newcost;
						c = 1;
					}
					else
						newcutoff = (newcutoff > newcost) ? newcost : newcutoff;
				}
			}
			
			// blank down
			if ((x < N - 1) && (p->action != UP)){
				p1 = newNode();
				numOfNode++;
				setNode.insert(p1);
				copy(p1, p);
				p1->cell[x + 1][y] = 0;
				p1->cell[x][y] = p->cell[x + 1][y];
				p1->g = p->g + 1;
				calculate(p1);
				p1->parent = p;
				p1->action = DOWN;
				int newcost = p1->f;
				if (newcost <= cutoff){
					listNode.push_front(p1);
//					mark(p1);
				}
				else{
					if (c == 0){
						newcutoff = newcost;
						c = 1;
					}
					else
						newcutoff = (newcutoff > newcost) ? newcost : newcutoff;
				}
			}
			
			// blank left
			if ((y > 0) && (p->action != RIGHT)){
				p1 = newNode();
				numOfNode++;
				setNode.insert(p1);
				copy(p1, p);
				p1->cell[x][y - 1] = 0;
				p1->cell[x][y] = p->cell[x][y - 1];
				p1->g = p->g + 1;
				calculate(p1);
				p1->parent = p;
				p1->action = LEFT;
				int newcost = p1->f;
				if (newcost <= cutoff){
					listNode.push_front(p1);
//					mark(p1);
				}
				else{
					if (c == 0){
						newcutoff = newcost;
						c = 1;
					}
					else
						newcutoff = (newcutoff > newcost) ? newcost : newcutoff;
				}
			}
			
			// blank right
			if ((y < N - 1) && (p->action != LEFT)){
				p1 = newNode();
				numOfNode++;
				setNode.insert(p1);
				copy(p1, p);
				p1->cell[x][y + 1] = 0;
				p1->cell[x][y] = p->cell[x][y + 1];
				p1->g = p->g + 1;
				calculate(p1);
				p1->parent = p;
				p1->action = RIGHT;
				int newcost = p1->f;
				if (newcost <= cutoff){
					listNode.push_front(p1);
//					mark(p1);
				}
				else{
					if (c == 0){
						newcutoff = newcost;
						c = 1;
					}
					else
						newcutoff = (newcutoff > newcost) ? newcost : newcutoff;
				}
			}
			//system("cls");
	//		printf("%i:%i blank [%i] [%i]\n", numOfNode, listNode.size(), x, y);
		}
	}
	return 0;
}


Node* pick(void){
	list<Node*>::iterator it = listNode.begin();
	Node *p = *it;
	for(; it != listNode.end(); it++){
		if (p->f > (*it)->f)
			p = *it;
	}
	listNode.remove(p);
	return p;
}

Node* pick1(void){
	list<Node*>::iterator it = listNode.begin();
	Node *p = *it;
	listNode.remove(p);
	return p;
}

int check(Node *p){
	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++)
			if (p->cell[i][j] != (i * N + j))
				return 0;
	return 1;
}

int blank_x(Node *p){
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			if (p->cell[i][j] == 0)
				return i;
		}
	}
}

int blank_y(Node *p){
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			if (p->cell[i][j] == 0)
				return j;
		}
	}
}

int freeNode(Node *p){
//	puts("start free node");
	if (!p){
//		puts("Node null. Free cl a");
		return 0;
	}
	else{
//		puts("p not null. Free now");
	}
	if (p->cell == NULL){
//		puts("p->cell null. Free cl.");
		return 0;
	}
	else{
//		puts("p->cell not null. Free now");
	}
	for(int i = 0; i < N; i++){
//		printf("start freeing p->cell[%i] %i\n", i, p->cell[i]);
		free(p->cell[i]);
//		printf("p->cell[%i] ok\n", i);
	}
	free(p->cell);
//	puts("free p->cell ok");
	free(p);
//	puts("free p ok");
}

int copy(Node *a, Node *b){
	a->action = b->action;
	a->f = b->f;
	a->g = b->g;
	a->parent = b->parent;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			a->cell[i][j] = b->cell[i][j];
		}
	}
	return 0;
}

int calculate(Node *p){
	int sum;
	switch (choice){
		case '1':
			sum = heuristic1(p);
			break;
		case '2':
			sum = heuristic2(p);
			break;
		case '3':
			sum = heuristic3(p);
			break;
		case '4':
			sum = heuristic4(p);
			break;
		case '5':
			sum = heuristic5(p);
			break;
	}
	p->f = p->g + sum;
}

int checkSolvable(Node *p){
	int *a = (int*) malloc(N * N * sizeof(int));
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			a[mapIndex(i, j)] = p->cell[i][j];
		}
	}
	int inversions = 0;
	for(int i = 0; i < N * N - 1; i++){
		for(int j = i + 1; j < N * N; j++){
			if (!a[j]){
				continue;
			}
			if (a[i] > a[j]){
				inversions++;
			}
		}
	}
	
	// Puzzle is solvable if and only if:
	
//	(N odd && inversions even) OR
//	(N even && blank on even row from top && inversion even) OR
//	(N even && blank on odd  row from top && inversion odd )
	if (((N % 2) && (inversions % 2 == 0))  ||  ((N % 2 == 0) && ((blank_x(p) % 2) == (inversions % 2)))){
		return 1;
	}
	return 0;
}

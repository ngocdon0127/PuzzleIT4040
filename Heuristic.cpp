struct Node{
	char **cell;
	int f;
	int g;
	Node *parent;
	int action;
};

int N;									// Width of Puzzle


Node* newNode(void);
int heuristic1(Node *p); 				// Manhattan
int heuristic2(Node *p); 				// Manhattan + Linear Conflict
int heuristic3(Node *p); 				// Tiles out of row and column
int heuristic4(Node *p); 				// Pythagorean - not admissible
int heuristic5(Node *p);				// N-MaxSwap - Gaschnig's Heuristic
int inRow(int position, int row);
int inColumn(int position, int column);
int mapIndex(int row, int column); 		// Convert 2D coordinates to 1D coordinates

int heuristic1(Node *p){
	int cost;
	int sum = 0;
	int num;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			num = p->cell[i][j];
			if (num == 0){
				//printf("  ");
				continue;
			}
				
			cost = abs(num / N - i) + abs(num % N - j);
			//printf("%i ", cost);
			sum += cost;
		}
//		puts("");
	}
//	sum = 0;
	return sum;
}

int inRow(int position, int row){
	if (position / N == row){
		return 1;
	}
	return 0;
}

int inColumn(int position, int column){
	if (position % N == column){
		return 1;
	}
	return 0;
}

int heuristic2(Node *p){
	
	int sum = heuristic1(p);
	int num = 0;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N - 1; j++){
			int x1 = p->cell[i][j];
			if (!x1){
				continue;
			}
			if (!inRow(x1, i))
				continue;
			for(int k = j + 1; k < N; k++){
				int x2 = p->cell[i][k];
				if (!x2){
					continue;
				}
				if (!inRow(x2, i)){
					continue;
				}
				if (x1 > x2){
					num++;
				}
			}
		}
	}
	
	for(int j = 0; j < N; j++){
		for(int i = 0; i < N - 1; i++){
			int x1 = p->cell[i][j];
			if (!x1){
				continue;
			}
			if (!inColumn(x1, j)){
				continue;
			}
			for(int k = i + 1; k < N; k++){
				int x2 = p->cell[k][j];
				if (!x2){
					continue;
				}
				if (!inColumn(x2, j)){
					continue;
				}
				if (x1 > x2){
					num++;
				}
			}
		}
	}
	
	num *= 2;
	return sum += num;
}

int heuristic4(Node *p){
	int cost;
	int sum = 0;
	int num;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			num = p->cell[i][j];
			if (num == 0){
				//printf("  ");
				continue;
			}
				
			cost = (num / N - i) * (num / N - i) + (num % N - j) * (num % N - j);
			//printf("%i ", cost);
			sum += cost;
		}
//		puts("");
	}
	return sum;
}

int heuristic5(Node *p){
	int size = N * N;
//	int *p = (int*) calloc(size, sizeof(int));
//	int *b = (int*) calloc(size, sizeof(int));
	int P[size];
	int B[size];
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			P[mapIndex(i, j)] = p->cell[i][j];
		}
	}
	for(int i = 0; i < size; i++){
		B[P[i]] = i;
	}
	int count = 0;
//	for(int i = 0; i < size; i++){
//		printf("%2i ", i);
//	}
//	printf(" | ");
//	for(int i = 0; i < size; i++){
//		printf("%2i ", i);
//	}
//	puts("");
	while (1){
		int check = 1;
		for(int i = 0; i < size; i++){
			if (P[i] != i){
				check = 0;
				break;
			}
		}
		if (check){
			break;
		}
		if (B[0]){
			swap(P[B[0]], P[B[B[0]]]);
			swap(B[0], B[B[0]]);
		}
		else{
			for(int i = 0; i < size; i++){
				if (!P[i]){
					continue;
				}
				if (P[i] == i){
					continue;
				}
				swap(P[i], P[B[0]]);
				swap(B[P[i]], B[0]);
				break;
			}
		}
//		for(int i = 0; i < size; i++){
//			B[P[i]] = i;
//		}
//		for(int i = 0; i < size; i++){
//			printf("%2i ", P[i]);
//		}
//		printf(" | ");
//		for(int i = 0; i < size; i++){
//			printf("%2i ", B[i]);
//		}
//		puts("");
//		getch();
		count++;
	}
//	free(p);
//	free(b);
	return count;
}

int heuristic3(Node *p){
	int sum = 0;
	int num;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			num = p->cell[i][j];
			//printf("%i ", num);
			if (num / N != i)
				sum++;
			if (num % N != j)
				sum++;
		}
	}
	//puts("hehe");
	return sum;
}

int mapIndex(int row, int column){
	return row * N + column;
}

Node* newNode(void){
	Node *p = new Node;
	p->cell = (char**) calloc(N, sizeof(char*));
	if (p->cell == NULL){
		return NULL;
	}
//	puts("p->cell ok");
	for(int i = 0; i < N; i++){
		p->cell[i] = (char*) calloc(N, sizeof(char));
		if (p->cell[i] == NULL){
//			printf("p->cell[%i] null\n", i);
			for(int j = 0; j < i; j++){
				free(p->cell[j]);
			}
			free(p->cell);
			return NULL;
		}
//		printf("p->cell[%i] ok\n", i);
	}
	return p;
}


#include <stdio.h>
#include <memory.h>

/*Note: for k=2, the program runs in about 5 seconds and returns nothing,
  which means that there is no resovable 1-factorization of H(6,2).
  
For k=3, the programs will output many solutions, 
  which are resovable 1-fatorizations of H(8,3).*/


const int d = 2;

/*const int k = 3;
const int n = k + k + d;
const int bins[n] = {1,2,4,8,16,32,64,128};
const int kad = k+d;    // the value of k + d
const int pkad = 120;   // the value of (k+d)!
const int number = 56;  // the value of {n choose k)*/

const int k = 2;
const int n = k + k + d;
const int bins[n] = {1,2,4,8,16,32};
const int kad = k+d;    // the value of k + d
const int pkad = 24;   // the value of (k+d)!
const int number = 15;  // the value of {n choose k)*/


const int tcolors = ((k + 1) * (k + 2)) / 2; // the value of {k+d choose k}

int perms[pkad][kad];

int A[number], B[number];

int countbits(int x){
	int s = 0;
	for (int i = 0; i < n; i++)
		if ((bins[i] & x) > 0) s++;
	return s;
}

int tp = 0;
bool used[kad];
int pnow[kad];

void searchPerms(int i){
	if (i == kad){
		memcpy(perms[tp], pnow, sizeof(pnow));
		tp++;
	}
	for (int j = 0; j < kad; j++)
		if (!used[j]){
			pnow[i] = j; used[j] = true;
			searchPerms(i + 1);
			used[j] = false;
		}
}

unsigned long long bpower[number];
unsigned long long link[number][pkad][tcolors];

void prepare(){
	bpower[0] = 1;
	for (int i = 1; i < number; i++)
		bpower[i] = bpower[i - 1] + bpower[i - 1];

	int tA = 0, tB = 0;
	for (int i = 0; i < (1 << n); i++){
		int j = countbits(i);
		if (j == k) A[tA++] = i;
		if (j == kad) B[tB++] = i;
	}
	for (int i = 0; i < number; i++){
		int x = A[i];
		int sc[kad], tc = 0;
		for (int j = 0; j < n; j++)
			if ((bins[j] & x) == 0) sc[tc++] = j;

		for (int j = 0; j < pkad; j++){
			int c = 0;
			for (int a = 0; a < kad; a++)
				for (int b = a + 1; b < kad; b++){
					int u = sc[perms[j][a]];
					int v = sc[perms[j][b]];
					int y = x + bins[u] + bins[v];
					int k = 0; while (B[k] != y) k++;
					link[i][j][c] = bpower[k];
					c++;
				}
		}
	}
}

unsigned long long mark[tcolors];
int choice[number];
int tsol;

void printAns(){
	printf("find solution #%d\n", ++tsol);
	for (int i = 0; i < tcolors; i++) printf("%d ", choice[i]);
	printf("\n");
	for (int i = 0; i < number; i++){
		int x = A[i];
		
		int a[n], sc[kad], tc = 0;
		memset(a, 0, sizeof(a));
		for (int k = 0; k < n; k++)
			if ((bins[k] & x) == 0) sc[tc++] = k;

		int j = choice[i];

		for (int k = 0; k < kad; k++){
			int u = sc[perms[j][k]];
			a[u] = k + 1;
		}
		for (int k = 0; k < n; k++) printf("%d ", a[k]);
		printf("\n");
	}
	printf("\n");

}

void search(int i){
	if (i == number){
		printAns();
	}
	else{
		for (int j = 0; j < pkad; j++){
			bool good = true;
			for (int c = 0; c < tcolors; c++)
				if ((link[i][j][c] & mark[c]) > 0) good = false;
			if (good){
				choice[i] = j;
				for (int c = 0; c < tcolors; c++) mark[c] += link[i][j][c];
				search(i + 1);
				for (int c = 0; c < tcolors; c++) mark[c] -= link[i][j][c];
			}
		}
	}
}

void main(){
	searchPerms(0);
	prepare();
	search(0);
}
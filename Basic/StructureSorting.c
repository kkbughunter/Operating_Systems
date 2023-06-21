#include<stdio.h>
#include<stdlib.h>

struct processor{
	int burst_time;
	int arrival_time;
};

void sortStructure(struct processor **p, int n){
	int i,j;
	struct processor *temp;
	for(i=0; i<n; i++){
		for(j=i+1; j<n; j++){
			if(p[i]->arrival_time > p[j]->arrival_time){
				temp = p[i];
				p[i] = p[j];
				p[j] = temp;
			}
		}
	}
}

int main()
{
	int i,n = 5;
	
	struct processor **p = (struct processor **) malloc(n * sizeof(struct processor *));
	
	for(i=0; i<n; i++){
		p[i] = (struct processor *) malloc(sizeof(struct processor));
		p[i]->arrival_time = n-i;
		p[i]->burst_time = n + 1;
	}
	for(i=0; i<n; i++){
		printf("%d ",p[i]->arrival_time);
	}
	sortStructure(p, n);
	
	for(i=0; i<n; i++){
		printf("%d ",p[i]->arrival_time);
	}
	return 0;
}

/*
author: Elaine Zhao
*/
#include<stdio.h>
#include<stdlib.h>
#define INFINITY 65535
#define MaxVNum 1011
/*----definition of graph---*/
typedef int Vertex;
typedef int WeightType;
typedef struct GNode *graph;
struct GNode
{
	int Nv;//the number of vertexs
	int Ne;//the number of edges
	WeightType dist[MaxVNum][MaxVNum];
	//the distance between v1 and v2;
	Vertex start,desti;
	//start vertex and destination
};
/*-----definition of the functions-----*/ 
graph ReadG(graph G,int n);
//in the next n lines, the user will input the information about the graph, then we build it.
void buildVertex(int *v1,int *v2);
int Search(graph G,int v);
//search for the nearest ambulance.
int findMinDist(graph G,int dist[],int visited[]);
void printVertex(int v);

/*--------variables--------*/
int spot=0, center=0,maxVertexNum=0; 
int num[11];//num[]is used to store the number of ambulances in each center
int dist[1010],number[1010];prev[1010];
int visited[1010];
//dist[]is used to store the currDist;
//number[]is used to store the number of strees
//prev[]is used to store the pre-vertex.

int main()
{
	int i,j, line,numv,v,cen;
	char c;
	graph G;
	G=(graph)malloc(sizeof(struct GNode));
	G->Ne=0;
	G->Nv=0;
	scanf("%d %d",&spot, &center);
	maxVertexNum=spot+center;
	G->Nv=maxVertexNum;
	for(i=1;i<=center;i++)
		scanf("%d",&num[i]);
	scanf("%d",&line);
	G->Ne=line;
	scanf("%c",&c);  //absorb the backspace;
	G=ReadG(G,line); //read in and build the graph
	scanf("%d",&numv);   //the number of pick-up spots
		 
	for(i=0;i<numv;i++)
	{
		scanf("%d",&v);
		cen=Search(G,v);
		if(cen!=-1)
		{
			printf("A-%d",cen-spot);//print the center
			for(j=cen;prev[j]!=v;j=prev[j])
				printVertex(prev[j]);
			printVertex(prev[j]);    //print the path
			printf("\n");
			printf("%d\n",dist[cen]);   //print the length of the path
		}
	}
	return 0;
}

/*---int Search---*/
/*
	function: Searching for the ambulance center which has the shortest path
	algorithm: Using Dijkstra's algorithm
	variables: 
		-graph G: the graph we had built
		-int v:	the index of the pick-up spot
	return value: the index of the decent ambulance center
*/
int Search(graph G,int v)
{
	int i,j,cnt=0,minv;
	for(i=1;i<=G->Nv;i++)//initialization
	{
		if(G->dist[v][i]!=0) 
			dist[i]=G->dist[v][i];
		else dist[i]=INFINITY;
		number[i]=0;
		visited[i]=0;
		prev[i]=0;
	}	
	dist[v]=0;
	visited[v]=1;//the first vertex is visited
	number[v]=1;//the number of streets that the ambulance has to pass.
	for(i=1;i<=G->Nv;i++)
		if(G->dist[v][i])
		{
			number[i]=number[v]+1;
			prev[i]=v;
		}
	while(1)
	{
		i=findMinDist(G,dist,visited); 
	//	prev[i]=v;
	//	printf("minV=%d\n",i);
		if(i==-1)	break;//donnot find the minimum
		visited[i]=1;
		for(j=1;j<=G->Nv;j++)
		{
			if(G->dist[i][j]<INFINITY&&!visited[j])
			{
				if(G->dist[i][j]!=0&&dist[i]+G->dist[i][j]<dist[j])//renew the vertex j
				{
					dist[j]=dist[i]+G->dist[i][j];
					number[j]=number[i]+1;
					prev[j]=i;
				}	
				else if(G->dist[i][j]!=0&&dist[i]+G->dist[i][j]==dist[j]&&number[j]>number[i]+1)
				//the length of the streets are equal,but the numbers of streets are different
				{
					number[j]=number[i]+1;
					prev[j]=i;
				}
			}
		}
		v=i;
	}
	int unbusy=0;
	//output the result
	for(i=1;i<=center;i++)
	{
		if(!num[i])	unbusy++;
		else minv=i+spot;
	}
	if(unbusy==center)
	{
		printf("All Busy\n");
		return -1;
	}
	for(i=spot+1;i<=G->Nv;i++)
	{
		if(dist[i]<dist[minv]&&num[i-spot])
			minv=i;
		else if(dist[i]==dist[minv]&&num[i-spot]&&num[i-spot]>num[minv-spot])
			minv=i;
		else if(dist[i]==dist[minv]&&num[i-spot]&&num[i-spot]==num[minv-spot]&&number[i]<number[minv])
			minv=i;
	}
	if(!num[minv])
	{
		num[minv-spot]--;
		return minv;
	}
	else	return -1;
}

/*---int findMinDist---*/
/*
	function: to find the vertex which has the shortest path and not visited
	variables: 
		-graph G: the graph we had built
		-int dist[]: store the weight of each verge
	return value: the index of the minDist
*/
int findMinDist(graph G,int dist[],int visited[])
{
	int minV,minDist,i;
	minDist=INFINITY;
	for(i=1;i<=G->Nv;i++)
	{
		if(!visited[i]&&dist[i]<minDist)
		{
			minDist=dist[i];
			minV=i;
		}		
	}
	if(minDist<INFINITY)
		return minV;
	else	return -1;//error
}

/*---graph ReadG---*/
/*
	function: read in and build the graph
	variables: 
		-graph G: it's a null graph that we need to put in.
		-int n:	the number of edges we need to read in.
	return value: the graph we've built.
*/
graph ReadG(graph G,int n)
{
	char c;
	int i,w=0,v1=0,v2=0;
	for(i=0;i<n;i++)
	{
		buildVertex(&v1,&v2);
		scanf("%d%c",&w,&c);
		//char c is used to absorb the space
		G->dist[v1][v2]=w;
		G->dist[v2][v1]=w; 
	}
	return G;
}

/*---void buildVertex---*/
/*
	function: to deal with the input edges and their weights
	variables: 
		-int *v1: the first edge
		-int *v2: the second edge
	we use the pointers to return the indexes of the vertexes
*/
void buildVertex(int *v1,int *v2)
{
	int c=0;
	char a;
	scanf("%c",&a);
	if(a=='A')
	{
		scanf("%c%d",&a,&c);
		c=c+spot;
		scanf("%c",&a);//absorb the spacekey
	}
	else if(a>='0'&&a<='9')
	{
		c=a-'0';
		do
		{
			scanf("%c",&a);
			if(a!=' ')
				c=c*10+a-'0';
		}while(a!=' ');
	}
	*v1=c;
	
	c=0;
	scanf("%c",&a);
	if(a=='A')
	{
		scanf("%c%d",&a,&c);
		c=c+spot;
	}
	else if(a>='0'&&a<='9')
	{
		c=a-'0';
		do
		{
			scanf("%c",&a);
			if(a!=' ')
				c=c*10+a-'0';
		}while(a!=' ');
	}
	*v2=c;
}
/*---void printVertex---*/
/*
	function: print the index of vertex in the right form
	variables: 
		-int v: the index of vertex
	we need to judge this vertex is a dispatch center or a pick-up spot.
*/
void printVertex(int v)
{
	if(v>spot)
		printf(" A-%d",v-spot);
	else
		printf(" %d",v);
}

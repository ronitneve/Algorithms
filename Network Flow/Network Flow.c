// Ronit Neve
// 1001856292
// References
// http://www.aduni.org/courses/algorithms/courseware/handouts/Reciation_09.html
// http://ranger.uta.edu/~weems/NOTES5311/ff.c

#include <stdio.h>
#include <stdlib.h>
#define WHITE 0
#define GRAY 1
#define BLACK 2
#define oo 1000000000

void minCut(int numOfEdges);
int n;  // number of nodes
int residualEdges;  // number of edges in residual network
struct edge {
  int tail;
  int head;
  int capacity;
  int flow;
  int inverse; 
};

typedef struct edge edgeType;
edgeType *edgeTab;

int *headEdge;  // Table indicating first in range of edges with a common tail
int *flag;      // Needed for breadth-first search               
int *augPath;       // Array to store augmenting path
int *predEdge;   // edgeTab subscript of edge used to reach vertex i in BFS


int min (int x, int y)
{
return x<y ? x : y;  // returns minimum of x and y
}

// Queue for breadth-first search
int head,tail;
int *q;

void enqueue (int x)
{
	q[tail] = x;
	tail++;
	flag[x] = GRAY;
}

int dequeue ()
{
	int x = q[head];
	head++;
	flag[x] = BLACK;
	return x;
}

// Breadth-First Search for an augmenting path

int BFS (int start, int target)
// Searches for path from start to target.
// Returns 1 if found, otherwise 0.
{
	int u,v,i;

	for (u=0; u<n; u++)
	  flag[u] = WHITE;
	head = tail = 0;  // Since q is not circular, it is reinitialized for each BFS
	enqueue(start);
	augPath[start] = -1;
	while (head!=tail)
	{
	  u=dequeue();
	  if (u==target)
	    return 1;

	  // Search all adjacent white nodes v. If the residual capacity
	  // from u to v in the residual network is positive,
	  // enqueue v.
	  for (i=headEdge[u]; i<headEdge[u+1]; i++)
	  {
	    v=edgeTab[i].head;
	    if (flag[v]==WHITE && edgeTab[i].capacity-edgeTab[i].flow>0)
	    {
	      enqueue(v);
	      augPath[v] = u;
	      predEdge[v]=i;
	    }
	  }
	}
	// No augmenting path remains, so a maximum flow and minimum cut
	// have been found.  Black vertices are in the
	// source side (S) of the minimum cut, while white vertices are in the
	// sink side (T).

	return 0;
}

// Ford-Fulkerson Algorithm
int max_flow (int source, int sink)
{
	int i,j,u;
	int max_flow;
	int APcount=0;

	flag=(int*) malloc(n*sizeof(int));
	augPath=(int*) malloc(n*sizeof(int));
	predEdge=(int*) malloc(n*sizeof(int));
	q=(int*) malloc(n*sizeof(int));
	if (!flag || !augPath || !predEdge || !q)
	{
	  printf("malloc failed %d\n",__LINE__);
	  exit(0);
	}

	// Initialize empty flow.
	max_flow = 0;
	for (i=0; i<residualEdges; i++)
	  edgeTab[i].flow=0;

	// While there exists an augmenting path,
	// increment the flow along this path.
	while (BFS(source,sink))
	{
	  // Determine the amount by which we can increment the flow.
	  int increment = oo;
	  APcount++;
	  for (u=sink; augPath[u]!=(-1); u=augPath[u])
	  {
	    i=predEdge[u];
	    increment = min(increment,edgeTab[i].capacity-edgeTab[i].flow);
	  }
	  // Now increment the flow.
	  for (u=sink; augPath[u]!=(-1); u=augPath[u])
	  {
	    i = edgeTab[predEdge[u]].inverse;
	    edgeTab[predEdge[u]].flow += increment;
	    edgeTab[i].flow -= increment;  // Reverse in residual
	  }
	  max_flow += increment;
	}
	// No more augmenting paths, so cut is based on reachability from last BFS.
	return max_flow;
}

// Reading the input file and organize adjacency lists for residual network.

int TailHead(const void* xin, const void* yin)
// Used in calls to qsort() and bsearch() for read_input_file()
{
	int result;
	edgeType *x,*y;

	x=(edgeType*) xin;
	y=(edgeType*) yin;
	result=x->tail - y->tail;
	if (result!=0)
	  return result;
	else
	  return x->head - y->head;
	}


void getInput()
{
	int tail,head,capacity,i,j;
	int inputEdges;     // Number of edges in input file.
	int workingEdges;   // Number of residual network edges initially 
	                    // generated from input file.
	edgeType work;
	edgeType *ptr;

	// read number of nodes and edges
	scanf("%d %d",&n,&inputEdges);
	// Table is allocated at worst-case size, since space for inverses is needed.
	edgeTab=(edgeType*) malloc(2*inputEdges*sizeof(edgeType));
	if (!edgeTab)
	{
	  printf("edgeTab malloc failed %d\n",__LINE__);
	  exit(0);
	}
	// read edges, each with a capacity
	workingEdges=0;
	for (i=0; i<inputEdges; i++)
	{
	  scanf("%d %d %d",&tail,&head,&capacity);
	  // Test for illegal edge, including incoming to source and outgoing from
	  // sink.
	  if (tail<0 || tail>=n || head<0 || head>=n || capacity<0)
	  {
	    printf("Invalid input %d %d %d at %d\n",tail,head,capacity,__LINE__);
	    exit(0);
	  }
	  // Save input edge
	  edgeTab[workingEdges].tail=tail;
	  edgeTab[workingEdges].head=head;
	  edgeTab[workingEdges].capacity=capacity;
	  workingEdges++;
	  // Save inverse of input edge
	  edgeTab[workingEdges].tail=head;
	  edgeTab[workingEdges].head=tail;
	  edgeTab[workingEdges].capacity=capacity;
	  workingEdges++;
	}
	// Sort edges to make edges with common tail contiguous in edgeTab,
	// along with making parallel edges contiguous.
	qsort(edgeTab,workingEdges,sizeof(edgeType),TailHead);


	// Coalesce parallel edges into a single edge by adding their capacities.
	for (i=1; i<workingEdges; i++)
	  if (edgeTab[residualEdges].tail==edgeTab[i].tail
	  &&  edgeTab[residualEdges].head==edgeTab[i].head)
	    edgeTab[residualEdges].capacity+=edgeTab[i].capacity;  // || case
	  else
	  {
	    residualEdges++;
	    edgeTab[residualEdges].tail=edgeTab[i].tail;
	    edgeTab[residualEdges].head=edgeTab[i].head;
	    edgeTab[residualEdges].capacity=edgeTab[i].capacity;
	  }
	residualEdges++;

	// Set field in each edgeTab struct to point to inverse
	for (i=0; i<residualEdges; i++)
	{
	  work.tail=edgeTab[i].head;
	  work.head=edgeTab[i].tail;
	  ptr=(edgeType*) bsearch(&work,edgeTab,residualEdges,sizeof(edgeType),
	    TailHead);
	  if (ptr==NULL)
	  {
	    printf("bsearch %d failed %d\n",i,__LINE__);
	    exit(0);
	  }
	  edgeTab[i].inverse=ptr-edgeTab;  // ptr arithmetic to get subscript
	}
	// For each vertex i, determine first edge in edgeTab with
	headEdge=(int*) malloc((n+1)*sizeof(int));
	if (!headEdge)
	{
	  printf("malloc failed %d\n",__LINE__);
	  exit(0);
	}
	j=0;
	for (i=0; i<n; i++)
	{
	  headEdge[i]=j;
	  // Skip over edges with vertex i as their tail.
	  for ( ;
	       j<residualEdges && edgeTab[j].tail==i;
	       j++)
	    ;
	}
	headEdge[n]=residualEdges;  //Sentinel
	minCut(n);
}

//Calculate the cut graph
void minCut(int numOfEdges)
{
	int i, j, s, p[n], t, f1[n], minimumCut;
	int ** tree = (int **) malloc(n * sizeof(int*));
	for (i = 0; i < n; i++)
	{
		p[i] = 0;
		f1[i] = 0;
		tree[i] = (int*)malloc(n*sizeof(int));
		for (j = 0; j < n; j++)
		{
			tree[i][j] = 0;
		}
	}

	for (s = 1; s < n; s++)
	{
		//Initialize sink
		t = p[s];
		//Calculate the minimum cut
		minimumCut = max_flow(s, t);
		f1[s] = minimumCut;
		//Mark the nodes in the supernode with its representative
		for (i = 0; i < n; i++)
		{
			if (i != s && p[i] == t && flag[i] == BLACK)
			{
				p[i] = s;
			}
		}

		//Change the label
		if (flag[p[t]] == BLACK)
		{
			p[s] = p[t];
			p[t] = s;
			f1[s] = f1[t];
			f1[t] = minimumCut;
		}
		//Store the final cut tree when s is the last node of the input graph.
		if (s == n - 1)
		{
			for (i = 1; i <= s; i++)
			{
				tree[i][p[i]] = f1[i];
			}
		}
	}
	//Print the Cut Tree
	printf("\n");
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (tree[i][j] > 0)
				printf("%d %d %d\n", i, j, tree[i][j]);
	free(flag);
	free(augPath);
	free(predEdge);
	free(q);
}

int main ()
{
	int i,j;
	getInput();
	free(edgeTab);
	free(headEdge);
}
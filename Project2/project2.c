/*
author: Elaine Zhao
*/
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
int pre[105], in[105], post[105],level[105];
int lc[105], rc[105], root, n;
int f[105];//0-not visited,1-visited
void quit();
void bfs(int root);
int solve(int L1, int R1, int L2, int R2, int L3, int R3);
int main() 
{
	int i;
    scanf("%d",&n);
    for (i = 1; i <= n; i++)
    {
    	scanf("%d",&in[i]);
		f[in[i]] = 1;
	}
    for (i = 1; i <= n; i++)
    {
    	scanf("%d",&pre[i]);
		f[pre[i]] = 1;
	} 
    for (i = 1; i <= n; i++)
    {
    	scanf("%d",&post[i]);
		f[post[i]] = 1;
	}
	
    root = solve(1, n, 1, n, 1, n);
    bfs(root);
    
    //print in-order
    printf("%d",in[1]);
    for(i=2;i<=n;i++)
    	printf(" %d",in[i]);
    printf("\n");
    //pre-order
    printf("%d",pre[1]);
    for(i=2;i<=n;i++)
    	printf(" %d",pre[i]);
    printf("\n");
    //post-order
    printf("%d",post[1]);
    for(i=2;i<=n;i++)
    	printf(" %d",post[i]);
    printf("\n");
    //lever-order 
	printf("%d",level[0]);
    for(i=1;i<n;i++)
    	printf(" %d",level[i]);
    return 0;
}
void quit() 
{
    printf("Impossible");
    exit(0);
}
void bfs(int root)
{
    int head = -1, tail = 0;
	//head represent the number of nodes we have asserted,tail represent the present one.
    level[0]=root;
	//the first node when we traverse in level-order is the root.
    while(head!=tail)
    {
        head++;
        //has a left child
		if (lc[level[head]])
		{
            tail++;
			level[tail]=lc[level[head]];
		}
		//has a right child
		if (rc[level[head]])
		{
            tail++;
			level[tail]=rc[level[head]];
		} 
    }
}
int solve(int L1, int R1, int L2, int R2, int L3, int R3)
{
	int i,j,k,tmp,size,root,p;
	int cnt=0, cnt1 = 0, cnt2 = 0;
	int fl = 0, flag = 0;
    if (L1 > R1) return 0;
    //have traversaled in-order
    if (L1 == R1)
	{
        tmp = in[L1] | pre[L2] | post[L3];
        if (!tmp) 
		{
            for (i = n; i >= 1; i--)
                if (!f[i]) 
				{
                    tmp = i;
                    f[i] = 1;
                    break;
                }
        }
        in[L1] = pre[L2] = post[L3] = tmp;
        return in[L1];
    }
    //impossible case: the last node of post-order doesn't equals to the first node of pre-order
    if (pre[L2] && post[R3] && pre[L2]!=post[R3]) 
		quit();
    //not both of the first node of pre-order and the last node of post-order are 0
    if (!pre[L2]||!post[R3])
        pre[L2] = post[R3] = pre[L2] + post[R3];
        //let pre[L2] and post[R3] contains the same node
    //we can't find the node
    if (!pre[L2]) 
		quit();
    root = pre[L2];
	p = L1;
	cnt = 0;
    for (i = L1; i <= R1; i++)
    {
    	if(in[i] == 0)
    		cnt++;
    	//count the nodes missing in in-order[L1,R1].
		if (root == in[i]) 
		{
            flag = 1;
    		p=i;
        }
    }
    //cant find the root and only one node missing, then it's the root.
    if(!flag && cnt==1)
    	for(i=L1;i<=R1;i++)
    		if(!in[i])	
			{
				in[i]=root;
				p = i;
				flag=1;
			}
    if(flag)	size = p - L1;
    else
	{
        for (p = L1 + 1; p < R1; p++)
        	if (in[p] == 0) 
			{
               size = p - L1;
               //we need to judge which one is the root.
                //pre[L2 + 1 ~ L2 + size]   root,L2+1,L2+2��R2 
                //post[L3 ~ L3 + size - 1]  L3,L3+1,L3+2��R3 
                int cnt1 = 0, cnt2 = 0;
                for (i = L2 + 1; i <= L2 + size; i++)
                    if (!pre[i]) cnt1++;//the number of 0 in pre-order 
                for (i = L3; i <= L3 + size - 1; i++)
                    if (!post[i]) cnt2++;//the number of 0 in post-order
                for (i = L2 + 1; i <= L2 + size; i++)
                    if (pre[i]) 
					{
                        fl = 0;
                        for (j = L3; j <= L3 + size - 1; j++)
                            if (pre[i] == post[j]) fl = 1;
                        if (!fl) cnt2--;
                    }
                for (i = L3; i <= L3 + size - 1; i++)
                    if (post[i]) 
					{
                        fl = 0;
                        for (j = L2 + 1; j <= L2 + size; j++)
                            if (post[i] == pre[j]) fl = 1;
                        if (!fl) cnt1--;
                    }
                if (cnt1 >= 0&&cnt2 >= 0) break;
                //the number is all missing in pre-order and post-order -> it's the root.
        	}
    }
    pre[L2] = root;
    post[R3] = root;
    in[p] = pre[L2];
	size = p - L1;
    lc[root] = solve(L1, p - 1, L2 + 1, L2 + size, L3, L3 + size - 1);//searching the left tree
    rc[root] = solve(p + 1, R1, L2 + size + 1, R2, L3 + size, R3 - 1);//searching the right tree
    return root;
}

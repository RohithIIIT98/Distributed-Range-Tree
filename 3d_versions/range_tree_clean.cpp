#include<bits/stdc++.h>
using namespace std;

struct values{
	int t;
	int x;
	int y;
};

bool compY(values a, values b)
{
	return a.y<b.y;
}

bool compX(values a, values b)
{
	return a.x<b.x;
}

bool CompT(values a, values b)
{
	return a.t<b.t;
}

struct RTY
{
	int yval;
	RTY* left;
	RTY* right;
	int size;
	int ycount;
};

struct RTX
{
	int xval;
	RTX* left;
	RTX* right;
	RTY* assoc;
};

struct RTT{
	int tval;
	RTT *left;
	RTT *right;
	RTX *assoc;
};

RTY* BuildRangeTreeY(vector<values>&points,int l, int r)
{		
	if(l>r) return NULL;

	int mid = l+(r-l)/2;
	int left,right;

	int y =points[mid].y;
	left = mid;
	right = mid;
		
	while(left>=l && y==points[left].y) left--;
	while(right<=r && y==points[right].y) right++;

	left++;
	right--;

	RTY* root = new RTY();
	root->yval = y;
	root->ycount = right-left+1;
	root->size = r-l+1;
	root->left = BuildRangeTreeY(points,l,left-1);
	root->right = BuildRangeTreeY(points,right+1,r);

	return root;
} 

RTX* BuildRangeTreeX(vector<values>points,int l, int r)
{
	if(l>r) return NULL;

	int mid = l+(r-l)/2;
	int left,right;

	int x =points[mid].x;
	left = mid;
	right = mid;
		
	while(left>=l && x==points[left].x) left--;
	while(right<=r && x==points[right].x) right++;

	left++;
	right--;

	RTX* root = new RTX(); 
	root->xval = x;
	root->left = BuildRangeTreeX(points,l,left-1);
	root->right = BuildRangeTreeX(points,right+1,r);

	sort(points.begin()+l,points.begin()+r+1,compY);
	root->assoc = BuildRangeTreeY(points,l,r);

	return root;
}


RTT* BuildRangeTreeT(vector<values>points,int l, int r)
{
	if(l>r) return NULL;

	int mid = l+(r-l)/2;
	int left,right;

	int t =points[mid].t;
	left = mid;
	right = mid;
		
	while(left>=l && t==points[left].t) left--;
	while(right<=r && t==points[right].t) right++;

	left++;
	right--;

	RTT* root = new RTT(); 
	root->tval = t;
	root->left = BuildRangeTreeT(points,l,left-1);
	root->right = BuildRangeTreeT(points,right+1,r);

	sort(points.begin()+l,points.begin()+r+1,compX);
	root->assoc = BuildRangeTreeX(points,l,r);

	return root;
}

int countY(RTY* root, int ylower, int yupper)
{	
    if(!root)
        return 0;
	if(root->yval>ylower && root->yval>yupper)
		return countY(root->left,ylower,yupper);

	else if(root->yval<ylower && root->yval<yupper)
		return countY(root->right,ylower,yupper);

	else
	{
		
		/*
			Cases that can happen here are
			1. Lower node is ancestor of upper node
			2. Upper node is ancestor of lower node
			3. Both have no ancestor decendant relation and present on different subtrees

			Note: Lower node here mean the closest(value) node >= the xlower and upper node 
			mean the closest(value) node  <= xupper

		*/

		int res=0;
		RTY* helper;

		if(root->yval!= ylower)
		{
			/*
				For case 2&3 
				binary search for lower_bound of lower
			*/
			helper = root->left;
			while(helper)
			{
				if(helper->yval > ylower)
				{
					res+=helper->ycount;
					if(helper->right)
						res+=helper->right->size;

					helper = helper->left;
				}
				else if(helper->yval < ylower)
					helper = helper->right;
				else
				{
					res+=helper->ycount;
					if(helper->right)
						res+=helper->right->size;
					break;
				}
			}
		}

		if(root->yval!= yupper)
		{
			/*
				For case 1&3 
				binary search for lower_bound of lower
			*/
			helper = root->right;
			while(helper)
			{
				if(helper->yval > yupper)
					helper = helper->left;
				else if(helper->yval < yupper)
				{
					res+=helper->ycount;
					if(helper->left)
						res+=helper->left->size;

					helper = helper->right;
				}
				else
				{
					res+=helper->ycount;
					if(helper->left)
						res+=helper->left->size;
					break;
				}
			}
		}

		/*
			For case 1,2,3 add no of times split node appeared 
		*/

		res+=root->ycount;
		return res;
	}
}

int OrthogonalRangeSearch(RTX* root, int xlower, int xupper, int ylower, int yupper)
{
	if(!root)
		return 0;

	if(root->xval>xlower && root->xval>xupper)
		return OrthogonalRangeSearch(root->left,xlower,xupper,ylower,yupper);

	else if(root->xval<xlower && root->xval<xupper)
		return OrthogonalRangeSearch(root->right,xlower,xupper,ylower,yupper);
	else
	{
		/*
			Cases that can happen here are
			1. Lower node is ancestor of upper node
			2. Upper node is ancestor of lower node
			3. Both have no ancestor decendant relation and present on different subtrees

			Note: Lower node here mean the closest(value) node >= the xlower and upper node 
			mean the closest(value) node  <= xupper
		*/
            
		int ans=0;
		RTX* helper;
		
		if(root->xval != xlower)
		{
			cout<<"getting into xlower part"<<endl;	
			/*
				For case 2&3 
				binary search for lower_bound of lower
			*/
		
			helper = root->left;
			while(helper)
			{
				if(helper->xval > xlower)
				{
					int temp1=countY(helper->assoc,ylower,yupper);
					cout<<"adding "<<helper->xval<<" "<<temp1<<endl;
					ans += temp1;
					
					if(helper->left){
						int temp2=countY(helper->left->assoc,ylower,yupper);
						cout<<"subtracting "<<helper->left->xval<<" "<<temp2<<endl;
						ans -= temp2;
					}
					
					helper = helper->left;
				}
				else if(helper->xval < xlower)
					helper = helper->right;
				else
				{
					// ans += countY(helper->assoc,ylower,yupper);

					// if(helper->left)
					// 	ans -= countY(helper->left->assoc,ylower,yupper);
					int temp1=countY(helper->assoc,ylower,yupper);
					cout<<"adding "<<helper->xval<<" "<<temp1<<endl;
					ans += temp1;
					
					if(helper->left){
						int temp2=countY(helper->left->assoc,ylower,yupper);
						cout<<"subtracting "<<helper->left->xval<<" "<<temp2<<endl;
						ans -= temp2;
					}
					
					break;
				}
			}
		}

		if(root->xval != xupper)
		{
		    cout<<"getting into xupper part"<<endl;	
			/*
				For case 1&3 
				binary search for closest(value) element <= upper
			*/
			helper = root->right;

			while(helper)
			{
				
				if(helper->xval > xupper)
					helper = helper->left;
				else if(helper->xval < xupper)
				{
					int temp2=countY(helper->assoc,ylower,yupper);
					ans += temp2;
					cout<<"adding "<<helper->xval<<" "<<temp2<<endl;

					if(helper->right){
						int temp1;
						temp1=countY(helper->right->assoc,ylower,yupper);
						ans -= temp1;
						cout<<"subtracting "<<helper->right->xval<<" "<<temp1<<endl;
					}

					helper = helper->right;
				}
				else
				{	
					// ans += countY(helper->assoc,ylower,yupper);

					// if(helper->right)
					// 	ans -= countY(helper->right->assoc,ylower,yupper);
					int temp2=countY(helper->assoc,ylower,yupper);
					ans += temp2;
					cout<<"adding "<<helper->xval<<" "<<temp2<<endl;

					if(helper->right){
						int temp1;
						temp1=countY(helper->right->assoc,ylower,yupper);
						ans -= temp1;
						cout<<"subtracting "<<helper->right->xval<<" "<<temp1<<endl;
					}

					break;
				}
				
			}
			
		}
		
		/*
			For case 1,2,3 add the y's in the given range that corresponds to split node 
		*/

		ans+= countY(root->assoc,ylower,yupper);
		cout<<"adding "<<root->xval<<" "<<countY(root->assoc,ylower,yupper)<<endl;

		if(root->left){
			ans-=countY(root->left->assoc,ylower,yupper);
			cout<<"subtracting "<<root->left->xval<<" "<<countY(root->left->assoc,ylower,yupper)<<endl;
		}
		if(root->right){
			ans-=countY(root->right->assoc,ylower,yupper);
			cout<<"subtracting "<<root->right->xval<<" "<<countY(root->right->assoc,ylower,yupper)<<endl;
		}

		return ans;
	}

}


int OrthogonalRangeSearch3d(RTT* root, int tlower, int tupper, int xlower, int xupper,int ylower,int yupper)
{
	if(!root)
		return 0;

	if(root->tval>tlower && root->tval>tupper)
		return OrthogonalRangeSearch3d(root->left,tlower,tupper,xlower,xupper,ylower,yupper);

	else if(root->tval<tlower && root->tval<tupper)
		return OrthogonalRangeSearch3d(root->right,tlower,tupper,xlower,xupper,ylower,yupper);
	else
	{
		/*
			Cases that can happen here are
			1. Lower node is ancestor of upper node
			2. Upper node is ancestor of lower node
			3. Both have no ancestor decendant relation and present on different subtrees

			Note: Lower node here mean the closest(value) node >= the xlower and upper node 
			mean the closest(value) node  <= xupper
		*/
            
		int ans=0;
		RTT* helper;
		
		if(root->tval != tlower)
		{
			cout<<"\ngetting into tlower part\n"<<endl;	
			/*
				For case 2&3 
				binary search for lower_bound of lower
			*/
		
			helper = root->left;
			while(helper)
			{
				if(helper->tval > tlower)
				{
					int temp1=OrthogonalRangeSearch(helper->assoc,xlower,xupper,ylower,yupper);
					cout<<"adding "<<helper->tval<<" "<<temp1<<endl;
					ans += temp1;
					
					if(helper->left){
						int temp2=OrthogonalRangeSearch(helper->left->assoc,xlower,xupper,ylower,yupper);
						cout<<"subtracting "<<helper->left->tval<<" "<<temp2<<endl;
						ans -= temp2;
					}
					
					helper = helper->left;
				}
				else if(helper->tval < tlower)
					helper = helper->right;
				else
				{
					// ans += countY(helper->assoc,ylower,yupper);

					// if(helper->left)
					// 	ans -= countY(helper->left->assoc,ylower,yupper);
					int temp1=OrthogonalRangeSearch(helper->assoc,xlower,xupper,ylower,yupper);
					cout<<"adding "<<helper->tval<<" "<<temp1<<endl;
					ans += temp1;
					
					if(helper->left){
						int temp2=OrthogonalRangeSearch(helper->left->assoc,xlower,xupper,ylower,yupper);
						cout<<"subtracting "<<helper->left->tval<<" "<<temp2<<endl;
						ans -= temp2;
					}
					
					break;
				}
			}
		}

		if(root->tval != tupper)
		{
		    cout<<"\ngetting into tupper part\n"<<endl;	
			/*
				For case 1&3 
				binary search for closest(value) element <= upper
			*/
			helper = root->right;

			while(helper)
			{
				
				if(helper->tval > tupper)
					helper = helper->left;
				else if(helper->tval < tupper)
				{
					int temp2=OrthogonalRangeSearch(helper->assoc,xlower,xupper,ylower,yupper);
					ans += temp2;
					cout<<"adding "<<helper->tval<<" "<<temp2<<endl;

					if(helper->right){
						int temp1;
						temp1=OrthogonalRangeSearch(helper->right->assoc,xlower,xupper,ylower,yupper);
						ans -= temp1;
						cout<<"subtracting "<<helper->right->tval<<" "<<temp1<<endl;
					}

					helper = helper->right;
				}
				else
				{	
					// ans += countY(helper->assoc,ylower,yupper);

					// if(helper->right)
					// 	ans -= countY(helper->right->assoc,ylower,yupper);
					int temp2=OrthogonalRangeSearch(helper->assoc,xlower,xupper,ylower,yupper);
					ans += temp2;
					cout<<"adding "<<helper->tval<<" "<<temp2<<endl;

					if(helper->right){
						int temp1;
						temp1=OrthogonalRangeSearch(helper->right->assoc,xlower,xupper,ylower,yupper);
						ans -= temp1;
						cout<<"subtracting "<<helper->right->tval<<" "<<temp1<<endl;
					}

					break;
				}
				
			}
			
		}
		
		/*
			For case 1,2,3 add the y's in the given range that corresponds to split node 
		*/

		ans+= OrthogonalRangeSearch(root->assoc,xlower,xupper,ylower,yupper);
		cout<<"adding "<<root->tval<<" "<<OrthogonalRangeSearch(root->assoc,xlower,xupper,ylower,yupper)<<endl;

		if(root->left){
			ans-=OrthogonalRangeSearch(root->left->assoc,xlower,xupper,ylower,yupper);
			cout<<"subtracting "<<root->left->tval<<" "<<OrthogonalRangeSearch(root->left->assoc,xlower,xupper,ylower,yupper)<<endl;
		}
		if(root->right){
			ans-=OrthogonalRangeSearch(root->right->assoc,xlower,xupper,ylower,yupper);
			cout<<"subtracting "<<root->right->tval<<" "<<OrthogonalRangeSearch(root->right->assoc,xlower,xupper,ylower,yupper)<<endl;
		}

		return ans;
	}

}



int main()
{
	vector<values>test;

	// int points;
	// cin>>points;

	// while(points--){
	// 	values temp;
	// 	int t,x,y;
	// 	cin>>t;
	// 	cin>>x;
	// 	cin>>y;
	// 	temp.t=t;
	// 	temp.x=x;
	// 	temp.y=y;
	// 	test.push_back(temp);
	// }
	test.push_back({2,3,1});
	test.push_back({3,4,2});
	test.push_back({3,5,4});
	test.push_back({4,6,5});
	test.push_back({4,7,6});
	test.push_back({5,6,1});
	test.push_back({5,6,2});
	test.push_back({5,8,7});
	test.push_back({5,9,8});
	test.push_back({6,9,9});
	test.push_back({7,9,9});
	test.push_back({7,9,9});
	test.push_back({12,13,10});
	test.push_back({13,14,10});
	test.push_back({14,15,11});

	sort(test.begin(),test.end(),CompT);
	int n = test.size();
	RTT* root = BuildRangeTreeT(test,0,n-1);
	int ans=OrthogonalRangeSearch3d(root,2,5,3,6,1,2);
	cout<<"\nPoints inside box: "<<ans<<endl;
	cout<<endl;

	return 0;
}
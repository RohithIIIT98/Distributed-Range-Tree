#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

#define prime 9973


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
					ans += temp1;
					
					if(helper->left){
						int temp2=countY(helper->left->assoc,ylower,yupper);
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
					ans += temp1;
					
					if(helper->left){
						int temp2=countY(helper->left->assoc,ylower,yupper);
						ans -= temp2;
					}
					
					break;
				}
			}
		}

		if(root->xval != xupper)
		{
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

					if(helper->right){
						int temp1;
						temp1=countY(helper->right->assoc,ylower,yupper);
						ans -= temp1;
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

					if(helper->right){
						int temp1;
						temp1=countY(helper->right->assoc,ylower,yupper);
						ans -= temp1;
					}

					break;
				}
				
			}
			
		}
		
		/*
			For case 1,2,3 add the y's in the given range that corresponds to split node 
		*/

		ans+= countY(root->assoc,ylower,yupper);

		if(root->left){
			ans-=countY(root->left->assoc,ylower,yupper);
		}
		if(root->right){
			ans-=countY(root->right->assoc,ylower,yupper);
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
					ans += temp1;
					
					if(helper->left){
						int temp2=OrthogonalRangeSearch(helper->left->assoc,xlower,xupper,ylower,yupper);
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
					ans += temp1;
					
					if(helper->left){
						int temp2=OrthogonalRangeSearch(helper->left->assoc,xlower,xupper,ylower,yupper);
						ans -= temp2;
					}
					
					break;
				}
			}
		}

		if(root->tval != tupper)
		{
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

					if(helper->right){
						int temp1;
						temp1=OrthogonalRangeSearch(helper->right->assoc,xlower,xupper,ylower,yupper);
						ans -= temp1;
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

					if(helper->right){
						int temp1;
						temp1=OrthogonalRangeSearch(helper->right->assoc,xlower,xupper,ylower,yupper);
						ans -= temp1;
					}

					break;
				}
				
			}
			
		}
		
		/*
			For case 1,2,3 add the y's in the given range that corresponds to split node 
		*/

		ans+= OrthogonalRangeSearch(root->assoc,xlower,xupper,ylower,yupper);

		if(root->left){
			ans-=OrthogonalRangeSearch(root->left->assoc,xlower,xupper,ylower,yupper);
		}
		if(root->right){
			ans-=OrthogonalRangeSearch(root->right->assoc,xlower,xupper,ylower,yupper);
		}

		return ans;
	}

}
class consistentHashing
{
    size_t *processor_hash;
    int proc;
    
    public:
    consistentHashing(int np)
    {
        proc = np;
        processor_hash = new size_t[np];
        int cs = prime/np;
		processor_hash[0]=0;
        for(int i=1;i<np;i++)
            processor_hash[i] = i*cs;
    }
    int get_processor(values pt)
    {
        string x = to_string(pt.x)+to_string(pt.y)+to_string(pt.t);
        hash<std::string> hash_function;
        size_t hash_val = hash_function(x)%prime;
        int processor = upper_bound(processor_hash,processor_hash+proc, hash_val) - processor_hash;
        return processor!=0?processor-1:0;
    }
};

int main(int argc,char *argv[])
{
    int size;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    MPI_Datatype point_type;
    int lengths[3] = { 1, 1, 1 };
    const MPI_Aint displacements[3] = { 0, sizeof(int), 2*sizeof(int) };
    MPI_Datatype types[3] = { MPI_INT, MPI_INT, MPI_INT };
    MPI_Type_create_struct(3, lengths, displacements, types, &point_type);
    MPI_Type_commit(&point_type);

    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    values *points = NULL;
    int siz[size];
    int disp[size];

    if(my_rank==0)
    {
    
	vector<values>test;

    	test.push_back({-3,-456,7});
    	test.push_back({-6,100,1122});
    	test.push_back({4,5,23});
    	test.push_back({5,6,7});
    	test.push_back({-5,3,8});
    	test.push_back({-6,97,865});
    	test.push_back({34,67,654});
    	test.push_back({-2,3,4});
    	test.push_back({-7,6,5});
    	test.push_back({-2,-2,-2});


//        vector<values>test;
//        test.push_back({2,3,1});
//        test.push_back({3,4,2});
//        test.push_back({3,5,4});
//        test.push_back({4,6,5});
//        test.push_back({4,7,6});
//        test.push_back({5,6,1});
//        test.push_back({5,6,2});
//        test.push_back({5,8,7});
//        test.push_back({5,9,8});
//        test.push_back({6,9,9});
//        test.push_back({7,9,9});
//        test.push_back({7,9,9});
//        test.push_back({12,13,10});
//        test.push_back({13,14,10});
//        test.push_back({14,15,11});

        sort(test.begin(),test.end(),CompT);        
        unordered_map<int,vector<values>> proc_details;
        consistentHashing ch(size);
        for(int t = 0; t<test.size();t++)
        {
                int proc_id = ch.get_processor(test[t]);
		cout<<proc_id<<" ";
		proc_details[proc_id].push_back(test[t]);
        }
	cout<<endl;

        points = new values[test.size()];
        int k=0;
        for(int x=0;x<size;x++)
        {
            for(int j=0;j<proc_details[x].size();j++)
            {
                points[k++] = proc_details[x][j];
            } 
            siz[x] =  proc_details[x].size();
        }

	disp[0]=0;
        for(int x=1;x<size;x++)
        {
            disp[x] =  disp[x-1]+siz[x-1];
        }
    }

    MPI_Bcast(siz,size, MPI_INT,0, MPI_COMM_WORLD);
    MPI_Bcast(disp,size, MPI_INT,0, MPI_COMM_WORLD);

    values rec_points[siz[my_rank]];
    MPI_Scatterv(points, siz, disp, point_type,rec_points,siz[my_rank],point_type,0, MPI_COMM_WORLD);

    vector<values>vec_point;
    for(int i=0;i<siz[my_rank];i++)
    {
        vec_point.push_back(rec_points[i]);
    }

    int n = vec_point.size();
    RTT* root = BuildRangeTreeT(vec_point,0,n-1);
    int ans = OrthogonalRangeSearch3d(root,-10,10,0,10000,0,10000);
	
    int reduction_result = 0;
    MPI_Reduce(&ans, &reduction_result, 1, MPI_INT, MPI_SUM,0, MPI_COMM_WORLD);

    if(my_rank==0)
    {
        cout<<reduction_result;
    }
    
    MPI_Finalize();
    return 0;
}



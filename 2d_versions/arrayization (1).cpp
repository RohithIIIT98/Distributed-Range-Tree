#include<bits/stdc++.h>
using namespace std;

vector<pair<int,int>> coords;
vector<int> x_array;
vector<vector<int>> y_array,count_ele;


void construct_y(int fr,int rr,int outer_ind,vector<int>&sorted_y,int inner_ind)
{

if(y_array.size()<outer_ind+1)
{
	y_array.resize(outer_ind+1);
	count_ele.resize(outer_ind+1);
}

if(y_array[outer_ind].size()<inner_ind+1)
{
	y_array[outer_ind].resize(inner_ind+1);
	count_ele[outer_ind].resize(inner_ind+1);
}

if(rr<fr)
{
	y_array[outer_ind][inner_ind]=INT_MIN;
	count_ele[outer_ind][inner_ind]=0;
	return;
}

int mid=fr+(rr-fr)/2;

int new_rr=mid-1,new_fr=mid+1;

while(new_rr>=fr and sorted_y[mid]==sorted_y[new_rr])
	new_rr--;
while(new_fr<=rr and sorted_y[mid]==sorted_y[new_fr])
	new_fr++;

y_array[outer_ind][inner_ind]=sorted_y[mid];
count_ele[outer_ind][inner_ind]=rr-fr+1;

construct_y(fr,new_rr,outer_ind,sorted_y,inner_ind*2+1);
construct_y(new_fr,rr,outer_ind,sorted_y,inner_ind*2+2);

}


void construct_x(int fr,int rr,int cur_index)
{

if(x_array.size()<cur_index+1)
x_array.resize(cur_index+1);

if(rr<fr)
{
	x_array[cur_index]=INT_MIN;
	return;
}

int mid=fr+(rr-fr)/2;


x_array[cur_index]=coords[mid].first;

int new_rr=mid-1,new_fr=mid+1;

while(new_rr>=fr and coords[new_rr].first==coords[mid].first)
	new_rr--;

while(new_fr<=rr and coords[new_fr].first==coords[mid].first)
	new_fr++;

vector<int> sorted_y;
int temp=fr;

while(temp<=rr)
	sorted_y.push_back(coords[temp++].second);

sort(sorted_y.begin(),sorted_y.end());

construct_y(0,rr-fr,cur_index,sorted_y,0);

construct_x(fr,new_rr,cur_index*2+1);
construct_x(new_fr,rr,cur_index*2+2);
}

int range_ysearch(int outer_ind,int min_y,int max_y,int inner_ind)
{
if(y_array[outer_ind][inner_ind]==INT_MIN)
	return 0;
if(y_array[outer_ind][inner_ind]<min_y)
	return range_ysearch(outer_ind,min_y,max_y,2*inner_ind+2);

if(y_array[outer_ind][inner_ind]>max_y)
	return range_ysearch(outer_ind,min_y,max_y,2*inner_ind+1);

int temp=inner_ind;
int temp_res=0;

if(y_array[outer_ind][temp]!=min_y)
{
	temp=2*inner_ind+1;
	while(y_array[outer_ind][temp]!=INT_MIN)
	{
		if(y_array[outer_ind][temp]==min_y)
		{
			temp_res+=count_ele[outer_ind][temp];
			if(y_array[outer_ind][2*temp+1]!=INT_MIN)
				temp_res-=count_ele[outer_ind][2*temp+1];
			break;
		}
		if(y_array[outer_ind][temp]<min_y)
		temp=2*temp+2;
		else
		{
			temp_res+=count_ele[outer_ind][temp];
			if(y_array[outer_ind][2*temp+1]!=INT_MIN)
				temp_res-=count_ele[outer_ind][2*temp+1];
			temp=2*temp+1;
		}
	}
}


temp=inner_ind;

if(y_array[outer_ind][temp]!=max_y)
{
	temp=2*inner_ind+2;
	while(y_array[outer_ind][temp]!=INT_MIN)
	{
		if(y_array[outer_ind][temp]==max_y)
		{
			temp_res+=count_ele[outer_ind][temp];
			if(y_array[outer_ind][2*temp+2]!=INT_MIN)
				temp_res-=count_ele[outer_ind][2*temp+2];
			break;
		}
		if(y_array[outer_ind][temp]>max_y)
		temp=2*temp+1;
		else
		{
			temp_res+=count_ele[outer_ind][temp];
			if(y_array[outer_ind][2*temp+2]!=INT_MIN)
				temp_res-=count_ele[outer_ind][2*temp+2];
			temp=2*temp+2;
		}
	}
}

temp_res+=count_ele[outer_ind][inner_ind];
if(y_array[outer_ind][2*inner_ind+1]!=INT_MIN)
	temp_res-=count_ele[outer_ind][2*inner_ind+1];
if(y_array[outer_ind][2*inner_ind+2]!=INT_MIN)
	temp_res-=count_ele[outer_ind][2*inner_ind+2];
return temp_res;
}



int orthogonal_search(int cur_index,int min_x,int min_y,int max_x,int max_y)
{

if(x_array[cur_index]==INT_MIN)
	return 0;

if(x_array[cur_index]<min_x)
return orthogonal_search(2*cur_index+2,min_x,min_y,max_x,max_y);

if(x_array[cur_index]>max_x)
return orthogonal_search(2*cur_index+1,min_x,min_y,max_x,max_y);

int temp=cur_index;
int temp_res=0;

if(x_array[temp]!=min_x)
{
	temp=2*cur_index+1;
	while(x_array[temp]!=INT_MIN)
	{
		if(x_array[temp]==min_x)
		{
			temp_res+=range_ysearch(temp,min_y,max_y,0);
			if(x_array[2*temp+1]!=INT_MIN)
				temp_res-=range_ysearch(2*temp+1,min_y,max_y,0);
			break;
		}
		if(x_array[temp]<min_x)
		temp=2*temp+2;
		else
		{
			temp_res+=range_ysearch(temp,min_y,max_y,0);
			if(x_array[2*temp+1]!=INT_MIN)
				temp_res-=range_ysearch(2*temp+1,min_y,max_y,0);
			temp=2*temp+1;
		}
	}
}

temp=cur_index;

if(x_array[temp]!=max_x)
{
	temp=2*cur_index+2;
	while(x_array[temp]!=INT_MIN)
	{
		if(x_array[temp]==max_x)
		{
			temp_res+=range_ysearch(temp,min_y,max_y,0);
			if(x_array[2*temp+2]!=INT_MIN)
				temp_res-=range_ysearch(2*temp+2,min_y,max_y,0);
			break;
		}
		if(x_array[temp]>max_x)
		temp=2*temp+1;
		else
		{
			temp_res+=range_ysearch(temp,min_y,max_y,0);
			if(x_array[2*temp+2]!=INT_MIN)
				temp_res-=range_ysearch(2*temp+2,min_y,max_y,0);
			temp=2*temp+2;
		}
	}
}

temp_res+=range_ysearch(cur_index,min_y,max_y,0);
if(x_array[2*cur_index+1]!=INT_MIN)
	temp_res-=range_ysearch(2*cur_index+1,min_y,max_y,0);
if(x_array[2*cur_index+2]!=INT_MIN)
	temp_res-=range_ysearch(2*cur_index+2,min_y,max_y,0);

return temp_res;

}

bool comp(pair<int,int> left,pair<int,int> right)
{
return left.first<right.first;
}



int main()
{
int no_points,fr,se;

cin>>no_points;

while(no_points--)
{
cin>>fr>>se;
coords.push_back({fr,se});
}

sort(coords.begin(),coords.end(),comp);

construct_x(0,coords.size()-1,0);


// for(int i=0;i<x_array.size();i++)
// cout<<x_array[i]<<" ";

// cout<<endl;

// for(int j=0;j<y_array.size();j++)
// {
// 	for(int t=0;t<y_array[j].size();t++)
// 		cout<<y_array[j][t]<<" ";
// 	cout<<endl;
// }

// cout<<"done"<<endl;

// for(int j=0;j<y_array.size();j++)
// {
// 	for(int t=0;t<y_array[j].size();t++)
// 		cout<<count_ele[j][t]<<" ";
// 	cout<<endl;
// }

int min_x,min_y,max_x,max_y,nqueries;

cin>>nqueries;

while(nqueries--)
{
cin>>min_x>>min_y>>max_x>>max_y;
cout<<orthogonal_search(0,min_x,min_y,max_x,max_y);
}

}


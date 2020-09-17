#include<bits/stdc++.h>
using namespace std;

struct threedim{
	int first;
	int second;
	int third;
};

bool compx(threedim a,threedim b){
	return a.first<b.first;
}
bool compy(pair<int,int>a,pair<int,int>b){
	return a.first<b.first;
}

vector<threedim> coords;
vector<int> x_array;
vector<vector<int>> y_array;
vector<vector<vector<int>>>z_array,count_ele;


void construct_z(int fr,int rr,int level1,int level2,vector<int>&sorted_z,int level3)
{

if(z_array.size()<level1+1)
{
	z_array.resize(level1+1);
	count_ele.resize(level1+1);
}

if(z_array[level1].size()<level2+1){
	z_array[level1].resize(level2+1);
	count_ele[level1].resize(level2+1);
}

if(z_array[level1][level2].size()<level3+1)
{
	z_array[level1][level2].resize(level3+1);
	count_ele[level1][level2].resize(level3+1);
}

if(rr<fr)
{
	z_array[level1][level2][level3]=INT_MIN;
	count_ele[level1][level2][level3]=0;
	return;
}

int mid=fr+(rr-fr)/2;

int new_rr=mid-1,new_fr=mid+1;

while(new_rr>=fr and sorted_z[mid]==sorted_z[new_rr])
	new_rr--;
while(new_fr<=rr and sorted_z[mid]==sorted_z[new_fr])
	new_fr++;

z_array[level1][level2][level3]=sorted_z[mid];
count_ele[level1][level2][level3]=rr-fr+1;

construct_z(fr,new_rr,level1,level2,sorted_z,level3*2+1);
construct_z(new_fr,rr,level1,level2,sorted_z,level3*2+2);

}




void construct_y(int fr,int rr,int level1,vector<pair<int,int>>&sorted_y,int level2)
{

if(y_array.size()<level1+1)
{
	y_array.resize(level1+1);
	// count_ele.resize(level1+1);
}

if(y_array[level1].size()<level2+1)
{
	y_array[level1].resize(level2+1);
	//count_ele[level1].resize(level2+1);
}

if(rr<fr)
{
	y_array[level1][level2]=INT_MIN;
	//count_ele[level1][level2]=0;
	return;
}

int mid=fr+(rr-fr)/2;

int new_rr=mid-1,new_fr=mid+1;

while(new_rr>=fr and sorted_y[mid].first==sorted_y[new_rr].first)
	new_rr--;
while(new_fr<=rr and sorted_y[mid].first==sorted_y[new_fr].first)
	new_fr++;

y_array[level1][level2]=sorted_y[mid].first;
//count_ele[level1][level2]=rr-fr+1;
vector<int> sorted_z;
int temp=fr;

while(temp<=rr)
	sorted_z.push_back(sorted_y[temp++].second);

sort(sorted_z.begin(),sorted_z.end());

construct_z(0,rr-fr,level1,level2,sorted_z,0);


construct_y(fr,new_rr,level1,sorted_y,level2*2+1);
construct_y(new_fr,rr,level1,sorted_y,level2*2+2);

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

vector<pair<int,int>> sorted_y;
int temp=fr;

while(temp<=rr){
	sorted_y.push_back({coords[temp].second,coords[temp].third});
	temp++;
}

sort(sorted_y.begin(),sorted_y.end(),compy);

construct_y(0,rr-fr,cur_index,sorted_y,0);

construct_x(fr,new_rr,cur_index*2+1);
construct_x(new_fr,rr,cur_index*2+2);
}



int range_zsearch(int level1,int level2,int min_z,int max_z,int level3)
{
if(z_array[level1][level2][level3]==INT_MIN)
	return 0;
if(z_array[level1][level2][level3]<min_z)
	return range_zsearch(level1,level2,min_z,max_z,2*level3+2);

if(z_array[level1][level2][level3]>max_z)
	return range_zsearch(level1,level2,min_z,max_z,2*level3+1);

int temp=level3;
int temp_res=0;

if(z_array[level1][level2][temp]!=min_z)
{
	temp=2*level3+1;
	while(z_array[level1][level2][temp]!=INT_MIN)
	{
		if(z_array[level1][level2][temp]==min_z)
		{
			temp_res+=count_ele[level1][level2][temp];
			if(z_array[level1][level2][2*temp+1]!=INT_MIN)
				temp_res-=count_ele[level1][level2][2*temp+1];
			break;
		}
		if(z_array[level1][level2][temp]<min_z)
		temp=2*temp+2;
		else
		{
			temp_res+=count_ele[level1][level2][temp];
			if(z_array[level1][level2][2*temp+1]!=INT_MIN)
				temp_res-=count_ele[level1][level2][2*temp+1];
			temp=2*temp+1;
		}
	}
}


temp=level3;

if(z_array[level1][level2][temp]!=max_z)
{
	temp=2*level3+2;
	while(z_array[level1][level2][temp]!=INT_MIN)
	{
		if(z_array[level1][level2][temp]==max_z)
		{
			temp_res+=count_ele[level1][level2][temp];
			if(z_array[level1][level2][2*temp+2]!=INT_MIN)
				temp_res-=count_ele[level1][level2][2*temp+2];
			break;
		}
		if(z_array[level1][level2][temp]>max_z)
		temp=2*temp+1;
		else
		{
			temp_res+=count_ele[level1][level2][temp];
			if(z_array[level1][level2][2*temp+2]!=INT_MIN)
				temp_res-=count_ele[level1][level2][2*temp+2];
			temp=2*temp+2;
		}
	}
}

temp_res+=count_ele[level1][level2][level3];
if(z_array[level1][level2][2*level3+1]!=INT_MIN)
	temp_res-=count_ele[level1][level2][2*level3+1];
if(z_array[level1][level2][2*level3+2]!=INT_MIN)
	temp_res-=count_ele[level1][level2][2*level3+2];
return temp_res;
}

int range_ysearch(int level1,int min_y,int min_z,int max_y,int max_z,int cur_index)
{

if(y_array[level1][cur_index]==INT_MIN)
	return 0;

if(y_array[level1][cur_index]<min_y)
return range_ysearch(level1,min_y,min_z,max_y,max_z,2*cur_index+2);

if(y_array[level1][cur_index]>max_y)
return range_ysearch(level1,min_y,min_z,max_y,max_z,2*cur_index+1);

int temp=cur_index;
int temp_res=0;

if(y_array[level1][temp]!=min_y)
{
	temp=2*cur_index+1;
	while(y_array[level1][temp]!=INT_MIN)
	{
		if(y_array[level1][temp]==min_y)
		{
			temp_res+=range_zsearch(level1,temp,min_z,max_z,0);
			if(y_array[level1][2*temp+1]!=INT_MIN)
				temp_res-=range_zsearch(level1,2*temp+1,min_z,max_z,0);
			break;
		}
		if(y_array[level1][temp]<min_y)
		temp=2*temp+2;
		else
		{
			temp_res+=range_zsearch(level1,temp,min_z,max_z,0);
			if(y_array[level1][2*temp+1]!=INT_MIN)
				temp_res-=range_zsearch(level1,2*temp+1,min_z,max_z,0);
			temp=2*temp+1;
		}
	}
}

temp=cur_index;

if(y_array[level1][temp]!=max_y)
{
	temp=2*cur_index+2;
	while(y_array[level1][temp]!=INT_MIN)
	{
		if(y_array[level1][temp]==max_y)
		{
			temp_res+=range_zsearch(level1,temp,min_z,max_z,0);
			if(y_array[level1][2*temp+2]!=INT_MIN)
				temp_res-=range_zsearch(level1,2*temp+2,min_z,max_z,0);
			break;
		}
		if(y_array[level1][temp]>max_y)
		temp=2*temp+1;
	    else
		{
			temp_res+=range_zsearch(level1,temp,min_z,max_z,0);
			if(y_array[level1][2*temp+2]!=INT_MIN)
				temp_res-=range_zsearch(level1,2*temp+2,min_z,max_z,0);
			temp=2*temp+2;
		}
	}
}

temp_res+=range_zsearch(level1,cur_index,min_z,max_z,0);
if(y_array[level1][2*cur_index+1]!=INT_MIN)
	temp_res-=range_zsearch(level1,2*cur_index+1,min_z,max_z,0);
if(y_array[level1][2*cur_index+2]!=INT_MIN)
	temp_res-=range_zsearch(level1,2*cur_index+2,min_z,max_z,0);

return temp_res;

}


int orthogonal_search(int cur_index,int min_x,int min_y,int min_z,int max_x,int max_y,int max_z)
{

if(x_array[cur_index]==INT_MIN)
	return 0;

if(x_array[cur_index]<min_x)
return orthogonal_search(2*cur_index+2,min_x,min_y,min_z,max_x,max_y,max_z);

if(x_array[cur_index]>max_x)
return orthogonal_search(2*cur_index+1,min_x,min_y,min_z,max_x,max_y,max_z);

int temp=cur_index;
int temp_res=0;

if(x_array[temp]!=min_x)
{
	temp=2*cur_index+1;
	while(x_array[temp]!=INT_MIN)
	{
		if(x_array[temp]==min_x)
		{
			temp_res+=range_ysearch(temp,min_y,min_z,max_y,max_z,0);
			if(x_array[2*temp+1]!=INT_MIN)
				temp_res-=range_ysearch(2*temp+1,min_y,min_z,max_y,max_z,0);
			break;
		}
		if(x_array[temp]<min_x)
		temp=2*temp+2;
		else
		{
			temp_res+=range_ysearch(temp,min_y,min_z,max_y,max_z,0);
			if(x_array[2*temp+1]!=INT_MIN)
				temp_res-=range_ysearch(2*temp+1,min_y,min_z,max_y,max_z,0);
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
			temp_res+=range_ysearch(temp,min_y,min_z,max_y,max_z,0);
			if(x_array[2*temp+2]!=INT_MIN)
				temp_res-=range_ysearch(2*temp+2,min_y,min_z,max_y,max_z,0);
			break;
		}
		if(x_array[temp]>max_x)
		temp=2*temp+1;
	    else
		{
			temp_res+=range_ysearch(temp,min_y,min_z,max_y,max_z,0);
			if(x_array[2*temp+2]!=INT_MIN)
				temp_res-=range_ysearch(2*temp+2,min_y,min_z,max_y,max_z,0);
			temp=2*temp+2;
		}
	}
}

temp_res+=range_ysearch(cur_index,min_y,min_z,max_y,max_z,0);
if(x_array[2*cur_index+1]!=INT_MIN)
	temp_res-=range_ysearch(2*cur_index+1,min_y,min_z,max_y,max_z,0);
if(x_array[2*cur_index+2]!=INT_MIN)
	temp_res-=range_ysearch(2*cur_index+2,min_y,min_z,max_y,max_z,0);

return temp_res;

}


int main()
{
int no_points,fr,se,ti;

cin>>no_points;

while(no_points--)
{
	cin>>fr;
	cin>>se;
	cin>>ti;
	threedim temp;
	temp.first=fr;
	temp.second=se;
	temp.third=ti;
	coords.push_back(temp);
}

sort(coords.begin(),coords.end(),compx);


construct_x(0,coords.size()-1,0);

int min_x,min_y,min_z,max_x,max_y,max_z,nqueries;

cin>>nqueries;

while(nqueries--)
{
cin>>min_x>>min_y>>min_z>>max_x>>max_y>>max_z;
cout<<orthogonal_search(0,min_x,min_y,min_z,max_x,max_y,max_z)<<endl;
}


// for(int i=0;i<x_array.size();i++)
// cout<<x_array[i]<<" ";

// cout<<endl;

// cout<<"y array"<<endl;
// for(int j=0;j<y_array.size();j++)
// {
// 	for(int t=0;t<y_array[j].size();t++){
// 		cout<<y_array[j][t]<<" ";
// 	}
// 	cout<<endl;
// }

// cout<<"done"<<endl;

// cout<<"z array"<<endl;
// for(int j=0;j<z_array.size();j++)
// {
// 	for(int t=0;t<z_array[j].size();t++){
// 		for(int k=0;k<z_array[j][t].size();k++){
// 			cout<<z_array[j][t][k]<<" ";
// 		}
// 	}
// 	cout<<endl;
// }

// cout<<"count array"<<endl;
// for(int j=0;j<count_ele.size();j++)
// {
// 	for(int t=0;t<count_ele[j].size();t++){
// 		for(int k=0;k<count_ele[j][t].size();k++){
// 			cout<<count_ele[j][t][k]<<" ";
// 		}
// 	}
// 	cout<<endl;
// }
}


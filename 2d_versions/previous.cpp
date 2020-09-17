#include<bits/stdc++.h>
using namespace std;

struct 3d{
        int x;
        int y;
        int z;
};

vector<pair<int,int>> coords;
vector<int> x_array;
vector<vector<int>> y_array,count_ele;



void construct_y(int fr,int rr,int level1,vector<int>&sorted_y,int level2)
{

if(y_array.size()<level1+1)
{
        y_array.resize(level1+1);
        count_ele.resize(level1+1);
}

if(y_array[level1].size()<level2+1)
{
        y_array[level1].resize(level2+1);
        count_ele[level1].resize(level2+1);
}

if(rr<fr)
{
        y_array[level1][level2]=-1;
        count_ele[level1][level2]=0;
        return;
}

int mid=fr+(rr-fr)/2;

int new_rr=mid-1,new_fr=mid+1;

while(new_rr>=fr and sorted_y[mid]==sorted_y[new_rr])
        new_rr--;
while(new_fr<=rr and sorted_y[mid]==sorted_y[new_fr])
        new_fr++;

y_array[level1][level2]=sorted_y[mid];
count_ele[level1][level2]=rr-fr+1;

construct_y(fr,new_rr,level1,sorted_y,level2*2+1);
construct_y(new_fr,rr,level1,sorted_y,level2*2+2);

}


void construct_x(int fr,int rr,int cur_index)
{

if(x_array.size()<cur_index+1)
x_array.resize(cur_index+1);

if(rr<fr)
{
        x_array[cur_index]=-1;
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



int range_ysearch(int level1,int min_y,int max_y,int level2)
{
if(y_array[level1][level2]==-1)
        return 0;
if(y_array[level1][level2]<min_y)
        return range_ysearch(level1,min_y,max_y,2*level2+2);

if(y_array[level1][level2]>max_y)
        return range_ysearch(level1,min_y,max_y,2*level2+1);

int temp=level2;
int temp_res=0;

if(y_array[level1][temp]!=min_y)
{
        temp=2*level2+1;
        while(y_array[level1][temp]!=-1)
        {
                if(y_array[level1][temp]==min_y)
                {
                        temp_res+=count_ele[level1][temp];
                        if(y_array[level1][2*temp+1]!=-1)
                                temp_res-=count_ele[level1][2*temp+1];
                        break;
                }
                if(y_array[level1][temp]<min_y)
                temp=2*temp+2;
                else
                {
                        temp_res+=count_ele[level1][temp];
                        if(y_array[level1][2*temp+1]!=-1)
                                temp_res-=count_ele[level1][2*temp+1];
                        temp=2*temp+1;
                }
        }
}


temp=level2;

if(y_array[level1][temp]!=max_y)
{
        temp=2*level2+2;
        while(y_array[level1][temp]!=-1)
        {
                if(y_array[level1][temp]==max_y)
                {
                        temp_res+=count_ele[level1][temp];
                        if(y_array[level1][2*temp+2]!=-1)
                                temp_res-=count_ele[level1][2*temp+2];
                        break;
                }
                if(y_array[level1][temp]>max_y)
                temp=2*temp+1;
                else
                {
                        temp_res+=count_ele[level1][temp];
                        if(y_array[level1][2*temp+2]!=-1)
                                temp_res-=count_ele[level1][2*temp+2];
                        temp=2*temp+2;
                }
        }
}

temp_res+=count_ele[level1][level2];
if(y_array[level1][2*level2+1]!=-1)
        temp_res-=count_ele[level1][2*level2+1];
if(y_array[level1][2*level2+2]!=-1)
        temp_res-=count_ele[level1][2*level2+2];
return temp_res;
}


int orthogonal_search(int cur_index,int min_x,int min_y,int max_x,int max_y)
{

if(x_array[cur_index]==-1)
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
        while(x_array[temp]!=-1)
        {
                if(x_array[temp]==min_x)
                {
                        temp_res+=range_ysearch(temp,min_y,max_y,0);
                        if(x_array[2*temp+1]!=-1)
                                temp_res-=range_ysearch(2*temp+1,min_y,max_y,0);
                        break;
                }
                if(x_array[temp]<min_x)
                temp=2*temp+2;
                else
                {
                        temp_res+=range_ysearch(temp,min_y,max_y,0);
                        if(x_array[2*temp+1]!=-1)
                                temp_res-=range_ysearch(2*temp+1,min_y,max_y,0);
                        temp=2*temp+1;
                }
        }
}

temp=cur_index;

if(x_array[temp]!=max_x)
{
        temp=2*cur_index+2;
        while(x_array[temp]!=-1)
        {
                if(x_array[temp]==max_x)
                {
                        temp_res+=range_ysearch(temp,min_y,max_y,0);
                        if(x_array[2*temp+2]!=-1)
                                temp_res-=range_ysearch(2*temp+2,min_y,max_y,0);
                        break;
                }
                if(x_array[temp]>max_x)
                temp=2*temp+1;
                {
                        temp_res+=range_ysearch(temp,min_y,max_y,0);
                        if(x_array[2*temp+2]!=-1)
                                temp_res-=range_ysearch(2*temp+2,min_y,max_y,0);
                        temp=2*temp+2;
                }
        }
}

temp_res+=range_ysearch(cur_index,min_y,max_y,0);
if(x_array[2*cur_index+1]!=-1)
        temp_res-=range_ysearch(2*cur_index+1,min_y,max_y,0);
if(x_array[2*cur_index+2]!=-1)
        temp_res-=range_ysearch(2*cur_index+2,min_y,max_y,0);

return temp_res;

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


construct_x(0,coords.size()-1,0);

// for(int i=0;i<x_array.size();i++)
// cout<<x_array[i]<<" ";

// cout<<endl;

// for(int j=0;j<y_array.size();j++)
// {
//      for(int t=0;t<y_array[j].size();t++)
//              cout<<y_array[j][t]<<" ";
//      cout<<endl;
// }

// cout<<"done"<<endl;

// for(int j=0;j<y_array.size();j++)
// {
//      for(int t=0;t<y_array[j].size();t++)
//              cout<<count_ele[j][t]<<" ";
//      cout<<endl;
// }

int min_x,min_y,max_x,max_y,nqueries;

cin>>nqueries;

while(nqueries--)
{
cin>>min_x>>min_y>>max_x>>max_y;
cout<<orthogonal_search(0,min_x,min_y,max_x,max_y);
}




}


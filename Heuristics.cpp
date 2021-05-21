
#include<iostream>
using namespace std;

#define INT_MAX 999999

vector<int> dist;
vector<int> price;

bool compare(int i , int j){
  
  return price[i] * dist[j] >= price[j] * dist[i];

//price[i]/dist[i] >= price[j]/dist[j] 

}


int main(){
  int n;
  cin>>n;
  int tot_time = 0;
  bool used[n] = {false};
  int cur = n;
  int num = 0;


  
  while(num < n ){
        vector<int> left;

        for(int i=0;i<n;i++){
            if(!used[i])
                left.push_back(i);
        }

        sort(left.begin(),left.end(),compare);

        for(int i=0;i<left.size();i++){

            if( tot_time + dist[cur][left[i]] + dist[left[i]][n] <= MAX_TIME){
                cur = left[i];
                tot_time = tot_time + dist[cur][left[i]];
                used[left[i]] = true;
                ++num;
                break;
            }else{

                used[left[i]] = true;
                ++num;
            }
        }
        cout<<cur<<"\n";
  }

return 0;
}
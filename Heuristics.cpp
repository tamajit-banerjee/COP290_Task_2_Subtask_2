
#include<iostream>
using namespace std;

#define INT_MAX 999999

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
        sort(left.begin(),left.end(),compare)
  }


return 0;
}
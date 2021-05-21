#include<iostream>
using namespace std;

#define INT_MAX 999999

int main(){
    /* init the dp array */
    for(int i=0;i<(1<<n);i++){

        for(int j=0;j<n;j++){
            dp_cost[i][j] = INT_MAX ;
        }

    }



    for(int mask=0;mask<(1<<n)-1;mask++){
        if(mask == 0 ){
            for(int bomb= 0;bomb<n;bomb++){
                            dp_cost[mask|(1<<bomb)][bomb] = min(dp_cost[mask|(1<<bomb)][bomb],cost[n][bomb]);
                    }
        }else{
            for(int bomb= 0;bomb<n;bomb++){

                if( ( mask&(1<<bomb) == 0 )  ){
                    for(int pos = 0;pos<n;pos++){
                        if( ( mask&(1<<pos)   ){
                            dp_cost[mask|(1<<bomb)][bomb] = min(dp_cost[mask|(1<<bomb)][bomb],dp_cost[mask][pos] + cost[pos][bomb]);
                        }
                    }
                }
            }
        }
    }

    int answer = 0;
    for(int mask=1;mask<(1<<n);mask++){

    int tot_cost = 0;

    for(int bomb=0;bomb<n;bomb++){
        if(mask&(1<<bomb)){
            tot_cost += price[bomb];
        }
    }

    for(int pos = 0;pos<n;pos++){
        if( ( mask & (1<<pos)   ){
            if( dp_cost[mask][pos] + cost[pos][n] < TOT_TIME )
            answer = max(tot_cost,ans);
        }
        }
    }

    cout<<answer<<"\n";

return 0;
}
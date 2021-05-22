#include "simulation.h"

Droid::Droid(){
    name = "Not Entered yet!";
    xpos = 0, ypos = 0;
    old_xpos = 0, old_ypos = 0;
    score = 0;
    time = 500;
    right = 0, left = 0, up = 0, down = 0;
    width = 32; height = 48;
    droidId = 0;
    renderCycle = 1;
    droid_no = 1;
    changeDirCounter = 0;
    last_i = -1; last_j = -1;
    isBackTracking = false;
}


Droid::Droid(const Droid &p){
    name = p.name;
    xpos = p.xpos, ypos = p.ypos;
    score = p.score;
    time = p.time;
    right = 0, left = 0, up = 0, down = 0;
    width = p.width, height = p.height;
    droidId = p.droidId;
    renderCycle = p.renderCycle;
    droid_no = p.droid_no;
}

void Droid::encode(int x[]){
    x[0] = xpos;
    x[1] = ypos;
    x[2] = score;
    x[3] = time;
}

void Droid::decode(int y[]){
    old_xpos = xpos;
    old_ypos = ypos;
    xpos = y[0];
    ypos = y[1];
    score = y[2];
    time = y[3];
}

void Droid::setPosCenter(int i, int j){
    xpos = i*CELL_SIZE + CELL_SIZE/2 - width/2;
    ypos = j*CELL_SIZE + CELL_SIZE/2 - height/2;
}

void Droid::draw(SDL_Renderer *renderer, TTF_Font *font){
    SDL_Rect destR;
    destR.h = height;
    destR.w = width;
    destR.x = xpos + PADDING_LEFT;
    destR.y = ypos + PADDING_TOP;

    SDL_Rect srcR;
    srcR.h = droid_HEIGHT_SRC;
    srcR.w = droid_WIDTH_SRC;
    
    if(xpos > old_xpos)
        srcR.y = 2*droid_HEIGHT_SRC;
    else if(xpos < old_xpos)
        srcR.y = 3*droid_HEIGHT_SRC;
    else if(ypos < old_ypos)
        srcR.y = 1*droid_HEIGHT_SRC;
    else if(ypos > old_ypos)
        srcR.y = 0;
    else{
        srcR.y = 0;
        renderCycle = 0;
    }
    srcR.x = (int(renderCycle/RENDER_droid_DELAY)) * droid_WIDTH_SRC;

    // SDL_RenderCopy(renderer, Tex,  srcR, &destR);
    if(SDL_RenderCopyEx(renderer, Tex,  &srcR, &destR, 0.0, NULL, SDL_FLIP_NONE) < 0){
        std::cout<<"droids not rendered properly\n";
        std::cout<<SDL_GetError()<<"\n";
        exit(EXIT_FAILURE);
    }
    renderCycle = (renderCycle+1)%(3*RENDER_droid_DELAY) ;
}

void Droid::dispName(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos){
    disp_text(renderer, name, font, xpos, ypos);
}
void Droid::dispScore(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos){
    disp_text(renderer, "Score: ", font, xpos, ypos);
    std::string temp_str = std::to_string(score);
    char* char_type = (char*) temp_str.c_str();
    disp_text(renderer, char_type, font, xpos+50, ypos);
}
void Droid::dispTime(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos){
    disp_text(renderer, "Time: ", font, xpos, ypos);
    std::string temp_str = std::to_string(time);
    char* char_type = (char*) temp_str.c_str();
    disp_text(renderer, char_type, font, xpos+50, ypos);
}
std::pair<int,int> Droid::move(int s){
    old_xpos = xpos, old_ypos = ypos;
    int new_x = xpos, new_y = ypos;
    // std::cout<<xpos<<" "<<left<<'\n';
    if(right)
        new_x+=s;
    if(left)
        new_x-=s;
    if(up)
        new_y-=s;
    if(down)
        new_y+=s;
    return std::make_pair(new_x, new_y);
}

void Droid::handleKeyDown(int key){
    switch(key){
        case SDLK_LEFT:
            left = 1;
            break;
        case SDLK_RIGHT:
            right = 1;
            break;
        case SDLK_UP:
            up = 1;
            break;
        case SDLK_DOWN:
            down = 1;
            break;
        default:
            break;
    }
}
void Droid::handleKeyUp(int key){
    switch(key){
        case SDLK_LEFT:
            left = 0;
            break;
        case SDLK_RIGHT:
            right = 0;
            break;
        case SDLK_UP:
            up = 0;
            break;
        case SDLK_DOWN:
            down = 0;
            break;
        default:
            break;
    }
}

std::pair<int, int> Droid::getMazeCoordinates(SDL_Rect &r){
    int i = 0;
    int j = 0;
    while(i<MAZEROWS){
        while(j<MAZECOLS){
            if(xpos+width/2>=r.w * j)
                j+=1;
            if(ypos+height/2>=r.h * i)
                i+=1;
            if(xpos+width/2<r.w * j && ypos+height/2<r.h * i)
                return std::make_pair(i-1, j-1);
        }
    }
    return std::make_pair(-1, -1);
}

bool Simulation::centre(){
    int x = droid.xpos + droid.width/2 - CELL_SIZE/2;
    int y = droid.ypos + droid.height/2 - CELL_SIZE/2;
    return ((x%CELL_SIZE == 0) && (y%CELL_SIZE == 0));
}

int distSquare(int ran, std::pair<int, int> i_j){
    int x1 = i_j.first; 
    int y1 = i_j.second;
    int x2 = ran / MAZECOLS;
    int y2 = ran % MAZECOLS;

    return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
}

void Simulation::calc_path(  int n, int *price , std::vector<std::vector<int> > cost , int * mapping  ){
    std::vector<int> path;
    if(algorithm_type == 0){
        path = TSP_Dynamic_Prog(n, price, cost);
    }
    else{
        path = CCTSP_Heuristic(n, price, cost, mapping);
    }
        simulation_path.clear();
        for(int i=0;i<path.size();i++){
            simulation_path.push_back(mapping[path[i]]);
        }

}

std::vector<int> Simulation::TSP_Dynamic_Prog( int n, int *price , std::vector<std::vector<int> > cost){


    int dp_cost[(1<<n)][n];
        for(int i=0;i<(1<<n);i++){

        for(int j=0;j<n;j++){
            dp_cost[i][j] = INT_MAX ;
        }

    }

    std::vector<int> path;

    int parent[(1<<n)][n];

    for(int mask=0;mask<(1<<n)-1;mask++){
        if(mask == 0 ){
            for(int bomb= 0;bomb<n;bomb++){
                            dp_cost[mask|(1<<bomb)][bomb] = cost[n][bomb];
                            parent[mask|(1<<bomb)][bomb] = n;
                    }
        }else{
            for(int bomb= 0;bomb<n;bomb++){

                if( ( (mask&(1<<bomb)) == 0 )  ){
                    for(int pos = 0;pos<n;pos++){
                        if(  mask&(1<<pos)   ){
                            if( dp_cost[mask|(1<<bomb)][bomb] > dp_cost[mask][pos] + cost[pos][bomb] ){
                                    dp_cost[mask|(1<<bomb)][bomb] = dp_cost[mask][pos] + cost[pos][bomb];
                                    parent[mask|(1<<bomb)][bomb] = pos;
                            }
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
        if(  mask & (1<<pos)   ){
            if( dp_cost[mask][pos] + cost[pos][n] <= 100){
                if(answer < tot_cost ){
                    path.clear();
                    answer = tot_cost;
                    int temp = mask;
                    int cur = pos;
                    while(temp > 0 ){
                        int old = cur;
                        path.push_back(cur);
                        cur = parent[temp][cur];
                        temp -= (1<<old);
                    }
                    path.push_back(n);
                }
            }
        }
        }

    }

    reverse(path.begin(),path.end());

    return path;

}


std::vector<int> dist;
std::vector<int> price;

bool compare(int i , int j ){
  
  return price[i] * dist[j] >= price[j] * dist[i];

}

std::vector<int> Simulation::CCTSP_Heuristic( int n, int *v , std::vector<std::vector<int> > cost , int * mapping ){

  int tot_time = 0;
  bool used[n];
  for(int i=0;i<n;i++)
    used[i] = false;
  int cur = n;
  int num = 0;

    price.clear();
  for(int i=0;i<n;i++)
    price.push_back(v[i]);
  
  std::vector<int> path;
  path.push_back(cur);
  while(num < n ){

      //std::cout<<num<<"\n";
        // int start = mapping[cur];

        std::vector<int> left;

        for(int i=0;i<n;i++){
            if(i==cur)
                dist.push_back(100);
            else{
                    dist.push_back(cost[cur][i]);
            }
        }

        for(int i=0;i<n;i++){
            if(!used[i])
                left.push_back(i);
        }

        sort(left.begin(),left.end(),compare);

        for(int i=0;i<left.size();i++){

            //  std::cout<<"hello2\n";

            // for(int k=0;k<path.size();k++){
            //     std::cout<<mapping[path[k]]<<"\n";
            // }

            if( tot_time + cost[cur][left[i]] + cost[left[i]][n] <= 1000){
                tot_time = tot_time + cost[cur][left[i]];
                used[left[i]] = true;
                int start = mapping[cur];
                int end = mapping[left[i]];

                while(start != end ){

                //    std::cout<<start<<" "<<end<<"\n";


                    int direction = maze[start/MAZECOLS][start%MAZECOLS].to_go[end];

                    switch(direction){
                        case 0:
                            --start; break;
                        case 1:
                            ++start; break;
                        case 2:
                            start -= MAZECOLS; break;
                        case 3:
                            start += MAZECOLS; break;
                    }

                    for(int j=i+1;j<left.size();j++){

                        if(mapping[left[j]] == start){
                            path.push_back(left[j]);
                            used[left[j]] = true;
                            ++num;
                        }

                    }

                }   

                cur = left[i];
                path.push_back(cur);
                ++num;
                break;

            }else{
                used[left[i]] = true;
                ++num;
            }
        }    
  }
    return path;
}


//Better heuristic

// bool compare(int i , int j){ 
  
//   return price[i] * dist[j] >= price[j] * dist[i];

// //price[i]/dist[i] >= price[j]/dist[j] 

// }

// void reverse(vector<int> &path, int start, int end, int n)
// {
// 	while (end - start > 0)
// 	{
// 		// Start, end is relative value to the start,
// 		// the index is start|slut % size
// 		int temp = path[start % n];
// 		path[start % n] = path[end % n];
// 		path[end % n] = temp;
// 		start++;
// 		end--;
// 	}
// }


// int is_path_shorter(int **graph, int v1, int v2, int v3, int v4, int &total_dist)
// {
// 	if ((graph[v1][v3] + graph[v2][v4]) < (graph[v1][v2] + graph[v3][v4]))
// 	{
// 		total_dist -= (graph[v1][v2] + graph[v3][v4] - graph[v1][v3]
// 				- graph[v2][v4]);
// 		return 1;
// 	}
// 	return 0;
// }


// // Non-looping version of two-opt heuristic
// int twoOpt(int **graph, vector<int> &path, int &pathLength, int n)
// {
// 	int counter = 0;
// 	int term_cond = 5;
// 	int old_distance = pathLength;
// 	//int size = path.size();
// 	int v1, v2, u1, u2;

// 	// Iterate over each edge
// 	for (int i = 0; i < n; i++)
// 	{
// 		// first edge
// 		u1 = i;
// 		v1 = (i+1) % n; // wrap around to first node if u1 is last node

// 		// Skip adjacent edges, start with node one past v1
// 		for (int j = i + 2; (j + 1) % n != i; j++)
// 		{
// 			// mod by length to go back to beginning
// 			u2 = j % n;
// 			v2 = (j+1) % n;

// 			// Check if new edges would shorten the path length
// 			// If so, decreases pathLength
// 			if (is_path_shorter(graph, path[u1], path[v1], path[u2],
// 					path[v2], pathLength))
// 			{

// 				// Swap u1--v1 and u2--v2
// 				reverse(path, i + 1, j, n); // v1, u2

// 				if (pathLength - old_distance < 5 && counter == term_cond)
// 					break;

// 				// reset i
// 				if (pathLength - old_distance > term_cond )
// 					i = 0;

// 				old_distance = pathLength;
// 				counter++;
// 			}
// 		}
// 	}
// 	return pathLength;
// }



// int get_path_length(int **graph, vector<int> &path, int size){
// 	int length = 0;
// 	for (int i = 0; i < size-1; i++)
// 	{
// 		length += graph[path[i]][path[i+1]];
// 	}
// 	length += graph[path[size-1]] [path[0]]; // back home
// 	return length;
// }


// std::vector<int> Simulation::CCTSP_Heuristic( int n, int *v , std::vector<std::vector<int> > cost , int * mapping ){
//   int tot_time = 0;
//   bool used[n];
//   for(int i=0;i<n;i++)
//     used[i] = false;
//   int cur = n;
//   int num = 0;

// vector<int> path;
// path.push_back(n);

  
//   while(num < n ){
//         vector<int> left;

//         for(int i=0;i<n;i++){
//             if(!used[i])
//                 left.push_back(i);
//         }

//         sort(left.begin(),left.end(),compare);

//         for(int i=0;i<left.size();i++){

//             if( tot_time + dist[cur][left[i]] + dist[left[i]][n] - dist[cur][n] <= MAX_TIME){
//                 tot_time = tot_time + dist[cur][left[i]] + dist[left[i]][n] - dist[cur][n];
//                 cur = left[i];
//                 used[left[i]] = true;
//                 ++num;
//                 break;
//             }else{
//                 if( path.size() >= 4 )
//                 twoOpt(dist,path,tot_time, path.size());
//                 used[left[i]] = true;
//                 ++num;
//             }
//         }
//         cout<<cur<<"\n";
//   }

// return 0;
// }



void Simulation::updateDroid(){    

    SDL_Rect rect;
    rect.w = CELL_SIZE;
    rect.h = CELL_SIZE;
    rect.x = 0;
    rect.y = 0;
    
    std::pair<int, int> i_j = droid.getMazeCoordinates(rect);
    //std::cout<<i_j.first <<" "<<i_j.second<<"\n";
    if(centre()){
        // std::cout<<droid.dest <<" "<<path_counter<<"\n";
        // std::cout<<"musti"<<i_j.first <<" "<<i_j.second<<"\n";
        if(path_counter==0 || (i_j.first == droid.dest/MAZECOLS && i_j.second == droid.dest% MAZECOLS )) {
            ++path_counter;
            path_counter = (path_counter%simulation_path.size());
            droid.dest = simulation_path[path_counter];
            // std::cout<<"dest modified\n";
        }

        droid.left = 0;
        droid.right = 0;
        droid.up = 0;
        droid.down = 0;
        if(i_j.first * MAZECOLS + i_j.second == droid.dest)
            return;
        int direction = maze[i_j.first][i_j.second].to_go[droid.dest];

        switch(direction){
            case 0:
                droid.left = 1; break;
            case 1:
                droid.right = 1; break;
            case 2:
                droid.up = 1; break;
            case 3:
                droid.down = 1; break;
            default:
                break;
        }

    }
    
    std::pair<int, int> s_p = droid.move(SPEED);
    droid.xpos = s_p.first;
    droid.ypos = s_p.second;

}

void makeRect(int x1, int y1, int x2, int y2, SDL_Rect * rect, int type){
    int width = 5;

   // int offset = -10*(2*type - 1);
     int offset = 0;


    if(x1 == x2){
        rect->w = width;
        rect->h = abs(y2 - y1);
        rect->x = x1 + offset;
        if(y1>y2)
            rect->y = y2 + offset;
        else
            rect->y = y1 + offset;
    }
    if(y1 == y2){
        rect->h = width;
        rect->w = abs(x2 - x1);
        rect->y = y1 + offset;
        if(x1>x2)
            rect->x = x2 + offset;
        else
            rect->x = x1 + offset;
    }
    rect->x += PADDING_LEFT;
    rect->y += PADDING_TOP;
    
}

void Simulation::addLines(){

    SDL_Rect rect;
    rect.w = CELL_SIZE;
    rect.h = CELL_SIZE;
    rect.x = 0;
    rect.y = 0;
    std::pair<int, int> i_j = droid.getMazeCoordinates(rect);

    if(droid.last_j != -1 && droid.last_i !=-1){
        SDL_Rect rect;
        // if(droid.isBackTracking){
          //  makeRect(CELL_SIZE/2 + CELL_SIZE * i_j.second, CELL_SIZE/2 + CELL_SIZE * i_j.first, CELL_SIZE/2 + CELL_SIZE * droid.last_j, CELL_SIZE/2 + CELL_SIZE * droid.last_i, &rect, 1);
         //   linesBacktrack.push_back(rect);
        // }
        // else{
            makeRect(CELL_SIZE/2 + CELL_SIZE * i_j.second, CELL_SIZE/2 + CELL_SIZE * i_j.first, CELL_SIZE/2 + CELL_SIZE * droid.last_j, CELL_SIZE/2 + CELL_SIZE * droid.last_i, &rect, 0);
              linesForward.push_back(rect);
        // }
    }

    droid.last_i = i_j.first;
    droid.last_j = i_j.second;

}

void Simulation::drawLines(){
    SDL_SetRenderDrawColor(renderer, 50, 255, 50, 255);
    for(int i = 0; i < linesForward.size(); i++){
        if(SDL_RenderCopyEx(renderer, forwardTex,  NULL, &linesForward[i], 0.0, NULL, SDL_FLIP_NONE) < 0){
            std::cout<<"Forward lines not rendered properly\n";
            std::cout<<SDL_GetError()<<"\n";
            exit(EXIT_FAILURE);
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
    for(int i = 0; i < linesBacktrack.size(); i++){
        if(SDL_RenderCopyEx(renderer, backTex,  NULL, &linesBacktrack[i], 0.0, NULL, SDL_FLIP_NONE) < 0){
            std::cout<<"BackTracking lines not rendered properly\n";
            std::cout<<SDL_GetError()<<"\n";
            exit(EXIT_FAILURE);
        }
    }
}
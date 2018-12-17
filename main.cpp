//============================================================================
// Name        : OKM_BM.cpp
// Author      : Josh Purtell
// Version     :
// Copyright   : Your copyright notice
// Description : Brownian Motion Approach to Pricing Exotic Options
//============================================================================

#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
using namespace std;

//create struct with Knock-out prices for commodities, Guaranteed buy price
	typedef struct {
		double kprice;
		double bprice;
		double velocity;
	} price;
//create struct with cube location and average value
	typedef struct {
			double coord1;
			double coord2;
			double coord3;
			double coord4;
			double coord5;
			double coord6;
			double coord7;
			double coord8;
			double coord9;
			double coord10;
			double avg_value;
			bool passed;
		} cube;
int main(){
	double price_option;
	double expiry;
	int number;
	int bound_res;
	int dom_res;
	int path_num;
	double min_profit;
	
	price prices[10];
	cout << "Please input information for option: price of option in $" << endl;
	cin >> price_option;
	cout << "Please input information for option: expiry time in seconds" << endl;
	cin >> expiry;
	cout << "Please input information for option: number of underlying assets" << endl;
	cin >> number;
	for (int i = 0; i < (number-1); i++){
		cout << "Please input information for option: knock-out price for underlying asset" ;
		cout << i << endl;
		cin >> prices[i].kprice;
		cout << "Please input information for option: guaranteed buy price for underlying asset" ;
		cout << i << endl;
		cin >> prices[i].bprice;
		cout << "Please input information for option: proportion factor, normed velocity ($/s) of underlying asset" << endl;
		cout << i << endl;
		cin >> prices[i].velocity;	
	}
	for (int i = number; i < 10; i++){
		prices[i].kprice = 0;
		prices[i].bprice = 0;
		prices[i].velocity = 0;
	}
	
	cout << "Please input information for standards: boundary mesh resolution per unit price in exponent of 2" << endl;
	cin >> bound_res;
	cout << "Please input information for standards: domain mesh resolution per unit price. Please set 10" << endl;
	cin >> dom_res;
	cout << "Please input information for standards: number of Brownian paths tested in exponent of 10" << endl;
	cin >> path_num;
	cout << "Please input information for standards: minimum profit preferred" << endl;
	cin >> min_profit;
	//Have program output appropriate entrance values.
	
	//calculate velocity in n dimensions: sqrt(v dot v)
	float total_vel = 0;
	for (int i = 0; i < (number-1); i++){
		total_vel = total_vel + (prices[i].velocity * prices[i].velocity);
	}
	total_vel = sqrt(total_vel);
	
	//create domain mesh
	dom_res = pow (2 , dom_res);
	double increment_value[10];
	for (int i = 0; i < (number-1); i++){
		increment_value[i] = (prices[i].kprice)/dom_res;
	}
	int num_cubes = pow(10, number);
	cube cubes[num_cubes];
	for (int k = 0; k < (num_cubes-1); k++){
		double avg_value;
		int n = 1;
		int tmp1 = (int)(k / pow(10,n));   // tmp1 = 12345
		int tmp2 = ((int)(tmp1/10))*10;    // tmp2 = 12340
		int digit = tmp1 - tmp2;
		cubes[k].coord1 = (digit+.5)*increment_value[0];
		n = 2;
		tmp1 = (int)(k / pow(10,n));   // tmp1 = 12345
		tmp2 = ((int)(tmp1/10))*10;    // tmp2 = 12340
		digit = tmp1 - tmp2;
		cubes[k].coord2 = (digit+.5)*increment_value[1];
		n = 3;
		tmp1 = (int)(k / pow(10,n));   // tmp1 = 12345
		tmp2 = ((int)(tmp1/10))*10;    // tmp2 = 12340
		digit = tmp1 - tmp2;
		cubes[k].coord3 = (digit+.5)*increment_value[2];
		n = 4;
		tmp1 = (int)(k / pow(10,n));   // tmp1 = 12345
		tmp2 = ((int)(tmp1/10))*10;    // tmp2 = 12340
		digit = tmp1 - tmp2;
		cubes[k].coord4 = (digit+.5)*increment_value[3];
		n = 5;
		tmp1 = (int)(k / pow(10,n));   // tmp1 = 12345
		tmp2 = ((int)(tmp1/10))*10;    // tmp2 = 12340
		digit = tmp1 - tmp2;
		cubes[k].coord5 = (digit+.5)*increment_value[4];
		n = 6;
		tmp1 = (int)(k / pow(10,n));   // tmp1 = 12345
		tmp2 = ((int)(tmp1/10))*10;    // tmp2 = 12340
		digit = tmp1 - tmp2;
		cubes[k].coord6 = (digit+.5)*increment_value[5];
		n = 7;
		tmp1 = (int)(k / pow(10,n));   // tmp1 = 12345
		tmp2 = ((int)(tmp1/10))*10;    // tmp2 = 12340
		digit = tmp1 - tmp2;
		cubes[k].coord7 = (digit+.5)*increment_value[6];
		n = 8;
		tmp1 = (int)(k / pow(10,n));   // tmp1 = 12345
		tmp2 = ((int)(tmp1/10))*10;    // tmp2 = 12340
		digit = tmp1 - tmp2;
		cubes[k].coord8 = (digit+.5)*increment_value[7];
		n = 9;
		tmp1 = (int)(k / pow(10,n));   // tmp1 = 12345
		tmp2 = ((int)(tmp1/10))*10;    // tmp2 = 12340
		digit = tmp1 - tmp2;
		cubes[k].coord9 = (digit+.5)*increment_value[8];
		n = 10;
		tmp1 = (int)(k / pow(10,n));   // tmp1 = 12345
		tmp2 = ((int)(tmp1/10))*10;    // tmp2 = 12340
		digit = tmp1 - tmp2;
		cubes[k].coord10 = (digit+.5)*increment_value[9];
	}
	//cycle through the domain mesh
	for (int k = 0; k < (num_cubes-1); k++){
		bool hit,timeout;
		cube path;
		cube oldpath;
		path.coord1 = cubes[k].coord1;
		path.coord2 = cubes[k].coord2;
		path.coord3 = cubes[k].coord3;
		path.coord4 = cubes[k].coord4;
		path.coord5 = cubes[k].coord5;
		path.coord6 = cubes[k].coord6;
		path.coord7 = cubes[k].coord7;
		path.coord8 = cubes[k].coord8;
		path.coord9 = cubes[k].coord9;
		path.coord10 = cubes[k].coord10;
		float tot_distance = 0;
		float tot_time = 0;
		//records which variable hit the barrier
		int hit_number = 11;
		//making matrix to put "value"s in
		float path_vals[path_num];
		//run the paths
		for (int p = 0; p < (path_num-1); p++){
			while ((hit == false)&&(timeout == false)){
				//seed the random generator
				srand(p);
				//see which coord is closest to either barrier or 0
				float radius;
				float min[10];
				min[0] = min(path.coord1,prices[0].kprice);
				min[1] = min(path.coord2,prices[1].kprice);
				min[2] = min(path.coord3,prices[2].kprice);
				min[3] = min(path.coord4,prices[3].kprice);
				min[4] = min(path.coord5,prices[4].kprice);
				min[5] = min(path.coord6,prices[5].kprice);
				min[6] = min(path.coord7,prices[6].kprice);
				min[7] = min(path.coord8,prices[7].kprice);
				min[8] = min(path.coord9,prices[8].kprice);
				min[9] = min(path.coord3,prices[9].kprice);
				radius = min[0];
				for (int j = 0; j < 8; j++){
					radius = min(radius,min[j+1]);
				}
				//move to random point on sphere
				int random_number[10];
				for (int j = 0; j < 9; j++){
					random_number[j] = rand() % 1000 + 0; // number in the range 0 to 1000
				}
				//calculate normalizer
				int normalizer = 0;
				for (int j = 0; j < 9; j++){
					normalizer = normalizer + random_number[j]*random_number[j];
				}
				normalizer = 1/(sqrt(normalizer));
				//record location in oldpath
				oldpath.coord1 = path.coord1;
				oldpath.coord2 = path.coord2;
				oldpath.coord3 = path.coord3;
				oldpath.coord4 = path.coord4;
				oldpath.coord5 = path.coord5;
				oldpath.coord6 = path.coord6;
				oldpath.coord7 = path.coord7;
				oldpath.coord8 = path.coord8;
				oldpath.coord9 = path.coord9;
				oldpath.coord10 = path.coord10;
				//get new location
				path.coord1 = normalizer*random_number[0]+path.coord1;
				path.coord2 = normalizer*random_number[1]+path.coord2;
				path.coord3 = normalizer*random_number[2]+path.coord3;
				path.coord4 = normalizer*random_number[3]+path.coord4;
				path.coord5 = normalizer*random_number[4]+path.coord5;
				path.coord6 = normalizer*random_number[5]+path.coord6;
				path.coord7 = normalizer*random_number[6]+path.coord7;
				path.coord8 = normalizer*random_number[7]+path.coord8;
				path.coord9 = normalizer*random_number[8]+path.coord9;
				path.coord10 = normalizer*random_number[9]+path.coord10;
			
				//check distance from barrier
				if (min((path.coord1),(prices[0].kprice-path.coord1)) < .0000001){
					hit == true;
					hit_number = 1;
				}
				if (min((path.coord2),(prices[1].kprice-path.coord2)) < .0000001){
					hit == true;
					hit_number = 2;
				}
				if (min((path.coord3),(prices[2].kprice-path.coord3)) < .0000001){
					hit == true;
					hit_number = 3;
				}
				if (min((path.coord4),(prices[3].kprice-path.coord4)) < .0000001){
					hit == true;
					hit_number = 4;
				}
				if (min((path.coord5),(prices[4].kprice-path.coord5)) < .0000001){
					hit == true;
					hit_number = 5;
				}
				if (min((path.coord6),(prices[5].kprice-path.coord6)) < .0000001){
					hit == true;
					hit_number = 6;
				}
				if (min((path.coord7),(prices[6].kprice-path.coord7)) < .0000001){
					hit == true;
					hit_number = 7;
				}
				if (min((path.coord8),(prices[7].kprice-path.coord8)) < .0000001){
					hit == true;
					hit_number = 8;
				}
				if (min((path.coord9),(prices[8].kprice-path.coord9)) < .0000001){
					hit == true;
					hit_number = 9;
				}
				if (min((path.coord10),(prices[9].kprice-path.coord10)) < .0000001){
					hit == true;
					hit_number = 10;
				}
				//check time elapsed
				//calculate distance
				float dist_sum = (oldpath.coord1-path.coord1)*(oldpath.coord1-path.coord1)+(oldpath.coord2-path.coord2)*(oldpath.coord2-path.coord2);
				dist_sum = dist_sum + (oldpath.coord3-path.coord3)*(oldpath.coord3-path.coord3)+(oldpath.coord4-path.coord4)*(oldpath.coord4-path.coord4);
				dist_sum = dist_sum + (oldpath.coord5-path.coord5)*(oldpath.coord5-path.coord5)+(oldpath.coord6-path.coord6)*(oldpath.coord6-path.coord6);
				dist_sum = dist_sum + (oldpath.coord7-path.coord7)*(oldpath.coord7-path.coord7)+(oldpath.coord8-path.coord8)*(oldpath.coord8-path.coord8);
				dist_sum = dist_sum + (oldpath.coord9-path.coord9)*(oldpath.coord9-path.coord9)+(oldpath.coord10-path.coord10)*(oldpath.coord10-path.coord10);
				tot_distance = tot_distance + sqrt(dist_sum);
				tot_time = tot_distance/(total_vel);
				if (tot_time > expiry){
					timeout = true;
				}
			
				//record terminating values
				if (timeout==1){
					double temp_val = -price_option;
					temp_val = temp_val + (path.coord1-prices[0].bprice);
					temp_val = temp_val + (path.coord2-prices[1].bprice);
					temp_val = temp_val + (path.coord3-prices[2].bprice);
					temp_val = temp_val + (path.coord4-prices[3].bprice);
					temp_val = temp_val + (path.coord5-prices[4].bprice);
					temp_val = temp_val + (path.coord6-prices[5].bprice);
					temp_val = temp_val + (path.coord7-prices[6].bprice);
					temp_val = temp_val + (path.coord8-prices[7].bprice);
					temp_val = temp_val + (path.coord9-prices[8].bprice);
					temp_val = temp_val + (path.coord10-prices[9].bprice);
					//fill value
					path_vals[p] = temp_val;
				
				}
				if (hit==1){
					//nullify the "hit" variable
					if (hit_number==1){
						path.coord1 = 0;
				    }
					if (hit_number==2){
						path.coord2 = 0;
					}
					if (hit_number==3){
						path.coord3 = 0;
					}
					if (hit_number==4){
						path.coord4 = 0;
					}
					if (hit_number==5){
						path.coord5 = 0;
					}
					if (hit_number==5){
						path.coord5 = 0;
					}
					if (hit_number==6){
						path.coord6 = 0;
					}
					if (hit_number==7){
						path.coord7 = 0;
					}
					if (hit_number==8){
						path.coord8 = 0;
					}
					if (hit_number==9){
						path.coord9 = 0;
					}
					if (hit_number==10){
						path.coord10 = 0;
					}
					//procede to calculate value
					double temp_val = -price_option;
					temp_val = temp_val + (path.coord1-prices[0].bprice);
					temp_val = temp_val + (path.coord2-prices[1].bprice);
					temp_val = temp_val + (path.coord3-prices[2].bprice);
					temp_val = temp_val + (path.coord4-prices[3].bprice);
					temp_val = temp_val + (path.coord5-prices[4].bprice);
					temp_val = temp_val + (path.coord6-prices[5].bprice);
					temp_val = temp_val + (path.coord7-prices[6].bprice);
					temp_val = temp_val + (path.coord8-prices[7].bprice);
					temp_val = temp_val + (path.coord9-prices[8].bprice);
					temp_val = temp_val + (path.coord10-prices[9].bprice);
					//fill value
					path_vals[p] = temp_val;	
				}
			}
			
		}
		double avg_total=0;
		//calculate average
		for (int p = 0; p < (path_num-1); p++){
			avg_total=avg_total+path_vals[p];
		}
		cubes[k].avg_value = avg_total/path_num;
		if (cubes[k].avg_value > min_profit){
			cubes[k].passed = true;
		}
		if (cubes[k].avg_value == min_profit){
			cubes[k].passed = true;
		}
		if (cubes[k].avg_value < min_profit){
			cubes[k].passed = false;
		}
		
	}
	//find all cubes w average value that satisfy minimum profit tolerance
	char response;
	for (int k = 0; k < (num_cubes-1); k++){
		if (cubes[k].passed ==true){
			cout << "Cube Number" << k << "works" << endl;
			cout << "Want prices? Y/N" << endl;
			cin >> response;
			if (response == 'Y'){
				cout << "Price 1:" << cubes[k].coord1 << endl;
				cout << "Price 2:" << cubes[k].coord2 << endl;
				cout << "Price 3:" << cubes[k].coord3 << endl;
				cout << "Price 4:" << cubes[k].coord4 << endl;
				cout << "Price 5:" << cubes[k].coord5 << endl;
				cout << "Price 6:" << cubes[k].coord6 << endl;
				cout << "Price 7:" << cubes[k].coord7 << endl;
				cout << "Price 8:" << cubes[k].coord8 << endl;
				cout << "Price 9:" << cubes[k].coord9 << endl;
				cout << "Price 10:" << cubes[k].coord10 << endl;
			}
			else;
		}
		else;
	}	
}


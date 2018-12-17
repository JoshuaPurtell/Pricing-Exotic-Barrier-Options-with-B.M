//============================================================================
// Name        : OKM_BM.cpp
// Author      : Josh Purtell
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
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
			double avg_value;
			bool passed;
		} cube;


int main(){
	//define variables
	double price_option;
	double expiry;
	int number;
	int dom_res;
	int path_num;
	double min_profit;


	//get user option info
	price prices[3];
	/*
	cout << "Please input information for option: price of option in $" << endl;
	cin >> price_option;
	cout << "Please input information for option: expiry time in seconds" << endl;
	cin >> expiry;
	cout << "Please input information for option: number of underlying assets" << endl;
	cin >> number;
	*/
	price_option = 1;
	expiry = 1000;
	number = 3;
	for (int i = 0; i < number; i++){
		/*
		cout << "Please input information for option: knock-out price for underlying asset" << i << endl;
		cin >> prices[i].kprice;
		cout << "Please input information for option: guaranteed buy price for underlying asset" << i << endl;
		cin >> prices[i].bprice;
		cout << "Please input information for option: proportion factor, normed velocity ($/s) of underlying asset" << i << endl;
		cin >> prices[i].velocity;
		*/
		prices[i].kprice = 100;
		prices[i].bprice = 50;
		prices[i].velocity = 2;
	}

	//get user test info
	/*
	cout << "Please input information for standards: domain mesh resolution per unit price. Please set 10" << endl;
	cin >> dom_res;
	cout << "Please input information for standards: number of Brownian paths tested in exponent of 10" << endl;
	cin >> path_num;
	*/
	dom_res = 10;
	path_num = 1;
	path_num = pow(10,path_num);
	if (path_num < 0){
		path_num = 1000000;
	}
	/*
	cout << "Please input information for standards: minimum profit preferred" << endl;
	cin >> min_profit;
	*/
	min_profit = 10;


	//calculate velocity in n dimensions: sqrt(v dot v)
	float total_vel = 0;
	for (int i = 0; i < number; i++){
		total_vel = total_vel + (prices[i].velocity * prices[i].velocity);
	}
	total_vel = sqrt(total_vel);

	//:::::::::::create domain mesh

	//find increment
	double increment_value[10];
	for (int i = 0; i < number; i++){
		increment_value[i] = (prices[i].kprice)/dom_res;
		cout << "increment value: " << increment_value[i] << endl;
	}

	//make cubes array
	int num_cubes = pow(10, number);
	cout << "number of cubes" << num_cubes << endl;
	cube cubes[num_cubes];

	//set cubes at appropriate location
	for (int k = 0; k < num_cubes; k++){
		int n = 1;
		int tmp1 = (int)(k / pow(10,n));
		int tmp2 = ((int)(tmp1/10))*10;
		int digit = tmp1 - tmp2;
		cubes[k].coord1 = (digit+.5)*increment_value[0];
		cout << "Cube coord 1:" << cubes[k].coord1 << endl;
		n = 2;
		tmp1 = (int)(k / pow(10,n));
		tmp2 = ((int)(tmp1/10))*10;
		digit = tmp1 - tmp2;
		cubes[k].coord2 = (digit+.5)*increment_value[1];
		cout << "Cube coord 2:" << cubes[k].coord2 << endl;
		n = 3;
		tmp1 = (int)(k / pow(10,n));
		tmp2 = ((int)(tmp1/10))*10;
		digit = tmp1 - tmp2;
		cubes[k].coord3 = (digit+.5)*increment_value[2];
		cout << "Cube coord 3:" << cubes[k].coord3 << endl;
	}

	//cycle through the domain mesh
	for (int k = 0; k < num_cubes; k++){
		bool hit,timeout;
		cube path;
		cube oldpath;
		path.coord1 = cubes[k].coord1;
		path.coord2 = cubes[k].coord2;
		path.coord3 = cubes[k].coord3;
		double tot_distance = 0;
		double tot_time = 0;

		//records which variable hit the barrier
		int hit_number = 4;

		//making matrix to put "value"s in
		double path_vals[path_num];

		//debugging
		for (int p = 0; p < path_num; p++){
			path_vals[p] = 1;
		}


		//run the paths
		/*
		for (int p = 0; p < path_num; p++){
			while ((hit == false)&&(timeout == false)){
				cout << "got to 144" << endl;
				//see which coord is closest to either barrier or 0
				double radius;
				double minim [3];
				//find minimal distance between coordinates and barriers
				minim[0] = min(prices[0].kprice-path.coord1,path.coord1);
				minim[1] = min(prices[1].kprice-path.coord2,path.coord2);
				minim[2] = min(prices[2].kprice-path.coord3,path.coord3);
				radius = minim[0];
				for (int j = 0; j < 2; j++){
					cout << "radius: " << radius << endl;
					if (minim[j+1] > 0){
						radius = min(radius,minim[j+1]);
					}
				}
				cout << "radius: " << radius << endl;
				//move to random point on sphere
				int random_number[3];
				for (int j = 0; j < 3; j++){
					random_number[j] = rand() % 1000 + 0; // number in the range 0 to 1000
				}
				//calculate normalizer
				double normalizer = 0;
				for (int j = 0; j < 3; j++){
					normalizer = normalizer + random_number[j]*random_number[j];
				}
				cout << "1 normalizer " << normalizer << endl;
				normalizer = sqrt(normalizer);
				cout << "2 normalizer " << normalizer << endl;
				normalizer = 1/normalizer;
				cout << "3 normalizer " << normalizer << endl;
				//record location in oldpath
				oldpath.coord1 = path.coord1;
				oldpath.coord2 = path.coord2;
				oldpath.coord3 = path.coord3;
				//get new location
				cout << "First random number " << random_number[0] << endl;
				cout << "radius: " << radius << endl;
				cout << "Addition: " << radius*normalizer*random_number[0] << endl;
				path.coord1 = radius*normalizer*random_number[0]+path.coord1;
				path.coord2 = radius*normalizer*random_number[1]+path.coord2;
				path.coord3 = radius*normalizer*random_number[2]+path.coord3;
				cout << "Distance moved in coord 1: " << path.coord1 - oldpath.coord1 << endl;
				cout << "Path coord 1: " << path.coord1 << endl;
				cout << "Oldpath coord 1: " << oldpath.coord1 << endl;
				cout << "I got to 232" << endl;
				//check distance from barrier
				if (prices[0].kprice-path.coord1 < .0000001){
					hit = true;
					hit_number = 1;
				}
				if (prices[1].kprice-path.coord2 < .0000001){
					hit = true;
					hit_number = 2;
				}
				if (prices[2].kprice-path.coord3 < .0000001){
					hit = true;
					hit_number = 3;
				}
				//check time elapsed
				//calculate distance
				float dist_sum = (oldpath.coord1-path.coord1)*(oldpath.coord1-path.coord1)+(oldpath.coord2-path.coord2)*(oldpath.coord2-path.coord2);
				cout << "distance sum: " << dist_sum << endl;
				cout << "difference 1: " << oldpath.coord1-path.coord1 << endl;
				dist_sum = dist_sum + (oldpath.coord3-path.coord3)*(oldpath.coord3-path.coord3);
				cout << "total distance before sum: " << tot_distance << endl;
				tot_distance = tot_distance + sqrt(dist_sum);
				cout << "total distance: " << tot_distance << endl;
				tot_time = tot_distance/(total_vel);
				cout << "total time: " << tot_time << endl;
				cout << "expiry time: " << expiry << endl;
				if (tot_time > expiry){
					timeout = true;
					cout << "TIMEOUT" << endl;
				}

				//record terminating values
				if (timeout==1){
					double temp_val = -price_option;
					temp_val = temp_val + (path.coord1-prices[0].bprice);
					temp_val = temp_val + (path.coord2-prices[1].bprice);
					temp_val = temp_val + (path.coord3-prices[2].bprice);
					//fill value
					path_vals[p] = temp_val;

				}
				cout << "I got to 248" << endl;
				if (hit==1){
					//nullify the "hit" variable
					if (hit_number==1){
						path.coord1 = 0.0;
				    }
					if (hit_number==2){
						path.coord2 = 0.0;
					}
					if (hit_number==3){
						path.coord3 = 0.0;
					}
					//procede to calculate value
					double temp_val = -price_option;
					cout << "temp val: " << temp_val << endl;
					temp_val = temp_val + (path.coord1-prices[0].bprice);
					cout << "temp val: " << temp_val << endl;
					cout << "path_coord val: " << path.coord1 << endl;
					cout << "buy price val: " << prices[0].bprice << endl;
					temp_val = temp_val + (path.coord2-prices[1].bprice);
					cout << "temp val: " << temp_val << endl;
					temp_val = temp_val + (path.coord3-prices[2].bprice);
					cout << "temp val: " << temp_val << endl;
					//fill value
					path_vals[p] = temp_val;
				}
			}

		}*/

		double avg_total= 0.0;
		//calculate average
		for (int p = 0; p < path_num; p++){
			cout << "path val: " << path_vals[p] << endl;
			avg_total=avg_total+path_vals[p];
		}
		cubes[k].avg_value = avg_total/path_num;
		cout << "Avg: " << cubes[k].avg_value << endl;
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
	cout << "I got to 376" << endl;
	//find all cubes w average value that satisfy minimum profit tolerance
	char response;
	for (int k = 0; k < num_cubes; k++){
		if (cubes[k].passed ==true){
			cout << "I got to 381" << endl;
			cout << "Cube Number" << k << "works" << endl;
			cout << "Want prices? Y/N" << endl;
			cin >> response;
			if (response == 'Y'){
				cout << "Price 1:" << cubes[k].coord1 << endl;
				cout << "Price 2:" << cubes[k].coord2 << endl;
				cout << "Price 3:" << cubes[k].coord3 << endl;
			}
			else;
		}
		else;
	}
}


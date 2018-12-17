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
	
	cout << "Please input information for option: price of option in $" << endl;
	cin >> price_option;
	cout << "Please input information for option: expiry time in seconds" << endl;
	cin >> expiry;
	cout << "Please input information for option: number of underlying assets" << endl;
	cin >> number;
	
	//price_option = 4;
	///expiry = 100000;
	//number = 3;
	for (int i = 0; i < number; i++){
		cout << "Please input information for option: knock-out price for underlying asset" << i << endl;
		cin >> prices[i].kprice;
		cout << "Please input information for option: guaranteed buy price for underlying asset" << i << endl;
		cin >> prices[i].bprice;
		cout << "Please input information for option: proportion factor, normed velocity ($/s) of underlying asset" << i << endl;
		cin >> prices[i].velocity;
		
		//prices[i].kprice = 100;
		//prices[i].bprice = 50;
		//prices[i].velocity = 5;
	}

	//get user test info
	
	cout << "Please input information for standards: domain mesh resolution per unit price. Please set 10" << endl;
	cin >> dom_res;
	cout << "Please input information for standards: number of Brownian paths tested in exponent of 10" << endl;
	cin >> path_num;
	
	//dom_res = 10;
	//path_num = 2;
	
	path_num = pow(10,path_num);
	if (path_num < 0){
		path_num = 1000000;
	}
	
	cout << "Please input information for standards: minimum profit preferred" << endl;
	cin >> min_profit;
	
	//min_profit = 10;


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
	}

	//make cubes array
	int num_cubes = pow(10, number);
	cube cubes[num_cubes];

	//set cubes at appropriate location
	for (int k = 0; k < num_cubes; k++){
		int n = 0;
		int tmp1 = (k / pow(10,n));
		int tmp2 = ((tmp1/10))*10;
		int digit = tmp1 - tmp2;
		//cout << "Cube digit 1:" << digit << endl;
		cubes[k].coord1 = (digit+.5)*increment_value[0];
		//cout << "Cube coord 1:" << cubes[k].coord1 << endl;
		n = 1;
		tmp1 = (int)(k / pow(10,n));
		tmp2 = ((int)(tmp1/10))*10;
		digit = tmp1 - tmp2;
		//cout << "Cube digit 2:" << digit << endl;
		cubes[k].coord2 = (digit+.5)*increment_value[1];
		//cout << "Cube coord 2:" << cubes[k].coord2 << endl;
		n = 2;
		tmp1 = (int)(k / pow(10,n));
		tmp2 = ((int)(tmp1/10))*10;
		digit = tmp1 - tmp2;
		//cout << "Cube digit 3:" << digit << endl;
		cubes[k].coord3 = (digit+.5)*increment_value[2];
		//cout << "Cube coord 3:" << cubes[k].coord3 << endl;

	}

	//cycle through the domain mesh
	for (int k = 0; k < num_cubes; k++){
		cout << "cube: " << k << endl;



		//initialize paths
		cube path;
		cube oldpath;

		//assign cube coordinates to begin path
		path.coord1 = cubes[k].coord1;
		path.coord2 = cubes[k].coord2;
		path.coord3 = cubes[k].coord3;

		//initialize time and distance totals
		double tot_distance = 0;
		double tot_time = 0;

		//record which variable hit the barrier
		int hit_number = 4;

		//making matrix to put "value"s in
		double path_vals[path_num];

		//run the paths
		for (int p = 0; p < path_num; p++){


			//initialize booleans
			bool hit = false;
			bool timeout = false;


			cout << "path: " << p << endl;
			cout << "Hit true or false?: " << hit << endl;
			cout << "Timeout true or false?: " << timeout << endl;


			//exit once T is attained
			while ((hit == false)&&(timeout == false)){

				//:::see which coordinate is closest to either barrier or 0

				//initialize radius and min array
				double radius;
				double minim [3];

				//find minimal distance between coordinates and barriers
				minim[0] = min(prices[0].kprice-path.coord1,path.coord1);
				minim[1] = min(prices[1].kprice-path.coord2,path.coord2);
				minim[2] = min(prices[2].kprice-path.coord3,path.coord3);


				//get most minimal radius
				for (int j = 0; j < 2; j++){
					radius = min(minim[j],minim[j+1]);
				}


				//::: move to random point on sphere

				//get random numbers
				int random_number[3];
				for (int j = 0; j < 3; j++){
					random_number[j] = rand() % 1000 + 0; // number in the range 0 to 1000
				}

				//calculate normalizer from numbers
				double normalizer = 0;
				for (int j = 0; j < 3; j++){
					normalizer = normalizer + random_number[j]*random_number[j];
				}
				normalizer = sqrt(normalizer);
				normalizer = 1/normalizer;

				//record location in old path
				oldpath.coord1 = path.coord1;
				oldpath.coord2 = path.coord2;
				oldpath.coord3 = path.coord3;


				//get new location
				path.coord1 = radius*normalizer*random_number[0]+path.coord1;
				cout << "Path1: " << path.coord1 << endl;
				path.coord2 = radius*normalizer*random_number[1]+path.coord2;
				cout << "Path2: " << path.coord2 << endl;
				path.coord3 = radius*normalizer*random_number[2]+path.coord3;
				cout << "Path3: " << path.coord3 << endl;


				//check distance from barrier
				if (path.coord1 > 99.9){
					hit = true;
					//cout << "HIT" << endl;
					hit_number = 1;
				}
				if (path.coord2 > 99.9){
					hit = true;
					//cout << "HIT" << endl;
					hit_number = 2;
				}
				if (path.coord3 > 99.9){
					hit = true;
					//cout << "HIT" << endl;
					hit_number = 3;
				}
				if (path.coord1 < .1){
					hit = true;
				}
				if (path.coord2 < .1){
					hit = true;
				}
				if (path.coord3 < .1){
					hit = true;
				}

				//calculate total distance
				float dist_sum = (oldpath.coord1-path.coord1)*(oldpath.coord1-path.coord1)+(oldpath.coord2-path.coord2)*(oldpath.coord2-path.coord2)+(oldpath.coord3-path.coord3)*(oldpath.coord3-path.coord3);
				tot_distance = tot_distance + sqrt(dist_sum);

				//calculate time elapsed
				tot_time = tot_distance/(total_vel);
				cout << "time: " << tot_time << endl;

				//pop out if timeout
				if (tot_time > expiry){
					timeout = true;
				}

			}
			//record terminating values

			if (timeout==true){
				double temp_val = (path.coord1-prices[0].bprice)+(path.coord2-prices[1].bprice)+(path.coord3-prices[2].bprice)-price_option;

				//fill value
				path_vals[p] = temp_val;

			}
			if (hit==true){

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
				double temp_val =(path.coord1-prices[0].bprice)+(path.coord2-prices[1].bprice)+(path.coord3-prices[2].bprice)-price_option;

				//fill value
				path_vals[p] = temp_val;
			}

		}

		//calculate average
		double avg_total= 0.0;
		for (int p = 0; p < path_num; p++){
			avg_total=avg_total+path_vals[p];
		}
		cubes[k].avg_value = avg_total/path_num;

        //evaluate which cubes are good enough

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
	for (int k = 0; k < num_cubes; k++){
		if (cubes[k].passed ==true){
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


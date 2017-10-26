#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
#include <algorithm>
#include <cstring>
#include <cmath>
#include <map>
using namespace std;

// This file makes input into boolean values by splitting on continuous values

vector <vector <string>> data;
int arr[] = {27,0,209912,0,12,0,0,0,0,0,7073,1820,42}; // Obtained from split_values file

int main()
{
	ifstream att,input;
	ofstream output;
	input.open("sorted_test.txt");
	output.open("bool_test.txt");	
	int i=0;
	while(input)
	{
		string line;
		if(!getline(input,line))
		break;
		int i=0;
		istringstream ss(line);
		string token;
		vector <string> data_line;
		vector <string>::iterator it;
		while(getline(ss,token,' '))
		{
			data_line.push_back(token);
		}
		data.push_back(data_line);
	}
	for(int j=0;j<data.size();j++)
	{
		for(i=0;i<15;i++)
		{
			if(i == 0 || i == 2 || i == 4 || i == 10 || i == 11 || i == 12)
			{
				string::size_type sz;
				int num = stoi(data[j][i],&sz);
				if(num > arr[i])
				{
					data[j][i] = "1";
				}
				else
				{
					data[j][i] = "0";
				}
			}
			output << data[j][i] << " ";
			
		}
		output << endl;
	}

}
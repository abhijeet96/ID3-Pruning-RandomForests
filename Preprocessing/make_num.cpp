#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
#include <algorithm>
#include <cstring>
#include <cmath>
#include <map>
using namespace std;

vector <vector <string>> data;

// This file codifies the data into numbers as given by attributes file

int main()
{
	ifstream att,input;
	ofstream output;
	att.open("attrib.txt");
	input.open("bool_test.txt");
	output.open("int_test_data.txt");	
	map<string,int> m[14];
	vector <string> s[14];
	int i=0;
	data.clear();
	while(att)
	{		
		string line;
		if(!getline(att,line))
		break;
		if(i == 1 || i == 3 || i == 5 || i == 6 || i == 7 || i == 8 || i == 9 || i == 13)
		{
			istringstream ss(line);
			string token;
			vector <string>::iterator it;
			while(getline(ss,token,' '))
			{
				s[i].push_back(token);
			}
		}
		i++;
	}
	for(i=0;i<14;i++)
	{
		for(int j=0;j<s[i].size();j++)
		{
			m[i][s[i][j]] = j;
		}
	}

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
			if(i == 1 || i == 3 || i == 5 || i == 6 || i == 7 || i == 8 || i == 9 || i == 13)
			{
				data[j][i] = to_string(m[i][data[j][i]]);
			}
			output << data[j][i] << " ";
			
		}
		output << endl;
	}

}
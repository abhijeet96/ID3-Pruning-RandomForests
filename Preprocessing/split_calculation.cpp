#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;

vector <vector <string>> data;

// This program gives the optimal split given the data

class StringListCompare
{
public:
  explicit StringListCompare(int column) : m_column(column) {}
 bool operator()(const vector<string>& lhs, const vector<string>& rhs)
{
   for (int i = m_column; i < lhs.size(); ++i) {         
     if (rhs.size() <= i) return false; //rhs has fewer strings than lhs
     if (lhs[i] < rhs[i]) return true;
     if (lhs[i] > rhs[i]) return false;
     //for loop continues while the two vectors are equal
   }
   return true; //rhs must be equal or a have more strings than lhs
}
private:
  int m_column;
};

float entropy(int a,int b)
{
	float frac = (float)a/((float)(a+b));
	return -1*(frac*log(frac) + (1-frac)*log(1-frac));
}
float calculate_gain(int split,int index)
{
	int i,j;
	int count[2][2] = {0};
	string::size_type sz;
	int num;
	int out;
	for(i=0;i<data.size();i++)
	{
		num = stoi(data[i][index],&sz);
		out = stoi(data[i][14],&sz);
		if(num > split)
		{
			count[1][out]++;
		}
		else
		{
			count[0][out]++;
		}
	}
	float gain = 0;
	int zero_count = count[0][0] + count[0][1];
	int one_count = count[1][0] + count[1][1];
	float zero_frac = (float)zero_count/(float)(zero_count+one_count);
	float one_frac = 1 - zero_frac;
	gain = -1*(zero_frac*entropy(count[0][0],count[0][1]) + one_frac*entropy(count[1][0],count[1][1]));
	return gain;
}	

int main()
{
	ifstream input;
	ofstream output;
	input.open("sorted_final.txt");
	output.open("split_values.txt");	
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
	vector <int> averages;
	for(int i=0;i<12;i++)
	{
		if(i==0 || i==2 || i==4 || i==10|| i==11|| i ==12)
		{
			averages.clear();
			sort(data.begin(), data.end(), StringListCompare(i));
			string temp;
			int average = 0;
			int num1=0,num2=0;	
			string::size_type sz;
			float max_gain = -100;
			int n3=0,n4=0; 		
			for(int j=0;j<data.size()-1;j++)
			{
				num1 = stoi(data[j][i],&sz);
				num2 = stoi(data[j+1][i],&sz);
				n3 = stoi(data[j][14],&sz);
				n4 = stoi(data[j+1][14],&sz);
				if(n3 != n4)
				{
					averages.push_back((num1+num2)/2);
				}
			}
			for(int j=0;j<averages.size();j++)
			{
				float gain = calculate_gain(averages[j],i);
				if(gain > max_gain)
				{
					max_gain = gain;
					average = averages[j];
				}
			}
			output << average << " at index " << i << endl;
		}
	}	
}




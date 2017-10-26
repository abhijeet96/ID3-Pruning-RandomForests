#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
#include <algorithm>
#include <cstring>
using namespace std;

// This file replaces missing data with max_info gain attribute with missing data's classification

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

int main()
{
	ifstream input;
	ofstream output,sorted;
	output.open("preprocessed_test_data.txt");
	sorted.open("sorted_test.txt");
	input.open("training_data.txt");
	vector <vector <string>> data;
	int i = 0;
	while(input)
	{
		string line;
		if(!getline(input,line))
		break;
		istringstream ss(line);
		string token;
		vector <string> data_line;
		vector <string>::iterator it;
		while(getline(ss,token,','))
		{
			if(token == " <=50K")
			{
				token = "0";
			}
			else if(token == " >50K")
			{
				token = "1";
			}
			string::iterator end_pos = remove(token.begin(), token.end(), ' ');
			token.erase(end_pos, token.end());
			data_line.push_back(token);
			//
			//output << token << " ";
		}
		for(it = data_line.begin();it!=data_line.end();it++)
		{
			output << *it << " ";
		}
		data.push_back(data_line);
		output << endl;
	}
	 
	for(int i=0;i<12;i++)
	{
		cout << i << endl;
		sort(data.begin(), data.end(), StringListCompare(i));
		int count[2] = {0};
		string s[2];
		int temp_count[2] = {0};
		string temp = data[0][i];
		s[0] = data[0][i];
		s[1] = data[0][i];
		for(int j=1;j<data.size();)
		{
			if(data[j][i] == "?")
			{
				j++;
			}
			else if(data[j][i] == temp)
			{
				string::size_type sz;
				int num = stoi(data[j][14],&sz);
				temp_count[num]++;
				if(temp_count[0] > count[0])
				{
					count[0] = temp_count[0];
					s[0] = temp;
				}
				else if(temp_count[1] > count[1])
				{
					count[1] = temp_count[1];
					s[1] = temp;
				}
				j++;
			}
			else
			{
				temp = data[j][i];
				temp_count[0] = 0;
				temp_count[1] = 0;
			}	
		}
		for (int j = 0; j < data.size(); j++)
		{
		    if(data[j][i] == "?")
		    {
		    	string::size_type sz1;
		    	int t_no = stoi(data[j][14],&sz1);
		    	data[j][i] = s[t_no];
		    }
		}
	  	
	}
		for (int i = 0; i < data.size(); i++)
		{
		    for (int j = 0; j < data[i].size(); j++)
		    {
		        sorted << data[i][j] << " ";
		    }
		    sorted << endl;
		}	
  	
}
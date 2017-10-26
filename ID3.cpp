#include<bits/stdc++.h>

using namespace std;

/*******************  Node structure of Decision Tree *************************/
struct node
{
	vector<int> pve;
	vector<int> nve;
	int aid;
	int aval;
	int attr_cov[15];
	int label;
	struct node* children[42];		
};
int size_attr=14;  //number of attributes. also assumes target variable is at index-14

vector < vector <int> > data; // Steres the training data
vector < vector <int> > test_data; // Stores the test data
vector < vector <int> > validation_data; // Stores the validation data
vector < vector <int> > rnd_train_data; // Stores randomly generated training data
const int pp=3;

/**************** Function to store the preprocessed training data in data vector *****************/

void preprocess()
{
	ifstream input;
	ofstream output;
	input.open("train_data.txt");	
	while(input)
	{		
		string line;
		if(!getline(input,line))
		break;
		istringstream ss(line);
		string token;
		vector <string>::iterator it;
		vector <int> data_line;
		while(getline(ss,token,' '))
		{
			int num = atoi(token.c_str());
			data_line.push_back(num);
		}
		data.push_back(data_line);
	}
}

/**************** Function to store the preprocessed test data in test vector *****************/

void preprocess_test()
{
	ifstream input;
	ofstream output;
	input.open("int_test_data.txt");	
	while(input)
	{		
		string line;
		if(!getline(input,line))
		break;
		istringstream ss(line);
		string token;
		vector <string>::iterator it;
		vector <int> data_line;
		while(getline(ss,token,' '))
		{
			int num = atoi(token.c_str());
			data_line.push_back(num);
		}
		test_data.push_back(data_line);
	}
}

/**************** Function to store the preprocessed validation data in validation_data vector *****************/

void preprocess_validation()
{
	ifstream input;
	ofstream output;
	input.open("validation_data.txt");	
	while(input)
	{		
		string line;
		if(!getline(input,line))
		break;
		istringstream ss(line);
		string token;
		vector <string>::iterator it;
		vector <int> data_line;
		while(getline(ss,token,' '))
		{
			int num = atoi(token.c_str());
			data_line.push_back(num);
		}
		validation_data.push_back(data_line);
	}
}

/**************** Function to create a new decision tree node  *****************/

struct node* newnode()
{

	struct node *temp( new struct node );
	temp->aid=-1;
	temp->aval=-1;	
	
	(temp->nve).clear();
	(temp->pve).clear();
	
	temp->label=-1;
	
	for(int i=0;i<15;i++)
	{
		temp->attr_cov[i]=0;
	}	
	
	for(int i=0;i<42;i++)
	{
		temp->children[i]=NULL;
	}
	return temp;	
}

/**************** Function to calculate entropy given no. of +ve and -ve examples *****************/

double entropy(int p,int n)
{
	return -(1.0*(p/((p+n)*1.0))*log2(p/((p+n)*1.0)))-(1.0*(n/((p+n)*1.0))*log2(n/((p+n)*1.0)));
}

int attr_n[]={2,8,2,16,2,7,14,6,5,2,2,2,2,42}; // Stores the number of values each feature can take

//fisher-yates algo to shuffle an array

void randomize ( int arr[], int n )
{
    srand ( time(NULL) ); 
    
    for (int i = n-1; i > 0; i--)
    {
        int j = rand() % (i+1);        
        swap(arr[i],arr[j]);
    }
}

//returns attribute to be added in the following node
//picks 3 attributes at random sqrt(14), returns node having highes info gain among those three
// Using for Random Forests

int ret_attr_f(struct node *curr)
{	
	double ma=0;
	int ma_ind=-1;
	int p_plus_n=(int)curr->nve.size()+(int)curr->pve.size();
	int psize=(int)curr->pve.size();
	int nsize=(int)curr->nve.size();
	
	int avlble[size_attr+2];
	int avlble_ind=0;
	
	vector<int> rand_index;
	
	for(int i=0;i<size_attr;i++)
	{
		if(curr->attr_cov[i]==0)
		{
			avlble[avlble_ind++]=i;
		}
	}
	
	randomize(avlble,avlble_ind);
	
	
	for(int i=0;i<min((int)sqrt(size_attr),avlble_ind);i++)  //attr no.
	{
		double tot=entropy(psize,nsize);
		
		if(curr->attr_cov[avlble[i]]==0)
		{			
			int p=0;
			int n=0;
			for(int j=0;j<attr_n[avlble[i]];j++)  // attr ctegory
			{
				for(int k=0;k<psize;k++)
				{
					if(data[curr->pve[k]][avlble[i]]==j)  //contains +ve index to be checked for
					{						
						p++;						
					}					    
				}
				
				for(int k=0;k<nsize;k++)
				{
					if(data[curr->nve[k]][avlble[i]]==j)  //contains -ve index to be checked for
					{
						n++;											
					}					    
				}	
				
				tot-= (((p+n)*1.0)/(p_plus_n*1.0))*entropy(p,n);
							
				if(tot>ma)
				{
					ma=tot;
					ma_ind=avlble[i];
				}									
				p=0;
				n=0;						
			}
			
			//use p nd n here for each attribute category
					}		
	}
	return ma_ind;
}

/********************* Function to return id of the node with maximum info_gain *******************/

int ret_attr(struct node *curr)
{	
	double ma=0;
	int ma_ind=-1;
	int p_plus_n=(int)curr->nve.size()+(int)curr->pve.size();
	int psize=(int)curr->pve.size();
	int nsize=(int)curr->nve.size();
	for(int i=0;i<14;i++)  //attr no.
	{
		double tot=entropy(psize,nsize);
		
		if(curr->attr_cov[i]==0)
		{			
			int p=0;
			int n=0;
			for(int j=0;j<attr_n[i];j++)  // attr ctegory
			{
				for(int k=0;k<psize;k++)
				{
					if(data[curr->pve[k]][i]==j)  //contains +ve index to be checked for
					{						
						p++;						
					}					    
				}
				
				for(int k=0;k<nsize;k++)
				{
					if(data[curr->nve[k]][i]==j)  //contains +ve index to be checked for
					{
						n++;											
					}					    
				}	
				
				tot-= (((p+n)*1.0)/(p_plus_n*1.0))*entropy(p,n);
							
				if(tot>ma)
				{
					ma=tot;
					ma_ind=i;
				}									
				p=0;
				n=0;						
			}
			
			//use p nd n here for each attribute category
					}		
	}
	return ma_ind;
}

int ct=0; // counts number of nodes in tree

/****************** Function to add the next node at given head position ******************/

void add_nodes(struct node * head,int par_maj)
{
	if(head->pve.size()==0||head->nve.size()==0)
	{
		if(head->pve.size()==0 && head->nve.size()==0)
		{
			head->label=par_maj;
			return;
		}
		else if(head->nve.size()==0)
		{
			head->label=1;
			return;
		}
		else
		{
			head->label=0;
			return;
		}
	}
	
	int attr=ret_attr(head);
	
	if(attr==-1)
	{
		head->label=par_maj;
		return;
	}
	
	head->attr_cov[attr]=1;
	
	int fl=-1;   // par_maj;

	if(head->pve.size()>head->nve.size())
	{
		fl=1;
	}
	else
	{
		fl=0;
	}
	
	int psize=(int)head->pve.size();
	int nsize=(int)head->nve.size();
	
	for(int j=0;j<attr_n[attr];j++)
	{
		struct node * add_this= newnode();
		
		for(int k=0;k<psize;k++)
		{
			if(data[head->pve[k]][attr]==j)  //contains +ve index to be checked for
			{						
				add_this->pve.push_back(head->pve[k]);					
			}					    
		}
		
		for(int k=0;k<nsize;k++)
		{
			if(data[head->nve[k]][attr]==j)  //contains -ve index to be checked for
			{
				add_this->nve.push_back(head->nve[k]);												
			}					    
		}				
		
		for(int k=0;k<size_attr;k++)   //checking attributes
		{
			add_this->attr_cov[k]=head->attr_cov[k];
		}
		
		add_this->aid=attr;  // marking attribute id
		add_this->aval=j;    // marking attr value
		
		head->children[j]=add_this;	
		ct++;
		add_nodes(add_this,fl);			
	}
	
	
}

void add_nodes_f(struct node * head,int par_maj)
{
	if(head->pve.size()==0||head->nve.size()==0)
	{
		if(head->pve.size()==0 && head->nve.size()==0)
		{
			head->label=par_maj;
			return;
		}
		else if(head->nve.size()==0)
		{
			head->label=1;
			return;
		}
		else
		{
			head->label=0;
			return;
		}
	}
	
	int attr=ret_attr_f(head);
	
	if(attr==-1)
	{
		head->label=par_maj;
		return;
	}
	
	head->attr_cov[attr]=1;
	
	int fl=-1;   // par_maj;

	if(head->pve.size()>head->nve.size())
	{
		fl=1;
	}
	else
	{
		fl=0;
	}
	
	int psize=(int)head->pve.size();
	int nsize=(int)head->nve.size();
	
	for(int j=0;j<attr_n[attr];j++)
	{
		struct node * add_this= newnode();
		
		for(int k=0;k<psize;k++)
		{
			if(data[head->pve[k]][attr]==j)  //contains +ve index to be checked for
			{						
				add_this->pve.push_back(head->pve[k]);					
			}					    
		}
		
		for(int k=0;k<nsize;k++)
		{
			if(data[head->nve[k]][attr]==j)  //contains -ve index to be checked for
			{
				add_this->nve.push_back(head->nve[k]);												
			}					    
		}				
		
		for(int k=0;k<size_attr;k++)   //checking attributes
		{
			add_this->attr_cov[k]=head->attr_cov[k];
		}
		
		add_this->aid=attr;  // marking attribute id
		add_this->aval=j;    // marking attr value
		
		head->children[j]=add_this;	
		ct++;
		add_nodes_f(add_this,fl);			
	}
	
	
}

// assuming test_data. this fn takes one instance of test_data and  returns predicted_val

int ctt=0;

int predicted_val(vector<int> tdata1,struct node * head)
{
		int fl=0;
		for(int i=0;i<42;i++)
		{
			if(head->children[i]!=NULL)
			{
				if(tdata1[head->children[i]->aid]==head->children[i]->aval)
				{
					return predicted_val(tdata1,head->children[i]);
				}
				fl=1;
			}
		}
		
		if(fl==0)
		{
			return head->label;
		}
}

// Calculates the accuracy of the tree on test data

double calc_accuracy_test(struct node * head)
{
	int tcct=test_data.size();
	int cct=0;
	for(int i=0;i<test_data.size();i++)
	{
		if(predicted_val(test_data[i],head)==test_data[i][14])
		cct++;	
	}
	
	return (cct*1.0)/(tcct*1.0);
}

//accuracy test of forest different from that of id3
double calc_accuracy_test_f(struct node * forest[],int n)
{	
	int tcct=test_data.size();
	int cct=0;
	
	for(int i=0;i<test_data.size();i++)
	{
		int pred_p=0;
		int pred_n=0;
		
		for(int j=0;j<n;j++)
		{			
			if(predicted_val(test_data[i],forest[j])==1)
			pred_p++;
			else
			pred_n++;
		}
		
		if(pred_p>=pred_n)
		{
			if(test_data[i][size_attr]==1)
			cct++;
		}
		else
		{
			if(test_data[i][size_attr]==0)
			cct++;
		}
	
	}	
	return (cct*1.0)/(tcct*1.0);
}

//populates rnd_train_data with random samples from training data. n samples
void get_rnd_train_data()
{
	vector<int> rand_index;
	int sz=data.size();
	
	srand ( time(NULL) ); 
	
	for(int i=0;i<sz;i++)
	{
		int y=rand()%sz;
		rand_index.push_back(y);
	}
	
	rnd_train_data.clear();
	
	for(int i=0;i<sz;i++)
	{
		rnd_train_data.push_back(data[rand_index[i]]);
	}
}

void build_random_forest(struct node * forest[],int size)
{
	for(int i=0;i<size;i++)
	{
		get_rnd_train_data();  //rnd_train_data now has random train data
		add_nodes_f(forest[i],((forest[i]->pve.size()>forest[i]->nve.size())?1:0));	
		cout<<ct<<endl;  //o/p of nodes in each forest
		ct=0;
	}
}


ofstream output;

// Calculates the accuracy of the tree on validation data

double calc_accuracy_validation(struct node * head)
{
	int tcct=validation_data.size();
	int cct=0;
	for(int i=0;i<validation_data.size();i++)
	{
		if(predicted_val(validation_data[i],head)==validation_data[i][size_attr])
		cct++;	
	}
	return (cct*1.0)/(tcct*1.0);
}

double max_valdn_accuracy;
double prev_max_valdn_accuracy;
struct node *node_to_rem;
struct node *t;

// Calculates the accuracy by removing the curr node, restores the node and returns the accuracy

double reduced_node(struct node * curr)
{
	struct node * temp[42];
	int tlabel;
	for(int i=0;i<42;i++)
	{
		temp[i]=curr->children[i];
	}

	tlabel=curr->label;
	
	for(int i=0;i<42;i++)
	{
		curr->children[i]=NULL;
	}

	curr->label=((curr->pve.size()>curr->nve.size())?1:0);

	double accuracy=calc_accuracy_validation(t);
		
	curr->label=tlabel;

	for(int i=0;i<42;i++)
	{
		curr->children[i]=temp[i];
	}
	
	return accuracy;
}

// Counts the number of nodes in the subtree rooted at the node

int count(struct node* head)
{
	int temp = 0;
	if(head == NULL)
		return 0;
	temp++;
	for(int i=0;i<42;i++)
	{
		temp += count(head->children[i]);
	}
	return temp;
}

// Counts the number of nodes in the subtree rooted at the node
// Stores the node with best_pruned accuracy and stores in node_to_rem

void acc_inorder(struct node* head)
{
	if(head == NULL)
		return;
	
	double accuracy=reduced_node(head);

	if(accuracy>max_valdn_accuracy)
	{
		max_valdn_accuracy=accuracy;
		node_to_rem=head;
	}		

	for(int i=0;i<42;i++)
	{
		acc_inorder(head->children[i]);
	}
}

// Takes the node stored in node_to_rem and removes it

void find_and_prune()
{
	if(node_to_rem==NULL)
		return;
	for(int i=0;i<42;i++)
	{
		node_to_rem->children[i]=NULL;
	}
	node_to_rem->label=((node_to_rem->pve.size()>node_to_rem->nve.size())?1:0);
	output << "Node to be removed has " << node_to_rem->pve.size() + node_to_rem->nve.size() << " nodes under it at validation accuracy " << max_valdn_accuracy << "." << endl;
	node_to_rem=NULL;
	return;
}

// Reduce nodes till validation set accuracy increases
// Pruning method 1 - selects best of all

void reduced_prune(struct node * head)
{
	//get node having max validation-set-accuracy greater than before
	while(prev_max_valdn_accuracy < max_valdn_accuracy)
	{
		prev_max_valdn_accuracy = max_valdn_accuracy;
		acc_inorder(head);
		find_and_prune();
		output << count(t) - 1 << endl;
		output << calc_accuracy_test(t) << endl;
	}
	output << "Max validation accuracy is " << prev_max_valdn_accuracy << endl;
}

// Returns 1 if present node is a leaf else returns 0

int is_leaf(struct node* t)
{
	if(t==NULL)
		return 1;
	if(t->label != -1)
	{
		return 1;
	}
	return 0;
}

double r_node(struct node * curr)
{
	struct node * temp[42];
	int tlabel;
	for(int i=0;i<42;i++)
	{
		temp[i]=curr->children[i];
	}

	tlabel=curr->label;
	
	for(int i=0;i<42;i++)
	{
		curr->children[i]=NULL;
	}

	curr->label=((curr->pve.size()>curr->nve.size())?1:0);

	double accuracy=calc_accuracy_validation(t);
		
	curr->label=tlabel;

	for(int i=0;i<42;i++)
	{
		curr->children[i]=temp[i];
	}
	return accuracy;
}


void rem_prune()
{
	for(int i=0;i<42;i++)
	{
		node_to_rem->children[i]=NULL;
	}
	node_to_rem->label=((node_to_rem->pve.size()>node_to_rem->nve.size())?1:0);
	output << "Node to be removed has " << node_to_rem->pve.size() + node_to_rem->nve.size() << "nodes under it. At accuracy " << max_valdn_accuracy << endl;
	node_to_rem=NULL;
	output<<"Node removed"<<endl;
	return;
}

void update_inorder(struct node* head)
{
	
	if(head == NULL)
		return;

	if(is_leaf(head))
	{
		return ;
	}
	
	int temp = 0;

	for(int i=0;i<42;i++)
	{
		if(!is_leaf(head->children[i]))
		{
			temp = 1;
			break;
		}
	}
	if(temp != 0)
	{
		for(int i=0;i<42;i++)
		{
			update_inorder(head->children[i]);
		}
	}
	double accuracy=r_node(head);
	if(accuracy>max_valdn_accuracy)
	{
		max_valdn_accuracy=accuracy;
		node_to_rem=head;
		rem_prune();
		output << calc_accuracy_test(t) << endl;
	}

}

// Prune method 2 -------> Prunes leaves first

void prune2(struct node * head)
{
	update_inorder(head);
	output << "Max acc is " << max_valdn_accuracy << endl;
}
ofstream out;

// Prints the inorder traversal of the decision tree

void print_tree(struct node* head)
{
	
	if(head == NULL)
		return;
	out << "{ Start: ";
	out << "attribut_id is " << head->aid << " and attribute_val is " << head->aval << " and label/classification is " << head->label << endl;
	for(int i=0;i<42;i++)
	{
		print_tree(head->children[i]);
	}
	out << endl;
	out << "End }";
	out << endl;
}

int main()
{
	preprocess();
	preprocess_test();

	preprocess_validation();
	
	output.open("po.txt");
	out.open("pt.txt");

	int p=0,n=0;
	struct node *head=newnode();
	
	for(int i=0;i<data.size();i++)
	{
		if(data[i][size_attr]==1)
		{
			p++;			
			(head->pve).push_back(i);
			
		}
		else
		{
			n++;
			(head->nve).push_back(i);			
		}
	}	
	
	add_nodes(head,(p>n)?1:0);

	t = head;

	output<<ct<<endl;

	output << "Tree size before pruning is " << count(head) - 1 << endl;

	print_tree(head);

	output <<"Test accuracy before Pruning is "<< calc_accuracy_validation(head)<<endl;

	max_valdn_accuracy=calc_accuracy_validation(head);

	int forest_size=100;  // change forest_size to make forest of diff # of trees
	//lines upto build_forest can be added in an another function

	struct node * forest[forest_size];
	
	for(int i=0;i<forest_size;i++)
	{
		forest[i]=newnode();
	}
	
	vector<int> pos; // to populate intial +,- for head node
	vector<int> neg;	
	
	for(int i=0;i<data.size();i++)
	{
		if(data[i][size_attr]==1)
		{
			p++;			
			pos.push_back(i);
			
		}
		else
		{
			n++;
			neg.push_back(i);			
		}
	}	
	
	for(int j=0;j<forest_size;j++)
	{
		for(int i=0;i<pos.size();i++)
		{
			forest[j]->pve.push_back(pos[i]);
		}
		
		for(int i=0;i<neg.size();i++)
		{
			forest[j]->nve.push_back(neg[i]);
		}		
	}	

	build_random_forest(forest,forest_size);
	
	cout<<endl;

	cout<<calc_accuracy_test_f(forest,forest_size)<<endl;

	/*
	reduced_prune(head); // Pruning method 1

	output << endl;
	
	output << "Test accuracy after Pruning method 1 is " << endl;

	output<<calc_accuracy_test(head)<<endl;

	output << "Tree size after pruning is " << count(head) -1 << endl;

	out << endl;

	out << "Tree after Pruning is : " << endl;
	
	print_tree(head); */

	
	prune2(head); // Pruning method 2

	output << endl;
	
	output << "Test accuracy after Pruning method 2 is " << endl;

	output<<calc_accuracy_test(head)<<endl;

	output << "Tree size after pruning is " << count(head) -1 << endl;

	out << endl;

	out << "Tree after Pruning is : " << endl;
	
	print_tree(head);

	

	return 0;
}

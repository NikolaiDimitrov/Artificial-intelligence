#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<cmath>


using namespace std;


class Node
{
    public:
        char dataset[3][3];
        int score;
        bool isLeaf;
        vector<Node> childrens;
    Node(){
    	for (int i = 0; i < 3; ++i)
    	{
    		for (int k = 0; k < 3; ++k)
    		{
    			dataset[i][k] = ' ';
    		}
    	}
    	isLeaf = false;
    }
    Node(int index,char player,char field[3][3])
    {
    	for (int i = 0; i < 3; ++i)
    	{
    		for (int k = 0; k < 3; ++k)
    		{
    			dataset[i][k] = field[i][k];
    		}
    	}
    	dataset[index/3][index%3] = player;
    	score = checkForWinner(player);
    	if(boardIsFull() || (score != -1))
    		isLeaf = true;
    	else
    		isLeaf = false;
    }

    Node& operator=(Node other)
    {
      	this->score = other.score;
      	this->isLeaf = other.isLeaf;
      	for (int i = 0; i < 3; ++i)
    	{
    		for (int k = 0; k < 3; ++k)
    		{
    			this->dataset[i][k] = other.dataset[i][k];
    		}
    	}
        return *this;
    }
    void printBoard()
    {    
    	cout << "-------"	<< endl;
    	for (int i = 0; i < 3; ++i)
    	{
    		cout << "|";
    		for (int k = 0; k < 3; ++k)
    		{
    			cout << dataset[i][k] << "|";
    		}
    		cout << endl;
    	}
    	cout << "-------"	<< endl;
    }
    int checkForWinner(char player)
    {
    	for (int row = 0; row<3; row++) 
    	{ 
	        if (dataset[row][0]==dataset[row][1] && 
	            dataset[row][1]==dataset[row][2]) 
	        { 
	            if (dataset[row][0]==player) 
	                return +10; 
	            else if(dataset[row][0]!=' ')
	                return -10; 
	        } 
    	} 
	    for (int col = 0; col<3; col++) 
	    { 
	        if (dataset[0][col]==dataset[1][col] && 
	            dataset[1][col]==dataset[2][col]) 
	        { 
	            if (dataset[0][col]==player) 
	                return +10; 
	            else if(dataset[0][col]!=' ')
	                return -10; 
	        } 
	    } 
	    if (dataset[0][0]==dataset[1][1] && dataset[1][1]==dataset[2][2]) 
	    { 
	        if (dataset[0][0]==player) 
	            return +10; 
	        else if(dataset[0][0]!=' ')
	            return -10; 
	    } 
	  
	    if (dataset[0][2]==dataset[1][1] && dataset[1][1]==dataset[2][0]) 
	    { 
	        if (dataset[0][2]==player) 
	            return +10; 
	        else if(dataset[0][2]!=' ')
	            return -10; 
	    } 
	  
	  return -1;
    }

    bool boardIsFull() 
	{ 
	    for (int i = 0; i<3; i++) 
	        for (int j = 0; j<3; j++) 
	            if (dataset[i][j]==' ') 
	                return false; 
	    return true; 
	} 
    void generateTree(char player)
    {    	    	
    	if(this->isLeaf)
	    {
	    	return;
	    }
    	for (int i = 0; i < 9; ++i)
    	{
	    	if(dataset[i/3][i%3] == ' ')
	    	{
	    		Node child(i,player,dataset);
	    		if(player == 'x')
	    			child.generateTree('o');
	    		else
	    			child.generateTree('x');
	    		this->childrens.push_back(child);
	    	}
	    }
    }	
};
int globalIndex;
Node minMaxAlgorithm(Node tree,int alpha,int beta,bool maxPlayer,int depth)
{
	if(tree.isLeaf)
	{
		return tree;
	}
	if(maxPlayer)
	{
		int v = -100;
		Node n,retNode;
		for (int i = 0; i < tree.childrens.size(); ++i)
		{
			n = minMaxAlgorithm(tree.childrens[i],alpha,beta,false,depth+1);
			if((n.score != -1) && (n.score < 11))
			{
				if(n.score > v)
				{
					v = n.score;
					retNode = n;
					if(depth == 1)
					{
						globalIndex = i;
					}
				}
				if(v > alpha)
				{
					alpha = v;
				}
			}
			if(beta <= alpha)
				break;
		}
		return retNode;
	}
	else
	{
		int v = 100;
		Node n,retNode;
		for (int i = 0; i < tree.childrens.size(); ++i)
		{
			n = minMaxAlgorithm(tree.childrens[i],alpha,beta,true,depth+1);
			if((n.score != -1) && (n.score < 11))
			{
				if(n.score < v)
				{
					v = n.score;
					retNode = n;
					if(depth == 1)
					{
						globalIndex = i;
					}
				}
				if(v < beta)
				{
					beta = v;
				}
			}
			if(beta <= alpha)
				break;
		}
		return retNode;
	}
}
int main()
{	
	Node n1;
	// n1.dataset[0][1] = 'x';
	// n1.dataset[1][2] = 'x';
	// n1.dataset[2][1] = 'o';
	// n1.dataset[1][1] = 'o';
	// n1.dataset[2][2] = 'x';
	// n1.printBoard();
	// n1.generateTree('o');
	// n3 = minMaxAlgorithm(n1,-100,100,true,1);
	
	// n3.printBoard();
	// cout << globalIndex << endl;
	// //Node n2(6,'o',n1.dataset);
	// //n2.printBoard();

	// //n2.generateTree('o');
	// //cout << n2.isLeaf << endl;
	// //n3 = minMaxAlgorithm(n2,-100,100,true);

	// //cout << "Hello" << endl;
	// //n3.printBoard();
	return 0;
}

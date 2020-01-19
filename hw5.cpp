#include<iostream>
#include<string>
#include<boost/algorithm/string.hpp>
#include<fstream>
#include<vector>
#include<cmath>


using namespace std;

class Data{
public:
	double probYY;
	double probYN;
	double probNY;
	double probNN;
	Data()
	{
		probYY = 0;
		probYN = 0;
		probNY = 0;
		probNN = 0;
	}
};

vector<Data> learnFromExamples(vector<vector<string>> str,int& republicansCounter,int& democratsCounter)
{
	vector<Data> arrVector;
	Data arr[16];
	for(int i = 0;i < 16;i++)
	{
		arrVector.push_back(arr[i]);
	}
	for(int k = 0;k<str.size();k++)
	{
		if(str[k][0] == "republican")
		{
			republicansCounter++;
		}
		else if(str[k][0] == "democrat")
		{
			democratsCounter++;
		}
		for(int i = 0,attrIndex =1;i < 16;i++,attrIndex++)	//TODO change 16 with define
		{
			if(str[k][0] == "republican")
			{
				if(str[k][attrIndex] == "y")
				{
					arrVector[i].probYY = arrVector[i].probYY+1; 
				}
				else if(str[k][attrIndex] == "n")
				{
					arrVector[i].probYN = arrVector[i].probYN+1; 
				}
			}
			else if(str[k][0] == "democrat")
			{
				if(str[k][attrIndex] == "y")
				{
					arrVector[i].probNY = arrVector[i].probNY+1; 
				}
				else if(str[k][attrIndex] == "n")
				{
					arrVector[i].probNN = arrVector[i].probNN+1; 
				}
			}
		}

	}
	for(int i = 0;i < 16;i++)
	{
		arrVector[i].probYY = arrVector[i].probYY / republicansCounter;
		arrVector[i].probYN = arrVector[i].probYN / republicansCounter; 
		arrVector[i].probNY = arrVector[i].probNY / democratsCounter; 
		arrVector[i].probNN = arrVector[i].probNN / democratsCounter;  
	}
	return arrVector;

}
bool makeSuggestion(vector<Data> data,vector<string> attributes,int demoCounter,int repCounter)
{
	double repProb = 1;
	double demProb = 1;
	for(int i = 0,attrIndex =1;i < 16;i++,attrIndex++)
	{
		if(attributes[attrIndex] == "y")
		{
			repProb += log2(data[i].probYY);
			demProb += log2(data[i].probNY);
		}
		else if(attributes[attrIndex] == "n")
		{
			repProb += log2(data[i].probYN);
			demProb += log2(data[i].probNN);
		}
	}
	if((demProb < repProb) && (attributes[0] == "republican"))
		return true;
	else if((demProb > repProb) && (attributes[0] == "democrat"))
		return true;
	else
		return false;
}

int main()
{
	ifstream input("house-votes-84.data");
	char* pch;
	int numberOfExamples = 0,lineCounter = 0,democratsCounter = 0,republicansCounter = 0,succsessCounter = 0;
	vector<Data> arrVector;
	vector<vector<string>> allExamples;
	vector<vector<string>> currentSet;

	for(string line; getline( input, line );)
	{	
		vector<string> str;
		boost::split(str,line,boost::is_any_of(","));
		allExamples.push_back(str);
	}
	cout << "allExamples " << allExamples.size() << endl; 
	for(int i = 1,index = 0;i <= 10;i++)
	{
		for (int k = 0; k < allExamples.size()/10; k++,index++)
		{
			currentSet.push_back(allExamples[index]);
		}
		if(i == 1)
		{
			arrVector = learnFromExamples(currentSet,republicansCounter,democratsCounter);
		}
		else
		{
			for (int p = 0; p < currentSet.size() ; ++p)
			{
				if(makeSuggestion(arrVector,currentSet[p],republicansCounter,democratsCounter))
					succsessCounter++;
			}
			cout << succsessCounter << " out of: " << currentSet.size() << endl;
			succsessCounter = 0;
		}
		currentSet.clear();
	}	
	return 0;
}

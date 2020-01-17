#include <iostream>
#include <cstdlib> 
#include <ctime>   

#define TOURNAMENT_NUMBER 5
#define POPULATION_SIZE 100
using namespace std;

class Individual
{
	public:
	int fitness;
	int* arr;
	static int size;
	
	Individual()
	{
		fitness = -1;
		arr = NULL;
	}

	~Individual()
	{
		delete [] arr;
	}

	Individual(int* arr,int* target)
	{
		fitness = 0;
		this->arr = new int[size];
		for(int i=0;i<size;i++)
		{
			this->arr[i] = arr[i];
		}
		calculateFitness(target); 
	}
	Individual(const Individual &ind)
	{
		this->arr = new int[size];
		for(int i=0;i<size;i++)
		{
			this->arr[i] = ind.arr[i];
		}
		this->fitness = ind.fitness;
	}
	Individual& operator=(const Individual& ind)
	{
		this->arr = new int[size];
		for(int i=0;i<size;i++)
		{
			this->arr[i] = ind.arr[i];
		}
		this->fitness = ind.fitness;
		return *this;
	}
	void generateRandomValues(int* target)
	{
		this->fitness = 0;
		this->arr = new int[size];
		getRandomArr(this->arr);
		calculateFitness(target); 
	}
	void calculateFitness(int* target)
	{
		for(int i = 0;i < size;i++)
		{
			if(arr[i] == target[i])
			{
				fitness++;
			}
		}
	}
	static void getRandomArr(int* arr)
	{
	bool valueIsTaken = false;
	int randomNumber;
	for(int i = 0;i<Individual::size;i++)
	{
		arr[i] = -1;
		do
		{
			valueIsTaken = false;
			randomNumber = rand() % size;
			for(int k = 0;k < i;k++)
			{
				if(arr[k] == randomNumber)
				{
					valueIsTaken = true;
					break;
				}
			}
			if(!valueIsTaken)
			{
				arr[i] = randomNumber;	
			}
		}while(arr[i] == -1);
	}
	}
 	bool operator > (const Individual& ind) {
         if(fitness > ind.fitness) {
            return true;
         }
         return false;
      }
	bool operator == (Individual &ind) {
		for(int i = 0;i< ind.size;i++)
		{
			if(this->arr[i] != ind.arr[i])
				return false;
		}
		return true;
	}
};

ostream & operator << (ostream &out,  Individual &ind)
{
	for(int i=0;i<Individual::size;i++)
    	out << ind.arr[i] << " ";
	out << "Fitness is " << ind.fitness << endl; 
    return out;
}

void swap(Individual* a, Individual* b) 
{ 
    Individual t = *a; 
    *a = *b; 
    *b = t; 
} 
void sortPopulation(Individual* population)
{
	int i, j, min_idx;  
  
    for (i = 0; i < POPULATION_SIZE; i++)  
    {  
        min_idx = i;  
        for (j = i+1; j < POPULATION_SIZE; j++)  
        	if (population[j] > population[min_idx])  
            	min_idx = j;   
        swap(population[min_idx],population[i]);  
    }  
	/*for(int i = 0;i < 100;i++)
	{
		for(int k = i+1;k < 100;k++)
		{
			if(p[i].fitness > p[k].fitness)
			{
				cout << "Here" << endl;
				swap(p[i],p[k]);
			}
		}
	}
	for(int i =0;i < 100;i++)
	{
		cout << p[i];
	}*/
}

int getTournamentWinner(Individual* population,int* selectedIndividuals)
{	int maxFitness = population[selectedIndividuals[0]].fitness,id = selectedIndividuals[0];
	for(int i=0;i<TOURNAMENT_NUMBER;i++)
	{
		if(population[selectedIndividuals[i]].fitness > maxFitness)
		{
			maxFitness = population[selectedIndividuals[i]].fitness;
			id = selectedIndividuals[i];
		}
	}
	return id;
}

Individual tournamentSelection(Individual* population)
{
	int brackets[TOURNAMENT_NUMBER],i = 0;
	bool valueIsTaken;
	do
	{
		valueIsTaken = false;
		int randomNumber = rand() % POPULATION_SIZE;
		for(int k = 0;k < i;k++)
		{
			if(brackets[k] == randomNumber)
			{
				valueIsTaken = true;
				break;
			}
		}
		if(!valueIsTaken)
		{
			brackets[i] = randomNumber;
			i++;	
		}
	}while(i < TOURNAMENT_NUMBER);
	
	int winnerId = getTournamentWinner(population,brackets);
	return population[winnerId];
}



bool getGenes(int* firstParrent,int* secondParrent,int* firstParrentGenes,int* secondParrentGenes,int index,bool flag)
{
	if(firstParrent[index] == -1)
		return flag;

	firstParrentGenes[index] = firstParrent[index];
	firstParrent[index] = -1;

	secondParrentGenes[index] = secondParrent[index];
	secondParrent[index] = -1;
	 	
	for(int i = 0;i < Individual::size;i++)
	{
		if(secondParrentGenes[index] == firstParrent[i])
			getGenes(firstParrent,secondParrent,firstParrentGenes,secondParrentGenes,i,flag);
	}
	return !flag;
}
void cycleCrossover(Individual parent1,Individual parent2,int* child1,int* child2)
{
	bool reverseChildrens = true;
	for(int i = 0;i < Individual::size;i++)
	{
		if(reverseChildrens)
			reverseChildrens = getGenes(parent1.arr,parent2.arr,child1,child2,i,reverseChildrens);
		else
			reverseChildrens = getGenes(parent1.arr,parent2.arr,child2,child1,i,reverseChildrens);
 	}
}
void onePointCrossover(Individual parent1,Individual parent2,int* child1,int* child2)
{
	int randomNumber = rand() % Individual::size;
	bool flag = true;
	for (int i = 0; i < randomNumber; ++i)
	{
		child1[i] = parent1.arr[i];
		child2[i] = parent2.arr[i];
	}
	for (int i = randomNumber,index = 0;i < Individual::size; i++,index++)
	{
		for (int k = 0; k < i; k++)
		{
			if(parent2.arr[index] == child1[k])
			{
				flag = false;
				break;
			}
		}
		if(flag)
		{
			child1[i] = parent2.arr[index];
		}
		else
		{
			flag = true;
		}
	}
	for (int i = randomNumber,index = 0;i < Individual::size; i++,index++)
	{
		for (int k = 0; k < i; k++)
		{
			if(parent1.arr[index] == child2[k])
			{
				flag = false;
				break;
			}
		}
		if(flag)
			child2[i] = parent1.arr[index];
		else
			flag = true;
	}
}
void mutation(Individual* i)
{
		int randomGene = rand() % Individual::size,secondGene = 0;
		do
		{
			secondGene = rand() % Individual::size;

		}while(randomGene == secondGene);
		int tmp = i->arr[randomGene];
		i->arr[randomGene] = i->arr[secondGene];
		i->arr[secondGene] = tmp;
}

void geneticSearch(Individual* population,int size,int* target)
{
	int index = 0;
	Individual childrens[POPULATION_SIZE+50];
	int* child1 = new int[size];
	int* child2 = new int[size];
	for(int i = 0;i < 50;i++)
	{
		Individual parent1 = tournamentSelection(population);
		Individual parent2;
		do{		
			parent2 = tournamentSelection(population);
		}
		while(parent1 == parent2);
		
		cycleCrossover(parent1,parent2,child1,child2);
	
		Individual c1(child1,target);
		Individual c2(child2,target);
		
		
		
		childrens[index++] = c1;
		childrens[index++] = c2;
		
	}
	for(int i =0;i<100;i++)
	{
		childrens[i+50] = population[i]; 
	}
	sortPopulation(childrens);
	for(int i =0;i<100;i++)
	{
		population[i] = childrens[i]; 
	}
	delete [] child1;
	delete [] child2;
}
int Individual::size = 0;
int main()
{
	srand(time(NULL));
	Individual::size = 20;
	int target[20];
	Individual::getRandomArr(target);	
	cout << "Target: " << endl; 
	for(int i = 0;i < 20;i++)
	{
		cout << target[i] << " ";
	}
	cout << endl;
	Individual population[POPULATION_SIZE];
	
	for(int i = 0;i < POPULATION_SIZE;i++)
	{
		population[i].generateRandomValues(target);
	}
	sortPopulation(population);
	for(int i = 0;i < 30000;i++)
	{	
		geneticSearch(population,Individual::size,target);
	}	
	for(int i = 0;i < 100;i++)
	{
		cout << population[i];
	}
	// int arr[10];
	// int arr2[10];
	// int asd[10] = {1,2,3,4,5,6,7,8,9,10};
	// int asd2[10] = {10,9,8,7,6,5,4,3,2,1};
	// onePointCrossover(asd,asd2,arr,arr2);
	// for (int i = 0; i < 10; ++i)
	// {
	// 	cout << arr[i] << " ";
	// }
	// cout << endl;
	// /*for (int i = 0; i < 10; ++i)
	// {
	// 	cout << arr2[i] << " ";
	// }*/
	return 0;
}	

#include <iostream>
#include <cstdlib> 
#include <ctime>   

#define TOURNAMENT_NUMBER 5
#define POPULATION_SIZE 100
#define UNINITIALIZED_TOWN_NAME -7
#define NUMBER_OF_ITERATIONS 50000
#define MUTATION_ON_ITERATIION 10
using namespace std;

class Town{
	public:
	int x;
	int y;
	int name;

	Town()
	{
		this->x = -1;
		this->y = -1;
		this->name = -1;
	}

	Town(int x, int y, int name)
	{
		this->x = x;
		this->y = y;
		this->name = name;
	}
	friend bool operator== (const Town &t1, const Town &t2);
};

bool operator== (const Town &t1, const Town &t2) {
	return t1.name == t2.name || (t1.x == t2.x && t1.y == t2.y);
};	

class Individual
{
	public:
	int fitness;
	Town* arr;
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

	Individual(Town* arr)
	{
		fitness = 0;
		this->arr = new Town[size];
		for(int i=0;i<size;i++)
		{
			this->arr[i] = arr[i];
		}
		calculateFitness(); 
	}
	Individual(const Individual &ind)
	{
		this->arr = new Town[size];
		for(int i=0;i<size;i++)
		{
			this->arr[i] = ind.arr[i];
		}
		this->fitness = ind.fitness;
	}
	Individual& operator=(const Individual& ind)
	{
		this->arr = new Town[size];
		for(int i=0;i<size;i++)
		{
			this->arr[i] = ind.arr[i];
		}
		this->fitness = ind.fitness;
		return *this;
	}
	void generateRandomValues()
	{
		this->fitness = 0;
		this->arr = new Town[size];
		getRandomArrOfCities();
		calculateFitness(); 
	}
	void calculateFitness()
	{
		for (int i = 0; i < size-1; ++i)
		{
			fitness += (abs(arr[i].x - arr[i+1].x) + abs(arr[i].y - arr[i+1].y)); 
		}
	}

	void getRandomArrOfCities()
	{
		bool sameTown = false;
		int randomX;
		int randomY;
		int randomName;
		for (int i = 0; i < Individual::size; ++i)
		{
			arr[i].name = UNINITIALIZED_TOWN_NAME;
			do
			{
				sameTown =false;
				// TO DO random x and Y coordinates not size
				randomX = rand() % size;
				randomY = rand() % size;
				randomName = rand() % size;
				Town newTown(randomX,randomY, randomName);
				for (int k = 0; k < i; ++k)
				{
					if(arr[k] == newTown)
					{
						sameTown = true;
						break;
					}
				}
				if(!sameTown)
				{
					arr[i]=newTown;
				}
			}while(arr[i].name == UNINITIALIZED_TOWN_NAME);
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
         if(fitness < ind.fitness) {
            return true;
         }
         return false;
      }
	bool operator == (Individual &ind) {
		for(int i = 0;i< ind.size;i++)
		{
			if(!(this->arr[i] == ind.arr[i]))
				return false;
		}
		return true;
	}
};

ostream & operator << (ostream &out,  Individual &ind)
{
	for(int i=0;i<Individual::size;i++)
    	out << "(" << ind.arr[i].name << "," << ind.arr[i].x << "," << ind.arr[i].y << ")" << " ";
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



bool getGenes(Town* firstParrent,Town* secondParrent,Town* firstParrentGenes,Town* secondParrentGenes,int index,bool flag)
{
	if(firstParrent[index].name == -1)
		return flag;

	firstParrentGenes[index] = firstParrent[index];
	firstParrent[index].name = -1;

	secondParrentGenes[index] = secondParrent[index];
	secondParrent[index].name = -1;
	 	
	for(int i = 0;i < Individual::size;i++)
	{
		if(secondParrentGenes[index].name == firstParrent[i].name)
			getGenes(firstParrent,secondParrent,firstParrentGenes,secondParrentGenes,i,flag);
	}
	return !flag;
}
void cycleCrossover(Individual parent1,Individual parent2,Town* child1,Town* child2)
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
void onePointCrossover(Individual parent1,Individual parent2,Town* child1,Town* child2)
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
		Town tmp = i->arr[randomGene];
		i->arr[randomGene] = i->arr[secondGene];
		i->arr[secondGene] = tmp;
}

void geneticSearch(Individual* population,int size)
{
	int index = 0;
	Individual childrens[POPULATION_SIZE+50];
	Town* child1 = new Town[size];
	Town* child2 = new Town[size];
	for(int i = 0;i < 50;i++)
	{
		Individual parent1 = tournamentSelection(population);
		Individual parent2;
		do{		
			parent2 = tournamentSelection(population);
		}
		while(parent1 == parent2);
		
		cycleCrossover(parent1,parent2,child1,child2);
	
		Individual c1(child1);
		Individual c2(child2);
		
		
		
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
	Individual population[POPULATION_SIZE];
	for(int i = 0;i < POPULATION_SIZE;i++)
	{
		population[i].generateRandomValues();
	}

	sortPopulation(population);
	/*for (int i = 0; i < POPULATION_SIZE; ++i)
	{
		cout << population[i];
	}*/
	cout << population[0] << endl << endl;
	for(int i = 0;i < NUMBER_OF_ITERATIONS;i++)
	{	
		if(i==10 || i==17859 || i==26578 || i==36554){
			cout<<population[0];
		}

		geneticSearch(population,Individual::size);
		if(i % MUTATION_ON_ITERATIION == 0)
		{
			int randomNumber = rand() % POPULATION_SIZE;
			mutation(&population[randomNumber]);
		}
	}	
		cout << population[0];
	return 0;
}	

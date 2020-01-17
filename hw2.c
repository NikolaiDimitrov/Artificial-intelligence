#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int* generate_board(int , int []);
int conflicts_on_row(int, int , int []);
int conflicts_on_col(int, int , int []);
int conflicts_on_left_diagonal(int, int , int []);
int conflicts_on_right_diagonal(int, int , int []);
int find_best_index(int, int , int []);
int check_for_conflicts(int, int , int []);
int check_for_conflicts_on_board(int, int [], int []);
int get_queen_with_max_conflicts(int , int [],int []);
void print_board(int , int []);
void switch_queen_position(int ,int , int , int [], int []);
int get_random_queen(int , int []);

int main(){

    srand(time(NULL));
    int *board;
    int n;
	clock_t t; 
    
    printf("Please enter the number of the queens\n");
    scanf("%d",&n);
    
    int queens_indices[n];

    board = generate_board(n,queens_indices);

    // for (int i = 0; i < n; ++i)
    // {
    // 	printf("%d\t", queens_indices[i]);
    // }
    // printf("\n");

    // print_board(n,board);
    int i = 0;
    t = clock();
    while(i<3*n){
    	if (check_for_conflicts_on_board(n,queens_indices,board) == 0 ){
			printf("Soved!\n");
			break;
		}
		int queen_with_max_conflicts = get_queen_with_max_conflicts(n, queens_indices, board);
		// int queen_with_max_conflicts = get_random_queen(n,queens_indices);
		// printf("Qeeen with max conflicts %d\n", queen_with_max_conflicts);
		// printf("Qeeen on index : %d ",queen_with_max_conflicts);
		// printf("has conflicts %d\n", check_for_conflicts(n,queen_with_max_conflicts,board));
		int best_new_index = find_best_index(n,queen_with_max_conflicts,board);
		// printf("Best new index will be %d \n",  best_new_index);
		// print_board(n,board);
		switch_queen_position(n,queen_with_max_conflicts, best_new_index,queens_indices, board);
		print_board(n,board);
	 	i++;

	}
	t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    printf("the algorithm took %f seconds to execute \n", time_taken); 



// printf("\ncol = %d\n",conflicts_on_col(n,queens_indices[0],board));
//    printf("row = %d\n",conflicts_on_row(n,queens_indices[0],board));
//    printf("Left Diagonal = %d\n",conflicts_on_left_diagonal(n,queens_indices[0],board));
//    printf("Right Diagonal = %d\n",conflicts_on_right_diagonal(n,queens_indices[0],board));
//    printf("Total number of conflicts %d\n", check_for_conflicts(n,queens_indices[0],board));

    print_board(n,board);
	free(board);
    return 0;
}

void switch_queen_position(int num_of_queens,int old_position, int new_position, int queens_indices[], int board[]){
	board[old_position] = 0;
	board[new_position] = 1;
	for (int i = 0; i < num_of_queens; ++i)
	{
		if(queens_indices[i] == old_position){
			queens_indices[i] = new_position;
		}
	}

}

int get_random_queen(int num_of_queens, int queens_indices[]){
	srand(time(0));
	int num = (rand() % num_of_queens) ;
	return queens_indices[num];
}


int get_queen_with_max_conflicts(int num_of_queens, int queens_indices[],int board[]){
	int max_conflicts = 0;
	int queen_with_max_conflicts = queens_indices[0];
	int queens_with_max_confilcts[num_of_queens];
	int counter = 1;

	for (int i = 0; i < num_of_queens; ++i)
	{
		int conflicts_per_index = check_for_conflicts(num_of_queens,queens_indices[i],board);

		if(conflicts_per_index > max_conflicts){
			max_conflicts = conflicts_per_index;
			queen_with_max_conflicts = queens_indices[i];
			queens_with_max_confilcts[0] = queens_indices[i];
		}else if (conflicts_per_index == max_conflicts){
			queens_with_max_confilcts[counter] = queens_indices[i];
			counter++;
		}
	}

	// printf("Queens with max conflicts : \n");
	// for (int i = 0; i < counter; ++i)
	// {
	// 	printf("%d\t",queens_with_max_confilcts[i] );
	// }
	
	int random_index = rand() % counter;
	// printf("\n RANDOM %d\n counter:%d\n", random_index,counter);

	return queens_with_max_confilcts[random_index];
}


int find_best_index(int num_of_queens, int queen_index, int board[]){
	int row_n = queen_index/num_of_queens;
	int min_confilcts = num_of_queens;
	int best_index = queen_index;

	int best_indices[num_of_queens];
	int counter = 1;

	for (int i = row_n*num_of_queens; i < row_n*num_of_queens+num_of_queens; ++i)
	{	
		if(i == queen_index){
			continue;
		}
		int conflicts_per_index = check_for_conflicts(num_of_queens,i,board)-1;
		if(conflicts_per_index<min_confilcts){
			min_confilcts = conflicts_per_index;
			best_index = i;
			best_indices[0] = i;
		}else if(conflicts_per_index == min_confilcts){
			best_indices[counter] = i;
			counter++;
		}
	}
		// printf("Best INDICES : \n");
		// for (int i = 0; i < counter; ++i)
		// {
		// 	printf("%d\t",best_indices[i] );
		// }

		int random_index = rand() % counter;
		// printf("\n RANDOM %d\n counter:%d\n", random_index,counter);

		return best_indices[random_index];
}


int check_for_conflicts_on_board(int num_of_queens, int queens_indices[], int board[]){
	int num_of_conflicts = 0;
	for (int i = 0; i < num_of_queens; ++i)
	{
		num_of_conflicts += check_for_conflicts(num_of_queens,queens_indices[i],board) != 0;
	}
	return num_of_conflicts;
}	

int check_for_conflicts(int num_of_queens, int queen_index, int board[]){
	int num_of_conflicts = 0;
	num_of_conflicts += conflicts_on_col(num_of_queens, queen_index,board);
    num_of_conflicts += conflicts_on_row(num_of_queens, queen_index,board);
    num_of_conflicts += conflicts_on_left_diagonal(num_of_queens, queen_index,board);
    num_of_conflicts += conflicts_on_right_diagonal(num_of_queens, queen_index,board);

    return num_of_conflicts;
}


int* generate_board(int num_of_queens, int queens_indices[]){

    // int *board;
    int board_size = num_of_queens*num_of_queens;
	int q = 0;
    int min = 0;

    int* board = (int*) malloc(board_size * sizeof(int));
    

    for (int i = 0; i < board_size; i++)
    {
        if((i+1)% num_of_queens ==0){
            int num = (rand() % (i - min + 1)) + min;
            min=i+1;
            queens_indices[q]=num;
            q++;
            board[num]=1;
        }
        else
        {
            board[i] = 0;
        }
    }
    return board;
}




int conflicts_on_row(int num_of_queens, int queen_index, int board[]){

    // Count Queens on row
    int row_n = queen_index/num_of_queens;
    int counter = 0;
    for (int i = row_n*num_of_queens; i < row_n*num_of_queens+num_of_queens; i++)
    {
        if(board[i]==1 && i != queen_index){
            counter++;
        }
    }   
    return counter;
}

int conflicts_on_col(int num_of_queens, int queen_index, int board[]){

    // Count Queens on column
	long size_of_board = num_of_queens*num_of_queens;
    int col_n = queen_index % num_of_queens;
    int counter = 0;
    for (int i = col_n; i < size_of_board; i+=num_of_queens)
    {
        if(board[i]==1 && i != queen_index){
            counter++;
        }
    }   

    return counter;
}

int conflicts_on_left_diagonal(int num_of_queens, int queen_index, int board[]){

    //Count Queens on left diagonal
    int col_n = queen_index % num_of_queens;
    int row_n = queen_index / num_of_queens;
    int counter = 0;


    for (int i = queen_index, col = col_n, row = row_n; row < num_of_queens && col >=0; col--, row++, i=row * num_of_queens + col)
    {
        //printf("col_n = %d, row_n=%d\n", col_n, row_n);
        if(board[i]==1 && i != queen_index){
            // printf("index: %d\n",index);
            counter++;
        }
    }

    for (int i = queen_index, col = col_n, row = row_n ; row >= 0 && col < num_of_queens; col++, row--, i=row * num_of_queens + col)
    {
        //printf("col_n = %d, row_n=%d\n", col_n, row_n);
        if(board[i]==1 && i != queen_index){
            // printf("index: %d\n",index);
            counter++;
        }
    }

    return counter;
}


int conflicts_on_right_diagonal(int num_of_queens, int queen_index, int board[]){

   //Count Queens on right diagonal
    int col_n = queen_index % num_of_queens;
    int row_n = queen_index / num_of_queens;
    int counter = 0;


    for (int i = queen_index, row = row_n, col = col_n; row <num_of_queens && col <num_of_queens; col++, row++, i=row * num_of_queens + col)
    {
        //printf("col_n = %d, row_n=%d\n", col_n, row_n);
        if(board[i]==1 && i != queen_index){
            //printf("index: %d\n",index);
            counter++;
        }
    }

    for (int i = queen_index, row = row_n, col = col_n; row >= 0 && col >=0; col--, row--, i=row * num_of_queens + col)
    {
        //printf("col_n = %d, row_n=%d\n", col_n, row_n);
        if(board[i]==1 && i != queen_index){
            //printf("index: %d\n",index);
            counter++;
        }
    }


    return counter;
}


void print_board(int num_of_queens, int board[]){

	int board_size = num_of_queens*num_of_queens;

    for (int i = 0; i < board_size; i++)
    {

        if(i%num_of_queens==0)
            printf("\n");
        if(board[i]==1){
            printf("*");
        }else
        {
            printf("_");
        }
    }
    printf("\n");
}
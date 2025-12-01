#include <stdio.h>

#define MAX_SIZE 10
#define MAX_SIZE_WITHOUT_SPECIAL_SYMBOLS 8 

int N;
char map[MAX_SIZE][MAX_SIZE];
char row_variants[100][MAX_SIZE][MAX_SIZE];
int groups_on_rows[MAX_SIZE][MAX_SIZE], groups_on_columns[MAX_SIZE][MAX_SIZE];
int num_of_rowgroups[MAX_SIZE], num_of_colgroups[MAX_SIZE];

//strtok(char* s, " "); // " " => /0
void get_islands(FILE* input, int groups_of_islands[MAX_SIZE][MAX_SIZE], int num_of_groups[MAX_SIZE]){
    for (int i = 0; i < N; i++){
        num_of_groups[i] = 0;
        while(1){
            int  num;
            fscanf(input, "%d", &num);
            if (num == 0) break;
            groups_of_islands[i][num_of_groups[i]++] = num;
        }
        printf("\n");
    }

}

int is_valid_row(int row){
    int index = 0;
    int counter  = 0;
    
    for (int column = 0; column < N; column++){
        if (map[row][column] == '*'){
            counter++;
        } else if (counter > 0){
            if (index >= num_of_rowgroups[row] || counter != groups_on_rows[row][index]){
                return 0;
            }
            index++;
            counter = 0;
        }
    }
    if (counter > 0){
        if (index >= num_of_rowgroups[row] || counter != groups_on_rows[row][index]){
                return 0;
            }
            index++;
    }
    return index == num_of_rowgroups[row];
}


int is_valid_column(int column){
    int index = 0;
    int counter  = 0;
    
    for (int row = 0; row < N; row++){
        if (map[row][column] == '*'){
            counter++;
        } else if (counter > 0){
            if (index >= num_of_colgroups[column] || counter != groups_on_columns[column][index]){
                return 0;
            }
            index++;
            counter = 0;
        }
    }
    if (counter > 0){
        if (index >= num_of_colgroups[column] || counter != groups_on_columns[column][index]){
                return 0;
            }
            index++;
    }
    return index == num_of_colgroups[column];
}

// шашлык

void variants_of_row(int row_number)
{
    char variant_of_row[MAX_SIZE];
    int indexes_of_groups[num_of_rowgroups[row_number]];
    for (int i = 0; i<num_of_rowgroups[row_number]; i++)
    {
        if (i == 0)
            indexes_of_groups[i] = 0;
        else
            indexes_of_groups[i] = indexes_of_groups[i-1] + groups_on_rows[i-1] + 1;
    }
}







int main(int argc, char* argv[]){
    if (argc < 3){
        printf("Need 3 arguments <programm, input, output>\n");
        return 1;
    }
    
    
    FILE* map_in;
    FILE* map_out;
    if (map_in == NULL || map_out == NULL){
        printf("Error\n");
        return 1;
    }

    map_in = fopen(argv[1], "r");
    map_out = fopen(argv[2], "w");

    fclose(map_in);
    fclose(map_out);
    return 0;
}


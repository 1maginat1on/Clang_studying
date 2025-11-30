#include <stdio.h>

#define MAX_SIZE 10
int N;
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
    while (fscanf(map_in, "%d", &N) != EOF){
        get_islands(map_in, groups_on_rows, num_of_rowgroups);
        get_islands(map_in, groups_on_columns, num_of_colgroups);
        printf("Rows:\n");
        // for(int i = 0; i < N; i++){
        //     printf("Row %d: ", i);
        //     for(int j = 0; j < num_of_rowgroups[i]; j++){
        //         printf("%d ", groups_on_rows[i][j]);
        //     }
        //     printf("Number of groups: %d\n", num_of_rowgroups[i]);


        // }  
        // printf("Columns:\n");
        // for(int i = 0; i < N; i++){
        //     printf("Column %d: ", i);
        //     for(int j = 0; j < num_of_colgroups[i]; j++){
        //         printf("%d ", groups_on_columns[i][j]);
        //     }
        //     printf("Number of groups: %d\n", num_of_colgroups[i]);
        // } 
        // printf("\n");
    
    }


    fclose(map_in);
    fclose(map_out);
    return 0;
}


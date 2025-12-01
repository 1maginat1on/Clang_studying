#include <stdio.h>

int main(void)
{
    char A[3][3][3];
    for (int i = 0; i<3;i++)
        for (int j = 0; j<3;j++)
            for (int k = 0; k<3;k++)
                A[i][j][k] = 'a';
    
    for (int i = 0; i<3;i++)
        for (int j = 0; j<3;j++)
        {
            printf("\n");    
            for (int k = 0; k<3;k++)
                printf("%c",A[i][j][k]);
        }



    return 0;
}
//while (fscanf(map_in, "%d", &N) != EOF){
    //     get_islands(map_in, groups_on_rows, num_of_rowgroups);
    //     get_islands(map_in, groups_on_columns, num_of_colgroups);
    //     printf("Rows:\n");
    //     for(int i = 0; i < N; i++){
    //         printf("Row %d: ", i);
    //         for(int j = 0; j < num_of_rowgroups[i]; j++){
    //             printf("%d ", groups_on_rows[i][j]);
    //         }
    //         printf("Number of groups: %d\n", num_of_rowgroups[i]);


    //     }  
    //     printf("Columns:\n");
    //     for(int i = 0; i < N; i++){
    //         printf("Column %d: ", i);
    //         for(int j = 0; j < num_of_colgroups[i]; j++){
    //             printf("%d ", groups_on_columns[i][j]);
    //         }
    //         printf("Number of groups: %d\n", num_of_colgroups[i]);
    //     } 
    //     printf("\n");
    
    // }
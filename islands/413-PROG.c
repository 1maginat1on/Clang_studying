#include <stdio.h>
#include <string.h>

#define MAX_SIZE 10
#define MAX_ROW_VARIANTS 100
#define MAX_SOLUTIONS 1000

char row_variant[MAX_SIZE];
int groups_on_rows[MAX_SIZE][MAX_SIZE], groups_on_columns[MAX_SIZE][MAX_SIZE];
int num_of_rowgroups[MAX_SIZE], num_of_colgroups[MAX_SIZE];
int groups_indexes[4] = {0, 0, 0, 0};
char row_variants[MAX_SIZE][MAX_ROW_VARIANTS][MAX_SIZE];
int row_variants_count[MAX_SIZE] = {0};
int N;
char map[MAX_SIZE][MAX_SIZE];
char solutions[MAX_SOLUTIONS][MAX_SIZE][MAX_SIZE];
int solution_count = 0;

// Функция для чтения подсказок из файла
void get_islands(FILE* input, int groups_of_islands[MAX_SIZE][MAX_SIZE], int num_of_groups[MAX_SIZE]) {
    for (int i = 0; i < N; i++) {
        num_of_groups[i] = 0;
        while(1) {
            int num;
            fscanf(input, "%d", &num);
            if (num == 0) break;
            groups_of_islands[i][num_of_groups[i]++] = num;
        }
    }
}

// Функция проверки корректности столбца
int is_valid_column(int column) {
    int index = 0;
    int counter = 0;
    
    for (int row = 0; row < N; row++) {
        if (map[row][column] == '*') {
            counter++;
        } else if (counter > 0) {
            if (index >= num_of_colgroups[column] || counter != groups_on_columns[column][index]) {
                return 0;
            }
            index++;
            counter = 0;
        }
    }
    
    if (counter > 0) {
        if (index >= num_of_colgroups[column] || counter != groups_on_columns[column][index]) {
            return 0;
        }
        index++;
    }
    
    return index == num_of_colgroups[column];
}

// Рекурсивная функция для генерации всех вариантов строки
void generate_row_variants_recursive(int row_num, int group_idx, int start_pos) {
    // Базовый случай: все группы размещены
    if (group_idx == num_of_rowgroups[row_num]) {
        // Сохраняем текущий вариант
        if (row_variants_count[row_num] < MAX_ROW_VARIANTS) {
            for (int i = 0; i < N; i++) {
                row_variants[row_num][row_variants_count[row_num]][i] = row_variant[i];
            }
            row_variants_count[row_num]++;
        }
        return;
    }
    
    // Получаем размер текущей группы
    int group_size = groups_on_rows[row_num][group_idx];
    
    // Вычисляем максимальную позицию для начала текущей группы
    int max_position = N - group_size;
    
    // Для не последней группы нужно оставить место для оставшихся групп
    if (group_idx < num_of_rowgroups[row_num] - 1) {
        int remaining_space = 0;
        for (int i = group_idx + 1; i < num_of_rowgroups[row_num]; i++) {
            remaining_space += groups_on_rows[row_num][i] + 1; // +1 для пробела между группами
        }
        max_position = N - remaining_space - group_size;
    }
    
    // Перебираем все возможные позиции для текущей группы
    for (int pos = start_pos; pos <= max_position; pos++) {
        // Сохраняем текущее состояние строки
        char temp[MAX_SIZE];
        for (int i = 0; i < N; i++) {
            temp[i] = row_variant[i];
        }
        
        // Размещаем текущую группу
        for (int i = 0; i < group_size; i++) {
            row_variant[pos + i] = '*';
        }
        
        // Рекурсивно размещаем следующую группу
        generate_row_variants_recursive(row_num, group_idx + 1, pos + group_size + 1);
        
        // Восстанавливаем строку
        for (int i = 0; i < N; i++) {
            row_variant[i] = temp[i];
        }
    }
}

// Обертка для генерации всех вариантов строки
void generate_all_row_variants(int row_num) {
    // Инициализируем строку пробелами
    for (int i = 0; i < N; i++) {
        row_variant[i] = ' ';
    }
    
    // Обнуляем счетчик вариантов для этой строки
    row_variants_count[row_num] = 0;
    
    // Если нет групп островов, то только один вариант - вся строка из пробелов
    if (num_of_rowgroups[row_num] == 0) {
        for (int i = 0; i < N; i++) {
            row_variants[row_num][0][i] = ' ';
        }
        row_variants_count[row_num] = 1;
        return;
    }
    
    // Запускаем рекурсивную генерацию
    generate_row_variants_recursive(row_num, 0, 0);
    
    // Если вариантов нет (что-то пошло не так), создаем хотя бы один вариант из пробелов
    if (row_variants_count[row_num] == 0) {
        for (int i = 0; i < N; i++) {
            row_variants[row_num][0][i] = ' ';
        }
        row_variants_count[row_num] = 1;
    }
}

// Рекурсивная функция поиска всех решений
void find_solutions(int row) {
    // Базовый случай: все строки размещены
    if (row == N) {
        // Проверяем все столбцы
        int valid = 1;
        for (int col = 0; col < N; col++) {
            if (!is_valid_column(col)) {
                valid = 0;
                break;
            }
        }
        
        // Если все столбцы валидны, сохраняем решение
        if (valid) {
            if (solution_count < MAX_SOLUTIONS) {
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N; j++) {
                        solutions[solution_count][i][j] = map[i][j];
                    }
                }
                solution_count++;
            }
        }
        return;
    }
    
    // Рекурсивный случай: перебираем все варианты для текущей строки
    for (int variant = 0; variant < row_variants_count[row]; variant++) {
        // Копируем вариант строки в карту
        for (int col = 0; col < N; col++) {
            map[row][col] = row_variants[row][variant][col];
        }
        
        // Рекурсивно переходим к следующей строке
        find_solutions(row + 1);
    }
}

// Функция записи решений в выходной файл
void write_solutions(FILE* output) {
    if (solution_count == 0) {
        fprintf(output, "no tar\n");
    } else {
        for (int k = 0; k < solution_count; k++) {
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    if (solutions[k][i][j] == '*') {
                        fprintf(output, "* ");
                    } else {
                        fprintf(output, "  ");
                    }
                }
                fprintf(output, "\n");
            }
            // Между различными картами - пустая строка
            if (k < solution_count - 1) {
                fprintf(output, "\n");
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    FILE* map_in;
    FILE* map_out;
    map_in = fopen(argv[1], "r");
    map_out = fopen(argv[2], "w");
    
    if (map_in == NULL || map_out == NULL) {
        printf("Error opening files\n");
        return 1;
    }

    int problem_counter = 0;
    while (fscanf(map_in, "%d", &N) != EOF) {
        problem_counter++;
        
        // Чтение данных
        get_islands(map_in, groups_on_rows, num_of_rowgroups);
        get_islands(map_in, groups_on_columns, num_of_colgroups);
        
        // Инициализация массивов
        for (int i = 0; i < N; i++) {
            row_variants_count[i] = 0;
        }
        solution_count = 0;
        
        // Генерация всех вариантов для каждой строки
        for (int i = 0; i < N; i++) {
            generate_all_row_variants(i);
        }
        
        // Поиск всех решений
        find_solutions(0);
        
        // Записываем решения в выходной файл
        write_solutions(map_out);
        
        // Между различными блоками - "next problem"
        fprintf(map_out, "next problem\n");
    }
    printf("Problem %d, N=%d\n", problem_counter, N);
    for (int i = 0; i < N; i++) {
        printf("Row %d has %d variants\n", i, row_variants_count[i]);
    }
    printf("Found %d solutions\n", solution_count);
    fclose(map_in);
    fclose(map_out);
    return 0;
}
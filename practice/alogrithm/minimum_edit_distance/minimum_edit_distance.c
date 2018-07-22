#include <stdio.h>
#include <stdlib.h>

int **table = NULL;
char *word1_alias, *word2_alias;

int get_table_value(int row, int column)
{
    unsigned int up = 32768, left = 32768, bevel = 32768, min = 32768;
    
    if (column == 0 && row == 0) {
        return 0;
    }
    
    if (row > 0) {
            if (table[row-1][column] != -1) {
                    up = table[row-1][column];
            } else {
                    up = get_table_value(row-1, column);
            }
    }
    
    if (column > 0) {
            if (table[row][column-1] != -1) {
                    left  = table[row][column-1];
            } else {
                    left = get_table_value(row, column-1);
            }
    }
    
    if (row > 0 && column > 0) {
            if (table[row-1][column-1] != -1) {
                    bevel = table[row-1][column-1];
            } else {
                    bevel = get_table_value(row-1, column-1);
            }
            
            if (word1_alias[row-1] != word2_alias[column-1])
                    bevel++;
    }
    
    min = ((up < left)?up:left) + 1;
    min = (min < bevel)?min:bevel;

    table[row][column] = min;

    return min;
}

int minDistance(char *word1, char *word2)
{
    int word1_len = 0, word2_len = 0;
    int i = 0, j = 0, ret = 0;
    
    printf("word1 = %s\n", word1);
    printf("word2 = %s\n", word2);

    word1_alias = word1;
    word2_alias = word2;
    
    //get length
    while (*word1_alias != '\0' || *word2_alias != '\0') {
        if (*word1_alias != '\0') {
            word1_alias++;
            word1_len++;
        }
        
        if (*word2_alias != '\0') {
            word2_alias++;
            word2_len++;
        }
    }
    
    printf("word1_len = %d\n", word1_len);
    printf("word2_len = %d\n", word2_len);
    
    word1_alias = word1;
    word2_alias = word2;
    
    //allocate heap
    table = (int **) calloc(word1_len+1, sizeof(int *));
    
    for (i = 0; i < word1_len+1; i++) {
        table[i] = (int *)calloc(word2_len+1, sizeof(int));
    }
    
    //init table
    for (i = 0; i < word1_len+1; i++) {
        for (j = 0; j < word2_len+1; j++) {
                table[i][j] = -1;
        }
    }
   
    for (i = 0; i < word1_len+1; i++) {
        for (j = 0; j < word2_len+1; j++) {
            table[i][j] = get_table_value(i, j);
        }
    }
   
    /*
    printf("\n");
    for (i = 0; i < word1_len+1; i++) {
        for (j = 0; j < word2_len+1; j++) {
            printf("[%d] ", table[i][j]);
        }
        printf("\n");
    }
    */
    
    //get answer
    ret = table[word1_len][word2_len];
    
    //free heap
    for (i = 0; i < word1_len+1; i++) {
        free(table[i]);
    }

    free(table);
    
   return ret;
}

int main(void)
{
        int ret = 0;
        ret = minDistance("dinitrophenylhydrazine", "acetylphenylhydrazine");
        printf("[%s][%d] ret = %d\n", __func__, __LINE__, ret);

        return ret;
}

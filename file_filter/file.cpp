#include <stdio.h>

int main()
{
    FILE* fin = fopen("50k_in.txt", "r");
    FILE* fout = fopen("2.5k_in.txt", "w");

    char word[10000] = "";
    int cnt = 0;

    while (fscanf(fin, "%s", word) != EOF)
    {
        cnt++;
        
       if (cnt % 20 == 0)
            fprintf(fout, "%s\n", word);
    }

    fclose(fin);
    fclose(fout);
}

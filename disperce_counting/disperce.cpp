#include <stdio.h>
#include <assert.h>
#include <math.h>

static const char* INPUT_FILE = "assets/output/fast_crc32_hash.txt";

static float CountAverage(const char* input_file);
static float CountDisperce(const char* input_file, const float avg);

// ====================================================

int main()
{
    float avg = CountAverage(INPUT_FILE);

    float disperce = CountDisperce(INPUT_FILE, avg);

    printf("%f\n", disperce);
}

// ----------------------------------------------------

static float CountAverage(const char* input_file)
{
    assert(input_file);

    FILE* fp = fopen(INPUT_FILE, "r");

    int cnt = 0;
    int sum = 0;
    int num = 0;
    int len = 0;

    while (fscanf(fp, "%d%d", &num, &len) != EOF)
    {
        cnt++;
        sum += len;
    }

    fclose(fp);

    return ((float) sum) / cnt;
}

// ----------------------------------------------------

static float CountDisperce(const char* input_file, const float avg)
{
    assert(input_file);

    FILE* fp = fopen(INPUT_FILE, "r");

    int cnt = 0;
    int num = 0;
    int len = 0;

    float sum = 0;

    while (fscanf(fp, "%d%d", &num, &len) != EOF)
    {
        cnt++;
        sum += ((float) len - avg) * ((float) len - avg);
    }

    fclose(fp);

    return sqrt((sum / cnt));
}



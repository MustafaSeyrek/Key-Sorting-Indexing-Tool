#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json/json.h>

void menu();                    //for screen
void open(char jsonFileName[]); //read json file
void create_index();            //create index file
//void search(char key[]);        //search key
void close();
int compare(const void *a, const void *b); //for sorting
int binaryFileSearch(FILE *fp, const char name1[22], int firstIdx, int lastIdx);
int findRecordByName(char name1[22]);
//keep json objects
struct json_object *parsed_json;
struct json_object *dataFileName;
struct json_object *indexFileName;
struct json_object *recordLength;
struct json_object *keyEncoding;
struct json_object *keyStart;
struct json_object *keyEnd;
struct json_object *order;

//for index file
struct _Index
{
    char name[30];
    int index;
};
typedef struct _Index Index;

int compare(const void *a, const void *b)
{ //control BIN, UTF and CHR
    if (strcmp("BIN", json_object_get_string(keyEncoding)) == 0)
    {
        return ((atoi(((Index *)a)->name) - (atoi(((Index *)b)->name))));
    }
    else
        return (strcmp(((Index *)a)->name, ((Index *)b)->name));
}

FILE *fp;        //for data file
FILE *fp1;       //for index file
int recordCount; //record count
int kS;          //key start
int kE;          //key end
Index indexes[30];
int rL; //record lenght
int flag = 1;
void main()
{

    while (flag)
    {
        menu();
    }
}

//menu
void menu()
{
    char input[30]; //for json name input
    char key[30];   //for search command
    int who;
    printf("\n1.Open Command\n2.Create Index Command\n3.Search Command\n4.Close Command\n5.Exit\n");
    printf("\nEnter a choose: ");
    int choose;
    scanf("%d", &choose);
    switch (choose)
    {
    case 1:
        printf("\nEnter Json File Name: ");
        scanf("%s", &input);
        open(strcat(input, ".json"));
        printf("Json file read.\n");
        break;

    case 2:
        create_index();
        printf("Index file created.\n");
        break;

    case 3:
        printf("\nEnter key for search: ");
        scanf("%s", &key);
        who = findRecordByName(key);
        FILE *file1;
        char data[50];
        file1 = fopen(json_object_get_string(dataFileName), "rb");
        if (who > -1)
        {
            fseek(file1, who * rL, SEEK_SET);
            fread(data, rL, 1, file1);
            printf("%s", data);
        }
        else
        {
            printf("not found");
        }
        break;

    case 4:
        close();
        break;
    case 5:
        printf("Program is closing..\n");
        flag = 0;
        break;
    default:
        printf("Enter true value!\n");
        break;
    }
}

//read json file
void open(char jsonFileName[])
{
    FILE *fp2; //for json file
    char buffer[1024];
    fp2 = fopen(jsonFileName, "r");
    fread(buffer, 1024, 1, fp2);
    fclose(fp2); //f closed
    //parsed json
    parsed_json = json_tokener_parse(buffer);
    json_object_object_get_ex(parsed_json, "dataFileName", &dataFileName);
    json_object_object_get_ex(parsed_json, "indexFileName", &indexFileName);
    json_object_object_get_ex(parsed_json, "recordLength", &recordLength);
    json_object_object_get_ex(parsed_json, "keyEncoding", &keyEncoding);
    json_object_object_get_ex(parsed_json, "keyStart", &keyStart);
    json_object_object_get_ex(parsed_json, "keyEnd", &keyEnd);
    json_object_object_get_ex(parsed_json, "order", &order);
    // print json objects
    printf("dataFileName: %s\n", json_object_get_string(dataFileName));
    printf("indexFileName: %s\n", json_object_get_string(indexFileName));
    printf("recordLength: %d\n", json_object_get_int(recordLength));
    printf("keyEncoding: %s\n", json_object_get_string(keyEncoding));
    printf("keyStart: %d\n", json_object_get_int(keyStart));
    printf("keyEnd: %d\n", json_object_get_int(keyEnd));
    printf("order: %s\n", json_object_get_string(order));
}

void create_index()
{

    fp = fopen(json_object_get_string(dataFileName), "rb"); //.dat file
    if (!fp)
        return;
    int i = 0;
    rL = json_object_get_int(recordLength); //recordLength
    fseek(fp, 0, SEEK_END);                 //for calculate record count
    recordCount = ftell(fp) / rL;           //calculate record count
    kS = json_object_get_int(keyStart);     //keyStart
    kE = json_object_get_int(keyEnd);       //keyEnd
    char data[30];                          //holds read data
    fseek(fp, ((i * rL) + kS), SEEK_SET);   //for read
    fread(data, (kE - kS + 1), 1, fp);      //read and put into data
    while (!feof(fp))
    {
        strcpy(indexes[i].name, data);
        indexes[i].index = i;
        i++;
        fseek(fp, ((i * rL) + kS), SEEK_SET);
        fread(data, (kE - kS + 1), 1, fp);
    }
    // ASC or DESC
    char odr[] = "ASC";
    char odr3[] = "DESC";
    if (strcmp(odr, json_object_get_string(order)) == 0) //order ASC
    {
        qsort(indexes, recordCount, sizeof(Index), compare);
    }
    else if (strcmp(odr3, json_object_get_string(order)) == 0) //order DESC
    {
        Index tempArr[30];
        int k;
        for (k = 0; k < 30; k++)
        {
            tempArr[k] = indexes[k];
        }
        //normal sorting and backward insertion
        qsort(tempArr, recordCount, sizeof(Index), compare);
        k = 0;
        int j;
        for (j = 30 - 1; j >= 0; j--)
        {
            indexes[k] = tempArr[j];
            k++;
        }
    }

    fp1 = fopen(json_object_get_string(indexFileName), "wb"); //.ndx file
    if (!fp1)
        return;
    fseek(fp1, 0, SEEK_SET);
    fwrite(indexes, sizeof(Index), recordCount, fp1);
    fclose(fp1);
    fclose(fp);
}

int binaryFileSearch(FILE *fp, const char name1[22], int firstIdx, int lastIdx)
{
    printf("\nRunning Binary Search %d %d\n", firstIdx, lastIdx);
    Index first, last, middle;
    int returnData;

    // Calculate the middle Index
    int middleIdx = (firstIdx + lastIdx) / 2;

    // Read first record and return if it is the searched one.
    fseek(fp, firstIdx * (sizeof(Index)), SEEK_SET);
    fread(&first, sizeof(Index), 1, fp);
    if (strcmp(first.name, name1) == 0)
    {
        returnData = first.index;
        return returnData;
    }
    // Read last record and return if it is the searched one.
    fseek(fp, lastIdx * sizeof(Index), SEEK_SET);
    fread(&last, sizeof(Index), 1, fp);
    if (strcmp(last.name, name1) == 0)
    {
        returnData = last.index;

        return returnData;
    }
    // Recursion exit condition, If middle index is equal to first or last index
    // required comparisons are already made, so record is not found.
    // Create and return an empty person.
    if (middleIdx == firstIdx || middleIdx == lastIdx)
    {
        int d = -1;
        return d;
    }

    // Read the middle record and return if it is the searched one.
    fseek(fp, middleIdx * sizeof(Index), SEEK_SET);
    fread(&middle, sizeof(Index), 1, fp);
    if (strcmp(middle.name, name1) == 0)
    {
        returnData = middle.index;
        return returnData;
    }
    // Determine the record position and recursively call with appropriate attributes.
    if (strcmp(middle.name, name1) > 0)
    {
        return binaryFileSearch(fp, name1, firstIdx + 1, middleIdx - 1);
    }
    else
    {
        return binaryFileSearch(fp, name1, middleIdx + 1, lastIdx - 1);
    }
}

int findRecordByName(char name1[22])
{
    // Open the file
    FILE *inputFile;
    inputFile = fopen(json_object_get_string(indexFileName), "rb");

    // Calculate initial first and last indexes.
    int firstIdx = 0;
    fseek(inputFile, recordCount * sizeof(Index), SEEK_SET);
    int lastIdx = (ftell(inputFile) / sizeof(Index)) - 1;

    // Initiate the search.
    int result = binaryFileSearch(inputFile, name1, firstIdx, lastIdx);
    fclose(inputFile);
    return result;
}

void close()
{
    printf("\nFiles are already closed!\n");
}
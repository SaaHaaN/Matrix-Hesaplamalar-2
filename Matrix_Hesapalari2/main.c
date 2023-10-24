/**
 * @file Şahan Aytekin Proje2
 * @description Iris.csv dosyasından verileri okuyup işlemler yaptıktan sonra IrisStatistic.txt dosyasına yazdıran program.
 * @assignment 2. Proje Ödevi
 * @date 23.12.2022
 * @author Şahan Aytekin sahan.aytekin@stu.fsm.edu.tr
 */

#include <stdio.h>
#include <stdlib.h>
#include "matrixLib.h"
#include <string.h>
#include <ctype.h>

enum IrisType // IrisType enum yapısı oluşturuldu.
{
    IrisSetosa = 1,
    IrisVersicolor = 2,
    IrisVirginica = 3
};

//-----------------------------------------

typedef struct // IrisDataType struct yapısı oluşturuldu.
{
    float sepalLength;
    float sepalWidth;
    float petalLength;
    float petalWidth;
    int irisType;
} IrisDataType;

//-----------------------------------------

char *stringTrim(char *str);

//-----------------------------------------

int main()
{
    FILE *fp = fopen("iris.csv", "r");           // iris.csv dosyası okunmak üzere açıldı.
    FILE *fp2 = fopen("IrisStatistic.txt", "w"); // IrisStatistic.txt dosyası yazmak üzere açıldı.

    if (fp == NULL)
    {
        printf("Dosya Bulunamadi..");
        return 0;
    }
    else
    {
        printf("Dosya acildi..");
    }

    char firstLine[1024];                               // Dosyadaki ilk satırı okumak için bir dizi oluşturuldu.
    int countOfData = atoi(fgets(firstLine, 1024, fp)); // Dosyadaki ilk satırı okuyup countOfData değişkenine atandı.
    fgets(firstLine, 1024, fp);                         // Dosyadaki ikinci satırı okuyup firstLine değişkenine atandı.

    IrisDataType *irisData = (IrisDataType *)malloc(countOfData * sizeof(IrisDataType)); // irisData değişkeni için bellekten yer ayırıldı.

    for (int i = 0; i < countOfData; i++)
    {

        char line[1024];
        fgets(line, 1024, fp);

        char *token = strtok(line, ","); // Dosyadaki verileri virgüllerden ayırmak için token değişkeni oluşturuldu.
        token = strtok(NULL, ",");       // Dosyadaki ilk virgülden sonraki veriye geçmek için token değişkeni kullanıldı.

        // Dosyadaki veriler irisData arayine atandı.
        irisData[i].sepalLength = atof(token);

        token = strtok(NULL, ",");
        irisData[i].sepalWidth = atof(token);

        token = strtok(NULL, ",");
        irisData[i].petalLength = atof(token);

        token = strtok(NULL, ",");
        irisData[i].petalWidth = atof(token);

        token = strtok(NULL, ",");
        stringTrim(token); // Dosyadaki verilerin başındaki ve sonundaki boşlukları kaldırmak için stringTrim fonksiyonu çağırıldı.

        if (strcmp(token, "Iris-setosa") == 0) // Dosyadaki verilerin IrisType enum yapısına göre irisData arayine atandı.
        {
            irisData[i].irisType = IrisSetosa;
        }
        else if (strcmp(token, "Iris-versicolor") == 0)
        {
            irisData[i].irisType = IrisVersicolor;
        }
        else if (strcmp(token, "Iris-virginica") == 0)
        {
            irisData[i].irisType = IrisVirginica;
        }
    }

    float **matrix = returnMatrix(countOfData, 5);

    for (int i = 0; i < countOfData; i++)
    {
        matrix[i][0] = irisData[i].sepalLength;
        matrix[i][1] = irisData[i].sepalWidth;
        matrix[i][2] = irisData[i].petalLength;
        matrix[i][3] = irisData[i].petalWidth;
        matrix[i][4] = irisData[i].irisType;
    }

    // Veriler için vektörler oluşturuldu.
    float *sepalL = returnVector(countOfData);
    float *sepalW = returnVector(countOfData);
    float *petalL = returnVector(countOfData);
    float *petalW = returnVector(countOfData);

    for (int i = 0; i < countOfData; i++)
    {
        sepalL[i] = matrix[i][0];
        sepalW[i] = matrix[i][1];
        petalL[i] = matrix[i][2];
        petalW[i] = matrix[i][3];
    }

    // Veriler için gerekli hesaplamalar yapılıp IrisStatistic.txt dosyasına yazıldı.

    float sepalLMean = mean(sepalL, countOfData);
    fprintf(fp2, "Sepal Length Ortalamasi: %f\n", sepalLMean);

    float sepalWMean = mean(sepalW, countOfData);
    fprintf(fp2, "Sepal Width Ortalamasi: %f\n", sepalWMean);

    float petalLMean = mean(petalL, countOfData);
    fprintf(fp2, "Petal Length Ortalamasi: %f\n", petalLMean);

    float petalWMean = mean(petalW, countOfData);
    fprintf(fp2, "Petal Width Ortalamasi: %f\n\n", petalWMean);

    //-----------------------------------------

    float sepalLVar = variance(sepalL, countOfData);
    fprintf(fp2, "Sepal Length Varyansi: %f\n", sepalLVar);

    float sepalWVar = variance(sepalW, countOfData);
    fprintf(fp2, "Sepal Width Varyansi: %f\n", sepalWVar);

    float petalLVar = variance(petalL, countOfData);
    fprintf(fp2, "Petal Length Varyansi: %f\n", petalLVar);

    float petalWVar = variance(petalW, countOfData);
    fprintf(fp2, "Petal Width Varyansi: %f\n\n", petalWVar);

    //-----------------------------------------

    float korelasyon1 = correlation(sepalL, sepalW, countOfData);
    fprintf(fp2, "Sepal Length - Sepal Width Korelasyonu: %f\n", korelasyon1);

    float korelasyon2 = correlation(sepalL, petalL, countOfData);
    fprintf(fp2, "Sepal Length - Petal Length Korelasyonu: %f\n", korelasyon2);

    float korelasyon3 = correlation(sepalL, petalW, countOfData);
    fprintf(fp2, "Sepal Length - Petal Width Korelasyonu: %f\n\n", korelasyon3);

    //-----------------------------------------

    float **matrix2 = returnMatrix(countOfData, 4);

    for (int i = 0; i < countOfData; i++)
    {
        matrix2[i][0] = irisData[i].sepalLength;
        matrix2[i][1] = irisData[i].sepalWidth;
        matrix2[i][2] = irisData[i].petalLength;
        matrix2[i][3] = irisData[i].petalWidth;
    }

    float **covarianceMat = returnMatrix(4, 4);
    covarianceMat = covarianceMatrix(matrix2, countOfData, 4); // Kovaryans matrisi hesaplandı.

    fprintf(fp2, "Kovaryans Matrix\n");

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            fprintf(fp2, "%f ", covarianceMat[i][j]); // Kovaryans matrisi dosyaya yazdırıldı.
        }
        fprintf(fp2, "\n");
    }

    // Bellekte ayrılan yerler boşaltıldı.

    free(sepalL);
    free(sepalW);
    free(petalL);
    free(petalW);
    free(irisData);

    freeMatrix(matrix, countOfData);
    freeMatrix(matrix2, countOfData);
    freeMatrix(covarianceMat, 4);

    fclose(fp);  // Okuma dosyası kapatıldı.
    fclose(fp2); // Yazma dosyası kapatıldı.

    return 0;
}

//-----------------------------------------------------------------

char *stringTrim(char *str) // Stringin içindeki mevcut boşlukları silen fonksiyon.
{
    char *end; // Stringin sonu.

    while (isspace((unsigned char)*str)) // Stringde boşluk varsa siler. Kontorole baştan başlar.
        str++;
    if (*str == 0) // Stringin içinde boşluk yoksa
        return str;
    end = str + strlen(str) - 1;

    while (end > str && isspace((unsigned char)*end)) // Stringde boşluk varsa siler. Kontorole sondan başlar.
        end--;

    end[1] = '\0'; // Stringin sonuna null karakteri ekler.
    return str;
}
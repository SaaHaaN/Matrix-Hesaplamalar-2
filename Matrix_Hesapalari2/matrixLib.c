/**
 * @file Şahan Aytekin Proje2
 * @description Iris.csv dosyasından verileri okuyup işlemler yaptıktan sonra IrisStatistic.txt dosyasına yazdıran program.
 * @assignment 2. Proje Ödevi
 * @date 23.12.2022
 * @author Şahan Aytekin sahan.aytekin@stu.fsm.edu.tr
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrixLib.h"

//******************************************************************************************************

float *returnVector(int size)
{
    float *vector = (float *)malloc(sizeof(float) * size); // Malloc ile eleman sayısı kadar yer açıyor..
    return vector;
}

//******************************************************************************************************

float **returnMatrix(int row, int col) // Heapte matrix için yer tutuyor..
{
    float **matrix = (float **)malloc(row * sizeof(float *)); // Malloc ile satır sayısı kadar yer açıyor..
    for (int i = 0; i < row; i++)
    {
        matrix[i] = (float *)malloc(col * sizeof(float)); // Daha sonradan sütun sayısı kadar yer açıyor..
    }
    return matrix;
}

//******************************************************************************************************

void freeMatrix(float **mat, int row) // Heapte matrix için tuttuğumuz yeri geri etmek için..
{

    for (int i = 0; i < row; i++) // Matrixin satırlarını tek tek geziyor..
    {
        free(mat[i]); // İlgili satırı geri iade ediyor..
    }
    free(mat);
}

//******************************************************************************************************

float mean(float *vec, int size)
{
    float toplam = 0;
    float ortalama;

    for (int i = 0; i < size; i++) // Tek tek elemanları alıyor ve toplama ekliyor..
    {
        toplam += vec[i];
    }
    ortalama = toplam / (float)size; // Ortalama hesabı için eleman sayısına bölüyor..
    return ortalama;
}

//******************************************************************************************************

float covariance(float *vec1, float *vec2, int size)
{
    float ortalama1, ortalama2, kovaryans, sayi1, sayi2;
    ortalama1 = mean(vec1, size); // Ortalama hesabı fonskiyonu..
    ortalama2 = mean(vec2, size); // Ortalama hesabı fonskiyonu..

    float toplam = 0;

    for (int i = 0; i < size; i++)
    {
        sayi1 = vec1[i] - ortalama1; // 1. vektörün her elemanından ortalamyı çıkartıyor..
        sayi2 = vec2[i] - ortalama2; // 2. vektörün her elemanından ortalamyı çıkartıyor..
        toplam = toplam + (sayi1 * sayi2);
    }
    kovaryans = toplam / (size - 1);
    return kovaryans;
}

//******************************************************************************************************

float correlation(float *vec, float *vec2, int size)
{
    float kovaryans = covariance(vec, vec2, size); // Kovaryans hesabı lazım olduğu için o fonksiyonu kullanıyor..
    float ortalama1 = mean(vec, size);             // 1. vektör ortalama hesabı..
    float ortalama2 = mean(vec2, size);            // 2. vektör ortalama hesabı..

    float korelasyon, varyans1 = 0, varyans2 = 0, standartSapmaVec1, standartSapmaVec2;

    for (int i = 0; i < size; i++)
    {
        varyans1 += pow(vec[i] - ortalama1, 2);  // 1. vektör için varyans hesabı yapılıyor.. (elemanın ortalamaya farkının karesi hesaplanıyor)
        varyans2 += pow(vec2[i] - ortalama2, 2); // 2. vektör için varyans hesabı yapılıyor.. (elemanın ortalamaya farkının karesi hesaplanıyor)
    }

    int payda = size - 1;                                             // Formülde gerekli olan uzunluk -1 sayisi için..
    standartSapmaVec1 = sqrt(varyans1 / payda);                       // 1. vektör için standart sapma hesabı..
    standartSapmaVec2 = sqrt(varyans2 / payda);                       // 2. vektör için standart sapma hesabı..
    korelasyon = kovaryans / (standartSapmaVec1 * standartSapmaVec2); // Korelasyon hesabı yapılıyor.. kovaryans bölü 1. ve 2. vektörün standart sapmasının çarpımı

    return korelasyon;
}

//******************************************************************************************************

float **matrixMultiplication(float **mat1, float **mat2, int row1, int col1, int row2, int col2)
{
    float **matrisCarpimi = returnMatrix(row1, col2);

    if (row2 == col1) // Matrixler Çarpılabilir mi diye kontrol ediliyor...
    {
        for (int i = 0; i < row1; i++)
        {
            for (int j = 0; j < col2; j++)
            {
                matrisCarpimi[i][j] = 0;

                for (int k = 0; k < col1; k++)
                {
                    matrisCarpimi[i][j] += mat1[i][k] * mat2[k][j];
                }
            }
        }
        return matrisCarpimi;
    }
    else // Eğerki matrixler çarpılamaz ise ekrana uyarısını yazdırıyor..
    {
        printf("Matrixler Carpilamaz...");
        return NULL;
    }
}

//******************************************************************************************************

float **matrixTranspose(float **mat, int row, int col)
{
    float **transpoz = returnMatrix(col, row);
    // Bir matrixin transpouzu satır ve sütünların yer deiştirmesi demektir. Bu yüzden yeni oluşan matrix "Col - Row" olarak oluşuyor..

    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            transpoz[j][i] = mat[i][j]; // elemanların yerleri değişiyor..
        }
    }
    return transpoz;
}

//******************************************************************************************************

float *rowMeans(float **mat, int row, int col) // her satırın toplamının ortalaması
{
    float *ortalamaVec = returnVector(row); // Sonuç olarak dönecek vektörü oluşturuyor..

    for (int i = 0; i < row; i++)
    {
        ortalamaVec[i] = mean(mat[i], col); // i. satırın ortalamsını alıyor..
    }
    return ortalamaVec;
}

//******************************************************************************************************

float *columnMeans(float **mat, int row, int col) // her sutünun toplamının ortalaması
{
    float **matrisinTranspozu = matrixTranspose(mat, row, col); // Bir matrixin traspozunun satır ortalamasını alırsak sütun ortalamasını alırız..
    float *ortalamaVec = returnVector(col);                     // Sonuç olarak dönecek vektörü oluşturuyor..

    for (int i = 0; i < col; i++)
    {
        ortalamaVec[i] = mean(matrisinTranspozu[i], row); // i. satırın ortalamsını alıyor..
    }
    return ortalamaVec;
}

//******************************************************************************************************

float **covarianceMatrix(float **mat, int row, int col)
{
    float *colM = columnMeans(mat, row, col); // Kovarsyan matrix için sütun ortalamasına ihtiyaç vardır..
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++) // elemanları gezdikten sonra
        {
            mat[i][j] -= colM[j]; // Hangi sutundaki eleman ise oluşan sütun ortalamsı vektörünki elemanın farkı alınıyor..
        }
    }

    float **kovMatrixTranspoze = matrixTranspose(mat, row, col); // Fakrı alındıktan sonra çarpa işleminin diğer elemanı olan transpoz matrixi oluşturuyor..
    float **kovMatrixlerinCarpimi = matrixMultiplication(kovMatrixTranspoze, mat, col, row, row, col);

    for (int i = 0; i < col; i++)
    {
        for (int j = 0; j < col; j++)
        {
            kovMatrixlerinCarpimi[i][j] /= (float)row; // Çarpım onu oluşan matrixin elemanlarını tek tek satır ana matrixin satır sayısına bölüyor..
        }
    }

    return kovMatrixlerinCarpimi;
}

//******************************************************************************************************

float determinant(float **mat, int row)
{
    if (row != 3) // Matrix uygunmu diye en başta kontrol ediyor.. 3x3 değilse hatayı ekrana yazdırıyor..
    {
        printf("Sarrus yontemi 3x3 Matrix icindir..\n");
    } // Eğerki 3x3 ise  bu if zaten çalışmıyor ve sarrus kuralı uygulanıyor..

    mat = (float **)realloc(mat, sizeof(float **) * row); // Realloc ile matrixi büyültmek için yer açıyor..

    for (int i = 0; i < row; ++i)
    {
        mat[i] = (float *)realloc(mat[i], sizeof(float) * (row + 2)); // Sonra bu matrixin sutunlarını  fazlalaştırıyor..
    }

    for (int i = 0; i < row; ++i)
    {
        for (int j = row; j < (row + 2); ++j)
        {
            mat[i][j] = mat[i][j - 3]; // Sarrus kuralı uygulamaak için 1. sütunu 4. sütuna, 2. sütunu ise 5. sütuna yazıyor..
        }
    }

    float determinant = 0;
    float detDegerleri[6] = {1, 1, 1, 1, 1, 1}; // Sonuçları tutmak için 6 elemanlı bir dizi oluşturuyor..

    int k = 0;

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < (row + 2); j++)
        {
            k = j - i; // i ve j nin farkını alıp förmülü uyguluyor..
            if (k < 3) // bu değer oluşturdugumuz dizinin eleman sayısına eşit oluyor ve o indexe elemanı atıyor..
            {
                detDegerleri[k] *= mat[i][j]; // Sarrus kuralı uyguluyor..
            }

            k = i + j;  // i ve j nin toplamını alıp förmülü uyguluyor..
            if (k >= 2) // bu değer oluşturdugumuz dizinin eleman sayısına eşit oluyor ve o indexe elemanı atıyor..
            {
                detDegerleri[k + 1] *= (mat[i][j]) * (-1); // Sarrus kuralı uyguluyor ve çıkarma işlemi için -1 çarpıyor..
            }
        }
    }

    for (int i = 0; i < 6; i++)
    {
        determinant += detDegerleri[i]; // Sonuçları topluyor..
    }
    return determinant;
}

//******************************************************************************************************

void printVector(float *vec, int N) // Vektörü ekrana yazdırıyor..
{
    for (int i = 0; i < N; i++)
    {
        printf("%.2f\t", vec[i]); // Vektörün elemanlarını yazdırıyor..
    }
    printf("\n");
}

//******************************************************************************************************

void printMatrix(float **mat, int row, int col) // Matrixi ekrana yazdırıyor..
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%f ", mat[i][j]);
        }
        printf("\n");
    }
}

//******************************************************************************************************

void fillVector(float *vec, int N, int seed)
{
    srand(seed); // seed argümanı ile sayıları dolduruyor..
    for (int i = 0; i < N; i++)
    {
        vec[i] = (rand() % 10) + 1; // 1 ile 10 arasında rastgele sayılarla dolduruyor..
    }
}
//******************************************************************************************************

void fillMatrix(float **mat, int row, int col, int seed)
{
    srand(seed); // seed argümanı ile sayıları dolduruyor..
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            mat[i][j] = (rand() % 10) + 1; // 1 ile 10 arasında rastgele sayılarla dolduruyor..
        }
    }
}

float variance(float *vec, int size)
{
    float variance = 0;
    float ortalama = 0;

    for (int i = 0; i < size; i++)
    {
        ortalama += vec[i];
    }
    ortalama /= size;
    for (int i = 0; i < size; i++)
    {
        variance += pow(vec[i] - ortalama, 2);
    }

    variance /= size;
    return variance;
}

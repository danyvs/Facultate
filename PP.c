#include <stdio.h>
#include <stdlib.h>

struct RGB {
    unsigned char B, G, R;
};

struct Imagine {
    unsigned char* header;
    struct RGB* continut;
    unsigned int latime, inaltime;
};

void xorshift32(unsigned int n, unsigned int** randomNumbers, unsigned int seed) {
    unsigned int r, i;
    r = seed;
    *randomNumbers = (unsigned int*)malloc((n + 1) * sizeof(unsigned int));
    (*randomNumbers)[0] = seed;
    for (i = 1; i <= n; ++i) {
        r ^= r << 13;
        r ^= r >> 17;
        r ^= r << 5;
        (*randomNumbers)[i] = r;
    }
}

/* Incarca imaginea in memorie in forma liniarizata
 *
 */
void incarcareImagineInMemorie(char *caleImagineSursa, struct Imagine* imagine) {
    FILE* fin = fopen(caleImagineSursa, "rb");
    if (fin == NULL) {
        printf("Imaginea nu a fost deschisa corect!\n");
        return;
    }

    // incarcare header
    imagine->header = (unsigned char*)malloc(54 * sizeof(unsigned char));
    fread(imagine->header, sizeof(unsigned char), 54, fin);

    // obtinere latime imagine
    fseek(fin, 18, SEEK_SET);
    fread(&imagine->latime, sizeof(unsigned int), 1, fin);

    // obtinere inaltime imagine
    fread(&imagine->inaltime, sizeof(unsigned int), 1, fin);

    fseek(fin, 54, SEEK_SET);
    imagine->continut = (struct RGB*)malloc(imagine->latime * imagine->inaltime * sizeof(struct RGB));
    int i, j;
    for (i = imagine->inaltime - 1; i >= 0; --i)
        for (j = 0; j < imagine->latime; ++j) {
            struct RGB pixel;
            fread(&pixel, sizeof(unsigned char), 3, fin);
            imagine->continut[i * imagine->latime + j] = pixel;
        }

    fclose(fin);
}

/*
 *
 */
void incarcareImagineInFisier(char* numeFisierDestinatie, struct Imagine imagine) {
    FILE* fout = fopen(numeFisierDestinatie, "wb");

    fwrite(imagine.header, sizeof(unsigned char), 54, fout);

    int i;
    for (i = imagine.inaltime - 1; i >= 0; --i)
        fwrite(&imagine.continut[i * imagine.latime], sizeof(struct RGB), imagine.latime, fout);

    fclose(fout);
}

/*
 *
 */
void durstenfeld(unsigned int** shuffleArray, unsigned int* randomNumbers, unsigned int n) {
    unsigned int i;

    *shuffleArray = (unsigned int*)malloc(n * sizeof(unsigned int));
    for (i = 0; i < n; ++i)
        (*shuffleArray)[i] = i;

    for (i = n - 1; i >= 1; --i) {
        unsigned int randNr = randomNumbers[i] % n;
        unsigned int temp = (*shuffleArray)[randNr];
        (*shuffleArray)[randNr] = (*shuffleArray)[i];
        (*shuffleArray)[i] = temp;
    }
}

/*
 *
 */
void shufflePixels(struct Imagine imagine, unsigned int* shuffleArray) {
    struct RGB* tempImage = (struct RGB*)malloc(imagine.inaltime * imagine.latime * sizeof(struct RGB));

    unsigned int i;
    for (i = 0; i < imagine.inaltime * imagine.latime; ++i)
        tempImage[shuffleArray[i]] = imagine.continut[i];

    for (i = 0; i < imagine.inaltime * imagine.latime; ++i)
        imagine.continut[i] = tempImage[i];
}

/*
 *
 */
void criptareImagine(struct Imagine imagine) {
    // deschid fisierul din care citesc r0 si sv
    FILE* fin = fopen("secret_key.txt", "r");
    if (!fin) {
        printf("Fisierul nu a fost deschis corect!\n");
        return;
    }

    unsigned int randomNumber0, startingValue;
    fscanf(fin, "%d%d", &randomNumber0, &startingValue);

    unsigned int* randomNumbers;
    xorshift32(2 * imagine.inaltime * imagine.latime - 1, &randomNumbers, randomNumber0);

    unsigned int* shuffleArray;
    durstenfeld(&shuffleArray, randomNumbers, imagine.inaltime * imagine.latime);

    shufflePixels(imagine, shuffleArray);

    imagine.continut[0].B ^= ((startingValue >> (8 * 2)) & 0xFF) ^ ((randomNumbers[imagine.inaltime * imagine.latime] >> (8 * 2)) & 0xFF);
    imagine.continut[0].G ^= ((startingValue >> (8 * 1)) & 0xFF) ^ ((randomNumbers[imagine.inaltime * imagine.latime] >> (8 * 1)) & 0xFF);
    imagine.continut[0].R ^= ((startingValue >> (8 * 0)) & 0xFF) ^ ((randomNumbers[imagine.inaltime * imagine.latime] >> (8 * 0)) & 0xFF);

    unsigned int i;
    for (i = 1; i <= imagine.inaltime * imagine.latime - 1; ++i) {
        imagine.continut[i].B ^= imagine.continut[i - 1].B ^ ((randomNumbers[imagine.inaltime * imagine.latime + i] >> (8 * 2)) & 0xFF);
        imagine.continut[i].G ^= imagine.continut[i - 1].G ^ ((randomNumbers[imagine.inaltime * imagine.latime + i] >> (8 * 1)) & 0xFF);
        imagine.continut[i].R ^= imagine.continut[i - 1].R ^ ((randomNumbers[imagine.inaltime * imagine.latime + i] >> (8 * 0)) & 0xFF);
    }

    fclose(fin);
}

int main() {
    struct Imagine imagine;
    incarcareImagineInMemorie("peppers.bmp", &imagine);
    criptareImagine(imagine);
    incarcareImagineInFisier("imagine.bmp", imagine);

    return 0;
}

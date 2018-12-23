#include <stdio.h>
#include <stdlib.h>

struct RGB {
    unsigned char B, G, R;
};

struct Image {
    unsigned char* header;
    struct RGB* content;
    unsigned int width, height;
    unsigned int padding;
};

void xorshift32(unsigned int n, unsigned int** randomNumbers, unsigned int seed) {
    unsigned int r, i;
    *randomNumbers = (unsigned int*)malloc((n + 1) * sizeof(unsigned int));
    (*randomNumbers)[0] = r = seed;
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
void loadImageIntoMemory(char *imagePath, struct Image* image) {
    FILE* fin = fopen(imagePath, "rb");
    if (fin == NULL) {
        printf("Imaginea nu a fost deschisa corect!\n");
        return;
    }

    // incarcare header
    image->header = (unsigned char*)malloc(54 * sizeof(unsigned char));
    fread(image->header, sizeof(unsigned char), 54, fin);

    // obtinere latime imagine
    fseek(fin, 18, SEEK_SET);
    fread(&image->width, sizeof(unsigned int), 1, fin);

    // obtinere inaltime imagine
    fread(&image->height, sizeof(unsigned int), 1, fin);

    // calculare padding
    if (image->width % 4)
        image->padding = 4 - (3 * image->width) % 4;
    else
        image->padding = 0;

    fseek(fin, 54, SEEK_SET);
    image->content = (struct RGB*)malloc(image->width * image->height * sizeof(struct RGB));


    int i, j;
    for (i = image->height - 1; i >= 0; --i) {
        for (j = 0; j < image->width; ++j) {
            struct RGB pixel;
            fread(&pixel, sizeof(unsigned char), 3, fin);
            image->content[i * image->width + j] = pixel;
        }
        fseek(fin, image->padding, SEEK_CUR);
    }

    fclose(fin);
}

/*
 *
 */
void loadImageIntoFile(char* filePath, struct Image image) {
    FILE* fout = fopen(filePath, "wb");

    fwrite(image.header, sizeof(unsigned char), 54, fout);

    int i;
    for (i = image.height - 1; i >= 0; --i) {
        fwrite(&image.content[i * image.width], sizeof(struct RGB), image.width, fout);

        unsigned char x = 0;
        fwrite(&x, sizeof(unsigned char), image.padding, fout);
    }

    fclose(fout);
}


void durstenfeld(unsigned int** shuffleArray, unsigned int* randomNumbers, unsigned int n) {
    unsigned int i;

    *shuffleArray = (unsigned int*)malloc(n * sizeof(unsigned int));
    for (i = 0; i < n; ++i)
        (*shuffleArray)[i] = i;

    for (i = n - 1; i >= 1; --i) {
        unsigned int randNr = randomNumbers[n - i] % (i + 1);
        unsigned int temp = (*shuffleArray)[randNr];
        (*shuffleArray)[randNr] = (*shuffleArray)[i];
        (*shuffleArray)[i] = temp;
    }
}

void shufflePixels(struct Image image, unsigned int* shuffleArray) {
    struct RGB* tempImage = (struct RGB*)malloc(image.height * image.width * sizeof(struct RGB));

    unsigned int i;
    for (i = 0; i < image.height * image.width; ++i)
        tempImage[shuffleArray[i]] = image.content[i];

    for (i = 0; i < image.height * image.width; ++i)
        image.content[i] = tempImage[i];
}

void encryptImage(struct Image image) {
    // deschid fisierul din care citesc r0 si sv
    FILE* fin = fopen("secret_key.txt", "r");
    if (!fin) {
        printf("Fisierul nu a fost deschis corect!\n");
        return;
    }

    unsigned int randomNumber0, startingValue;
    fscanf(fin, "%u%u", &randomNumber0, &startingValue);

    fclose(fin);

    unsigned int* randomNumbers;
    xorshift32(2 * image.height * image.width - 1, &randomNumbers, randomNumber0);

    unsigned int* shuffleArray;
    durstenfeld(&shuffleArray, randomNumbers, image.height * image.width);

    shufflePixels(image, shuffleArray);

    image.content[0].R ^= ((startingValue >> (8 * 2)) & 0xFF) ^ ((randomNumbers[image.height * image.width] >> (8 * 2)) & 0xFF);
    image.content[0].G ^= ((startingValue >> (8 * 1)) & 0xFF) ^ ((randomNumbers[image.height * image.width] >> (8 * 1)) & 0xFF);
    image.content[0].B ^= ((startingValue >> (8 * 0)) & 0xFF) ^ ((randomNumbers[image.height * image.width] >> (8 * 0)) & 0xFF);

    unsigned int i;
    for (i = 1; i <= image.height* image.width - 1; ++i) {
        image.content[i].R ^= image.content[i - 1].R ^ ((randomNumbers[image.height * image.width + i] >> (8 * 2)) & 0xFF);
        image.content[i].G ^= image.content[i - 1].G ^ ((randomNumbers[image.height * image.width + i] >> (8 * 1)) & 0xFF);
        image.content[i].B ^= image.content[i - 1].B ^ ((randomNumbers[image.height * image.width + i] >> (8 * 0)) & 0xFF);
    }

    free(randomNumbers);
    free(shuffleArray);

}

unsigned int* inversePermutation(unsigned int* shuffleArray, unsigned int n) {
    unsigned int* tempShuffleArray = (unsigned int*)malloc(n * sizeof(unsigned int));
    unsigned int i;
    for (i = 0; i < n; ++i)
        tempShuffleArray[shuffleArray[i]] = i;
    return tempShuffleArray;
}

void decryptImage(struct Image image) {
    // deschid fisierul din care citesc r0 si sv
    FILE* fin = fopen("secret_key.txt", "r");
    if (!fin) {
        printf("Fisierul nu a fost deschis corect!\n");
        return;
    }

    unsigned int randomNumber0, startingValue;
    fscanf(fin, "%u%u", &randomNumber0, &startingValue);

    fclose(fin);

    unsigned int* randomNumbers;
    xorshift32(2 * image.height * image.width - 1, &randomNumbers, randomNumber0);

    unsigned int* shuffleArray;
    durstenfeld(&shuffleArray, randomNumbers, image.height * image.width);

    // CALCULARE INVERSA
    shuffleArray = inversePermutation(shuffleArray, image.height * image.width);

    unsigned int i;
    for (i = image.height* image.width - 1; i; --i) {
        image.content[i].R ^= image.content[i - 1].R ^ ((randomNumbers[image.height * image.width + i] >> (8 * 2)) & 0xFF);
        image.content[i].G ^= image.content[i - 1].G ^ ((randomNumbers[image.height * image.width + i] >> (8 * 1)) & 0xFF);
        image.content[i].B ^= image.content[i - 1].B ^ ((randomNumbers[image.height * image.width + i] >> (8 * 0)) & 0xFF);
    }

    image.content[0].R ^= ((startingValue >> (8 * 2)) & 0xFF) ^ ((randomNumbers[image.height * image.width] >> (8 * 2)) & 0xFF);
    image.content[0].G ^= ((startingValue >> (8 * 1)) & 0xFF) ^ ((randomNumbers[image.height * image.width] >> (8 * 1)) & 0xFF);
    image.content[0].B ^= ((startingValue >> (8 * 0)) & 0xFF) ^ ((randomNumbers[image.height * image.width] >> (8 * 0)) & 0xFF);

    shufflePixels(image, shuffleArray);

    free(randomNumbers);
    free(shuffleArray);
}

void printChiSquareTest(char* imagePath) {
    struct Image image;
    loadImageIntoMemory(imagePath, &image);

    double fbar = image.height * image.width / 256.0;

    double chiR = 0, chiG = 0, chiB = 0;
    unsigned int i, j, intensity;
    for (intensity = 0; intensity < 256; ++intensity) {
        double sumR = 0, sumG = 0, sumB = 0;
        for (i = 0; i < image.height; ++i)
            for (j = 0; j < image.width; ++j) {
                double fR = image.content[i * image.width + j].R;
                double fG = image.content[i * image.width + j].G;
                double fB = image.content[i * image.width + j].B;

                if (fR == intensity) ++sumR;
                if (fG == intensity) ++sumG;
                if (fB == intensity) ++sumB;
            }
        chiR += (sumR - fbar) * (sumR - fbar) / fbar;
        chiG += (sumG - fbar) * (sumG - fbar) / fbar;
        chiB += (sumB - fbar) * (sumB - fbar) / fbar;
    }

    printf("R: %0.2lf\n", chiR);
    printf("G: %0.2lf\n", chiG);
    printf("B: %0.2lf\n", chiB);

    // dezalocare
    free(image.header);
    free(image.content);
}

int main() {
    struct Image image;
    loadImageIntoMemory("enc_peppers_ok.bmp", &image);
    decryptImage(image);
    loadImageIntoFile("imagine.bmp", image);

    // printChiSquareTest("peppers.bmp");

    return 0;
}

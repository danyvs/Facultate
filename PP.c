#include <stdio.h>
#include <stdlib.h>

struct Pixel {
    unsigned char B, G, R;
};

struct Image {
    unsigned char* header;
    struct Pixel* content;
    unsigned int width, height;
    unsigned int padding;
};

/*  Functia xorshift32 genereaza numere intregi fara semn pe 32 de biti, cu un caracter pseudo aleator
 *  Primul parametru al functiei este numarul de numere ce se doresc a fi generate
 *  Al doilea parametru al functiei este seed-ul, adica numarul de la care se incepe generarea
 *  Functia returneaza un vector alocat dinamic cu (n + 1)(seed-ul + n numere) numere pseudo aleatoare
 */
unsigned int* xorshift32(unsigned int n, unsigned int seed) {
    unsigned int* randomNumbers = (unsigned int*)malloc((n + 1) * sizeof(unsigned int));
    unsigned int r, i;
    randomNumbers[0] = r = seed;
    for (i = 1; i <= n; ++i) {
        r ^= r << 13;
        r ^= r >> 17;
        r ^= r << 5;
        randomNumbers[i] = r;
    }
    return randomNumbers;
}

/*  Functia incarca o imaginea BMP in memoria interna in forma liniarizata
 *  Parametrul functiei este calea imaginii BMP
 *  Functia returneaza o structura Image, care contine informatii despre imagine
 */
struct Image loadImageIntoMemory(char *imagePath) {
    // se deschide imaginea BMP
    FILE* fin = fopen(imagePath, "rb");
    if (fin == NULL) {
        printf("Imaginea nu a fost deschisa corect!\n");
        exit(EXIT_FAILURE);
    }

    struct Image image;
    // se obtine header-ul
    image.header = (unsigned char*)malloc(54 * sizeof(unsigned char));
    fread(image.header, sizeof(unsigned char), 54, fin);

    // se obtine latimea imaginii
    fseek(fin, 18, SEEK_SET);
    fread(&image.width, sizeof(unsigned int), 1, fin);

    // se obtine inaltimea imaginii
    fread(&image.height, sizeof(unsigned int), 1, fin);

    // se calculeaza padding-ul imaginii
    image.padding = (image.width % 4) ? 4 - (3 * image.width) % 4 : 0;

    // se revine la inceputul continutului imaginii
    fseek(fin, 54, SEEK_SET);
    image.content = (struct Pixel*)malloc(image.width * image.height * sizeof(struct Pixel));

    // se citeste continutul imaginii
    // se citeste incepand din coltul stanga jos datorita modului in care este stocata o imagine BMP
    int i, j;
    for (i = image.height - 1; i >= 0; --i) {
        for (j = 0; j < image.width; ++j) {
            struct Pixel pixel;
            fread(&pixel, sizeof(unsigned char), 3, fin);
            image.content[i * image.width + j] = pixel;
        }
        // se sare peste padding
        fseek(fin, image.padding, SEEK_CUR);
    }

    fclose(fin);

    return image;
}

/*  Functia salveaza in memoria externa o imagine BMP stocata in forma liniarizata
 *  Primul parametru al functiei este calea fisierului in care va fi salvat imaginea
 *  Al doilea parametru al functia este imaginea in forma liniarizara
 */
void saveImageIntoFile(char* filePath, struct Image image) {
    // se deschide fisierul in care va fi salvata imaginea
    FILE* fout = fopen(filePath, "wb");

    // se scrie header-ul
    fwrite(image.header, sizeof(unsigned char), 54, fout);

    // se scrie continutul imaginii
    // se scrie incepand cu linia image.height - 1 datorita modului in care este stocata imaginea
    int i;
    for (i = image.height - 1; i >= 0; --i) {
        fwrite(&image.content[i * image.width], sizeof(struct Pixel), image.width, fout);

        // se scrie padding-ul
        unsigned char x = 0;
        fwrite(&x, sizeof(unsigned char), image.padding, fout);
    }

    fclose(fout);
}

/*  Functia genereaza o permutare aleatoare cu ajutorul algoritmului lui Durstenfeld si a numerelor
 * pseudo-aleatoare generate anterior
 *  Primul parametru al functiei este reprezentat de numerele pseduo-aleatoare generate anterior
 *  Al doilea parametru al functiei este numarul de elemente al permutarii
 *  Functia returneaza un vector care contine permutarea obtinuta in urma aplicarii algoritmului
 */
unsigned int* durstenfeld(unsigned int* randomNumbers, unsigned int n) {
    unsigned int* shuffleArray = (unsigned int*)malloc(n * sizeof(unsigned int));
    unsigned int i;

    for (i = 0; i < n; ++i)
        shuffleArray[i] = i;

    for (i = n - 1; i >= 1; --i) {
        unsigned int randNr = randomNumbers[n - i] % (i + 1);
        unsigned int temp = shuffleArray[randNr];
        shuffleArray[randNr] = shuffleArray[i];
        shuffleArray[i] = temp;
    }

    return shuffleArray;
}

/*  Functia permuta pixelii unei imaginii conform permutarii obtinute in urma aplicarii algoritmului
 * lui Durstenfeld
 *  Primul parametru al functiei este imaginea ai carei pixeli se permuta
 *  Al doilea parametru al functiei este vectorul ce contine permutarea aleatoare generata
 */
void shufflePixels(struct Image image, unsigned int* shuffleArray) {
    // se creeaza un vector temporar pentru permutarea pixelilor
    struct Pixel* tempImage = (struct Pixel*)malloc(image.height * image.width * sizeof(struct Pixel));

    // se permuta efectiv pixelii imaginii
    unsigned int i;
    for (i = 0; i < image.height * image.width; ++i)
        tempImage[shuffleArray[i]] = image.content[i];

    // se copiaza din vectorul temporar
    for (i = 0; i < image.height * image.width; ++i)
        image.content[i] = tempImage[i];
}

/*  Functia encryptImage aplica algoritmul de criptare
 *  Parametrul functiei este imaginea ce urmeaza sa fie criptata
 */
void encryptImage(struct Image image) {
    // deschid fisierul din care citesc R0 si SV
    FILE* fin = fopen("secret_key.txt", "r");
    if (!fin) {
        printf("Fisierul nu a fost deschis corect!\n");
        return;
    }

    unsigned int randomNumber0, startingValue;
    fscanf(fin, "%u%u", &randomNumber0, &startingValue);

    fclose(fin);

    // se genereaza numerele pseudo-aleatoare
    unsigned int* randomNumbers = xorshift32(2 * image.height * image.width - 1, randomNumber0);

    // se genereaza permutarea aleatoare
    unsigned int* shuffleArray = durstenfeld(randomNumbers, image.height * image.width);

    // se permuta pixelii
    shufflePixels(image, shuffleArray);

    // se aplica formula de criptare
    image.content[0].R ^= ((startingValue >> (8 * 2)) & 0xFF) ^ ((randomNumbers[image.height * image.width] >> (8 * 2)) & 0xFF);
    image.content[0].G ^= ((startingValue >> (8 * 1)) & 0xFF) ^ ((randomNumbers[image.height * image.width] >> (8 * 1)) & 0xFF);
    image.content[0].B ^= ((startingValue >> (8 * 0)) & 0xFF) ^ ((randomNumbers[image.height * image.width] >> (8 * 0)) & 0xFF);

    unsigned int i;
    for (i = 1; i <= image.height* image.width - 1; ++i) {
        image.content[i].R ^= image.content[i - 1].R ^ ((randomNumbers[image.height * image.width + i] >> (8 * 2)) & 0xFF);
        image.content[i].G ^= image.content[i - 1].G ^ ((randomNumbers[image.height * image.width + i] >> (8 * 1)) & 0xFF);
        image.content[i].B ^= image.content[i - 1].B ^ ((randomNumbers[image.height * image.width + i] >> (8 * 0)) & 0xFF);
    }

    // se dezaloca
    free(randomNumbers);
    free(shuffleArray);
}

/*  Functia inverse permutation calculeaza inversa unei permutari date
 *  Primul parametru al functiei este un vector ce contine permutarea
 *  Al doilea parametru al functiei este numarul de elemente al permutarii
 *  Functia returneaza un vector ce contine inversa permutarii date
 */
unsigned int* inversePermutation(unsigned int* shuffleArray, unsigned int n) {
    unsigned int* tempShuffleArray = (unsigned int*)malloc(n * sizeof(unsigned int));

    unsigned int i;
    for (i = 0; i < n; ++i)
        tempShuffleArray[shuffleArray[i]] = i;

    return tempShuffleArray;
}

/*  Functia decryptImage aplica algoritmul de decriptare
 *  Parametrul functiei este imaginea ce urmeaza sa fie decriptata
 */
void decryptImage(struct Image image) {
    // deschid fisierul din care citesc R0 si SV
    FILE* fin = fopen("secret_key.txt", "r");
    if (!fin) {
        printf("Fisierul nu a fost deschis corect!\n");
        return;
    }

    unsigned int randomNumber0, startingValue;
    fscanf(fin, "%u%u", &randomNumber0, &startingValue);

    fclose(fin);

    // se genereaza numerele pseudo-aleatoare
    unsigned int* randomNumbers = xorshift32(2 * image.height * image.width - 1, randomNumber0);

    // se genereaza permutarea aleatoare
    unsigned int* shuffleArray = durstenfeld(randomNumbers, image.height * image.width);

    // se calculeaza inversa permutarii aleatoare
    shuffleArray = inversePermutation(shuffleArray, image.height * image.width);

    // se aplica algoritmul de decriptare
    unsigned int i;
    for (i = image.height* image.width - 1; i; --i) {
        image.content[i].R ^= image.content[i - 1].R ^ ((randomNumbers[image.height * image.width + i] >> (8 * 2)) & 0xFF);
        image.content[i].G ^= image.content[i - 1].G ^ ((randomNumbers[image.height * image.width + i] >> (8 * 1)) & 0xFF);
        image.content[i].B ^= image.content[i - 1].B ^ ((randomNumbers[image.height * image.width + i] >> (8 * 0)) & 0xFF);
    }

    image.content[0].R ^= ((startingValue >> (8 * 2)) & 0xFF) ^ ((randomNumbers[image.height * image.width] >> (8 * 2)) & 0xFF);
    image.content[0].G ^= ((startingValue >> (8 * 1)) & 0xFF) ^ ((randomNumbers[image.height * image.width] >> (8 * 1)) & 0xFF);
    image.content[0].B ^= ((startingValue >> (8 * 0)) & 0xFF) ^ ((randomNumbers[image.height * image.width] >> (8 * 0)) & 0xFF);

    // se permuta pixelii
    shufflePixels(image, shuffleArray);

    // se dezaloca
    free(randomNumbers);
    free(shuffleArray);
}

/*  Functia afiseaza valorile testului chi-patrat pentru o imagine BMP pe fiecare
 * canal de culoare (R, G, B)
 *  Functia are ca parametru calea imaginii
 */
void printChiSquareTest(char* imagePath) {
    /* se incarca imaginea din fisier in memoria interna pentru a determina mai usor valorile
     * testului chi-patrat */
    struct Image image = loadImageIntoMemory(imagePath);

    // se calculeaza frecventa estimata teoretic
    double fbar = image.height * image.width / 256.0;

    double chiR = 0, chiG = 0, chiB = 0; // valoarea testului chi-patrat pentru fiecare canal
    unsigned int i, j, intensity;
    for (intensity = 0; intensity < 256; ++intensity) {
        double sumR = 0, sumG = 0, sumB = 0; // frecventa
        for (i = 0; i < image.height; ++i)
            for (j = 0; j < image.width; ++j) {
                // se determina intensitatea pentru fiecare canal de culoare
                double fR = image.content[i * image.width + j].R;
                double fG = image.content[i * image.width + j].G;
                double fB = image.content[i * image.width + j].B;

                // actualizare frecventa
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

    // se dezaloca
    free(image.header);
    free(image.content);
}

int main() {
    struct Image image;
    image = loadImageIntoMemory("peppers.bmp");
    encryptImage(image);
    decryptImage(image);
    saveImageIntoFile("imagine.bmp", image);
    printChiSquareTest("imagine.bmp");

    return 0;
}

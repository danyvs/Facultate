#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct Pixel {
    unsigned char B, G, R;
};

struct Image {
    unsigned char* header;
    struct Pixel* content;
    unsigned int width, height;
    unsigned int padding;
};

struct Window {
    unsigned int startLine, startColumn;
    unsigned int height, width;
    double correlation;
    struct Pixel color;
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
 *  Functia are ca parametru calea imaginii BMP
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
 *  Primul parametru al functiei este calea fisierului in care va fi salvata imaginea
 *  Al doilea parametru al functiei este structura care contine imaginea in forma liniarizata
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
 *  Primul parametru al functiei este numarul de elemente al permutarii
 *  Al doilea parametru al functiei este reprezentat de numerele pseduo-aleatoare generate anterior
 *  Functia returneaza un vector care contine permutarea obtinuta in urma aplicarii algoritmului
 */
unsigned int* durstenfeld(unsigned int n, unsigned int* randomNumbers) {
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

/*  Functia permuta pixelii unei imaginii conform unei permutari date
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
 *  Primul parametru al functiei este calea imaginii intiale
 *  Al doilea parametru al functiei este calea imaginii criptate
 *  Al treilea parametru al functiei este calea unui fisier text care contine cheia secreta
 */
void encryptImage(char* sourceFilePath, char* destinationFilePath, char* keysFilePath) {
    // deschid fisierul din care citesc R0 si SV
    FILE* finKeys = fopen(keysFilePath, "r");
    if (!finKeys) {
        printf("Fisierul nu a fost deschis corect!\n");
        return;
    }

    unsigned int randomNumber0, startingValue;
    fscanf(finKeys, "%u%u", &randomNumber0, &startingValue);

    fclose(finKeys);

    struct Image image = loadImageIntoMemory(sourceFilePath);

    // se genereaza numerele pseudo-aleatoare
    unsigned int* randomNumbers = xorshift32(2 * image.height * image.width - 1, randomNumber0);

    // se genereaza permutarea aleatoare
    unsigned int* shuffleArray = durstenfeld(image.height * image.width, randomNumbers);

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

    saveImageIntoFile(destinationFilePath, image);

    free(randomNumbers);
    free(shuffleArray);
    free(image.header);
    free(image.content);
}

/*  Functia inverse permutation calculeaza inversa unei permutari date
 *  Primul parametru al functiei este numarul de elemente al permutarii
 *  Al doilea parametru al functiei este un vector ce contine permutarea
 *  Functia returneaza un vector ce contine inversa permutarii initiale
 */
unsigned int* inversePermutation(unsigned int n, unsigned int* shuffleArray) {
    unsigned int* tempShuffleArray = (unsigned int*)malloc(n * sizeof(unsigned int));

    unsigned int i;
    for (i = 0; i < n; ++i)
        tempShuffleArray[shuffleArray[i]] = i;

    return tempShuffleArray;
}

/*  Functia decryptImage aplica algoritmul de decriptare
 *  Primul parametru al functiei este calea imaginii initiale/ decriptate
 *  Al doilea parametru al functiei este calea imaginii criptate
 *  Al treilea parametru al functiei este calea unui fisier text care contine cheia secreta
 */
void decryptImage(char* filePath, char* encryptedFilePath, char* keysFilePath) {
    // deschid fisierul din care citesc R0 si SV
    FILE* fin = fopen("secret_key.txt", "r");
    if (!fin) {
        printf("Fisierul nu a fost deschis corect!\n");
        return;
    }

    unsigned int randomNumber0, startingValue;
    fscanf(fin, "%u%u", &randomNumber0, &startingValue);

    fclose(fin);

    // encryptImage(initFilePath, encryptedFilePath, keysFilePath);

    struct Image image = loadImageIntoMemory(encryptedFilePath);

    // se genereaza numerele pseudo-aleatoare
    unsigned int* randomNumbers = xorshift32(2 * image.height * image.width - 1, randomNumber0);

    // se genereaza permutarea aleatoare
    unsigned int* shuffleArray = durstenfeld(image.height * image.width, randomNumbers);

    // se calculeaza inversa permutarii aleatoare
    shuffleArray = inversePermutation(image.height * image.width, shuffleArray);

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

    saveImageIntoFile(filePath, image);

    free(randomNumbers);
    free(shuffleArray);
    free(image.header);
    free(image.content);
}

/*  Functia afiseaza valorile testului chi-patrat pentru o imagine BMP pe fiecare canal de culoare (R, G, B)
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

    free(image.header);
    free(image.content);
}

/*  Functia grayscaleImage transforma o imagine color in imagine grayscale
 *  Primul parametru al functiei este calea imaginii sursa
 *  Al doilea parametru al functiei este calea imaginii modificate (grayscale)
 */
void grayscaleImage(char* sourceFilePath, char* destinationFilePath) {
    struct Image image = loadImageIntoMemory(sourceFilePath);

    unsigned int i, j;
    for (i = 0; i < image.height; ++i)
        for (j = 0; j < image.width; ++j) {
            unsigned int idx = i * image.width + j;
            unsigned char gray = 0.299 * image.content[idx].R + 0.587 * image.content[idx].G + 0.114 * image.content[i].B;
            image.content[idx].R = image.content[idx].G = image.content[idx].B = gray;
        }

    saveImageIntoFile(destinationFilePath, image);

    free(image.header);
    free(image.content);
}

/*  Functia calculateCorrelation calculeaza corelatia, folosind formula data, dintre o imagine si un
 * sablon, incepand cu linia line si coloana column
 *  Primul parametru al functiei este structura ce contine imaginea
 *  Al doilea parametru al functiei este structura ce contine sablonul
 *  Al treilea parametru al functiei este linia la care se incepe calcularea corelatiei
 *  Al patrulea parametru al functiei este coloana la care se incepe calcularea corelatiei
 */
double calculateCorrelation(struct Image image, struct Image template, unsigned int line, unsigned int column) {
    // struct Image image = loadImageIntoMemory(imagePath);
    // struct Image template = loadImageIntoMemory(templatePath);

    unsigned int i, j;

    // numarul de pixeli
    unsigned int cntPixels = template.height * template.width;

    // media valorilor intensitatilor grayscale a pixelilor din fereastra
    double mediaPixelsIntensityImage = 0;
    for (i = 0; i < template.height; ++i)
        for (j = 0; j < template.width; ++j)
            mediaPixelsIntensityImage += image.content[(i + line) * image.width + (j + column)].R;
    mediaPixelsIntensityImage /= cntPixels;

    // media valorilor intensitatilor grayscale a pixelilor din sablon
    double mediaPixelsIntensityTemplate = 0;
    for (i = 0; i < template.height; ++i)
        for (j = 0; j < template.width; ++j)
            mediaPixelsIntensityTemplate += template.content[i * template.width + j].R;
    mediaPixelsIntensityTemplate /= cntPixels;

    // deviatia standard a valorilor intensitatilor grayscale a pixelilor din fereastra
    double standardDeviationImage = 0;
    for (i = 0; i < template.height; ++i)
        for (j = 0; j < template.width; ++j) {
            double pixelDeviation = image.content[(i + line) * image.width + (j + column)].R - mediaPixelsIntensityImage;
            pixelDeviation *= pixelDeviation;
            standardDeviationImage += pixelDeviation;
        }
    standardDeviationImage /= (cntPixels - 1);
    standardDeviationImage = sqrt(standardDeviationImage);

    // deviatia standard a valorilor intensitatilor grayscale a pixelilor din sablon
    double standardDeviationTemplate = 0;
    for (i = 0; i < template.height; ++i)
        for (j = 0; j < template.width; ++j) {
            double pixelDeviation = template.content[i * template.width + j].R - mediaPixelsIntensityTemplate;
            pixelDeviation *= pixelDeviation;
            standardDeviationTemplate += pixelDeviation;
        }
    standardDeviationTemplate /= (cntPixels - 1);
    standardDeviationTemplate = sqrt(standardDeviationTemplate);

    double correlation = 0;
    for (i = 0; i < template.height; ++i)
        for (j = 0; j < template.width; ++j) {
            unsigned char intensityPixelImage = image.content[(i + line) * image.width + (j + column)].R;
            unsigned char intensityPixelTemplate = template.content[i * template.width + j].R;

            double currentCorrelation = intensityPixelImage - mediaPixelsIntensityImage;
            currentCorrelation *= (intensityPixelTemplate - mediaPixelsIntensityTemplate);
            currentCorrelation /= standardDeviationImage;
            currentCorrelation /= standardDeviationTemplate;
            correlation += currentCorrelation;
        }
    correlation /= cntPixels;

    return correlation;
}

/*  Functia templateMatching returneaza prin intermediul parametrilor toate ferestrele sabloanelor
 * care au un grad de corelatie cu imaginea cel putin egal cu threshold
 *  Primul parametru al functiei este calea imagineii pe care se face template-matching
 *  Al doilea parametru al functiei este sablonul cu care se face template-matching
 *  Al treilea parametru al functiei este pragul pe care trebuie sa il aiba un sablon pentru a fi
 * considerat detectie corecta
 *  Al patrulea parametru al functiei este vectorul de ferestre prin intermediul caruia se returneaza
 * toate ferestrele care au gradul de corelatie cel putin egal cu pragul
 *  Al cincilea parametru al functiei este numarul de elemente al vectorului de detectii
 */
void templateMatching(char* imagePath, char* templatePath, double threshold, struct Window** matches, unsigned int* cntMatches) {
    struct Image image = loadImageIntoMemory(imagePath);
    struct Image template = loadImageIntoMemory(templatePath);

    *matches = NULL;
    *cntMatches = 0;

    unsigned int i, j;
    for (i = 0; i + template.height < image.height; ++i)
        for (j = 0; j + template.width < image.width; ++j) {
            double corr = calculateCorrelation(image, template, i, j);
            if (corr >= threshold) {
                ++(*cntMatches);
                struct Window* tempWindows = (struct Window*)realloc((*matches), (*cntMatches) * sizeof(struct Window));
                if (!tempWindows) {
                    printf("Nu s-a putut realoca memorie!\n");
                    free(*matches);
                    exit(EXIT_FAILURE);
                }
                else {
                    *matches = tempWindows;
                    (*matches)[*cntMatches - 1].startLine = i;
                    (*matches)[*cntMatches - 1].startColumn = j;
                    (*matches)[*cntMatches - 1].height = template.height;
                    (*matches)[*cntMatches - 1].width = template.width;
                    (*matches)[*cntMatches - 1].correlation = corr;
                }
            }
        }

    free(image.header);
    free(image.content);
    free(template.header);
    free(template.content);
}

/*  Functia drawBorderWindow deseneaza conturul unei ferestre date cu o culoare data
 *  Primul parametru al functiei este structura care contine imaginea pe care se va desena
 *  Al doilea parametru al functiei este structura care memoreaza date despre fereastra
 *  Al treilea parametru al functiei este culoare cu care se va desena conturul
 */
void drawBorderWindow(struct Image image, struct Window window, struct Pixel color) {
    unsigned int i;
    // vertical
    for (i = 0; i < window.height; ++i) {
        image.content[(i + window.startLine) * image.width + window.startColumn] = color;
        image.content[(i + window.startLine) * image.width + (window.startColumn + window.width - 1)] = color;
    }
    // orizontal
    for (i = 0; i < window.width; ++i) {
        image.content[window.startLine * image.width + window.startColumn + i] = color;
        image.content[(window.startLine + window.height - 1) * image.width + window.startColumn + i] = color;
    }
}

/*  Functia returneaza un vector ce contine culorile cu care vor fi conturate cifrele
 */
struct Pixel* initColorsForPixels() {
    struct Pixel* colorOfNumbers = (struct Pixel*)malloc(10 * sizeof(struct Pixel));

    colorOfNumbers[0].R = 255, colorOfNumbers[0].G =   0, colorOfNumbers[0].B =   0;
    colorOfNumbers[1].R = 255, colorOfNumbers[1].G = 255, colorOfNumbers[1].B =   0;
    colorOfNumbers[2].R =   0, colorOfNumbers[2].G = 255, colorOfNumbers[2].B =   0;
    colorOfNumbers[3].R =   0, colorOfNumbers[3].G = 255, colorOfNumbers[3].B = 255;
    colorOfNumbers[4].R = 255, colorOfNumbers[4].G =   0, colorOfNumbers[4].B = 255;
    colorOfNumbers[5].R =   0, colorOfNumbers[5].G =   0, colorOfNumbers[5].B = 255;
    colorOfNumbers[6].R = 192, colorOfNumbers[6].G = 192, colorOfNumbers[6].B = 192;
    colorOfNumbers[7].R = 255, colorOfNumbers[7].G = 140, colorOfNumbers[7].B =   0;
    colorOfNumbers[8].R = 128, colorOfNumbers[8].G =   0, colorOfNumbers[8].B = 128;
    colorOfNumbers[9].R = 128, colorOfNumbers[9].G =   0, colorOfNumbers[9].B =   0;

    return colorOfNumbers;
}

/*  Functia getAllMatches returneaza prin intermediul parametrilor toate detectiile care au un
 * grad de corespondenta cel putin egal cu un prag dat
 *  Primul parametru al functiei este vectorul care va memora detectiile
 *  Al doilea parametru al functiei este numarul de elemente al vectorului de detectii
 */
void getAllMatches(struct Window** allMatches, unsigned int *cntAllMatches) {
    struct Pixel* colorOfNumbers = initColorsForPixels();

    char filePath[20];
    printf("Introduceti numele fisierului care contine denumirile imaginilor: ");
    scanf("%s", filePath);

    FILE* fin = fopen(filePath, "r");
    if (!fin) {
        printf("Fisierul nu a fost deschis corect!\n");
        exit(EXIT_FAILURE);
    }

    // se face imaginea grayscale
    fscanf(fin, "%s", filePath);
    grayscaleImage(filePath, "grayscaleImage.bmp");

    // se stabileste pragul de detectie
    double threshold;
    printf("Introduceti pragul de detectie: ");
    scanf("%lf", &threshold);

    *allMatches = NULL;
    *cntAllMatches = 0;

    // se prea numarul de sabloane cu care se va face template-matching
    unsigned int cntTemplates;
    fscanf(fin, "%u", &cntTemplates);

    unsigned int i, j;
    for (i = 0; i < cntTemplates; ++i) {
        // se sablonul grayscale
        fscanf(fin, "%s", filePath);
        grayscaleImage(filePath, "grayscaleTemplate.bmp");

        // se obtin potrivirile pentru sablonul curent
        unsigned int cntMatches;
        struct Window* matches;
        templateMatching("grayscaleImage.bmp", "grayscaleTemplate.bmp", threshold, &matches, &cntMatches);

        // se memoreaza culoarea cu care va fi colorata potrivirea
        for (j = 0; j < cntMatches; ++j)
            matches[j].color = colorOfNumbers[i];

        // se realoca memorie pentru vectorul care contine toate potrivirile
        *cntAllMatches += cntMatches;
        struct Window* tempAllMatches = (struct Window*)realloc(*allMatches, (*cntAllMatches) * sizeof(struct Window));
        if (!tempAllMatches && cntMatches) {
            printf("Nu s-a putut realoca memorie!\n");
            free(*allMatches);
            exit(EXIT_FAILURE);
        }
        else {
            *allMatches = tempAllMatches;
            for (j = 0; j < cntMatches; ++j)
                (*allMatches)[*cntAllMatches - cntMatches + j] = matches[j];
        }
    }

    fclose(fin);
}

unsigned int min(unsigned int a, unsigned int b) {
    return a < b ? a : b;
}

unsigned int max(unsigned int a, unsigned int b) {
    return a > b ? a : b;
}

/*  Functia intersection calculeaza gradul de intersectie a 2 ferestre
 *  Parametrii functiei sunt cele 2 ferestre
 */
double intersection(struct Window a, struct Window b) {
    // (x1, y1) - coltul din stanga sus
    int x1 = max(a.startLine, b.startLine);
    int y1 = max(a.startColumn, b.startColumn);
    // (x2, y2) - coltul din dreapta jos
    int x2 = min(a.startLine + a.height, b.startLine + b.height);
    int y2 = min(a.startColumn + a.width, b.startColumn + b.width);

    double res = 0;
    if (a.startLine <= x1 && x1 <= a.startLine + a.height && a.startColumn <= y1 && y1 <= a.startColumn + a.width)
        if (a.startLine <= x2 && x2 <= a.startLine + a.height && a.startColumn <= y2 && y2 <= a.startColumn + a.width) {
            res = (x2 - x1) * (y2 - y1);
            res /= (a.height * a.width + b.height * b.width - res);
        }

    return res;
}

/*  Functia compareByCorrelation reprezinta criteriul de comparare pentru qsort
 *  In acest caz, aceasta functie ajuta la sortarea descrescatoare a ferestrelor dupa corelatie
 */
int compareByCorrelation(const void* a, const void* b) {
    struct Window x = *(struct Window*)a;
    struct Window y = *(struct Window*)b;
    if (x.correlation < y.correlation)
        return 1;
    else
        return -1;
}

/*  Functia nonMaximalElimination elimina toate detectiile care se suprapun, pastrandu-le doar pe
 * cele cu gradul de corelatie cel mai mare
 *  Primul parametru al functiei este vectorul care memoreaza detectiile
 *  Al doilea parametru al functiei este numarul de detectii
 */
void nonMaximalElimination(struct Window** allMatches, unsigned int* cntAllMatches) {
    // vector care memoreaza daca un element a fost eliminat (1) sau nu (0)
    int* eliminated = (int*)malloc(*cntAllMatches * sizeof(int));

    unsigned int i, j;

    for (i = 0; i < *cntAllMatches; ++i)
        eliminated[i] = 0;

    for (i = 0; i < *cntAllMatches; ++i)
        if (!eliminated[i])
            for (j = i + 1; j < *cntAllMatches; ++j)
                if (!eliminated[j])
                    if (intersection((*allMatches)[i], (*allMatches)[j]) > 0.2)
                        eliminated[j] = 1;

    // se pastreaza doar elementele care nu au fost marcate pentru eliminare
    unsigned int index = 0;
    for (i = 0; i < (*cntAllMatches); ++i)
        if (!eliminated[i])
            (*allMatches)[index++] = (*allMatches)[i];
    *cntAllMatches = index;

    struct Window* tempAllMatches = (struct Window*)realloc(*allMatches, (*cntAllMatches) * sizeof(struct Window));
    if (!tempAllMatches) {
        printf("Nu s-a putut realoca memorie!\n");
        free(*allMatches);
        exit(EXIT_FAILURE);
    }
    else
        *allMatches = tempAllMatches;
}

/*  Functia drawBorders deseneaza contururile ferestrelor
 *  Primul parametru al functiei este calea imaginii pe care se vor desena contururile
 *  Al doilea parametru al functiei este calea fisierului unde va fi salvata imaginea
 *  Al treilea parametru al functiei este vectorul care contine detectiile
 *  Al patrulea parametru al functiei este numarul de detectii
 */
void drawBorders(char* sourcefilePath, char* destinationFilePath, struct Window* allMatches, unsigned int cntAllMatches) {
    struct Image image = loadImageIntoMemory(sourcefilePath);

    unsigned int i;
    for (i = 0; i < cntAllMatches; ++i)
        drawBorderWindow(image, allMatches[i], allMatches[i].color);

    saveImageIntoFile(destinationFilePath, image);
}

int main() {
    char imagePath[20], encryptedImagePath[20], secretKeysTextPath[20], finalImagePath[20];

    printf("Introduceti calea imaginii initiale: ");
    scanf("%s", imagePath);
    printf("Introduceti calea imaginii criptate: ");
    scanf("%s", encryptedImagePath);
    printf("Introduceti calea fisierului care contine cheia secreta: ");
    scanf("%s", secretKeysTextPath);

    encryptImage(imagePath, encryptedImagePath, secretKeysTextPath);
    decryptImage(imagePath, encryptedImagePath, secretKeysTextPath);

    printf("Valorile testului chi-patrat pentru imaginea initiala sunt:\n");
    printChiSquareTest(imagePath);
    printf("Valorile testului chi-patrat pentru imaginea criptata sunt:\n");
    printChiSquareTest(encryptedImagePath);

    unsigned int cntAllMatches;
    struct Window* allMatches;

    getAllMatches(&allMatches, &cntAllMatches);

    qsort(allMatches, cntAllMatches, sizeof(struct Window), compareByCorrelation);
    nonMaximalElimination(&allMatches, &cntAllMatches);

    printf("Introduceti calea imaginii pe care se vor desena chenarele: ");
    scanf("%s", finalImagePath);
    drawBorders(imagePath, finalImagePath, allMatches, cntAllMatches);

    return 0;
}
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
        fseek(fin, (long)image.padding, SEEK_CUR);
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

void grayscaleImage(struct Image image) {
    unsigned int i, j;
    for (i = 0; i < image.height; ++i)
        for (j = 0; j < image.width; ++j) {
            unsigned int idx = i * image.width + j;
            unsigned char gray = 0.299 * image.content[idx].R + 0.587 * image.content[idx].G + 0.114 * image.content[i].B;
            image.content[idx].R = image.content[idx].G = image.content[idx].B = gray;
        }
}

double calculateCorrelation(struct Image image, struct Image template, unsigned int line, unsigned int column) {
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

void templateMatching(struct Image image, struct Image template, double threshold, struct Window** matches, unsigned int* cntMatches) {
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
}

void drawBorderWindow(struct Image image, struct Window window, struct Pixel color) {
    unsigned int i;
    // vertical
    for (i = 0; i < window.height; ++i) {
        image.content[(i + window.startLine) * image.width + window.startColumn] = color;
        image.content[(i + window.startLine) * image.width + (window.startColumn + window.width - 1)] = color;
    }
    // horizontal
    for (i = 0; i < window.width; ++i) {
        image.content[window.startLine * image.width + window.startColumn + i] = color;
        image.content[(window.startLine + window.height - 1) * image.width + window.startColumn + i] = color;
    }
}

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

void getAllMatches(struct Window** allMatches, unsigned int *cntAllMatches) {
    struct Pixel* colorOfNumbers = initColorsForPixels();

    char filePath[20];
    /// printf("Introduceti numele fisierului care contine denumirile imaginilor: ");
    /// scanf("%s", filePath);

    FILE* fin = fopen("ftm.txt", "r");
    if (!fin) {
        printf("Fisierul nu a fost deschis corect!\n");
        exit(EXIT_FAILURE);
    }

    // se incarca in memorie imaginea pe care se va face template-matching si se face grayscale
    fscanf(fin, "%s", filePath);
    struct Image image = loadImageIntoMemory(filePath);
    // saveImageIntoFile("image.bmp", image);
    grayscaleImage(image);

    // se stabileste pragul de detectie
    double threshold = 0.5;
    ///  printf("Introduceti pragul de detectie: ");
    /// scanf("%lf", &threshold);

    *allMatches = NULL;
    *cntAllMatches = 0;

    unsigned int i, j;
    for (i = 0; i < 10; ++i) {
        // se incarca in memorie sablonul si se face grayscale
        fscanf(fin, "%s", filePath);
        struct Image template = loadImageIntoMemory(filePath);
        grayscaleImage(template);

        // se obtin potrivirile pentru sablonul curent
        unsigned int cntMatches;
        struct Window* matches;
        templateMatching(image, template, threshold, &matches, &cntMatches);

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

double intersection(struct Window a, struct Window b) {
    unsigned int x1 = max(a.startLine, b.startLine);
    unsigned int y1 = max(a.startColumn, b.startColumn);
    unsigned int x2 = min(a.startLine + a.height, b.startLine + b.height);
    unsigned int y2 = min(a.startColumn + a.width, b.startColumn + b.width);

    double res = (x2 - x1) * (y2 - y1);
    res /= (a.height * a.width + b.height * b.width - res);

    return res;
}

int compareByCorrelation(const void* a, const void* b) {
    struct Window x = *(struct Window*)a;
    struct Window y = *(struct Window*)b;
    if (x.correlation < y.correlation)
        return 1;
    else
        return -1;
}

int main() {
    unsigned int cntAllMatches;
    struct Window* allMatches;
    getAllMatches(&allMatches, &cntAllMatches);
    qsort(allMatches, cntAllMatches, sizeof(struct Window), compareByCorrelation);
    return 0;
}

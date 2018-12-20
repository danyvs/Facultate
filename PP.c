#include <stdio.h>
#include <stdlib.h>

struct RGB {
    unsigned char B, G, R;
};

struct Imagine {
    unsigned char* header;
    struct RGB* continut;
    unsigned int latimeImagine, inaltimeImagine;
};

/*
void printXORSHIFT32(int n) {
    unsigned int r, seed, i;
    seed = 1000;
    r = seed;
    for (i = 0; i < n; ++i) {
        r = r ^ r << 13;
        r = r ^ r >> 17;
        r = r ^ r << 5;
        printf("%u\n", r);
    }
}
*/

/* Incarca imaginea in memorie in forma liniarizata
 *
 */
void incarcareImagineInMemorie(char *caleImagineSursa, struct Imagine* imagine) {
    FILE* fin;

    unsigned int i, j;

    fin = fopen(caleImagineSursa, "rb");
    if (fin == NULL) {
        printf("Imaginea nu a fost deschisa corect!\n");
        return;
    }

    // incarcare header
    imagine->header = (unsigned char*)malloc(54 * sizeof(unsigned char));
    fread(imagine->header, sizeof(unsigned char), 54, fin);

    // obtinere latime imagine
    fseek(fin, 18, SEEK_SET);
    fread(&imagine->latimeImagine, sizeof(unsigned int), 1, fin);

    // obtinere inaltime imagine
    fread(&imagine->inaltimeImagine, sizeof(unsigned int), 1, fin);

    fseek(fin, 54, SEEK_SET);
    imagine->continut = (struct RGB*)malloc(imagine->latimeImagine * imagine->inaltimeImagine * sizeof(struct RGB));
    for (i = 0; i < imagine->inaltimeImagine; ++i)
        for (j = 0; j < imagine->latimeImagine; ++j) {
            struct RGB pixel;
            fread(&pixel, sizeof(unsigned char), 3, fin);
            imagine->continut[i * imagine->latimeImagine + j] = pixel;
        }

    fclose(fin);
}

void incarcareImagineInFisier(char* numeFisierDestinatie, struct Imagine imagine) {
    FILE* fout = fopen(numeFisierDestinatie, "wb");

    fwrite(imagine.header, sizeof(unsigned char), 54, fout);

    fwrite(imagine.continut, sizeof(struct RGB), imagine.latimeImagine * imagine.inaltimeImagine, fout);

    fclose(fout);
}

int main() {
    struct Imagine imagine;
    incarcareImagineInMemorie("peppers.bmp", &imagine);
    incarcareImagineInFisier("imagine.bmp", imagine);

    return 0;
}

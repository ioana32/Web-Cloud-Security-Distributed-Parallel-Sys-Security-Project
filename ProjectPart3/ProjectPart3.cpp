#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <openssl/aes.h>
#include <omp.h>

// Funcție pentru a calcula dimensiunea fișierului
long int getFileSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

// Funcție pentru criptare ECB paralelă folosind OpenMP
void encryptECB(const char* inputFilePath, const char* outputFilePath, const unsigned char* key) {
    FILE* fSrc = fopen(inputFilePath, "rb");
    FILE* fDst = fopen(outputFilePath, "wb");

    if (!fSrc || !fDst) {
        printf("Error opening file.\n");
        return;
    }

    long int inLen = getFileSize(fSrc);
    long int outLen = ((inLen / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;

    unsigned char* inBuf = (unsigned char*)malloc(outLen);
    unsigned char* outBuf = (unsigned char*)malloc(outLen);

    memset(inBuf, 0x00, outLen);
    fread(inBuf, 1, inLen, fSrc);

    AES_KEY aes_key;
    AES_set_encrypt_key(key, 128, &aes_key);

    // Utilizăm OpenMP pentru a paraleliza criptarea ECB
#pragma omp parallel for
    for (int i = 0; i < outLen / AES_BLOCK_SIZE; i++) {
        AES_encrypt(&(inBuf[i * AES_BLOCK_SIZE]), &(outBuf[i * AES_BLOCK_SIZE]), &aes_key);
    }

    fwrite(&inLen, sizeof(inLen), 1, fDst);
    fwrite(outBuf, 1, outLen, fDst);

    free(outBuf);
    free(inBuf);
    fclose(fDst);
    fclose(fSrc);
}

// Funcție pentru decriptare ECB paralelă folosind OpenMP
void decryptECB(const char* inputFilePath, const char* outputFilePath, const unsigned char* key) {
    FILE* fSrc = fopen(inputFilePath, "rb");
    FILE* fDst = fopen(outputFilePath, "wb");

    if (!fSrc || !fDst) {
        printf("Error opening file.\n");
        return;
    }

    long int inLen = 0;
    fread(&inLen, sizeof(inLen), 1, fSrc);

    long int outLen = getFileSize(fSrc) - sizeof(long int);

    unsigned char* inBuf = (unsigned char*)malloc(outLen);
    unsigned char* outBuf = (unsigned char*)malloc(outLen);

    fread(inBuf, 1, outLen, fSrc);

    AES_KEY aes_key;
    AES_set_decrypt_key(key, 128, &aes_key);

    // Utilizăm OpenMP pentru a paraleliza decriptarea ECB
#pragma omp parallel for
    for (int i = 0; i < outLen / AES_BLOCK_SIZE; i++) {
        AES_decrypt(&(inBuf[i * AES_BLOCK_SIZE]), &(outBuf[i * AES_BLOCK_SIZE]), &aes_key);
    }

    fwrite(outBuf, 1, inLen, fDst);

    free(outBuf);
    free(inBuf);
    fclose(fDst);
    fclose(fSrc);
}

// Funcție pentru criptare CBC paralelă folosind OpenMP
void encryptCBC(const char* inputFilePath, const char* outputFilePath, const unsigned char* key, unsigned char* iv) {
    FILE* fSrc = fopen(inputFilePath, "rb");
    FILE* fDst = fopen(outputFilePath, "wb");

    if (!fSrc || !fDst) {
        printf("Error opening file.\n");
        return;
    }

    long int inLen = getFileSize(fSrc);
    long int outLen = ((inLen / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;

    unsigned char* inBuf = (unsigned char*)malloc(outLen);
    unsigned char* outBuf = (unsigned char*)malloc(outLen);

    memset(inBuf, 0x00, outLen);
    fread(inBuf, 1, inLen, fSrc);

    AES_KEY aes_key;
    AES_set_encrypt_key(key, 128, &aes_key);

    AES_cbc_encrypt(inBuf, outBuf, inLen, &aes_key, iv, AES_ENCRYPT);

    fwrite(&inLen, sizeof(inLen), 1, fDst);
    fwrite(outBuf, 1, outLen, fDst);

    free(outBuf);
    free(inBuf);
    fclose(fDst);
    fclose(fSrc);
}

// Funcție pentru decriptare CBC paralelă folosind OpenMP
void decryptCBC(const char* inputFilePath, const char* outputFilePath, const unsigned char* key, unsigned char* iv) {
    FILE* fSrc = fopen(inputFilePath, "rb");
    FILE* fDst = fopen(outputFilePath, "wb");

    if (!fSrc || !fDst) {
        printf("Error opening file.\n");
        return;
    }

    long int inLen = 0;
    fread(&inLen, sizeof(inLen), 1, fSrc);

    long int outLen = getFileSize(fSrc) - sizeof(long int);

    unsigned char* inBuf = (unsigned char*)malloc(outLen);
    unsigned char* outBuf = (unsigned char*)malloc(outLen);

    fread(inBuf, 1, outLen, fSrc);

    AES_KEY aes_key;
    AES_set_decrypt_key(key, 128, &aes_key);

    AES_cbc_encrypt(inBuf, outBuf, outLen, &aes_key, iv, AES_DECRYPT);

    fwrite(outBuf, 1, inLen, fDst);

    free(outBuf);
    free(inBuf);
    fclose(fDst);
    fclose(fSrc);
}

// Funcție de verificare a imaginilor criptate și decriptate
void verifyImages(const char* originalFilePath, const char* decryptedFilePath) {
    FILE* fOriginal = fopen(originalFilePath, "rb");
    FILE* fDecrypted = fopen(decryptedFilePath, "rb");

    if (!fOriginal || !fDecrypted) {
        printf("Error opening files for verification.\n");
        return;
    }

    long int originalSize = getFileSize(fOriginal);
    long int decryptedSize = getFileSize(fDecrypted);

    if (originalSize != decryptedSize) {
        printf("Decrypted file size doesn't match original size.\n");
        fclose(fOriginal);
        fclose(fDecrypted);
        return;
    }

    unsigned char* originalData = (unsigned char*)malloc(originalSize);
    unsigned char* decryptedData = (unsigned char*)malloc(decryptedSize);

    fread(originalData, 1, originalSize, fOriginal);
    fread(decryptedData, 1, decryptedSize, fDecrypted);

    if (memcmp(originalData, decryptedData, originalSize) == 0) {
        printf("Encryption and decryption successful. Images match.\n");
    }
    else {
        printf("Encryption and decryption failed. Images don't match.\n");
    }

    free(originalData);
    free(decryptedData);
    fclose(fOriginal);
    fclose(fDecrypted);
}

int main(int argc, char** argv) {
    if (argc != 5) {
        printf("\n Usage Mode: ProjectPart3.cpp <-e || -d> <-mode> img.bmp reslt.bmp");
        return 1;
    }

    char opt[3];
    char mode[7];
    strcpy(opt, argv[1]);
    strcpy(mode, argv[2]);

    const unsigned char userSymmetricKey[16] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                                0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
    unsigned char iv[16] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                             0x01, 0x02, 0x03, 0x4, 0xff, 0xff, 0xff, 0xff };

    if (strcmp(opt, "-e") == 0) {
        if (strcmp(mode, "-ecb") == 0) {
            encryptECB(argv[3], argv[4], userSymmetricKey);
        }
        else if (strcmp(mode, "-cbc") == 0) {
            encryptCBC(argv[3], argv[4], userSymmetricKey, iv);
        }
        else {
            printf("\n Invalid mode specified.\n");
            return 1;
        }
    }
    else if (strcmp(opt, "-d") == 0) {
        if (strcmp(mode, "-ecb") == 0) {
            decryptECB(argv[3], argv[4], userSymmetricKey);
        }
        else if (strcmp(mode, "-cbc") == 0) {
            decryptCBC(argv[3], argv[4], userSymmetricKey, iv);
        }
        else {
            printf("\n Invalid mode specified.\n");
            return 1;
        }
    }
    else {
        printf("\n Invalid option specified.\n");
        return 1;
    }

    // Verificare imagini
    if (strcmp(opt, "-e") == 0) {
        char decryptedFile[256];
        sprintf(decryptedFile, "%s.decrypted", argv[4]);
        verifyImages(argv[3], decryptedFile);
    }

    printf("\n Process done.\n");
    return 0;
}

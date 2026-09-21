#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void xorEncrypt(char *message, int messageLen, char *key) {
    int keyLen = strlen(key);
    if (keyLen == 0) {
        return;
    }
    for (int i = 0; i < messageLen; i++) {
        message[i] ^= key[i % keyLen];
    }
}

void encryptFile(const char *filename, char *message, const char *key) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }
	int messageLen = strlen(message);
    xorEncrypt(message, messageLen, key);
    size_t bytesWritten = fwrite(message, 1, messageLen, file);
    if (bytesWritten != messageLen) {
        printf("Error writing to file.\n");
	}

    fclose(file);
}

void decryptFile(const char *filename, const char *key) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file for reading.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    if (fileSize < 0) {
		printf("Error getting file size. \n");
        fclose(file);
        return;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        printf("Error seeking file.\n");
        fclose(file);
        return;
    }

    char *buffer = (char *)malloc(fileSize + 1);
    if (!buffer) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return;
    }

    size_t bytesRead = fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';

    xorEncrypt(buffer, bytesRead, key);

    printf("%s\n", buffer);

    free(buffer);
    fclose(file);
}

int main() {
    char choice[20];
    printf("Enter 'encrypt' or 'decrypt': ");

    if(scanf("%19s", choice) != 1) {
        printf("Invalid input. \n");
        return 1;
	}

    int c;
	while ((c = getchar()) != '\n' && c != EOF) {
    }

    if (strcmp(choice, "encrypt") == 0) {
        char message[1000], filename[50], key[50];

        printf("Enter the message: ");
        fgets(message, sizeof(message), stdin);
        if (strchr(message, '\n') != NULL) {
            message[strcspn(message, "\n")] = '\0';
        }
        else {
			printf("Input too long. Message truncated.\n");
            return 1;
        }
        

        printf("Enter the filename: ");
        if (scanf("%49s", filename) != 1) {
			printf("Invalid filename. \n");
            return 1;
        }

        printf("Enter the key: ");
        if (scanf("%49s", key) != 1) {
            printf("Invalid key. \n");
            return 1;
        }
        encryptFile(filename, message, key);
    } else if (strcmp(choice, "decrypt") == 0) {
        char filename[50], key[50];

        printf("Enter the filename: ");
        scanf("%49s", filename);

        printf("Enter the key: ");
        scanf("%49s", key);

        decryptFile(filename, key);
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}
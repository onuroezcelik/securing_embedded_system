#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_utils.h"
#include <unistd.h>

#define MAX_LINE_LENGTH 200
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_COMMAND_LENGTH 50
#define FILE_USERS "hashed_users.txt"

// Function to trim newline characters
void trim_newline(char* str) {
    char* pos;

    if ((pos = strchr(str, '\n')) != NULL)
        *pos = '\0';

    if ((pos = strchr(str, '\r')) != NULL)
        *pos = '\0';
}

void hex_to_bytes(const char* hex, unsigned char* bytes, size_t len) {
    for (size_t i = 0; i < len; i++) {
        sscanf(hex + 2*i, "%2hhx", &bytes[i]);
    }
}

void update_counter(const char* username, int counter) {
    FILE* file = fopen(FILE_USERS, "r+");
    if (!file) return;

    char line[MAX_LINE_LENGTH];
    long pos;

    char u[MAX_USERNAME_LENGTH];
    char salt[10];
    char hash[65];
    int c;

    while ((pos = ftell(file)), fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^:]:%[^:]:%[^:]:%d", u, salt, hash, &c);

        if (strcmp(u, username) == 0) {
            fseek(file, pos, SEEK_SET);
            fprintf(file, "%s:%s:%s:%d\n", u, salt, hash, counter);
            fflush(file);
            break;
        }
    }

    fclose(file);
}

// Function to check if username and password match an entry in users.txt
int check_login(const char* username, const char* password) {

    FILE* file = fopen(FILE_USERS, "r");
    if (file == NULL) {
        printf("Could not open hashed_users.txt\n");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    char file_username[MAX_USERNAME_LENGTH];
    char salt_hex[10];
    char stored_hash[65];
    int counter;

    while (fgets(line, sizeof(line), file)) {
        // Remove the newline character
        trim_newline(line);

        // Parses a colon-separated line in the format username:salt_hex:stored_hash 
        // and copies each field into its corresponding buffer if present.
        char* token = strtok(line, ":");
        if (token != NULL) {
            strcpy(file_username, token);

            token = strtok(NULL, ":");
            if (token != NULL) {
                strcpy(salt_hex, token);

                token = strtok(NULL, ":");
                if (token != NULL) {
                    strcpy(stored_hash, token);
                }
                
                token = strtok(NULL, ":");
                counter = token ? atoi(token) : 0;
            }
        }

        // Verifies the entered username and password by matching the username 
        // and comparing the computed salted hash against the stored hash.
        if (strcmp(username, file_username) == 0) {

            unsigned char salt[2];
            char computed_hash[65];

            hex_to_bytes(salt_hex, salt, 2);

            // hash input password + salt
            hash_password(password, salt, computed_hash);

            // compare hashes (NOT plaintext)
            if (strcmp(computed_hash, stored_hash) == 0) {
                fclose(file);
                update_counter(username, 0);
                return 1;
            }else{
                printf("number of failed attempt= %d\n", counter+1);
                counter++;
                update_counter(username, counter);
                if (counter >= 3){
                    printf("Account locked. Try again in 5 seconds later.\n");
                    sleep(5);
                    fclose(file);
                    return 0;
                }            
                fclose(file);
                return 0;
            }
        }else{
            printf("User not found\n");
            fclose(file);
            return 0;
        }
    }
    
    fclose(file);
    return 0;  // Login failed
}

int main() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char command[MAX_COMMAND_LENGTH];

    // Prompt user for username and password
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    trim_newline(username);  // Remove newline character

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    trim_newline(password);  // Remove newline character

    // Check login credentials
    if (check_login(username, password)) {
        printf("Login successful!\n");

        // Command prompt loop
        while (1) {
            printf("> ");
            scanf("%s", command);

            if (strcmp(command, "exit") == 0) {
                break;
            } else {
                printf("Unknown command.\nAllowed command is exit.\n");
            }
        }
    } else {
        printf("Login failed.\n");
    }

    return 0;
}
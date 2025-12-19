#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include<unistd.h>
#include <sched.h>

#define MAX_FILES 100
#define MAX_NAME 256
#define THREADS 4

char files[MAX_FILES][MAX_NAME];
int total_files = 0;
int current_file = 0;

char search_word[100];
char directory_path[200];
pthread_mutex_t lock;


void load_files(char *path) {
    DIR *dir = opendir(path);
    struct dirent *entry;

    if (dir == NULL) {
        perror("Directory error");
        exit(1);
    }

while ((entry = readdir(dir)) != NULL) {

    if (entry->d_type == DT_REG) {

        if (total_files >= MAX_FILES)
            break;

        snprintf(files[total_files], MAX_NAME,
            "%s/%s", directory_path, entry->d_name);
        total_files++;
    }
}

    closedir(dir);
}

int is_pdf(const char *filename) {
    const char *ext = strrchr(filename, '.');
    if (!ext) return 0;
    return strcmp(ext, ".pdf") == 0;
}

void pdf_to_text(const char *pdf_file, const char *txt_file) {
    char command[600];
    snprintf(command, sizeof(command),
         "pdftotext \"%s\" \"%s\" 2>/dev/null",
         pdf_file, txt_file);
    system(command);
}


int count_word_in_file(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    char word[100];
    int count = 0;

    while (fscanf(fp, "%s", word) != EOF) {
        if (strcmp(word, search_word) == 0) {
            count++;
        }
    }

    fclose(fp);
    return count;
}


void *worker(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);

        if (current_file >= total_files) {
            pthread_mutex_unlock(&lock);
            sched_yield();


            break;
        }

        int index = current_file;
        current_file++;

        pthread_mutex_unlock(&lock);
        sched_yield();

        int count;
        if (is_pdf(files[index])) {
            char temp_txt[300];

            snprintf(temp_txt, sizeof(temp_txt),
                "/tmp/thread_%lu.txt",
                (unsigned long)pthread_self());

            pdf_to_text(files[index], temp_txt);

            count = count_word_in_file(temp_txt);
            remove(temp_txt);
        } 
        else {
            count = count_word_in_file(files[index]);
        }

        printf("[Thread %lu] File: %s | Count: %d\n",
            (unsigned long)pthread_self(), files[index], count);
            fflush(stdout);
    }
    return NULL;
}

int main() {
    setbuf(stdout, NULL);

    printf("Enter directory path: ");
    scanf("%s", directory_path);

    printf("Enter search word: ");
    scanf("%s", search_word);

    load_files(directory_path);

    pthread_t tid[THREADS];
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < THREADS; i++) {
        pthread_create(&tid[i], NULL, worker, NULL);
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    printf("\n=== Program finished successfully ===\n");
    fflush(stdout);

    return 0;
}

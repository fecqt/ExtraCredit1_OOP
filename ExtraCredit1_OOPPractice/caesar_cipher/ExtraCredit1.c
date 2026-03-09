#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#define ALPHABET_SIZE 26
#define TOP_N 3
#define MAX_TEXT_LENGTH 1000
#define DISTRIBUTION_PATH "/Users/alex/CLionProjects/ExtraCredit1_OOPPractice/caesar_cipher/distribution.txt"
typedef double(*distance_function)(const double[], const double[]);

void read_distrubition(const char *filename, double distribution[ALPHABET_SIZE]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening the file.");
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; ++i)
        fscanf(file, "%lf", &distribution[i]);
    fclose(file);

}
void compute_histogram(const char *text, double histogram[ALPHABET_SIZE]) {
    int counts[ALPHABET_SIZE] = {0};
    int total = 0;
    for (int i = 0; text[i] != '\0'; ++i) {
        if (isalpha(text[i]))
            counts[tolower(text[i]) - 'a' ] ++, total ++;
    }
    for (int i = 0; i < ALPHABET_SIZE; ++i)
        histogram[i] = (total > 0) ? (double)counts[i] / total : 0.0;
}

double chi_squared_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double distance = 0;
    for (int i = 0; i < ALPHABET_SIZE; ++i)
        if (hist2[i] > 0)
            distance += pow(hist1[i] - hist2[i], 2) / hist2[i];
    return distance;
}

double euclidean_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double distance = 0;
    for (int i = 0; i < ALPHABET_SIZE; ++i)
        distance += pow(hist1[i] - hist2[i], 2);
    return sqrt(distance);

}

double cosine_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double dot_product = 0, norm1 = 0, norm2 = 0;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        dot_product += hist1[i] * hist2[i];
        norm1 += pow(hist1[i], 2);
        norm2 += pow(hist2[i], 2);
    }
    if (norm1 == 0 || norm2 == 0) return 1.0;
    return 1.0 - (dot_product / (sqrt(norm1) * sqrt(norm2)));
}

void apply_shift(const char *text, char *output_text, int shift) {
    for (int i = 0; text[i] != '\0'; ++i) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            output_text[i] = (text[i] - base - shift + 26) % 26 + base;
        }
        else
            output_text[i] = text[i];
    }
    output_text[strlen(text)] = '\0';

}

void break_cipher(const char *text, int top_shifts[TOP_N], double top_distances[TOP_N], double english_dist[ALPHABET_SIZE], distance_function dist_func) {
    double current_hist[ALPHABET_SIZE];
    char shifted_text[1000];
    for (int i = 0; i < TOP_N; ++i)
        top_distances[i] = 1e9;

    for (int shift = 0; shift < ALPHABET_SIZE; ++shift) {
        apply_shift(text, shifted_text, shift);
        compute_histogram(shifted_text, current_hist);
        double distance = dist_func(current_hist, english_dist);
        for (int i = 0; i < TOP_N; ++i)
            if (distance < top_distances[i]) {
                for (int j = TOP_N - 1; j > i; --j)
                    top_distances[j] = top_distances[j - 1], top_shifts[j] = top_shifts[j - 1];
                top_distances[i] = distance;
                top_shifts[i] = shift;
                break;
            }
    }
}

void print_menu() {
    printf("\n •─────⋅☾ ☽⋅─────• Caesar Cipher Tool •─────⋅☾ ☽⋅─────• \n");
    printf("1. Read text from keyboard.\n");
    printf("2. Read text from file.\n");
    printf("3. Encrypt text (specified shift).\n");
    printf("4 Decrypt text (specified shift).\n");
    printf("5. Show frequency distribution.\n");
    printf("6. Break cipher (Frequency Analysis).\n");
    printf("0. Exit.\n");
}

int main(void) {
    int choice;
    double english_dist[ALPHABET_SIZE];
    char text[MAX_TEXT_LENGTH];
    read_distrubition(DISTRIBUTION_PATH, english_dist);

    while (true) {
        print_menu();
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1: {
                printf("Enter text: ");
                fgets(text, MAX_TEXT_LENGTH, stdin);
                text[strcspn(text, "\n")] = 0;
                break;
            }
            case 2: {
                char file_name[101];
                printf("Enter filename: ");
                scanf("%s", file_name);
                FILE *file = fopen(file_name, "r");
                if (file) {
                    fgets(text, MAX_TEXT_LENGTH, file);
                    text[strcspn(text, "\n")] = 0;
                    fclose(file);
                    printf("File loaded successfully.");
                }
                else printf("Could not open file.");
                break;
            }
            case 3: {
                int shift;
                printf("Enter shift: ");
                scanf("%d", &shift);
                apply_shift(text, text, shift);
                printf("Result of encryption: %s", text);
                break;
            }
            case 4: {
                int shift;
                printf("Enter shift: ");
                scanf("%d", &shift);
                apply_shift(text, text, 26 - (shift % 26));
                printf("Result of decryption: %s", text);
                break;
            }
            case 5: {
                double hist[ALPHABET_SIZE];
                compute_histogram(text, hist);
                printf("\n •─────⋅☾ ☽⋅─────•Letter frequencies•─────⋅☾ ☽⋅─────• \n");
                for (int i = 0; i < ALPHABET_SIZE; ++i)
                    if (hist[i] > 0)
                        printf("%c : %.2f", i + 'a', hist[i] * 100);
                printf("\n");
                break;
            }
            case 6: {
                int distance_choice;
                double top_distances[TOP_N];
                int top_shifts[TOP_N];
                distance_function dist_func;
                printf("Select metric: 1. Chi Squared \n 2. Euclidian \n 3. Cosine");
                scanf("%d", &distance_choice);
                switch (distance_choice) {
                    case 1: {
                        dist_func = chi_squared_distance;
                        break;
                    }
                        case 2: {
                        dist_func = euclidean_distance;
                        break;
                    }
                        case 3: {
                        dist_func = cosine_distance;
                        break;
                    }
                }
                break_cipher(text, top_shifts, top_distances, english_dist, dist_func);

                printf("•─────⋅☾ ☽⋅─────• Printing Top 3 Candidates •─────⋅☾ ☽⋅─────•");
                for (int i = 0; i < TOP_N; ++i) {
                    char temporary_text[MAX_TEXT_LENGTH];
                    strcpy(temporary_text, text);
                    apply_shift(temporary_text, temporary_text, 26 - (top_shifts[i] % 26));\
                    printf("%d. Shift %d. (Dist: %.4f): %.50s... /n", i + 1, top_shifts[i], top_distances[i], temporary_text);

                }
                break;
            }
            case 0: return 0;
            default: printf("Invalid choice.");
        }

    }

    return 0;
}
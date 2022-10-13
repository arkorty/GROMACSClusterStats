#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int get_size(char *path) {
    FILE *file = NULL;
    file = fopen(path, "r");

    if (!file) {
        fprintf(stderr, "an error occured while counting the number of lines in the file\n");

        return 0;
    }

    int size = 0;
    for (int c = getc(file); c != EOF; c = getc(file)) {
        if (c == '\n') {
            ++size;
        }
    }

    fclose(file);

    return size;
}

void import_clusters(char *path, int *clusters, int size) {
    FILE *file = fopen(path, "r");

    if (!file) {
        fprintf(stderr, "an error occured while importing data from the file\n");

        return;
    }

    for (int index = 0; index < size; ++index) {
        int unused;
        fscanf(file, "%d %d", &unused, &clusters[index]);
    }

    fclose(file);
}

void import_weights(char *path, double *weights, int size) {
    FILE *file = fopen(path, "r");

    if (!file) {
        fprintf(stderr, "an error occured while importing data from the file\n");

        return;
    }

    for (int index = 0; index < size; ++index) {
        double unused;
        fscanf(file, "%lf %lf %lf", &unused, &unused, &weights[index]);
    }

    fclose(file);
}

int max(int *data, int size) {
    int max = data[0];
    for (int index = 1; index < size; ++index) {
        if (max < data[index]) {
            max = data[index];
        }
    }

    return max;
}

int max_index(double *data, int size) {
    int max_index = 0;
    for (int index = 1; index < size; ++index) {
        if (data[max_index] < data[index]) {
            max_index = index;
        }
    }

    return max_index;
}

double sum(double *data, int size) {
    if (data == NULL) {
        return 0.0;
    }

    double sum = data[0];

    for (int index = 1; index < size; ++index) {
        sum += data[index];
    }

    return sum;
}

void runner(char *clusters_path, char *weights_path) {
    int clusters_size = get_size(clusters_path);
    int *clusters = (int *)malloc(clusters_size * sizeof(int));
    import_clusters(clusters_path, clusters, clusters_size);
    int cluster_number = max(clusters, clusters_size);

    int weights_size = get_size(weights_path);
    double *weights = (double *)malloc(weights_size * sizeof(double));
    import_weights(weights_path, weights, weights_size);
    int max_weight_index = max_index(weights, weights_size);

    printf("maximum contributing frame is %d with a weight of %.3lf\n", max_weight_index, weights[max_weight_index]);

    int *nrw_cl = (int *)calloc(cluster_number + 1, sizeof(int));
    double *rw_cl = (double *)calloc(cluster_number + 1, sizeof(double));

    for (int index = 0; index < cluster_number + 1; ++index) {
        for (int jndex = 0; jndex < weights_size; ++jndex) {
            if (clusters[jndex] == index) {
                ++nrw_cl[index];
                rw_cl[index] += weights[index];
            }
        }
    }

    printf("---\n");

    for (int index = 1; index < cluster_number + 1; ++index) {
        printf("%d\t%d\t%.3lf\n", index, nrw_cl[index], rw_cl[index]);
    }

    printf("---\n");

    // makes sure that the reweighted things add up to number of frames?
    double some_sum = sum(rw_cl, cluster_number + 1);

    for (int index = 0; index < cluster_number + 1; ++index) {
        rw_cl[index] = rw_cl[index] / some_sum * weights_size;
    }

    // some normalization
    for (int index = 1; index < cluster_number + 1; ++index) {
        if (rw_cl[index] >= 0) {
            printf("%d\t%d\t%.3lf\n", index, nrw_cl[index], rw_cl[index]);
        }
    }
}

int main(int argc, char **argv) {
    if (argc == 3) {
        runner(argv[1], argv[2]);
    }

    return 0;
}

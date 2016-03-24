#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>


int main (int argc, char const *argv[]) {
    char util_name[FILENAME_MAX];
    FILE *tmp_file;
    pid_t pid;

    strcpy(util_name, (char*) basename(argv[0]));

    if (argc != 3) {
        fprintf(stderr, "%s: Wrong number of arguments\n", util_name);
        return -1;
    }

    if (argv[1] < 0 || argv[2] < 0) {
        fprintf(stderr,"%s: Options must be positive\n", util_name);
        return -1;
    }

    if ((tmp_file = fopen("/tmp/tmp.txt", "w+r")) == NULL) {
        fprintf(stderr, "%s: /tmp/tmp.txt: %s\n", util_name, strerror(errno));
        return -1;
    }

    int number_of_members = atoi(argv[1]);
    int set_length = atoi(argv[2]);
    int i, j, k;
    double x, member, sum;

    for (i = 0; i < number_of_members; i++) {
        x = 6.28 * i / number_of_members;
        for (j = 0; j < set_length; j++) {
            pid = fork ();
            if (pid == 0) {
                member = (j % 2) ? -1 : 1;
                for (k = 1; k <= 2 * j + 1; k++)
                    member *= x/k;
                printf("%d %d %f\n", getpid(), i, member);
                fprintf(tmp_file, "%d %d %f\n", getpid(), i, member);
                exit(0);
            } else {
                waitpid(pid);
            }
        }
    }

    printf("===================\n");
    rewind(tmp_file);
    i = 0;
    j = 0;
    sum = 0;
    while (!feof(tmp_file) {
        fscanf(tmp_file, "%d %d %lf", &pid, &k, &member);
        printf("%d %d %f\n", pid, k, member);
        sum += member;
        if (++i == number_of_members) {
            printf("y[%d] = %f\n", j++, sum);
            i = 1;
            sum = 0;
        }
    }
    fclose(tmp_file);
}

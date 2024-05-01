#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <pthread.h>

void *generate_pass(void *args);
int find_index(char ch, char *arr);
void help(char *arg);


typedef struct {

    char arr[100];
    int len;
    char ch;

} password_struct;



int main(int argc, char **argv)
{

    int cores = get_nprocs();


    char *Cflag = NULL; 
    int Lflag;

    int option;

    if (argc < 2) {
    
        help(argv[0]);
        exit(1);
    }

    while ((option = getopt(argc, argv, "l:c:")) != -1) {
    
        switch (option) {
        
            case 'l':
                Lflag = atoi(optarg);
                break;

            case 'c':
                Cflag = optarg;
                break;

            case '?':
                help(argv[0]);
                exit(1);
        }
    }

    char chars[4];
    strcpy(chars, Cflag);

    char* characters = malloc(100 * sizeof(char));

    for(int i = 0; i < strlen(chars); i++)
    {
        if (chars[i] == 'a')
        { 
            strcat(characters, "abcdefghijklmnopqrstuvwxyz");
        }
        else if (chars[i] == 'A')
        {

            strcat(characters, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        }
        else if (chars[i] == '0')
        {

            strcat(characters, "0123456789");
        }
        else if (chars[i] == 'g'){
            strcat(characters, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        }
        else {

            printf("\nError. Wrong choice.\n");
            exit(1);
        }
    }

    int step = 0;


    //Threads 
    pthread_t threads[cores];
    int rc;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    while (step < strlen(characters)) {


        for (int i = 0; i < cores; i++) {

            if (step == strlen(characters)) {

                pthread_attr_destroy(&attr);

                for (int j = 0; j < i; j++) {

                    rc = pthread_join(threads[j], NULL);

                    if (rc) {

                        printf("error. rc is %d\n", rc);
                        exit(-1);
                    }
                }



                free(characters);

                pthread_exit(NULL);
            }

            password_struct *args = malloc(sizeof(*args));
            args->ch = characters[step];
            args->len = Lflag;
            strcpy(args->arr, characters);

            rc = pthread_create(&threads[i], &attr, generate_pass, args);

            if (rc) {
            
                printf("error. rc is %d\n", rc);
                exit(-1);
            }

            step++;

        }

        for (int i = 0; i < cores; i++) {

            rc = pthread_join(threads[i], NULL);

            if (rc) {

                printf("error. rc is %d\n", rc);
                exit(-1);
            }
        }

    }

}



void *generate_pass(void *args)
{
    password_struct *actual_args = args;


    char* characters = malloc(100 * sizeof(char));
    strcpy(characters, actual_args->arr);

    int pass_length, characters_len;

    pass_length = actual_args->len;
    
    //Setting the password
    char password[pass_length];

    password[0] = actual_args->ch;

    for (int i = 1; i < pass_length; i++) {
        
        password[i] = characters[0];
    }

    password[pass_length] = '\0';

    characters_len = strlen(characters);


    for (int pass = 0; pass < pow(characters_len, pass_length - 2); pass++) {


        for (int poz = 0; poz < characters_len; poz++) {

            password[pass_length - 1] = characters[poz];
            printf("%s\n", password);
        }

        for (int i  = pass_length - 2; i >= 1; i--) {

            if (password[i] != characters[characters_len - 1]) {

                int index = find_index(password[i], characters);

                password[i] = characters[index + 1];
            } 
            else if (password[i] == characters[characters_len - 1]) {

                if (i == 1) {
                
                    free(characters);

                    free(actual_args);

                    pthread_exit(NULL);
                }
                password[i] = characters[0];
                continue;
            }
            break;
        }

    }

    pthread_exit(NULL);
}


int find_index(char ch, char *arr)
{
    int len = strlen(arr);
    
    for (int i = 0; i < len; i++) {
    
        if (arr[i] == ch) {
        
            return i;
        }
    }
    return -1;
}

void help(char *arg)
{
    printf("%s -l <> - c <>\n"
            "\n-l <args>\tThe length of the password to be generated"
            "\n-c <args>\tThe characters of the password."
            "\n         \ta: for chars in a-z"
            "\n         \tA: for chars in A-Z"
            "\n         \t0: for chars in 0-9\n\n"
	    "\n 	\tg: aA0 combined", arg);
}

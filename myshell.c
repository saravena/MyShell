/*************************************************************
* Project:   A Shell                                         *
* Name:      Sabrina Aravena                                 *
* Due :      Novemeber 27, 2016                              *
**************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int true = 1;   /* boolean variable to use for infinite loop */
char in[600];   /* arbitrary number used to fit all arguements in array */
char *points[600]; /* and array of pointers used to store tokens */
char delimiters[100] = "()|&; \t\n";    /* arbitrary number used to catch delimeter options */

/*******************************************************************
*                           WARNING:                               *
*  I had to remove both < and > as delimiters because they were    *
*  preventing me from opening/closing/appending files              *
********************************************************************/

int main() {
  printf("\nWelcome to the shell!\n");
  while (true) {
    printf("\nPlease enter a command and its following arguments (if required).\n");
    fgets(in, sizeof(in), stdin); /* Get the input from user */
    /* Populate pointer array with tokens */
    points[0] = strtok(in, delimiters);

    /* if there is no input */
    if (points[0] == NULL) {
      printf("No command was entered. Please enter a command and its following arguments (if required).\n");
      fgets(in, sizeof(in), stdin);   /* RETRY */
      /* Populate pointer array with tokens */
      points[0] = strtok(in, delimiters);
    }

    /* CONTINUE... */
    /* tokenize the whole input */
    int i;    /* index */
    for (i = 1; i < sizeof(in); i++) {
      points[i] = strtok(NULL, delimiters);
      /* break out of loop when token reaches NULL */
      if (points[i] == NULL) {
        break;
      }
    } /* end for */

    /* Checking specific commands */
    /* Change directory... (internal command)*/
    if (strcmp(points[0], "cd") == 0) {
      if (points[1] == NULL) {    /* check if directory was entered */
        printf("No directory was entered. TRY AGAIN.\n");
      } else if (chdir(points[1]) == -1) {    /* if directory does not exist */
        printf("Error: This directory does not exist! TRY AGAIN.\n");
      } else {
        /* SUCCESS */
        int ret = chdir(points[1]);
      }
    /* Exit program... (internal command)*/
    } else if (strcmp(points[0], "exit") == 0) {
      printf("\nEXITING PROGRAM...\n\n");
      exit(0);
    /* Any UNIX command with or without arguments */
    } else {
      int fork_ret = fork();  /* creating a child */
      /* If successful... (ret 0) */
      if (fork_ret == 0) {
        int j;
        for (j = 0; j < sizeof(in); j++) {
          if (points[j] == NULL)
            break;

          /* Output is redirected to a file */
          if (strcmp(points[j], ">") == 0) {
            points[j] = strtok(points[j], ">");
            /* check if argument exists for file */
            if (points[j + 1] == NULL) {
              printf("Error: No argument exists for the file. TRY AGAIN\n");
            }
            /* if it exists.. */
            freopen(points[j + 1], "w", stdout);
            /* if return value is negative */
            if (execvp(points[0], points) < 0) {
              printf("ERROR...TRY AGAIN\n");
              exit(0);
            }
          }

          /* Output is appended to a file */
          if (strcmp(points[j], ">>") == 0) {
            points[j] = strtok(points[j], ">>");
            /* check if argument exists for file */
            if (points[j + 1] == NULL) {
              printf("Error: No argument exists for the file. TRY AGAIN\n");
            }
            /* if it exists.. */
            freopen(points[j + 1], "a", stdout);
            /* if return value is negative */
            if (execvp(points[0], points) < 0) {
              printf("ERROR...TRY AGAIN\n");
              exit(0);
            }
          }

          /* Input is redirected from a file */
          if (strcmp(points[j], "<") == 0) {
            points[j] = strtok(points[j], "<");
            /* check if argument exists for file */
            if (points[j + 1] == NULL) {
              printf("Error: No argument exists for the file. TRY AGAIN\n");
            }
            /* if it exists */
            freopen(points[j + 1], "r", stdin);
            /* if return value is negative */
            if (execvp(points[0], points) < 0) {
              printf("ERROR...TRY AGAIN\n");
              exit(0);
            }
          }
        } /* end for */
        /* if command fails */
        if (execvp(points[0], points) < 0) {
          printf("\nError: TRY AGAIN\n");
        }
      /* If fails... (ret < 0) */
      } else if (fork_ret < 0) {
        printf("Error: TRY AGAIN\n");
      /* wait for children to terminate */
      } else {
        int status;
        wait(&status);
      }
    }
  } /* end while */
  return 0;
} /* end main */

/* Mangea Liviu Darius 334CA */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "functions.h"

#define LINE_SIZE 20000
#define DELIM "\n "


/*
 * Parses the commands and calls the necessary functons.
 * If the command is empthy it returns 0.
 * If the command is ok it return 1
 * If the command does not exist it return -1
 */
int parser(struct table *hash_table, char *operatie)
{
	char *tok;
	char *tok2;
	int found;
	FILE *fw;

	tok = strtok(operatie, DELIM);

	if (strcmp(operatie, "") == 0 || strcmp(operatie, "\n") == 0)
		return 0;

	if (strcmp(tok, "add") == 0) {
		tok = strtok(NULL, DELIM);

		if (tok == NULL)
			printf("Error, incomplete command add!\n");
		else
			add(hash_table, tok);

		return 1;
	}

	if (strcmp(tok, "find") == 0) {
		tok = strtok(NULL, DELIM);
		found = find(hash_table, tok);
		tok = strtok(NULL, DELIM);

		if (tok != NULL) {
			fw = fopen(tok, "a");

			if (found == 1)
				fprintf(fw, "True\n");
			else
				fprintf(fw, "False\n");

			fclose(fw);
			return 1;
		}

		if (found == 1)
			printf("True\n");
		else
			printf("False\n");

		return 1;
	}

	if (strcmp(tok, "remove") == 0) {
		tok = strtok(NULL, DELIM);

		if (tok == NULL)
			printf("Error, incomplete command remove!\n");
		else
			remove_word(hash_table, tok);

		return 1;
	}

	if (strcmp(tok, "clear") == 0) {
		clear(hash_table);
		return 1;
	}

	if (strcmp(tok, "print_bucket") == 0) {
		tok = strtok(NULL, DELIM);
		tok2 = strtok(NULL, DELIM);
		if (tok2 == NULL)
			print_bucket(hash_table, atoi(tok), "\0", 1);
		else
			print_bucket(hash_table, atoi(tok), tok2, 0);

		return 1;
	}

	if (strcmp(tok, "print") == 0) {
		tok = strtok(NULL, DELIM);

		if (tok == NULL) {
			print(hash_table, "\0", 1);
			printf("\n");
		} else {
			print(hash_table, tok, 0);
			fw = fopen(tok, "a");
			fprintf(fw, "\n");
			fclose(fw);
		}
		return 1;
	}


	if (strcmp(tok, "resize") == 0) {
		tok = strtok(NULL, DELIM);

		if (tok == NULL) {
			printf("Error, incomplete command resize!\n");
			return 1;
		}

		if (strcmp(tok, "double") == 0) {
			resize_double(hash_table);
			return 1;
		}

		if (strcmp(tok, "halve") == 0) {
			resize_halve(hash_table);
			return 1;
		}
	}

	return -1;
}


int main(int argc, char **argv)
{
	struct table *hash_table;
	int i;
	int sz;
	int parse;
	char linie[LINE_SIZE];
	FILE *fr;

	/* If the size of the hash table is not specified, error */
	if (argc < 2) {
		fprintf(stderr, "Hash size not given as the second argument\n");
		return -1;
	}

	sz = atoi(argv[1]);

	/* If the size is smaller than 1 then it's invalid */
	if (atoi(argv[1]) < 1) {
		fprintf(stderr, "Bad size for hash table");
		return -1;
	}

	hash_table = (struct table *)malloc(sizeof(struct table));
	hash_table->bucket = (struct node **)malloc(sz * sizeof(struct node *));
	hash_table->hash_size = atoi(argv[1]);

	for (i = 0; i < hash_table->hash_size; i++)
		hash_table->bucket[i] = NULL;

	/* If there are only 2 arguments the program reads from stdin */
	if (argc == 2) {
		while (fgets(linie, LINE_SIZE, stdin)) {
			parse = parser(hash_table, linie);
			if (parse == -1) {
				fprintf(stderr, "Wrong command\n");
				return -1;
			}
		}
	}

	/*
	 * If there are more than 2 arguments the program reads from the files
	 * given as arguments
	 */
	if (argc > 2) {
		for (i = 2; i < argc; i++) {
			fr = fopen(argv[i], "r");
			while (fgets(linie, LINE_SIZE, fr)) {
				parse = parser(hash_table, linie);
				if (parse == -1) {
					fprintf(stderr, "Wrong command\n");
					return -1;
				}
			}
			fclose(fr);
		}
	}

	clear(hash_table);

	for (i = 0; i < hash_table->hash_size; i++)
		free(hash_table->bucket[i]);

	free(hash_table->bucket);
	free(hash_table);

	return 0;
}

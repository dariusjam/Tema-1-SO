/* Mangea Liviu Darius 334CA */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "hash.h"
#include "functions.h"

/*
 * Checks if the bucket with the index returned by the hash function
 * contains the word given as parameter. If true, returns 1, otherwise
 * returns 0
 */
int find(struct table *hash_table, char *cuvant)
{
	unsigned int hash_val;
	struct node *aux;

	hash_val = hash(cuvant, hash_table->hash_size);
	aux = (struct node *)malloc(sizeof(struct node));
	aux = hash_table->bucket[hash_val];

	if (hash_table->bucket[hash_val] == NULL)
		return 0;

	while (aux != NULL) {
		if (strcmp(aux->cuvant, cuvant) == 0)
			return 1;
		aux = aux->next;
	}

	return 0;
}

/*
 * Adds the word in the bucket with the index returned
 * by the hash function
 */
void add(struct table *hash_table, char *cuvant)
{
	unsigned int hash_val;
	struct node *aux;
	struct node *new_node;

	/* If word is in the table it just return from the function. */
	if (find(hash_table, cuvant) == 1)
		return;

	hash_val = hash(cuvant, hash_table->hash_size);

	new_node = (struct node *)malloc(sizeof(struct node));
	new_node->cuvant = (char *)malloc((strlen(cuvant) + 1) * sizeof(char));
	strcpy(new_node->cuvant, cuvant);
	new_node->next = NULL;

	/* If the first node is null, add the word there and return */
	if (hash_table->bucket[hash_val] == NULL) {
		hash_table->bucket[hash_val] = new_node;
		return;
	}

	aux = hash_table->bucket[hash_val];

	/* Position on the last node. */
	while (aux->next != NULL)
		aux = aux->next;

	/* Add the new node at the end of the list. */
	aux->next = new_node;
}


void remove_word(struct table *hash_table, char *cuvant)
{
	unsigned int hash_val;
	struct node *aux;
	struct node *aux2;

	hash_val = hash(cuvant, hash_table->hash_size);

	aux = (struct node *)malloc(sizeof(struct node));
	aux = hash_table->bucket[hash_val];

	/*
	 * If the node to remove is the first one I just remove it, else go to
	 * the previos node of the one to be deleted, change its "next" pointer
	 * and remove the node to be deleted.
	 */
	if (hash_table->bucket[hash_val] == NULL)
		return;

	if (strcmp(aux->cuvant, cuvant) == 0) {
		hash_table->bucket[hash_val] = aux->next;
		return;
	}

	while (aux->next != NULL) {
		if (strcmp(aux->next->cuvant, cuvant) != 0) {
			aux = aux->next;
		} else {
			aux2 = (struct node *)malloc(sizeof(struct node));
			aux2 = aux->next;
			if (aux2->next == NULL) {
				aux->next = NULL;
				free(aux2);
			} else {
				aux->next = aux2->next;
				free(aux2);
			}
			return;
		}
	}
}

/* Dealocates the memory in the nodes and sets the first node to null */
void clear(struct table *hash_table)
{
	struct node *aux;
	struct node *aux2;
	int i;

	aux2 = (struct node *)malloc(sizeof(struct node));

	for (i = 0; i < hash_table->hash_size; i++) {
		aux = hash_table->bucket[i];

		if (aux == NULL)
			continue;

		while (aux->next != NULL) {
			aux2 = aux->next;
			free(aux->cuvant);
			free(aux);
			aux = aux2;
		}

		free(aux->cuvant);
		free(aux);
		hash_table->bucket[i] = NULL;
	}
}

/*
 * Goes through the nodes of a bucket and prints the words to stdout if
 * to_stdout argument is greater than 0, or to the write_file if to_stdout
 * is 0
 */
void print_bucket(struct table *hash_table, int index, char *w_f, int to_std)
{
	struct node *aux;
	FILE *fw;

	aux = hash_table->bucket[index];

	if (aux == NULL)
		return;

	if (to_std == 0)
		fw = fopen(w_f, "a");

	while (aux != NULL) {
		if (to_std == 0)
			fprintf(fw, "%s ", aux->cuvant);
		else
			printf("%s ", aux->cuvant);

		aux = aux->next;
	}

	if (to_std == 0) {
		fprintf(fw, "\n");
		fclose(fw);
	} else {
		printf("\n");
	}
}

/* Calls print_bucket function for all the bucket in the hash table */
void print(struct table *hash_table, char *write_file, int to_stdout)
{
	int i;

	for (i = 0; i < hash_table->hash_size; i++)
		print_bucket(hash_table, i, write_file, to_stdout);
}

/*
 * Creates a new hash table with the new size.
 * Iterates through the buckets of the old one and adds all the words
 * to their new location in the new hash table
 * Swaps the pointers between the new and the old hash table.
 */
void resize(struct table *hash_table, int new_size)
{
	struct table *new_hash;
	struct node *aux;
	int i;
	struct table *temp;

	new_hash = (struct table *)malloc(new_size * sizeof(struct table));
	new_hash->bucket =
		(struct node **)malloc(new_size * sizeof(struct node *));
	new_hash->hash_size = new_size;

	for (i = 0; i < new_size; i++)
		new_hash->bucket[i] = NULL;

	for (i = 0; i < hash_table->hash_size; i++) {
		aux = hash_table->bucket[i];
		while (aux != NULL) {
			add(new_hash, aux->cuvant);
			aux = aux->next;
		}
	}

	temp = (struct table *)malloc(new_size * sizeof(struct table));
	*temp = *hash_table;
	*hash_table = *new_hash;
	*new_hash = *temp;

	clear(new_hash);
	for (i = 0; i < new_hash->hash_size; i++)
		free(new_hash->bucket[i]);
	free(new_hash->bucket);
	free(new_hash);
	free(temp);
}

/* Calls the resize functions with the new size being 2 times the old size */
void resize_double(struct table *hash_table)
{
	resize(hash_table, 2 * hash_table->hash_size);
}

/* Calls the resize function with the new size being half of the old one*/
void resize_halve(struct table *hash_table)
{
	resize(hash_table, (int)(hash_table->hash_size / 2));
}

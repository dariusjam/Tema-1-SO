/* Mangea Liviu Darius 334CA */

struct node {
	char *cuvant;
	struct node *next;
};

struct table {
	struct node **bucket;
	int hash_size;
};

int find(struct table *hash_table, char *cuvant);

void add(struct table *hash_table, char *cuvant);

void remove_word(struct table *hash_table, char *cuvant);

void clear(struct table *hash_table);

void print_bucket(struct table *hash_table, int index, char *w_f, int to_std);

void print(struct table *hash_table, char *write_file, int to_stdout);

void resize(struct table *hash_table, int new_size);

void resize_double(struct table *hash_table);

void resize_halve(struct table *hash_table);

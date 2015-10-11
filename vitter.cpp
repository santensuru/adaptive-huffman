/**
 * Adaptive Huffman
 *
 * Author: Djuned Fernando Djusdek
 *         5112.100.071
 *         Informatics - ITS
 */

#include <iostream>
#include <algorithm>
#include <vector>

#define SYMBOL 256
#define NUMBER 512

typedef struct node{
	unsigned char symbol; // symbol, default string kosong
	int weight,           // bobot
	    number;           // nomor urut
	node *parent,         // orang tua
	     *left,           // anak kiri
	     *right;          // anak kanan
} node;

void create_node(node **leaf, unsigned char symbol, bool is_nyt) {
	node *temp =  (node*) malloc(sizeof(node));
	if (is_nyt) {
		temp->symbol = 0x00;
		temp->weight = 0;
	} else {
		temp->symbol = symbol;
		temp->weight = 1;	
	}
	temp->parent = NULL;
	temp->left = NULL;
	temp->right = NULL;
		
	*leaf = temp;
	
	return;
}

void insert_node(node **root, node *leaf) {
	if (*root == NULL) {
		*root = leaf;
	} else {
		node *temp = *root;
		node *temp2 = *root;
		while(1) {
			if(temp->left != NULL) {
				temp2 = temp;
				temp = temp->left;
			}
			else
				break;
		}
		temp2->left = leaf;
		temp2->left->parent = temp2;
	}
	return;
}

void merge_node(node **tree, node *left, node *right) {
	node *temp = (node*) malloc(sizeof(node));
	temp->weight = left->weight + right->weight;
	temp->left = left;
	temp->right = right;
	temp->left->parent = temp;
	temp->right->parent = temp;
	temp->symbol = 0x00;
	*tree = temp;
	
	return;
}

void swap_node(node *tree) {
	node *temp = tree->left;
	tree->left = tree->right;
	tree->right = temp;
	
	int temp_n = tree->left->number;
	tree->left->number = tree->right->number;
	tree->right->number = temp_n;
	
	return;
}

void numbering_node(node **tree, int number, int deep) {
	if (*tree == NULL)
		return;
	else
		(*tree)->number = number;
	
	if ((*tree)->right != NULL) {
		numbering_node(&(*tree)->right, number - 1, deep+1);
	} else {
		return;
	}
	
	if ((*tree)->left != NULL) {
		numbering_node(&(*tree)->left, number - 2, deep+1);
	} else {
		return;
	}
}

void increment_weight(node **tree) {
	if ((*tree)->left != NULL && (*tree)->right != NULL)
		(*tree)->weight = (*tree)->left->weight + (*tree)->right->weight;
	else
		(*tree)->weight++;
	
	return;
}

void find_external_symbol(node **tree, unsigned char symbol, node **position) {
//	std::cout << (*tree)->symbol << " " << symbol << '\n';
	
	if ((*tree)->left != NULL)
		find_external_symbol(&(*tree)->left, symbol, &*position);
		
	if ((*tree)->symbol == symbol) {
//		std::cout << (*tree)->number;
		*position = *tree;
	}
	
	if ((*tree)->right != NULL)
		find_external_symbol(&(*tree)->right, symbol, &*position);
		
	return;
}

void print_tree(node **tree, int deep) {
	if((*tree)->left != NULL)
		print_tree(&(*tree)->left, deep+1);
	
	if((*tree)->right != NULL)
		print_tree(&(*tree)->right, deep+1);
	
	printf("%d 0x%2x %d %d\n", deep, (*tree)->symbol, (*tree)->number, (*tree)->weight);
	
	return;
}

void update(node **tree, unsigned char symbol, std::vector<unsigned char> *dictionary) {
	// search in dictionary
	std::vector<unsigned char>::iterator it;
	it = std::search_n ((*dictionary).begin(), (*dictionary).end(), 1, symbol);
	
	node *temp;
	
	// NYT
	if (it != (*dictionary).end()) {
		find_external_symbol(&*tree, symbol, &temp);
		
//		std::cout << temp->number;
		increment_weight(&temp);
				
	} else {
		node *new_nyt;
		create_node(&new_nyt, 0x00, true);
		node *new_node;
		create_node(&new_node, symbol, false);

		node *old_nyt = NULL;
		merge_node(&old_nyt, new_nyt, new_node);
		
		insert_node(&*tree, old_nyt);
		
		numbering_node(&*tree, NUMBER, 0);
//		print_tree(&*tree, 0);
		
		// goto old nyt
		temp = old_nyt;
		increment_weight(&temp);
		
		(*dictionary).push_back(symbol);

	}
	
	while(temp->parent != NULL) {
		temp = temp->parent;
		if (temp->parent != NULL)
		{
			if (temp->parent->left->number < temp->parent->right->number) {
//				std::cout << temp->parent->left->number << " " << temp->parent->right->number << '\n';
//				std::cout << temp->parent->left->weight << " " << temp->parent->right->weight << '\n';
				swap_node(temp->parent);
			}
			increment_weight(&temp);
		}
	}
	
	increment_weight(&temp);
	*tree = temp;
	
	// to print the tree, after update
//	print_tree(&*tree, 0);
//	std::cout << '\n';
	
//	std::cout << (*tree)->right->symbol << '\n';

	return;
}

int main() {
	node *root;
	root = NULL;
	node *nyt;
	create_node(&nyt, 0x00, true);

	std::vector<unsigned char> dictionary;
	
	update(&root, (unsigned char)0x22, &dictionary);
	
	update(&root, (unsigned char)0x25, &dictionary);

	update(&root, (unsigned char)0x22, &dictionary);
	
	update(&root, (unsigned char)0x22, &dictionary);
	
	update(&root, (unsigned char)0x27, &dictionary);
	
	print_tree(&root, 0);
	
	return 0;
}

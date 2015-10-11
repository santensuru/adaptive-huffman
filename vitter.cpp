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
#include <cstring>

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

typedef std::pair<int, node*> my_pair;

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
		node *mass = *root;
		node *temp = *root;
		node *temp2 = *root;
		do {
			while(temp->left != NULL) {
				temp2 = temp;
				temp = temp->left;
			}
			if (temp2->left->weight == 0) {
				temp2->left = leaf;
				temp2->left->parent = temp2;
//				printf(">> %c %c\n", temp2->left->right->symbol, leaf->right->symbol);
				break;
			} else {
				temp = mass->right;
				temp2 = mass->right;
				mass = mass->right;
			}
		} while(1);
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

/**
 * TODO
 */
void search_higher_block(node **tree, int weight, int *number, int parent_number, node **position) {
//	if ((*tree)->weight == weight)
//		std::cout << (*tree)->number << " " << *number << '\n';
	if ((*tree)->weight == weight && (*tree)->number > *number && (*tree)->number != parent_number) {
		*position = (*tree)->parent;
		*number = (*tree)->number;
//		std::cout << (*tree)->number;
//		std::cout << (*position)->left->number << " " << (*position)->right->number;
	}
	
	if ((*tree)->left != NULL)
		search_higher_block(&(*tree)->left, weight, &*number, parent_number, &*position);
	
	if ((*tree)->right != NULL)
		search_higher_block(&(*tree)->right, weight, &*number, parent_number, &*position);
		
	return;
}

void switch_node(node *tree, char *l_r, node *sibling_parent, unsigned char symbol) {
//	std::cout << tree->left->symbol;
	
	if (strcmp(l_r, "left") == 0) {
		// one parent
		if (sibling_parent->left->number == tree->left->number) {
			node *temp = tree->left;
			tree->left = tree->right;
			tree->right = temp;
		
			int temp_n = tree->left->number;
			tree->left->number = tree->right->number;
			tree->right->number = temp_n;
		
		// left left
		} else if (tree->left->symbol == symbol) {
			node *temp = tree->left;
			tree->left = sibling_parent->left;
			tree->left->parent = sibling_parent;
			sibling_parent->left = temp;
			sibling_parent->left->parent = tree;
			
			int temp_n = tree->left->number;
			tree->left->number = sibling_parent->left->number;
			sibling_parent->left->number = temp_n;
		
		// left right
		} else {
			node *temp = tree->right;
			tree->right = sibling_parent->left;
			tree->right->parent = sibling_parent;
			sibling_parent->left = temp;
			sibling_parent->left->parent = tree;
			
			int temp_n = tree->right->number;
			tree->right->number = sibling_parent->left->number;
			sibling_parent->left->number = temp_n;
		}
	} else {
		// one parent
		if (sibling_parent->right->number == tree->right->number) {
			node *temp = tree->left;
			tree->left = tree->right;
			tree->right = temp;
		
			int temp_n = tree->left->number;
			tree->left->number = tree->right->number;
			tree->right->number = temp_n;
		
		// right left
		} else if (tree->left->symbol == symbol) {
			node *temp = tree->left;
			tree->left = sibling_parent->right;
			tree->left->parent = sibling_parent;
			sibling_parent->right = temp;
			sibling_parent->right->parent = tree;
			
			int temp_n = tree->left->number;
			tree->left->number = sibling_parent->right->number;
			sibling_parent->right->number = temp_n;
		
		// right right
		} else {
			node *temp = tree->right;
			tree->right = sibling_parent->right;
			tree->right->parent = sibling_parent;
			sibling_parent->right = temp;
			sibling_parent->right->parent = tree;
			
			int temp_n = tree->right->number;
			tree->right->number = sibling_parent->right->number;
			sibling_parent->right->number = temp_n;
		}
	}

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
//	std::cout << number << "" << (*tree)->symbol << " " << deep << '\n';
	
	if (*tree == NULL)
		return;
	else
		(*tree)->number = number;
	
	if ((*tree)->right != NULL) {
		numbering_node(&(*tree)->right, number-1, deep+1);
	} else {
		return;
	}
	
	if ((*tree)->left != NULL) {
		numbering_node(&(*tree)->left, number-2, deep+1);
	} else {
		return;
	}
}

void queueing_node(node **tree, std::vector<my_pair> *queue, int deep) {
	(*queue).push_back(std::make_pair(deep, *tree));
	
	if ((*tree)->right != NULL) {
		queueing_node(&(*tree)->right, &*queue, deep+1);
	}
	
	if ((*tree)->left != NULL) {
		queueing_node(&(*tree)->left, &*queue, deep+1);
	}
	
	return;
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
	
	printf("%d 0x%2x %c %d %d\n", deep, (*tree)->symbol, (*tree)->symbol, (*tree)->number, (*tree)->weight);
	
	return;
}

bool my_sort(my_pair p, my_pair q) {
	return p.first < q.first;
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
	
		node *inner_temp = NULL;
		
		if (temp->parent->right->symbol == symbol) {
			int number = temp->parent->right->number;
			search_higher_block(&*tree, temp->parent->right->weight, &number, temp->parent->number, &inner_temp);
			if (inner_temp != NULL) {
				std::cout<< inner_temp->right->number;
				switch_node(temp->parent, (char*)"right", inner_temp, symbol);
			}
	
//			std::cout << inner_temp << " " << symbol << " right" << '\n';
		} else {
			int number = temp->parent->left->number;
			search_higher_block(&*tree, temp->parent->left->weight, &number, temp->parent->number, &inner_temp);
			if (inner_temp != NULL) {
				std::cout<< inner_temp->left->number;
				switch_node(temp->parent, (char*)"left", inner_temp, symbol);
			}
			
//			std::cout << inner_temp << " " << symbol << " left" << '\n';
		}
		
//		if (temp->parent->left->weight == temp->parent->right->weight) {
//			swap_node(temp->parent);
//		}
		increment_weight(&temp);
		
		std::vector<my_pair> queue;
		queueing_node(&*tree, &queue, 0);
		
		std::sort(queue.begin(), queue.end(), my_sort);
		
		int num = NUMBER;
		for (int i=0; i<queue.size(); i++) {
			queue.at(i).second->number = num--;
//			std::cout << queue.at(i).first << " " << queue.at(i).second->number << '\n';
		}
	
				
	} else {
//		std::cout << symbol << '\n';
		node *new_nyt;
		create_node(&new_nyt, 0x00, true);
		node *new_node;
		create_node(&new_node, symbol, false);

		node *old_nyt = NULL;
		merge_node(&old_nyt, new_nyt, new_node);
		
		insert_node(&*tree, old_nyt);
		
		std::vector<my_pair> queue;
		queueing_node(&*tree, &queue, 0);
		
		std::sort(queue.begin(), queue.end(), my_sort);
		
		int num = NUMBER;
		for (int i=0; i<queue.size(); i++) {
			queue.at(i).second->number = num--;
//			std::cout << queue.at(i).first << " " << queue.at(i).second->number << '\n';
//			std::cout << queue.at(i).second->symbol << " " << queue.at(i).second->weight << " " << queue.at(i).second->number << '\n';
		}
		
//		numbering_node(&*tree, NUMBER, 0);
		
//		print_tree(&*tree, 0);
		
		// goto old nyt
		temp = old_nyt;
		increment_weight(&temp);
		
		(*dictionary).push_back(symbol);

	}
	
	print_tree(&*tree, 0);
	std::cout << '\n';
	
	while(temp->parent != NULL) {
		// go to parent node
		temp = temp->parent;
		
//		std::cout << (temp->symbol);
		
		// if not root
		if (temp->parent != NULL)
		{
			node *inner_temp = NULL;
			
			if (temp->parent->right->symbol == symbol) {
				int number = temp->parent->right->number;
				search_higher_block(&*tree, temp->parent->right->weight, &number, temp->parent->number, &inner_temp);
				if (inner_temp != NULL) {
					switch_node(temp->parent, (char*)"right", inner_temp, temp->symbol);
				}
		
//				std::cout << inner_temp << " " << symbol << " right" << '\n';
			} else {
				int number = temp->parent->left->number;
				search_higher_block(&*tree, temp->parent->left->weight, &number, temp->parent->number, &inner_temp);
				if (inner_temp != NULL) {
					switch_node(temp->parent, (char*)"left", inner_temp, temp->symbol);
				}
				
//				std::cout << inner_temp << " " << symbol << " left" << '\n';
			}
			
//			if (temp->parent->left->weight == temp->parent->right->weight) {
////				std::cout << temp->parent->left->number << " " << temp->parent->right->number << '\n';
////				std::cout << temp->parent->left->weight << " " << temp->parent->right->weight << '\n';
//				swap_node(temp->parent);
//			}
			increment_weight(&temp);
		}
		
		std::vector<my_pair> queue;
		queueing_node(&*tree, &queue, 0);
		
		std::sort(queue.begin(), queue.end(), my_sort);
		
		int num = NUMBER;
		for (int i=0; i<queue.size(); i++) {
			queue.at(i).second->number = num--;
		}
		print_tree(&*tree, 0);
		std::cout << '\n';
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
	
	update(&root, (unsigned char)'a', &dictionary);
	
	update(&root, (unsigned char)'a', &dictionary);

	update(&root, (unsigned char)'r', &dictionary);
	
	update(&root, (unsigned char)'d', &dictionary);
	
	update(&root, (unsigned char)'v', &dictionary);

	update(&root, (unsigned char)'j', &dictionary);

	update(&root, (unsigned char)'j', &dictionary);
//
//	update(&root, (unsigned char)'j', &dictionary);
//
//	update(&root, (unsigned char)'j', &dictionary);
					
	print_tree(&root, 0);
	
	return 0;
}

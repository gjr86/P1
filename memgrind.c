#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "mymalloc.h"

//Linked list node structure
typedef struct Node {
   int value;
   struct Node* next;
} Node;

//Binary tree node structure
typedef struct TreeNode {
   int value;
   struct TreeNode* left;
   struct TreeNode* right;
} TreeNode;

TreeNode* insert(TreeNode* node, int value) { // Standard BST insert with random size allocation
   if (!node) {
      TreeNode* new_node = sizeof(TreeNode) + (rand() % 32);
      new_node->value = value;
      new_node->left = NULL;
      new_node->right = NULL;
      printf("Tree: Insert node %d (size %zu)\n", value, sizeof(TreeNode) + (rand() % 32));
      return new_node;
   }
   if (value < node->value) {
      node->left = insert(node->left, value);
   } else if (value > node->value) {
      node->right = insert(node->right, value);
   }
   return node;
}

TreeNode* find_min(TreeNode* node) { // Find minimum value node in BST from the given node
   while (node && node->left) {
      node = node->left;
   }
   return node;
}

TreeNode* delete(TreeNode* node, int value) { // Standard BST delete
   if (!node) {
     return NULL;
   }
   if (value < node->value) {
      node->left = delete(node->left, value);
  } else if (value > node->value) {
      node->right = delete(node->right, value);
  } else {
      if (!node->left) {
         TreeNode* temp = node->right;
         printf("Tree: Free node %d\n", node->value);
         free(node);
         return temp;
      } else if (!node->right) {
         TreeNode* temp = node->left;
         printf("Tree: Free node %d\n", node->value);
         free(node);
         return temp;
      } else {
         TreeNode* min = find_min(node->right);
         node->value = min->value;
         node->right = delete(node->right, min->value);
      }
   }
   return node;
}

void freeTree(TreeNode* node) { // Post-order traversal to free nodes
   if (!node) {
      return;
   }
   freeTree(node->left);
   freeTree(node->right);
   printf("Tree: Final free %d\n", node->value);
   free(node);
}

int main () {
   srand((unsigned int)time(NULL));
   //1. Allocate 1 byte 120 times, freeing each allocation immediately after
   void* x;
   for (int i = 0; i <= 119; i++) {
      x = malloc(1);
      printf("Allocate  # %d\n", i);
      free(x);
   }

   //2. Allocate 1 byte 120 times, then free all 120 allocations
   void* y[120];
   for (int i = 0; i <= 119; i++) {
      y[i] = malloc(1);
      printf("Allocate  # %d\n", i);
   }
    for (int i = 0; i <= 119; i++) {
      free(y[i]);
      printf("Freed 1 byte  # %d\n", i);
   }

   //3. Randomly allocate or free 1 byte 120 times, then free any remaining allocations
   void* z[120];
   for (int i = 0; i <= 119; i++) {
      int r = rand() % 2;
      if (r == 0) {
         z[i] = malloc(1);
         printf("Allocate  # %d\n", i);
      } else {
         if (z[i] != NULL) {
            free(z[i]);
            printf("Freed 1 byte  # %d\n", i);
         }
      }
   }
   for (int i = 0; i <= 119; i++) {
      if (z[i] != NULL) {
         free(z[i]);
         printf("Freed 1 byte  # %d\n", i);
      }
   }

   //4. Allocate a mutable linked list with variable sizes of nodes, then free the entire list
   Node* head = NULL;
   int size = 0;
   for (int i = 0; i <= 120; i++) {
      int randAlNewNode = rand() % 3;
      if (randAlNewNode == 0 || size == 0) { //Allocate a new node
         Node* newNode = (Node*)malloc(sizeof(Node) + (rand() % 32)); //Random size between sizeof(Node) and sizeof(Node)+31
         if (newNode) {
            newNode->value = i;
            newNode->next = head;
            head = newNode;
            size++;
            printf("Allocated node %d of size %zu\n", i, sizeof(Node) + (rand() % 32));
         }
      } else if (randAlNewNode == 1 && head !=NULL) { //Remove head node
         Node* temp = head;
         head = head->next;
         free(temp);
         size--;
         printf("Freed head node\n");
      } else if (randAlNewNode == 2 && size > 1) { //Remove a random node
         int removeIndex = rand() % size;
         Node* current = head;
         Node* prev = NULL;
         for (int j = 0; j < removeIndex; j++) {
            prev = current;
            current = current->next;
         }
         if (prev) {
            prev->next = current->next;
            free(current);
            size--;
            printf("Freed node at index %d\n", removeIndex);
         }
      }
   }
   while(head) {
      Node* temp = head;
      head = head->next;
      printf("Final Freeing node with value %d\n", temp->value);
      free(temp);
   }

   //5. Allocate to a dynamic binary tree with variable sizes of nodes with insertoin and deletion, then free the entire tree
   TreeNode* root = NULL;
   int insertedValues[120];
   int insertCount = 0;

   for (int i = 0; i < 119; i++) {
     int randAlNewNode = rand() % 2;
     int value = rand() % 65535;

     if (randAlNewNode == 0 || insertCount == 0) {
         root = insert(root, value);
         insertedValues[insertCount++] = value;
     } else if (insertCount > 0) {
         int idx = rand() % insertCount;
         int val = insertedValues[idx];
         root = delete(root, val);
         insertedValues[idx] = insertedValues[--insertCount];
      }
   }

   freeTree(root);

   return 0;
}

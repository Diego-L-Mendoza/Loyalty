#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAXLEN 19
#define MAXN 100000

//Author: Diego Mendoza
//Date: 4/01/2024
//Assignment: ProgrammingAssignment_4

typedef struct customer {
    char name[MAXLEN+1];
    int points;
} customer;

typedef struct treenode {
    customer* cPtr;
    int size;
    struct treenode* left;
    struct treenode* right;
} treenode;

//max val function
struct treenode* maxVal(struct treenode *root)
{
    //base case
    if (root->right == NULL)
    {
        return root;
    }
    //recursively call maxval on the right side
    else
    {
        return maxVal(root->right);
    }
}

struct treenode* search(struct treenode *root, char* name, int* depth)
{
    //base case
    if (root == NULL)
    {
        return NULL;
    }
    
    //check if the name is the same as the root name
    if (strcmp(root->cPtr->name, name) == 0)
    {
        //return root
        return root;
    }
    else if(strcmp(name, root->cPtr->name) < 0)
    {
        //increment depth and recursively call search on the left side
        (*depth) += 1;
        return search(root->left, name, depth);
    }
    else if(strcmp(name, root->cPtr->name) > 0)
    {
        //increment depth and recursively call search on the right side
        (*depth) += 1;
        return search(root->right, name, depth);
    }
}

//creat customer function
customer* createCustomer(char name[], int points) 
{
    customer* cPtr = (customer*)malloc(sizeof(customer));
    strcpy(cPtr->name, name);
    cPtr->points = points;
    return cPtr;
}

struct treenode* add(struct treenode *root, char* name, int points, FILE* outfile)
{
    //if root is null, create a new customer and return the root
    if(root == NULL)
    {
        //allocate memory for root
        root = (struct treenode*)malloc(sizeof(struct treenode));

        //check if root is null
        if(root == NULL) 
        {
            fprintf(outfile, "Memory allocation failed\n");
            return NULL;
        }
        //initialize root values and print out the customer
        root->left = NULL;
        root->right = NULL;
        root->cPtr = createCustomer(name, points);
        root->size = 1; 
        fprintf(outfile, "%s %d\n", root->cPtr->name, root->cPtr->points);

        //return root
        return root;
    }
    //if the name comes before cptr name recursively call add on the left side
    if(strcmp(name, root->cPtr->name) < 0)
    {
        root->left = add(root->left, name, points, outfile);
    }
    //if the name comes after cptr name recursively call add on the right side
    else if(strcmp(name, root->cPtr->name) > 0)
    {
        root->right = add(root->right, name, points, outfile);
    }
    //if the name is the same as cptr name
    else
    {   
        //add points to the customer and print out their name and new points
        root->cPtr->points = root->cPtr->points + points;
        fprintf(outfile, "%s %d\n", root->cPtr->name, root->cPtr->points);

    }
    //increment the size of the tree and return root
    root->size += 1;
    return root;
}


struct customer* subtract(struct treenode *root, char name[], int points, int* depth, FILE* outfile)
{
    //search for the customer to see if they exist
    struct treenode* node = search(root, name, depth);
    
    //if customer exists, subtract points
    if(node != NULL)
    {   
        //if points they have are greater than or equal to points to subtract
        if(node->cPtr->points >= points)
        {
            //subtract points and print out their name and new points
            node->cPtr->points -= points;
            fprintf(outfile, "%s %d\n", node->cPtr->name, node->cPtr->points);
            
            //return the customer
            return node->cPtr;
        }
        //if they will run out of all points
        else
        {
            //set points to 0 and print out their name and new points 
            node->cPtr->points = 0;
            fprintf(outfile, "%s %d\n", node->cPtr->name, node->cPtr->points);

            //return the customer
            return NULL;
        }
    }
    //if customer does not exist
    else
    {
        fprintf(outfile, "%s not found.\n", name);
        return NULL;
    }
}

struct treenode* deleteNode(struct treenode* root, char* name, bool* deleted, FILE *outfile) 
{
    // base case
    if (root == NULL) 
    {
        fprintf(outfile, "%s not found\n", name);
        return root;
    }
    // If the name comes before root name recursively call delete from left subtree
    if (strcmp(name, root->cPtr->name) < 0)
        root->left = deleteNode(root->left, name, deleted, outfile);
  
    //if name comes after root name recursively call delete from right subtree
    else if (strcmp(name, root->cPtr->name) > 0)
        root->right = deleteNode(root->right, name, deleted, outfile);
  
    // if name is same as root name, then This is the node to be deleted
    else {
        // node with only one child or no child
        if (root->left == NULL) {
            struct treenode *temp = root->right;
            free(root);
            (*deleted) = true;
            return temp;
        }
        else if (root->right == NULL) {
            struct treenode *temp = root->left;
            free(root);
            (*deleted) = true;
            return temp;
        }
  
        // for a node with 2 children 
        struct treenode* temp = maxVal(root->left);
        
        // Copy the inorder successor's content to this node
        strcpy(root->cPtr->name, temp->cPtr->name);
        root->cPtr->points = temp->cPtr->points;
        root->size = temp->size;
        // Delete the inorder successor
        root->left = deleteNode(root->left, temp->cPtr->name, deleted, outfile);
        *deleted = true;
    }
    //lower tree size
    root->size -= 1;
    return root;
}

int count_smaller(struct treenode *root, char* name)
{
    //base case
    if(root == NULL)
    {
        return 0;
    }

    //compare name to root name
    int cmp = strcmp(name, root->cPtr->name);

    if(cmp <= 0)
    {
        return count_smaller(root->left, name);
    }
    else
    {
        return 1 + count_smaller(root->left, name) + count_smaller(root->right, name);
    }
}

int compareCustomer(const void* a, const void* b)
{
    // Typecast parameters to customer pointers
    struct customer* c1 = *(struct customer**)a;
    struct customer* c2 = *(struct customer**)b;

    // Compare points first
    if (c1->points != c2->points)
    {
        return c2->points - c1->points;
    }
    // If points are equal, compare names
    else
    {
        return strcmp(c1->name, c2->name);
    }
}

void merge(struct customer* pData[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    
    struct customer **L = (struct customer **)malloc(sizeof(struct customer*) * n1);
    struct customer **R = (struct customer **)malloc(sizeof(struct customer*) * n2);
    
    for (i = 0; i < n1; i++)
        L[i] = pData[l + i];
    for (j = 0; j < n2; j++)
        R[j] = pData[m + 1 + j];
    
    i = 0;
    j = 0;
    k = l;
    
    while (i < n1 && j < n2)
    {
        if (compareCustomer(&L[i], &R[j]) <= 0)
        {
            pData[k] = L[i];
            i++;
        }
        else
        {
            pData[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1)
    {
        pData[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2)
    {
        pData[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(struct customer* pData[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(pData, l, m);
        mergeSort(pData, m + 1, r);
        merge(pData, l, m, r);
    }
}

//print customers function to test add and subtract
void printNames(struct treenode *root, FILE* outfile) {
    //base case
    if (root == NULL) 
    {
        return;
    }

    // Print the name of the customer in the current node
    fprintf(outfile, "%s\n", root->cPtr->name);

    // Recursively print the names in the left and right subtrees
    printNames(root->left, outfile);
    printNames(root->right, outfile);
}

void treetoarray(struct treenode *root, struct customer* pData[], int* index) {
    //base case
    if (root == NULL)
    {
        return;
    }

    // Recursively add the customer in the current node to the array
    pData[*index] = root->cPtr;
    (*index)++;

    // Recursively add the customers in the left and right subtrees to the array
    treetoarray(root->left, pData, index);
    treetoarray(root->right, pData, index);
}

void freetree(struct treenode* root)
{
    if (root == NULL)
    {
        return;
    }
    freetree(root->left);
    freetree(root->right);
    free(root->cPtr);
    free(root);

}

int main(void)
{

    FILE *outfile = fopen("output.txt", "w");
    if (outfile == NULL)
    {
        printf("Failed to open file\n");
        return 1;
    }

    //file dictionary
    char *files[] = {"loyalty_sample_01.in", "loyalty_sample_02.in", "loyalty_sample_03.in"};

    for(int k = 0; k<3; k++)
    {
        //initialize root to null
        struct treenode *root = NULL;
        
        //open files
        FILE *file = fopen(files[k], "r");
        
        //check if files are null
        if (file == NULL)
        {
            printf("Failed to open file\n");
            return 1;
        }

        //initialize variables so we can read file
        int n, points;
        char cmd[MAXN+1], name[MAXLEN+1];
        fscanf(file, "%d", &n);

        //loop n times    
        for(int i = 0; i<n; i++)
        {   
            //scan values from file
            fscanf(file, "%s %s %d", cmd, name, &points);
            
            //if statements for cmds
            if(strcmp(cmd, "add") == 0)
            { 
                root = add(root, name, points, outfile);
                
                continue;
            }
            else if(strcmp(cmd, "sub") == 0)
            {
                int depth = 0;
                subtract(root, name, points, &depth, outfile);
            
                continue;
            }
            else if(strcmp(cmd, "del") == 0)
            {
                bool deleted = false;
                root = deleteNode(root, name, &deleted, outfile);

                if (deleted) 
                {
                    fprintf(outfile, "%s deleted\n", name);
                }

                continue;
            }
            else if(strcmp(cmd, "count_smaller") == 0)
            {
                fprintf(outfile, "%d\n", count_smaller(root, name));

                continue;
            }
            else if(strcmp(cmd, "search") == 0)
            {
                int depth = 0;
                struct treenode *temp = search(root, name, &depth);

                if(temp == NULL)
                {
                    fprintf(outfile, "%s not found\n", name);
                }
                else
                {
                    fprintf(outfile, "%s %d %d\n", temp->cPtr->name, temp->cPtr->points, depth);
                }

                continue;
            }
        }

        //printNames(root);


        //create array of pointers
        int index = 0;
        struct customer** array = malloc(MAXN * sizeof(struct customer*));;

        //check if null
        if(array == NULL)
        {
            printf("Memory allocation failed\n");
            return 1;
        }

        //call tree to array function to convert
        treetoarray(root, array, &index);
        
        //call mergesort
        mergeSort(array, 0, index-1);

        //print sorted array)
        for(int i = 0; i<index; i++)
        {
            fprintf(outfile, "%s %d\n", array[i]->name, array[i]->points);
        }

        //free memory
        freetree(root);
        free(array);
        fclose(file);
    }

    return 0;
}
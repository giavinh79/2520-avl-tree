#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Node {
    char key[200];
    struct Node *left;
    struct Node *right;
    int height;
    int count;
};
typedef struct Node Node;

int getBalance(Node * node);
int height(Node * node);
int max(int x, int y);
int verifyCommand(char * command);
Node * insert(Node * node, char key[200]);
Node * deleteNode(Node * node, char key[200]);
Node * leftRotate(Node * node);
Node * smallestNode(Node * node);
Node * createNode(char key[200]);
Node * rightRotate(Node * node);
void findAll(Node * node, int count);
void findSize(Node * node, int * size);
void searchNode(Node * node, char key[200]);

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Need to add argument for data file\n");
        exit(0);
    }

    // Variable declarations and initializations
    FILE * data = NULL;
    Node * tree = NULL;
    char command[200]; //all these index sizes will also create constraints where exceeding them may cause unexpected behaviour
    char dataText[10000];
    char parsedString[200];
    char key[200];
    int convertString = 0;
    int errorChecking = 0;
    int i = 0;
    int j = 0;
    int count = 0;
    int size = 0;

    printf("Welcome to the AVL tree program.\n");
    printf("This program was written by: Gia Vinh Lam (0925571)\n");
    while (convertString != 7)
    {
        while (errorChecking == 0)
        {
            printf("1. Initialization\n");
            printf("2. Find\n");
            printf("3. Insert\n");
            printf("4. Remove\n");
            printf("5. Check Height and Size\n");
            printf("6. Find All (above a given frequency)\n");
            printf("7. Exit\n");
            printf("avl/> ");
            scanf("%s", command);

            if (verifyCommand(command) == 1)
            {
                errorChecking = 1;
            }
            else
            {
                continue;
            }
        }

        errorChecking = 0;
        convertString = atoi(command);

        if (convertString == 1)
        {
            data = fopen(argv[1], "r");
            while (fgets(dataText, 10000, data))
            {
                i = 0;
                while (dataText[i] != '\n')
                {
                    if (isspace(dataText[i]))
                    {
                        i++;
                    }
                    else
                    {
                        while (!isspace(dataText[i]))
                        {
                            parsedString[j] = dataText[i];
                            i++;
                            j++;
                        }
                        tree = insert(tree, parsedString);
                        memset(parsedString, 0, 200);
                        j = 0;
                    }
                }
            }
            printf("Successfully parsed and stored information.\n");
        }
        else if (convertString == 2)
        {
            printf("find key: ");
            scanf("%s", key);
            searchNode(tree, key);
        }
        else if (convertString == 3)
        {
            printf("insert key: ");
            scanf("%s", key);
            tree = insert(tree, key);
            searchNode(tree, key);
        }
        else if (convertString == 4)
        {
            printf("remove key: ");
            scanf("%s", key);
            tree = deleteNode(tree, key);
            searchNode(tree, key);
        }
        else if (convertString == 5)
        {
            findSize(tree, &size); //calculates size of tree currently
            printf("height: %d, size: %d\n", height(tree), size);
            size = 0; //reset size so it doesn't increase everytime user asks for height and size of tree
        }
        else if (convertString == 6)
        {
            printf("frequency: ");
            scanf("%d", &count);
            findAll(tree, count);
        }
        else
        {
            printf("Thanks for using the program.\n");
            exit(0);
        }
    }
    return 0;
}

int height(Node * node) // returns height of tree
{
    if (node == NULL)
    {
        return 0;
    }
    return node->height; //height of root
}

int max(int x, int y) // returns the bigger integer out of two given
{
    if (x > y)
    {
        return x;
    }
    else // y > x or y == x
    {
        return y;
    }
}

int verifyCommand(char * command) // verifies user's input
{
    int input = 0;
    for (int i = 0; i < strlen(command); i++)
    {
        if (!isdigit(command[i]))
        {
                printf("Error: can only enter one of the following digits: '1', '2', '3', '4', '5', '6', '7'. Please try again:\n");
                return 0;
        }
        else
        {
            input = atoi(command);
            if (command[0] == '0' || input > 7)
            {
                printf("Error: can only enter one of the following digits: '1', '2', '3', '4', '5', '6', '7'. Please try again:\n");
                return 0;
            }
        }
    }
    return 1;
}

Node * insert(Node * node, char key[200])
{
    int balanceFactor = 0;
    if (node == NULL)
    {
        return createNode(key); // if empty tree, create the first node
    }

    if (strcmp(key, node->key) < 0)
    {
        node->left  = insert(node->left, key);
    }
    else if (strcmp(key, node->key) > 0)
    {
        node->right = insert(node->right, key);
    }
    else
    {
        node->count++;
        return node;
    }
    node->height = 1 + max(height(node->left), height(node->right)); // update node height

    //If this node is unbalanced (balance factor > 2 || < -2), need to balance tree
    balanceFactor = getBalance(node);

    if (balanceFactor > 1 && strcmp(key, node->left->key) < 0) //left left
    {
        return rightRotate(node);
    }

    if (balanceFactor < -1 && strcmp(key, node->right->key) > 0) // right right
    {
        return leftRotate(node);
    }

    if (balanceFactor > 1 && strcmp(key, node->left->key) > 0) // left right
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    if (balanceFactor < -1 && strcmp(key, node->right->key) < 0) // right left
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

Node * createNode(char key[200]) // creates node, allocates memory, and initializes values
{
    Node * node = (Node *)malloc(sizeof(Node));
    strcpy(node->key, key);
    node->left = NULL;
    node->right  = NULL;
    node->count = 1;
    node->height = 1;
    return node;
}

Node * rightRotate(Node * node) // right rotation
{
    Node * left = node->left;
    Node * leftChild = left->right;

    left->right = node;
    node->left = leftChild;

    node->height = max(height(node->left), height(node->right))+1;
    left->height = max(height(left->left), height(left->right))+1;

    return left;
}

Node * leftRotate(Node * node) // left rotation
{
    Node * right = node->right;
    Node * rightChild = right->left;

    right->left = node;
    node->right = rightChild;

    node->height = max(height(node->left), height(node->right)) + 1;
    right->height = max(height(right->left), height(right->right)) + 1;

    return right;
}

int getBalance(Node * node) // determines balance factor for a node
{
    int balanceFactor = 0;
    if (node == NULL)
    {
        return 0;
    }

    balanceFactor = height(node->left) - height(node->right);
    return balanceFactor;
}

void searchNode(Node * node, char key[100]) // searches for a node in Binary Search Tree using key given by user
{
    Node * temp = node;

    if (temp == NULL)
    {
        printf("no_such_key\n");
        return;
    }

    while (strcmp(key, temp->key) != 0)
    {
        if (temp->left == NULL && temp->right == NULL)
        {
            printf("no_such_key\n");
            return;
        }

        if (strcmp(key, temp->key) > 0)
        {
            temp = temp->right;
        }
        else
        {
            temp = temp->left;
        }
    }
    printf("key: %s, frequency: %d\n", key, temp->count);
    return;
}

Node * smallestNode(Node * node) // in order successor method
{
    Node * temp = node;

    while (temp->left != NULL)
    {
        temp = temp->left;
    }
    return temp;
}

Node * deleteNode(Node * node, char key[200]) // deletes a certain node in the tree given a key
{
    if (node == NULL)
    {
        return node;
    }

    if (strcmp(key, node->key) < 0) // go left, as given by the rules of a binary search tree
    {
        node->left = deleteNode(node->left, key);
    }
    else if (strcmp(key, node->key) > 0)
    {
        node->right = deleteNode(node->right, key);
    }
    else
    {
        if (node->count != 1) // if root frequency value is higher than 1, decrement it's value and keep the node
        {
            node->count--;
            return node;
        }

        Node * temp;
        if(node->left == NULL || node->right == NULL)
        {
            if (node->left)
            {
                temp = node->left;
            }
            else
            {
                temp = node->right;
            }

            if (temp == NULL)
            {
                temp = node;
                node = NULL;
            }
            else
            {
                *node = *temp;
            }
            free(temp);
        }
        else
        {
            temp = smallestNode(node->right); //find inorder successor
            strcpy(node->key, temp->key);
            node->right = deleteNode(node->right, temp->key);
        }
    }

    if (node == NULL)
    {
      return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    // Balance tree if necessary
    int balanceFactor = 0;
    balanceFactor = getBalance(node);

    if (balanceFactor > 1 && getBalance(node->left) >= 0)
    {
        return rightRotate(node);
    }

    if (balanceFactor > 1 && getBalance(node->left) < 0)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    if (balanceFactor < -1 && getBalance(node->right) <= 0)
    {
        return leftRotate(node);
    }

    if (balanceFactor < -1 && getBalance(node->right) > 0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

void findSize(Node * node, int * size) // calculates size of tree (number of nodes)
{
    if (node != NULL)
    {
        *size = *size + 1;
        findSize(node->left, size);
        findSize(node->right, size);
    }
    return;
}

void findAll(Node * node, int count) // finds keys with frequencies higher than what the user inputted and prints their information
{
    if(node != NULL)
    {
        if (count < node->count)
        {
            printf("key: %s, frequency: %d\n", node->key, node->count);
        }
        findAll(node->left, count);
        findAll(node->right, count);
    }
    return;
}

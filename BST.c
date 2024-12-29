// Implementation of binary search tree module
// Taken from Lab03

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "BST.h"
#include "List.h"

////////////////////////////////////////////////////////////////////////

static struct Node *newNode(char *val, char *url);
static void doBstPrint(struct Node *t, FILE *out);
static void showBstNode(struct Node *t, FILE *invertedIndex);

////////////////////////////////////////////////////////////////////////

// Creates a new empty bst
struct Node *bstNew(void) {
    return NULL;
}

// Frees all the memory associated with the given bst
void bstFree(struct Node *t) {
    if (t == NULL) {
        return;
    }

    bstFree(t->left);
    bstFree(t->right);
    free(t);
}

// Inserts a new value into the given BST and returns the root of the
// updated bst. Does not insert duplicate values.
struct Node *bstInsert(struct Node *t, char *val, char *url) {
    if (t == NULL) {
        return newNode(val, url);
    } else if (strcmp(val, t->value) < 0) {
        t->left = bstInsert(t->left, val, url);
    } else if (strcmp(val, t->value) > 0) {
        t->right = bstInsert(t->right, val, url);
    } else if (strcmp(val, t->value) == 0) { 
        //Insert duplicates's url
        t->urlList->nodes = appendNode(t->urlList, url);
        removeDuplicates(t->urlList->nodes);
        sortUrlAlphabetically(t->urlList);
    }
    return t;
}

// Creates a new Node containing the given value
static struct Node *newNode(char *val, char *url) {
    struct Node *new = malloc(sizeof(*new));
    assert(new != NULL);
    
    strcpy(new->value, val);
    new->urlList = createList();
    new->urlList->nodes = appendNode(new->urlList, url);
    new->left = NULL;
    new->right = NULL;
    return new;
}

// Checks whether a value is in the given bst
bool bstFind(struct Node *t, char *val) {
    if (t == NULL) {
        return false;
    } else if (strcmp(val, t->value) < 0) {
        return bstFind(t->left, val);
    } else if (strcmp(val, t->value) > 0) {
        return bstFind(t->right, val);
    } else { // (val == t->value)
        return true;
    }
}

// Prints a struct Node *to the given file.
// Assumes that the file is open for writing.
void bstPrint(struct Node *t, FILE *out) {
    doBstPrint(t, out);
}

// Prints a struct Node *to stdout.
void bstShow(struct Node *t) {
    bstPrint(t, stdout);
}

// Prints the in-order traversal of the given BST
void bstInOrder(struct Node *t, FILE *invertedIndex) {
    if (t == NULL) return;

    bstInOrder(t->left, invertedIndex);
    showBstNode(t, invertedIndex);
    bstInOrder(t->right, invertedIndex);
}

// Prints the value in the given node
static void showBstNode(struct Node *t, FILE *invertedIndex) {  
    if (t == NULL) return;
    //Write to file
    fprintf(invertedIndex, "%s ", t->value);

    // Print urls
    struct node *current = t->urlList->nodes;
    while (current != NULL) {
        fprintf(invertedIndex, "%s ", current->url);
        current = current->next;
    }

    fprintf(invertedIndex, "\n");
}

////////////////////////////////////////////////////////////////////////
// !!! DO NOT MODIFY THE CODE BELOW !!!

////////////////////////////////////////////////////////////////////////
// ASCII tree printer
// Courtesy: ponnada
// Via: http://www.openasthra.com/c-tidbits/printing-binary-trees-in-ascii/

// data structures
typedef struct asciiNode_struct asciiNode;
struct asciiNode_struct {
    asciiNode *left, *right;
    // length of the edge from this Node to its children
    int edge_length;
    int height;
    int lablen;
    // -1=I am left, 0=I am root, 1=I am right
    int parent_dir;
    // max supported unit32 in dec, 10 digits max
    char label[11];
};

// functions
static void print_level(asciiNode *Node, int x, int level, FILE *out);
static void compute_edge_lengths(asciiNode *Node);
static void compute_lprofile(asciiNode *Node, int x, int y);
static void compute_rprofile(asciiNode *Node, int x, int y);
static asciiNode *build_ascii_tree(struct Node *t);
static void free_ascii_tree(asciiNode *Node);

#define MAX_HEIGHT 1000
static int lprofile[MAX_HEIGHT];
static int rprofile[MAX_HEIGHT];
#define INFINITY (1 << 20)

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

static int gap = 3; // gap between left and right Nodes

// used for printing next Node in the same level,
// this is the x coordinate of the next char printed
static int print_next;

// prints ascii tree for given Tree structure
static void doBstPrint(struct Node *t, FILE *out)
{
    asciiNode *proot;
    int xmin, i;
    if (t == NULL)
        return;
    proot = build_ascii_tree(t);
    compute_edge_lengths(proot);
    for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
        lprofile[i] = INFINITY;
    compute_lprofile(proot, 0, 0);
    xmin = 0;
    for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
        xmin = MIN(xmin, lprofile[i]);
    for (i = 0; i < proot->height; i++) {
        print_next = 0;
        print_level(proot, -xmin, i, out);
        fprintf(out, "\n");
    }
    if (proot->height >= MAX_HEIGHT) {
        fprintf(out,
            "(Tree is taller than %d; may be drawn incorrectly.)\n",
            MAX_HEIGHT);
    }
    free_ascii_tree(proot);
}

// This function prints the given level of the given tree, assuming
// that the Node has the given x cordinate.
static void print_level(asciiNode *Node, int x, int level, FILE *out)
{
    int i, isleft;
    if (Node == NULL)
        return;
    isleft = (Node->parent_dir == -1);
    if (level == 0) {
        for (i = 0;
             i < (x - print_next - ((Node->lablen - isleft) / 2));
             i++)
            fprintf(out, " ");
        print_next += i;
        fprintf(out, "%s", Node->label);
        print_next += Node->lablen;
    } else if (Node->edge_length >= level) {
        if (Node->left != NULL) {
            for (i = 0; i < (x - print_next - (level)); i++) {
                fprintf(out, " ");
            }
            print_next += i;
            fprintf(out, "/");
            print_next++;
        }
        if (Node->right != NULL) {
            for (i = 0; i < (x - print_next + (level)); i++) {
                fprintf(out, " ");
            }
            print_next += i;
            fprintf(out, "\\");
            print_next++;
        }
    } else {
        print_level(
            Node->left,
            x - Node->edge_length - 1,
            level - Node->edge_length - 1,
            out);
        print_level(
            Node->right,
            x + Node->edge_length + 1,
            level - Node->edge_length - 1,
            out);
    }
}

// This function fills in the edge_length and
// height fields of the specified tree
static void compute_edge_lengths(asciiNode *Node)
{
    int h, hmin, i, delta;
    if (Node == NULL)
        return;
    compute_edge_lengths(Node->left);
    compute_edge_lengths(Node->right);

    /* first fill in the edge_length of Node */
    if (Node->right == NULL && Node->left == NULL)
        Node->edge_length = 0;
    else {
        if (Node->left == NULL)
            hmin = 0;
        else {
            for (i = 0; i < Node->left->height && i < MAX_HEIGHT; i++)
                rprofile[i] = -INFINITY;
            compute_rprofile(Node->left, 0, 0);
            hmin = Node->left->height;
        }
        if (Node->right == NULL)
            hmin = 0;
        else {
            for (i = 0; i < Node->right->height && i < MAX_HEIGHT; i++)
                lprofile[i] = INFINITY;
            compute_lprofile(Node->right, 0, 0);
            hmin = MIN(Node->right->height, hmin);
        }
        delta = 4;
        for (i = 0; i < hmin; i++) {
            int w = gap + 1 + rprofile[i] - lprofile[i];
            delta = (delta > w) ? delta : w;
        }

        // If the Node has two children of height 1, then we allow the
        // two leaves to be within 1, instead of 2
        if (((Node->left != NULL && Node->left->height == 1) ||
            (Node->right != NULL && Node->right->height == 1)) &&
            delta > 4)
            delta--;
        Node->edge_length = ((delta + 1) / 2) - 1;
    }

    // now fill in the height of Node
    h = 1;
    if (Node->left != NULL)
        h = MAX(Node->left->height + Node->edge_length + 1, h);
    if (Node->right != NULL)
        h = MAX(Node->right->height + Node->edge_length + 1, h);
    Node->height = h;
}

static asciiNode *build_ascii_tree_recursive(struct Node *t)
{
    asciiNode *Node;

    if (t == NULL)
        return NULL;
    Node = malloc(sizeof(asciiNode));
    assert(Node != NULL);
    Node->left = build_ascii_tree_recursive(t->left);
    Node->right = build_ascii_tree_recursive(t->right);
    if (Node->left != NULL)
        Node->left->parent_dir = -1;
    if (Node->right != NULL)
        Node->right->parent_dir = 1;
    sprintf(Node->label, "%s", t->value);
    Node->lablen = (int) strlen(Node->label);

    return Node;
}

// Copy the tree into the ascii Node structre
static asciiNode *build_ascii_tree(struct Node *t)
{
    asciiNode *Node;
    if (t == NULL)
        return NULL;
    Node = build_ascii_tree_recursive(t);
    Node->parent_dir = 0;
    return Node;
}

// Free all the Nodes of the given tree
static void free_ascii_tree(asciiNode *Node)
{
    if (Node == NULL)
        return;
    free_ascii_tree(Node->left);
    free_ascii_tree(Node->right);
    free(Node);
}

// The following function fills in the lprofile array for the given
// tree. It assumes that the center of the label of the root of this tree
// is located at a position(x,y).  It assumes that the edge_length
// fields have been computed for this tree.
static void compute_lprofile(asciiNode *Node, int x, int y)
{
    int i, isleft;
    if (Node == NULL)
        return;
    isleft = (Node->parent_dir == -1);
    lprofile[y] = MIN(lprofile[y], x - ((Node->lablen - isleft) / 2));
    if (Node->left != NULL) {
        for (i = 1; i <= Node->edge_length && y + i < MAX_HEIGHT; i++)
            lprofile[y + i] = MIN(lprofile[y + i], x - i);
    }
    compute_lprofile(
        Node->left,
        x - Node->edge_length - 1,
        y + Node->edge_length + 1);
    compute_lprofile(
        Node->right,
        x + Node->edge_length + 1,
        y + Node->edge_length + 1);
}

static void compute_rprofile(asciiNode *Node, int x, int y)
{
    int i, notleft;
    if (Node == NULL)
        return;
    notleft = (Node->parent_dir != -1);
    rprofile[y] = MAX(rprofile[y], x + ((Node->lablen - notleft) / 2));
    if (Node->right != NULL) {
        for (i = 1; i <= Node->edge_length && y + i < MAX_HEIGHT; i++)
            rprofile[y + i] = MAX(rprofile[y + i], x + i);
    }
    compute_rprofile(
        Node->left,
        x - Node->edge_length - 1,
        y + Node->edge_length + 1);
    compute_rprofile(
        Node->right,
        x + Node->edge_length + 1,
        y + Node->edge_length + 1);
}
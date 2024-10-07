#include "common.h"

void parser(Scanner *scanner)
{
    // Get the Token list from the scanner

    // Print the retrieved List
    printf("Token list:\n");
    for (int i = 0; i < scanner->token_count; i++) {
        printf("Token %d - Type %d - Literal %s\n ", i, scanner->tokens[i].type, scanner->tokens[i].literal);
    }
    // Free the scanner results
    free(scanner);
}

// The main problem here is to create a node with different structures
Expr* create_node_ast(Expr expression) {
    Expr* newNode = (Expr*)malloc(sizeof(Expr));
    newNode->op = expression.op;
    newNode->left = expression.left;
    newNode->right = expression.right;
    return newNode;
}

// Function to insert a new node into the AST
Expr* insert_node_ast(Expr* root, Expr expression) {
    if (root == NULL) {
        return create_node_ast(expression);
    }
    // We'll need to manage the tree with something else I think...
    if (expression.op.type < root->op.type) {
        root->left = insert_node_ast(root->left, expression);
    } else {
        root->right = insert_node_ast(root->right, expression);
    }
    return root;
}

void print_tree(Expr* root) {
    if (root == NULL) {
        return;
    }
    // Traverse the left subtree
    print_tree(root->left);
    // Print the current node
    printf("Node: %d\n", root->op.type); // Adjust this line based on what you want to print
    // Traverse the right subtree
    print_tree(root->right);
}

// Function to check the type of an input
void check_expr_type(Expr* expr) {
    switch (expr->type) {
        case EXPR_UNARY:
            printf("Expression is of type UNARY\n");
            break;
        case EXPR_GROUPING:
            printf("Expression is of type GROUPING\n");
            break;
        case EXPR_LITERAL:
            printf("Expression is of type LITERAL\n");
            break;
        case EXPR_BINARY:
            printf("Expression is of type BINARY\n");
            break;
        default:
            printf("Unknown expression type\n");
            break;
    }
}






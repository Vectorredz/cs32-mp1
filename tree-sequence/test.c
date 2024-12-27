
#include <stdlib.h>
#include "p_tree_list.c"

int main(){
    // TODO: REVERT TYPEDEFS TO INT64_T AND SIZE_T
    // TODO: free memory
    int n = 17;
    DATA seq[17] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
    PTreeList* list0 = make(n, seq);
    printf("Pass.");
    reverse(list0);
    for (int i = 0; i < n; i++){
        printf("Yeah: %d\n", get(list0, i));
    }
    printf("LEFT: %d\n", peek_left(list0));
    printf("RIGHT: %d\n", peek_right(list0));

    DATA seq1[4] = {1, 2, 3, 4};
    LENGTH l1, k1;
    _getGreatestPowerOfTwo(4, &l1, &k1);

    PTree* ptree1 = (PTree*) malloc(sizeof(PTree));
    ptree1->l = l1;
    ptree1->k = k1;
    ptree1->root = _constructPTreeNodesFromRange(seq1, 0, 0, 3);

    DATA seq2[4] = {5, 6, 7, 8};
    LENGTH l2, k2;
    _getGreatestPowerOfTwo(4, &l2, &k2);

    PTree* ptree2 = (PTree*) malloc(sizeof(PTree));
    ptree2->l = l2;
    ptree2->k = k2;
    ptree2->root = _constructPTreeNodesFromRange(seq2, 0, 0, 3);

    DATA seq3[8] = {9, 10, 11, 12, 13, 14, 15, 16};
    LENGTH l3, k3;
    _getGreatestPowerOfTwo(8, &l3, &k3);

    PTree* ptree3 = (PTree*) malloc(sizeof(PTree));
    ptree3->l = l3;
    ptree3->k = k3;
    ptree3->root = _constructPTreeNodesFromRange(seq3, 0, 0, 7);

    PTreeList* list = (PTreeList*) malloc(sizeof(PTreeList));
    list->n = 16;
    list->reversed = false;
    
    PTreeListNode* a = (PTreeListNode*) malloc(sizeof(PTreeListNode));
    a->prev = NULL;
    a->ptree = ptree1;

    PTreeListNode* b = (PTreeListNode*) malloc(sizeof(PTreeListNode));
    b->prev = a;
    a->next = b;
    b->ptree = ptree2;

    PTreeListNode* c = (PTreeListNode*) malloc(sizeof(PTreeListNode));
    c->next = NULL;
    c->prev = b;
    b->next = c;
    c->ptree = ptree3;

    list->head = a;
    list->tail = c;

    _mergeNonDistinctPTrees(list, list->head, false);
    printf("??\n");
    PTreeListNode* currentListNode = list->head;
    while (currentListNode != NULL){
        PTree* ptree = currentListNode->ptree;
        printf("KK: %d\n", ptree->k);
        _printLeaves(ptree->root);
        currentListNode = currentListNode->next;
        printf("-------------------\n");
    }

    printf("POP:\n");
    pop_left(list);
    currentListNode = list->head;
    while (currentListNode != NULL){
        PTree* ptree = currentListNode->ptree;
        printf("KK: %d\n", ptree->k);
        _printLeaves(ptree->root);
        currentListNode = currentListNode->next;
        printf("-------------------\n");
    }
}
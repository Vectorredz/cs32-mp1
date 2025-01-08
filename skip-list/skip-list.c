#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#include "skip-list.h"


/*
-----------------------------------------------
INITIALIZERS
-----------------------------------------------
*/

LENGTH ceil_lg(LENGTH n) {
    return __builtin_clz(0LL) - __builtin_clz(n - 1);
}

LENGTH _cap_height(LENGTH n){
    // return ceil_lg(n); // ceil up
    return (LENGTH)(log2(n));
}

bool _flip_coin(){
    return (bool)(rand() > RAND_MAX / 2);
}

SkipNode *_make_node(DATA v){
    SkipNode *new_node = (SkipNode*)malloc(sizeof(SkipNode));
    new_node->below = new_node->right = new_node->left = NULL; // initialize the pointers to NULL
    new_node->is_sentinel = false;
    new_node->val = v;
    new_node->width = 0;
    return new_node ; 
}

Level *_make_level(){
    Level *new_level = (Level*)malloc(sizeof(Level));
    new_level->up = new_level->down = NULL;

    new_level->cached_right_width = 0;

    // initialize sentinels
    SkipNode *head_sentinel = (SkipNode*)malloc(sizeof(SkipNode));
    head_sentinel->left = head_sentinel->right = head_sentinel->below = NULL;

    SkipNode *tail_sentinel = (SkipNode*)malloc(sizeof(SkipNode));
    tail_sentinel->left = tail_sentinel->right = tail_sentinel->below = NULL;

    head_sentinel->is_sentinel = true;
    head_sentinel->val = INT_MIN;
    head_sentinel->width = 0;

    tail_sentinel->is_sentinel = true;
    tail_sentinel->val = INT_MAX;
    tail_sentinel->width = 0;

    // link the sentinels 
    head_sentinel->right = tail_sentinel;
    tail_sentinel->left = head_sentinel;

    new_level->head_sentinel = head_sentinel;
    new_level->tail_sentinel = tail_sentinel;
    
    return new_level;
} 

SkipList *_init(){
    SkipList *skip_list = (SkipList*)malloc(sizeof(SkipList));

    // initalize level-related members
    skip_list->levels = (LevelList*)malloc(sizeof(LevelList));
    LevelList *level_list = skip_list->levels;
    level_list->bottom = level_list->top = _make_level();

    skip_list->node_top_levels = (LevelRecordsList*)malloc(sizeof(LevelRecordsList));
    LevelRecordsList *level_records = skip_list->node_top_levels;
    level_records->head = level_records->tail = NULL;
    
    // initialize values
    skip_list->leftmost = skip_list->rightmost = 0;
    skip_list->max_height = 1;
    skip_list->curr_height = 0;
    skip_list->size = 0;
    skip_list->is_reversed = false; // default : toggled off

    return skip_list;
}

/*
-----------------------------------------------
HELPERS
-----------------------------------------------
*/

Level *_promote_level(SkipList *l, bool from_right){
    Level *curr_level = l->levels->bottom;
    SkipNode *below_node = from_right ? curr_level->tail_sentinel->left : curr_level->head_sentinel->right;
    DATA v = below_node->val;
    while (l->curr_height < l->max_height && _flip_coin() == true){ // promote the level
        // move up a level (create a new one if topmost)
        if (curr_level->up == NULL){
            Level *new_level = _make_level();

            // update level connections
            new_level->down = curr_level;
            curr_level->up = new_level;
            l->levels->top = new_level;

            // update sentinel connections
            new_level->head_sentinel->below = curr_level->head_sentinel;
            new_level->tail_sentinel->below = curr_level->tail_sentinel;

            // if from right and there's an element to its left
            if (l->size >= 1){
                // update cached with to match entire list
                new_level->cached_right_width = l->size-1;
            }
            
            l->curr_height++;
        }
        curr_level = curr_level->up;

        SkipNode *new_prev_node = _make_node(v);
        SkipNode *sentinel = from_right ? curr_level->tail_sentinel : curr_level->head_sentinel;

        // below
        new_prev_node->below = below_node;
        below_node = new_prev_node;

        if (!from_right){
            // left
            new_prev_node->right = sentinel->right; // curr leftmost
            sentinel->right->left = new_prev_node;
            sentinel->right = new_prev_node;
            new_prev_node->left = sentinel;
        }
        else {
            // right
            new_prev_node->left = sentinel->left;
            sentinel->left->right = new_prev_node;
            sentinel->left = new_prev_node;
            new_prev_node->right = sentinel;

            new_prev_node->width = curr_level->cached_right_width; // update the new_prev_node width
            curr_level->cached_right_width = 0; // reset cached with
        }
    }

    return curr_level;
}

void _demote_level(SkipList *l, Level *curr_level, bool from_right){
    // update the height
    while (curr_level != NULL){
        SkipNode *sentinel = from_right ? curr_level->tail_sentinel : curr_level->head_sentinel;
        SkipNode *curr_node = from_right ? sentinel->left : sentinel->right;
        SkipNode *curr_prev_node = from_right ? curr_node->left : curr_node->right;
        Level *downCurr = curr_level->down;
        // Remove curr_node from the level
        if (!from_right){
            sentinel->right = curr_prev_node;
            curr_prev_node->left = sentinel;

        } else {
            sentinel->left = curr_prev_node;
            curr_prev_node->right = sentinel;
        }
        free(curr_node);

        // If level is empty and is NOT Level 0, then remove it from the list
        if (curr_level != l->levels->bottom && (curr_level->head_sentinel->right == curr_level->tail_sentinel)){
            if (l->levels->top == curr_level){
                l->levels->top = curr_level->down;
                l->curr_height--;
            }
            // Update sentinel pointers
            Level *up_level = curr_level->up;
            Level *down_level = curr_level->down;

            down_level->up = up_level;
            if (up_level != NULL){
                up_level->head_sentinel->below = down_level->head_sentinel;
                up_level->tail_sentinel->below = down_level->tail_sentinel;
                up_level->down = curr_level->down;
            }

            free(curr_level->head_sentinel);
            free(curr_level->tail_sentinel);
            free(curr_level);

        }
        curr_level = downCurr;
    }
}

SkipNode *_get_node(SkipList *l, LENGTH target, bool from_set, DATA v){
    // bounds check
    if (l->size == 0) return NULL;
    if (!(0 <= target && target < l->size)) return NULL; // index out of range and negative index
   
    SkipNode *header = l->levels->top->head_sentinel; // Topmost head_sentinel; Header
    LENGTH sum_offset = 0; // initialize the offset for the topmost
    if (header == NULL) return NULL;
    if (header->right == NULL) return NULL;
    
    // sentinel phase
    while (header && header->is_sentinel == true){
        // don't go right if it's tail_sentinel
        if (header->right->is_sentinel == true){
            header = header->below;
            continue;
        }
        // within index, can go right
        if (sum_offset + header->right->width <= target){  
            sum_offset += header->right->width;
            header = header->right; // not sentinel anymore
            break;
        }
        else { // if the leftmost SkipNode width >= target go down the head_sentinel
            header = header->below;
        }
    }

    // can go down as long as header->below != NULL
    // Main traversal
    while (header) {
        if (sum_offset == target) {
            if (header->below == NULL) {
                return header;
            }
            if (from_set){
                header->val = v;
            }
            header = header->below;
            continue;
        }
        if (header->right->is_sentinel == false && (sum_offset + header->right->width <= target)) {
            sum_offset += header->right->width;
            header = header->right;
        } 
        else {
            header = header->below;
        }
    }

    return NULL; 

}

void _push_left_base(SkipList *l, DATA v){
    // update values
    l->size++;
    l->leftmost = v;
    if (l->size == 1){
        l->rightmost = v;
        l->levels->bottom->cached_right_width = 1; // edge case
    }
    l->max_height = _cap_height(l->size);

    // update leftmost widths
    Level* curr_level = l->levels->bottom; // start from level 0 
    while (curr_level != NULL){
        SkipNode* prevNode = curr_level->head_sentinel->right;
        if (prevNode->is_sentinel == false){
            prevNode->width++;
        }
        curr_level = curr_level->up;
    }

    // insert new leftmost node at level 0
    curr_level = l->levels->bottom;
    SkipNode *below_node = _make_node(v);
    SkipNode *sentinel = curr_level->head_sentinel;

    below_node->right = sentinel->right;
    sentinel->right->left = below_node;
    sentinel->right = below_node;
    below_node->left = sentinel;

    // promote
    Level *max_level_reached = _promote_level(l, false);
    LevelRecord *new_record = (LevelRecord*) (malloc(sizeof(LevelRecord)));
    
    new_record->prev = NULL;
    new_record->next = l->node_top_levels->head;
    if (l->node_top_levels->head != NULL){
        l->node_top_levels->head->prev = new_record;
    }
    l->node_top_levels->head = new_record;
    if (l->node_top_levels->tail == NULL){
        l->node_top_levels->tail = new_record;
    }
    new_record->top_level = max_level_reached;
}

void _push_right_base(SkipList *l, DATA v){
    // update values
    l->size++;
    l->rightmost = v;
    if (l->size == 1) l->leftmost = v;
    l->max_height = _cap_height(l->size);

    // insert new rightmost node at level 0
    Level* curr_level = l->levels->bottom;
    SkipNode *below_node = _make_node(v);
    SkipNode *sentinel = curr_level->tail_sentinel; // header

    below_node->left = sentinel->left;
    sentinel->left->right = below_node;
    sentinel->left = below_node;
    below_node->right = sentinel;

    // width updates (lvl 0, all current levels)
    l->levels->bottom->cached_right_width = 0;
    if (l->size > 1){
        curr_level = l->levels->top;
        while (curr_level != NULL){
            curr_level->cached_right_width++;
            curr_level = curr_level->down;
        }
    }
    below_node->width = l->levels->bottom->cached_right_width;

    // promote
    Level *max_level_reached = _promote_level(l, true);
    LevelRecord *new_record = (LevelRecord*) (malloc(sizeof(LevelRecord)));
    
    new_record->next = NULL;
    new_record->prev = l->node_top_levels->tail;
    if (l->node_top_levels->tail != NULL){
        l->node_top_levels->tail->next = new_record;
    }
    l->node_top_levels->tail = new_record;
    if (l->node_top_levels->head == NULL){
        l->node_top_levels->head = new_record;
    }
    new_record->top_level = max_level_reached;
}

bool _pop_left_base(SkipList *l){
    // cannot pop an empty list
    if (l->size == 0) return false;

    // update values
    l->size--;
    if (l->size > 0){
        DATA leftmostVal = l->levels->bottom->head_sentinel->right->right->val;
        l->leftmost = leftmostVal;
        if (l->size == 1){
            l->rightmost = leftmostVal;
        }
    }
    l->max_height = _cap_height(l->size);
    if (l->size == 0){
        l->levels->bottom->cached_right_width = 0; // edge case
    }

    // remove leftmost node from list
    LevelRecord *currRecord = l->node_top_levels->head;
    LevelRecord *prevRecord = currRecord->next;
    Level *max_level_reached = currRecord->top_level;

    l->node_top_levels->head = prevRecord;
    if (prevRecord != NULL){
        prevRecord->prev = NULL;
    }
    if (l->node_top_levels->tail == currRecord){
        l->node_top_levels->tail = NULL;
    }
    free(currRecord);
    
    _demote_level(l, max_level_reached, false);


    // update widths
    if (l->size > 0){
        Level *curr_level = l->levels->bottom;
        while (curr_level != NULL){
            SkipNode* prevNode = curr_level->head_sentinel->right;
            prevNode->width--;
            curr_level = curr_level->up;
        }
    }

    return true;
}

bool _pop_right_base(SkipList *l){
    // cannot pop an empty list
    if (l->size == 0) return false;

    // update values
    l->size--;
    if (l->size > 0){
        DATA rightmostVal = l->levels->bottom->tail_sentinel->left->left->val;
        l->rightmost = rightmostVal;
        if (l->size == 1){
            l->rightmost = rightmostVal;
        }
    }
    l->max_height = _cap_height(l->size);
    if (l->size == 0){
        l->levels->bottom->cached_right_width = 0; // edge case
    }

    // rewind the cached widths for last touched levels
    LevelRecord *currRecord = l->node_top_levels->tail;
    Level *max_level_reached = currRecord->top_level;

    Level *curr_level = max_level_reached;
    while (curr_level != NULL){
        curr_level->cached_right_width = curr_level->tail_sentinel->left->width;
        curr_level = curr_level->down;
    }

    // update all widths
    if (l->size > 1){
        curr_level = l->levels->top;
        while (curr_level != NULL){
            curr_level->cached_right_width--;
            curr_level = curr_level->down;
        }
    }

    // remove rightmost node from list
    LevelRecord *prevRecord = currRecord->prev;
    l->node_top_levels->tail = prevRecord;
    if (prevRecord != NULL){
        prevRecord->next = NULL;
    }
    if (l->node_top_levels->head == currRecord){
        l->node_top_levels->head = NULL;
    }
    free(currRecord);

    _demote_level(l, max_level_reached, true);

    return true;
}


/*
-----------------------------------------------
MAIN OPERATIONS
-----------------------------------------------
*/

SkipList *make(LENGTH n, DATA *seq){
    srand(time(NULL));
    SkipList *sl = _init();
    if (n <= 0) return sl;
    
    // traverse through the sequence
    for (LENGTH i = 0; i < n; i++){
        push_right(sl, seq[i]);
    }
    return sl;
}

void reverse(SkipList *sl){
    sl->is_reversed = !(sl->is_reversed);
}

LENGTH size(SkipList *l){
    return l->size;
}

bool empty(SkipList *l){
    return (l->size == 0);
}

DATA get(SkipList *l, LENGTH i){
    if (l->size == 0) return 0;
    if (!(0 <= i && i < l->size)) return 0; // SkipNode doesn't exist, error
    SkipNode *retNode = _get_node(l, l->is_reversed == false ? i : l->size-1-i, false, 0);
    if (retNode == NULL) return 0;

    return retNode->val;
}

void set(SkipList *l, LENGTH i, DATA v){
    if (l->size == 0) return;
    if (!(0 <= i && i < l->size)) return; // SkipNode doesn't exist, error
    LENGTH index = l->is_reversed == false ? i : abs(l->size-1-i);
    SkipNode *retNode = _get_node(l, index, true, v);
    if (retNode == NULL) return;
    retNode->val = v;
    if (index == 0){
        l->leftmost = v;
    }
    if (index == l->size-1){
        l->rightmost = v;
    }
}

DATA peek_left(SkipList *l){
    if (l->size == 0) return 0;
    return l->is_reversed ? l->rightmost : l->leftmost;
}

DATA peek_right(SkipList *l){
    if (l->size == 0) return 0;
    return l->is_reversed ? l->leftmost : l->rightmost;
}

void push_left(SkipList *l, DATA v){
    l->is_reversed == false ? _push_left_base(l, v) : _push_right_base(l, v);
}

void push_right(SkipList *l, DATA v){
    l->is_reversed == false ? _push_right_base(l, v) : _push_left_base(l, v);
}

bool pop_left(SkipList *l){
    return l->is_reversed == false ? _pop_left_base(l) : _pop_right_base(l);
}

bool pop_right(SkipList *l){
    return l->is_reversed == false ? _pop_right_base(l) : _pop_left_base(l);
}

void displayWidth(SkipList *sl) {

    Level *curr_level = sl->levels->top;
    printf("Max Level: %d \n", sl->curr_height);
    printf("N: %d\n", sl->size);
    int curr_height = sl->curr_height;
    while (curr_level != NULL){
        // traverse right
        printf("Level %d (rightcache: %d): ", curr_height, curr_level->cached_right_width);
        SkipNode *curr = curr_level->head_sentinel; // leftmost
        int currWidth = 0;
        while (curr != NULL){
            currWidth += curr->width;
            if (curr->is_sentinel == false){
                int o = 0;
                while (o < curr->width){
                    if (o > 0){
                        printf("----- ");
                    }
                    printf("-> ");
                    o++;
                }
            } else if (curr == curr_level->tail_sentinel){
                int o = currWidth;
                while (o < sl->size){
                    if (o > currWidth){
                        printf("----- ");
                    }
                    printf("-> ");
                    o++;
                }
            }
            if (curr == curr_level->head_sentinel){
                printf("H -> ");
                if (curr->right->is_sentinel == false){
                    if (curr->right->width != 0){
                        printf("----- ");
                    }
                }
            } else if (curr == curr_level->tail_sentinel){
                printf("T");
            } else {
                printf("| %d | ", curr->width);
            }
            curr = curr->right;
        }
        curr_level = curr_level->down;
        curr_height--;
        printf("\n");
    }


    printf("LEVELS: ");
    LevelRecord *current = sl->node_top_levels->head;
    while (current != NULL){
        int count = -1;
        Level *curr2 = current->top_level;
        while (curr2 != NULL){
            count++;
            curr2 = curr2->down;
        }
        printf("%d ", count);
        current = current->next;
    }
    printf("\n");
    
}

void displayNodes(SkipList *sl) {
    Level *curr_level = sl->levels->top;
    LENGTH height = sl->curr_height;
    while(curr_level){
        printf("Level %ld: ", height--);
        SkipNode *curr = curr_level->head_sentinel;
        while (curr){
            printf("(%lld) <-> ", curr->val);
            curr = curr->right;
        }
        printf("\n");
        curr_level = curr_level->down;
    }
    printf("\n");

}
/*
-----------------------------------------------
TEST OPERATIONS
-----------------------------------------------
*/
void TEST_elements(SkipList* l, LENGTH* nRef, DATA** seqRef){   
    LENGTH i = 0;
    SkipNode *header = l->levels->bottom->head_sentinel->right;
    DATA *seq = (DATA*)malloc((l->size) * sizeof(DATA));
    while (header != l->levels->bottom->tail_sentinel){
        seq[i] = header->val;
        header = header->right;
        i++;
    }
    *nRef = l->size;
    *seqRef = seq;
}

bool TEST_internal(SkipList* l){
    return true;
}

bool TEST_reversed(SkipList* l){
  return (l->is_reversed);
}
// DOUBLY-LINKED LIST IMPLEMENTATION OF DYNAMIC LIST 
#include <stdio.h>
#include <stdlib.h>
#include "d_linked_list.h"

// Following are the OPERATIONS for the implementation

//*-------------------------------------------------------------*//

list init(void){
    list l = {NULL, NULL, 0,0};
    return l;
}

list *make(LENGTH n, DATA *seq){
    // undefined behavior
    if (n < 0) return NULL;
    node *head = (node*)malloc(sizeof(node));
    list *ptr = (list*)malloc(sizeof(list));
    node *curr = head;
    if (n == 0){
        curr->val = 0;
        curr->left = curr->right = NULL;
        ptr->head = ptr->tail = NULL;
        ptr->size = 0;
        return ptr;
    }
    ptr->head = head;
    ptr->size = n;
    curr->right = NULL;
    for (LENGTH i = 0; i < n; i++){
        // if the index = 0, means that it is the first element
        if (i == 0){
            curr->val = seq[i];
            curr->left = NULL;
        }
        else {
            node *new = (node*)malloc(sizeof(node));
            new->val = seq[i];
            curr->right = new;
            new->left = curr;
            new->right = NULL;
            curr = new;
        }
    }
    curr->right = NULL;
    ptr->tail = curr;
    return ptr;
}

void push_left(list *l, DATA v){
    node *newNode = (node*)malloc(sizeof(node));
    newNode->val = v;
    newNode->left = newNode->right = NULL;
    node *curr = l->reversed ? l->head : l->tail;
    //empty_list case
    if (!(curr)) l->head = l->tail = newNode;
    else {
        if (!(l->reversed)){
            newNode->right = l->head;
            l->head->left = newNode;
            l->head = newNode;
        }
        else {
            newNode->left = l->tail;
            l->tail->right = newNode;
            l->tail = newNode;
        }
    }
    (l->size)++;
}

void push_right(list *l, DATA v){
    node *newNode = (node*)malloc(sizeof(node));
    newNode->val = v;
    newNode->left = newNode->right = NULL;
    node *curr = l->reversed ? l->head : l->tail;
    //empty_list case
    if (!(curr)) l->head = l->tail = newNode;
    else {
        if (l->reversed){
            newNode->right = l->head;
            l->head->left = newNode;
            l->head = newNode;
        }
        else {
            newNode->left = l->tail;
            l->tail->right = newNode;
            l->tail = newNode;
        }
    }
    (l->size)++;
}

bool pop_left(list *l){
    // empty_list
    if (!(l->head) && !(l->tail)){
        fprintf(stderr, "\nIndexError: Pop From Empty List\n");
        return false;
    };
    node *curr = !(l->reversed) ? l->head : l->tail;
    node *store = !(l->reversed) ? curr->right : curr->left; // if left, then store next
    // single_node case
    if ((l->tail) == (l->head)){
        l->head = l->tail = NULL;
        free(curr);
        // reduce the size
        (l->size)--;
        // free the node
        return true; 
    }
    else {
        if (!(l->reversed)){ // reversed toggled ON
            store->left = NULL;
            l->head = store;
        }
        else {
            store->right = NULL;
            l->tail = store;
        }
    }
    (l->size)--;
    free(curr);
    return true;
}

bool pop_right(list *l){
    // empty_list
    if (!(l->head) && !(l->tail)) return false;
    node *curr = l->reversed ? l->head : l->tail;
    node *store = l->reversed ? curr->right : curr->left; // if right, then store prev
    // single_node case
    if ((l->tail) == (l->head)){
        l->head = l->tail = NULL;
        free(curr);
        // reduce the size
        (l->size)--;
        // free the node
        return true; 
    }
    else {
        if (l->reversed){ // reversed toggled ON
            store->left = NULL;
            l->head = store;
        }
        else {
            store->right = NULL;
            l->tail = store;
        }
    }
    (l->size)--;
    free(curr);
    return true;
}

DATA peek_left(list *l){
    // peek from an empty list
    if (!(l->head) && !(l->tail)){
        fprintf(stderr, "\nIndexError: Peek From Empty List\n");
        exit(1);\
    }
    node *curr = !(l->reversed) ? l->head : l->tail;
    return curr->val;
}

DATA peek_right(list *l){
    // peek from an empty list
    if (!(l->head) && !(l->tail)){
        fprintf(stderr, "\nIndexError: Peek From Empty List\n");
        exit(1);\
    }
    node *curr = (l->reversed) ? l->head : l->tail;
    return curr->val;
}

LENGTH size(list *l){
    if (l->size < 0){
        fprintf(stderr, "\nIndexError: invalid size\n");
        exit(1);\
    }
    return l->size;
}

bool empty(list *l){
    return (l->size == 0);
}

DATA get(list *l, LENGTH i){
    node *curr = l->reversed ? l->tail : l->head;
    // raise an exception
    if (i < 0){
        fprintf(stderr, "\nIndexError: list index out of range\n");\
        exit(1);\
    }
    if (i >= size(l)){
        fprintf(stderr, "\nIndexError: list index out of range\n");\
        exit(1);\
    }
    LENGTH j = 0;
    while (j != i){
        if (l->reversed){
            curr = curr->left; 
        }
        else {
            curr = curr->right; 
        }
        j++;
    }
    return curr->val;
}

void set(list *l, LENGTH i, DATA v){
    node *curr = l->reversed ? l->tail : l->head;
    if (i < 0){
        fprintf(stderr, "IndexError: list index out of range\n");\
        exit(1);\
    }
    if (i >= size(l)){
        fprintf(stderr, "IndexError: list index out of range\n");\
        exit(1);\
    }
    LENGTH j = 0;
    while (j != i){
        if (l->reversed){
            curr = curr->left; 
        }
        else {
            curr = curr->right; 
        }
        j++;
    }
    curr->val = v;

}

void reverse(list *l){
    // l->reversed = l->reversed == 0 ? 1 : 0;
    // l->reversed = !(l->reversed);
    if (l->reversed == 0){
        l->reversed = 1;
    }
    else {
        l->reversed = 0;
    }
}

void display(list *l){
    // if (l->size == 0) return;
    node *curr = l->head;
    printf("\n");
    while (curr){
        printf("%ld <-> ", curr->val);
        curr = curr->right;
    }
    printf("NULL \n");
}

DATA *make_seq(LENGTH size, DATA *array){
    for (LENGTH i = 0; i < size; i++){
        array[i] = i;
    }
    return array;
}

LENGTH main(){
    // list l1 = init();
    // // make operation
    // LENGTH n1 = 6;
    // DATA *array = malloc(n1 * sizeof(DATA));
    // DATA *seq1 = make_seq(n1, array);
    // LENGTH *array = malloc(148 * sizeof(LENGTH));
    
    signed long long int array[149] = {572325033031663466,858796565699423048,123437038793970800,798694308249383628,
    806077550475980790,932333242204692314,830334454307253693,39180793219366885,-582449584120391611,-125598510826682808,
    -222554278469421849,-24782518322402526,377067835053941027,-562912215413767900,-81723267832826212,-912884657545813928,
    282229191300292810,827771766905531958,527590859810354224,681367704687985070,-120299424278699268,-222370107460300129,
    -657040519457700569,817088624985560780,-967900671698605452,74572607771757295,-977450149834013690,-51144286213921425,
    -199449624091085480,-940328821628032417,-64513431265012841,751821108659951503,-989587542472555132,996128234591120093,
    864171743525163090,-227917146990197593,289583601782136544,-580079497558350364,276849829282559036,392136005536818252,
    -724754236527259968,-937893955377674071,-887869689176891338,-328944635065173759,170027277560872270,-982689049927908073,
    862931849931790378,884309484021753634,-559877792417697502,-277770868155725419,618093787864707523,918104314947354097,
    -322686004379643408,731063531749291032,-155202859234897480,104143510519907659,-570549317191063325,911536628124956321,
    -44638554411301988,12207804515232052,-14798611444654562,-330961601539199878,-573139469791586741,-641644366632744182,
    597876163530137057,120150730937778219,-829460361453312615,-500095091833488714,-420865209625048133,-130669643444728202,
    296494414465561836,754196360312088229,696854384379605683,321773241981570568,-638209986683034201,-909722055125830507,
    -149165679579634419,342019185099940013,-571973229445219283,-58868764786088061,-720593943917470504,154526452816526331,
    -874661900642546904,-901483298772974370,-515744832370915253,-20305565982622058,682508205253986777,-371610121025311528,
    -596356051889187557,420817954390156887,-22570464122776352,-876459153067160782,999176695888475080,-24792983003719000,
    -994254149419809447,128447603851019544,451465551613620946,102766238341617257,915658148025580387,-7191215273555500,
    488860756278719349,-142576665385623517,39082524428952347,4544622271630216,292722490251827720,338126762756014026,
    -944035646714477151,-882862179005310519,-595170113206452376,54669884463526994,-632221399709870098,860193768006981111
    ,79893390434688304,942467701446328610,511379543887898984,241273336071673272,-910931915150316895,525338418215769882,
    281773043978235298,-600192047175113787,268603094076633886,-377811009129743437,146241894964569404,829165118562903267,
    -963270973203030492,153590451979154364,-365303714651442146,-220808704114881727,-893189543074139219,580549239117874493,
    340898282895574765,-607275186654402465,503129346250195678,-434009103922115155,-887632729257954508,299008696818110797,-799320791175430567,
    -94755607744322003,-261792258275821816,-732115902240620518,904850316786635088,566593231720473400,237259347861390443,-772737516256558277,
    7779602904219261,-81749737671516269,-197173390283223016,469739955546984275|572325033031663466,858796565699423048,123437038793970800,
    798694308249383628,806077550475980790,932333242204692314,830334454307253693,39180793219366885,-582449584120391611,-125598510826682808,
    -222554278469421849,-24782518322402526,377067835053941027,-562912215413767900,-81723267832826212,-912884657545813928,282229191300292810,
    827771766905531958,527590859810354224,681367704687985070,-120299424278699268,-222370107460300129,-657040519457700569,817088624985560780,
    -967900671698605452,74572607771757295,-977450149834013690,-51144286213921425,-199449624091085480,-940328821628032417,-64513431265012841,
    751821108659951503,-989587542472555132,996128234591120093,864171743525163090,-227917146990197593,289583601782136544,-580079497558350364,
    276849829282559036,392136005536818252,-724754236527259968,-937893955377674071,-887869689176891338,-328944635065173759,170027277560872270,
    -982689049927908073,862931849931790378,884309484021753634,-559877792417697502,-277770868155725419,618093787864707523,918104314947354097,
    -322686004379643408,731063531749291032,-155202859234897480,104143510519907659,-570549317191063325,911536628124956321,-44638554411301988,
    12207804515232052,-14798611444654562,-330961601539199878,-573139469791586741,-641644366632744182,597876163530137057,120150730937778219,
    -829460361453312615,-500095091833488714,-420865209625048133,-130669643444728202,296494414465561836,754196360312088229,696854384379605683,
    321773241981570568,-638209986683034201,-909722055125830507,-149165679579634419,342019185099940013,-571973229445219283,-58868764786088061,
    -720593943917470504,154526452816526331,-874661900642546904,-901483298772974370,-515744832370915253,-20305565982622058,682508205253986777,
    -371610121025311528,-596356051889187557,420817954390156887,-22570464122776352,-876459153067160782,999176695888475080,-24792983003719000,
    -994254149419809447,128447603851019544,451465551613620946,102766238341617257,915658148025580387,-7191215273555500,488860756278719349,-
    142576665385623517,39082524428952347,4544622271630216,292722490251827720,338126762756014026,-944035646714477151,-882862179005310519,
    -595170113206452376,54669884463526994,-632221399709870098,860193768006981111,79893390434688304,942467701446328610,511379543887898984,
    241273336071673272,-910931915150316895,525338418215769882,281773043978235298,-600192047175113787,268603094076633886,-377811009129743437,
    146241894964569404,829165118562903267,-963270973203030492,153590451979154364,-365303714651442146,-220808704114881727,-893189543074139219,
    580549239117874493,340898282895574765,-607275186654402465,503129346250195678,-434009103922115155,-887632729257954508,299008696818110797,
    -799320791175430567,-94755607744322003,-261792258275821816,-732115902240620518,904850316786635088,566593231720473400,237259347861390443,
    -772737516256558277,7779602904219261,-81749737671516269,-197173390283223016,469739955546984275};
    list *l2 = make(149,array);
    display(l2);

}

// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- TEST OPERATIONS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>


void TEST_elements(list* l, LENGTH* nRef, DATA** seqRef){
    DATA *seq = malloc((l->size)*sizeof(DATA));
    LENGTH i = 0;
    list *dl = l;
    node *curr = dl->head;
    while (curr){
        curr = curr->right;
        seq[i] = curr->val;
    }
    *nRef = l->size;
    *seqRef = seq;
}

bool TEST_internal(list* l){
    return true;
}



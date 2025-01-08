# Implementation Details

<details>
<summary>Global</summary>

### Global Implementation

Global types were used to make certain conventions consistent across all the list types.

#### Type: `LENGTH` (`size_t`)
This type is used for values that represent the length of something, whether it be the size of the list, an index, or a counter based on the size/index.

#### Type: `DATA` (`int64_t`)
This type is used for values that represent the data that the list holds.

</details>

<hr>
<hr>

<details>
<summary>Doubly Linked List</summary>

## Doubly Linked List

### Summary
`Doubly Linked List` is one of the most common data structures that offers advantages over the others. It is an extension to `Singly-linked List` but instead of being a one-way traversal, doubly-linked List provides a two-way connection from front to back and vice-versa.

<hr>

<details>
<summary>Structs</summary>

#### Struct: `List`
A **doubly-linked list** being an extension of the singly-linked list inherits the same pointer referencing to `head` but with additional `tail` which is the leftmost `ListNode` and rightmost `ListNode` respectively.
The struct also have a field `size` that stores the size `n` of the list.
Lastly, the struct also have a (`boolean`) field `reversed` that flags whether the `reverse` function is called.
This property allows the implementation to have an $O(1)$ worst-case time complexity for its `reverse` operation.

#### Struct: `ListNode`
This represents the doubly-linked list node for the `List`.
It has `left` pointer that references to the previous `ListNode` and `right` pointer which references to the next adjacent `ListNode`.
Moreover, each `ListNode` has its corresponding `DATA` field `val` that stores the value of the said node.

<hr>
</details>

<details>
<summary>Initializer</summary>

### Operation: `MAKE`

#### Helper: `initList()`
It allocates memory for the `List: l` using `malloc()`.
It initially points `head` and `tail` pointers  to `NULL`. Moreover, it sets `reversed` and `size` to `bool: false` and `LENGTH: 0`  respectively.\
Lastly it returns the initialized `List: l` to the caller.

#### Main: `*make(n, seq) -> list`
This is the main operation of `Make`. It aims to create a working doubly-linked list out of a passed `sequence` with size `n`. 
Returns `List` once passed with arguments and called.

<hr>
</details>

<details>
<summary>Flags/List Info</summary>

### Operation: `reverse(l)`
Reversing a `list` with `n` elements will take time complexity of $O(n)$. Hence, the group found an elegant way of achieving the `reverse` operation at $O(1)$.\
Instead this function will **toggle** the (`boolean`) field in `List` called `reversed`.
It simply flips the `reversed` flag for the other operations to appropriately use, resulting in $O(1)$ worst case time complexity for this operation.

### Operation: `size(l) -> LENGTH`
It simply returns a `LENGTH` which is the current `size: n` of the `list`.

### Operation: `empty(l) -> bool`
It simply returns `bool` that checks whether the `list` has no elements or has `size == 0`.

<hr>
</details>

<details>
<summary>Getters/Setters</summary>

#### Helper: `_get_node(l, i, from_set, v) -> DATA`
This is a helper function that once called, traverses the `List` from left to right until the target index `i` is found, returning the `DATA` of the `ListNode` found. If the (`boolean`) field `from_set` is `true` means that the helper is called by the `set` operation then instead of just return the val at target i, it would also change it's val to `v`. 

### Helper: `_peek(l, from_right) -> DATA`
Helper function that reduces the __code duplication__ between the two (2) `peek_?` operations. 

#### Main: `get(l, i) -> DATA`
it calls the helper `_get_node` and returns the `DATA` of the `ListNode` found at target `i`. 

#### Main: `set(l, i, v)`
it calls the helper `_get_node` and changes the `DATA` of the `ListNode` found at target `i` with `DATA` passed`v`. 

#### Main: `peek_left(l) -> DATA`
This simply returns the leftmost `DATA` of the `ListNode`. If `reversed` is toggled **ON** it would return the rightmost `DATA` instead.

#### Main: `peek_right(l) -> DATA`
This simply returns the rightmost `DATA` of the `ListNode`. If `reversed` is toggled **ON** it would return the leftmost `DATA` instead.

<hr>
</details>

<details>
<summary>Insertions/Deletions</summary>

### Helper: `push_left_base(l, current_node, v)`
This is the "true" `push_left` operation. 
This operation **insert** a new `ListNode` to the `List` by updating the left pointer of the leftmost node to the **new node** and updating it as the new `head` of the list. If reversed, it will update the right pointer of the righmost node and change its new `tail` to the **new node**.

### Helper: `push_right_base(l, current_node, v)`
This is the "true" `push_right` operation.
This operation **insert** a new `ListNode` to the `List` by updating the right pointer of the rightmost node to the **new node** and updating it as the new `tail` of the list. If reversed, it will update the left pointer of the leftmost node and change its new `head` to the **new node**.

### Helper: `pop_left_base(l, current_node)`
This is the "true" `pop_left` operation.
This operation **deletes** the leftmost `ListNode` of the `List` by disconnecting the `ListNode` at index `i = 0` and reconnecting the **next node** left pointer to NULL. It will also update the `head` of the `List` hence making **next node** be the new leftmost `ListNode`. If reversed it will make use of **prev node** instead and update the `tail` of the `List`.

### Helper: `pop_right_base(l) -> bool`
This is the "true" `pop_right` operation.
This operation **deletes** the rightmost `ListNode` of the `List` by disconnecting the `ListNode` at index `i = 0` and reconnecting the **prev node** right pointer to NULL. It will also update the `tail` of the `List` hence making **prev node** be the new leftmost `ListNode`. If reversed it will make use of **prev node** instead and update the `tail` of the `List`.

#### Main: `push_left(l, v)`
Calls `_push_left_base` (or `_push_right_base` if `reversed` flag is enabled.)

#### Main: `push_right(l, v)`
Calls `_push_right_base` (or `_push_left_base` if `reversed` flag is enabled.)

#### Main: `pop_left(l) -> bool`
Calls `_pop_left_base` (or `_pop_right_base` if `reversed` flag is enabled.)


#### Main: `pop_left(l) -> bool`
Calls `_pop_right_base` (or `_pop_left_base` if `reversed` flag is enabled.)

> [!NOTE]
> `node *store = !(l->reversed) ? curr->right : curr->left; `\
> `curr->right` pertains to **next node**\
> `curr->left` pertains to **prev node**

<hr>
</details>


<hr>
</details>

<hr>
<hr>

<details>
<summary>Dynamic Array</summary>

## Dynamic Array

### Summary
The implemented `Dynamic Array` is a variation of the array data structure. Unlike a standard array, which has a fixed size and lacks flexibility, the Dynamic Array can automatically resize itself, providing flexibility optimizing both efficiency and memory usage. This was successfully implemented using a circular indexing system to allow for quicker insertion and remove operations.
<hr>

<details>
<summary>Structs</summary>

#### Struct: `dynamic_array`

The first field is essential for implementing a working array data structure,

`DATA *array` : This field stores elements of the given sequence into an array

The following fields are used to store the first and last elements' indexes of the array. This is important because the array implements a circular indexing system.

`LENGTH start` : This field saves the index of the first element

`LENGTH last` : This field saves the index of the last element

The following fields are used to store anything size-related informations. This is important because the size of the array is not equal to the amount of elements stored inside it.

`LENGTH size` : This field saves the current amount of elements stored in the array

`LENGTH capacity` : This field saves the current capacity of the array

Lastly, the following field was implemented to complete the reverse operation in constant time, $O(1)$.

`bool reverse` : This field tells us whether or not the current list is reversed.

<hr>
</details>

<details>
<summary>Initializer</summary>

### Operation: `MAKE`

#### Main: `*make(n, seq) -> dynamic_array`
This is the main operation of `make`. It creates a dynamic array and returns the said array.

It initalizes all fields of the data structure by creating `array` of size $2\ell$, where $\ell$ is the size of the given sequence. It stores $2\ell$ and $\ell$ into `capacity` and `size` respectively. Moreover, it saves the indexes of the first and last element as `start = 0` and `last = l - 1`. Lastly, since the given sequence is not reversed, it sets the boolean, `reverse = false`, and copies the elements of the given sequence into `array` in the exact same order starting with index $0$.

<hr>
</details>

<details>
<summary>Flags/List Info</summary>

### Operation: `reverse(l)`
Reversing the `array` with `n` elements will take time complexity of $O(n)$. Hence, the group found an elegant way of achieving the `reverse` operation at $O(1)$.

Instead this function will **toggle** the (`boolean`) field in `array` called `reversed`.
It simply flips the `reversed` flag for the other operations to appropriately use, resulting in $O(1)$ worst case time complexity for this operation.

### Operation: `size(l) -> LENGTH`
It simply returns a `LENGTH` which is the current `l->size` of the `list`.

### Operation: `empty(l) -> bool`
It simply returns `bool` that checks whether the `array` has no elements or has `l->size == 0`.

<hr>
</details>

<details>
<summary>Getters/Setters</summary>

### Operation: `get(l, i) -> DATA`
This operation returns the `i`-th element of the array at constant time, $O(1)$. This is the normal time complexity of `get` in a standard array. However, since the group used a circular indexing system, it uses a formula to calculate the `new_i` with respect to the current `l->start` and `l->last` of the array. Moreover, if `l->reverse == true`, it would also find `reverse_index`. This allows the operation `reverse` to run in constant time, $O(1)$.

### Operation: `set(l, i, v)`
Similar to `get`, instead of just returning the element of the given index `i`, it would set a new value `v` to index `i`. That is shown by `l->array[new_i] = v` or `l->array[reverse_index]` if `l->reverse == true`.

### Operation: `peek_left(l) -> DATA`
This simply returns the leftmost element of the `dynamic_array`. If `l->reverse == true`, it will return the rightmost element of the `dynamic_array` instead. The operation utilizes `l->start` and `l->last` to return the leftmost and the rightmost element respectively.

### Operation: `peek_right(l) -> DATA`
This simply returns the rightmost element of the `dynamic_array`. If `l->reverse == true`, it will return the leftmost element of the `dynamic_array` instead. The operation utilizes `l->start` and `l->last` to return the leftmost and the rightmost element respectively.

<hr>
</details>

<details>
<summary>Insertions/Deletions</summary>

### Helper: `_expand_array(l) -> dynamic_array`

This operation doubles the current capacity of the `dynamic_array` if the current array is full. This is achieved by copying `l->array` into a temporary array `temp`. This resets the current indexing system of the array into a zero based indexing system.

After, the current `l->array` is deleted and then recreated, now with double the size using `malloc`. `l->start` and `l->last` are then reset using the current indexing system. `l->capacity` is also doubled. Lastly, `temp` is then copied into the new `l->array`.

### Helper: `_decrease_array(l) -> dynamic_array`

This operation halves the current capacity of the `dynamic_array` if the current array is a quarter of `l->size`. This is achieved by copying `l->array` into a temporary array `temp`. This resets the current indexing system of the array into a zero based indexing system.

After, the current `l->array` is deleted and then recreated, now with half the size using `malloc`. `l->start` and `l->last` are then reset using the current indexing system. `l->capacity` is also halved. Lastly, `temp` is then copied into the new `l->array`.

Lastly, if the size of `l->size` reaches 0, it creates an empty array with `l->capacity = 1` with all the fields of the `dynamic_array` set to default.

### Operation: `pop_left(l) -> bool`
This operation, instead of deleting, just moves `l->start` to the next index which should be the second element of the list. By doing so, `l->start` is incremented by 1 and `l->size` is decremented by 1.

Note that if the array is too sparsed, it will run the operation `_decrease_array(l)`. Moreover, if `l->reversed == true`, it will run the `pop_right(l)` operation instead. 

### Operation: `pop_right(l) -> bool`
This operation, instead of deleting, just moves `l->last` to the index before it which should be the second to the last element of the list. By doing so, `l->last` and `l->elements` are decremented by 1.

Note that if the array is *too sparsed*, it will run the operation `_decrease_array(l)`. Moreover, if `l->reversed == true`, it will run the `pop_left(l)` operation instead. 

### Operation: `push_left(l, v)`
This operation **inserts** a new value `v` to the left side of the list now becoming the first element of the list. Because the array uses a circular indexing system, we are able to `push` an element to the left of an `array` at $O(1)$ by wrapping around the array when `l->start == 0`.

As a result, if `l->start == 0` and the array is not yet *too full*, `l->start = l->capacity - 1`, wrapping around the index of into the other side of the array and then pushing the new value `v`. However, if `l->start != 0`, the index will just decrement by 1.

Note that if the array is *too full*, it will run the operation `_expand_array(l)`. Moreover, if `l->reversed == true`, it will run the `push_right(l, v)` operation instead.

### Operation: `push_right(l, v)`
This operation **inserts** a new value `v` to the right side of the list now becoming the last element of the list. Because the array uses a circular indexing system, we are able to `push` an element to the right of an `array` at $O(1)$ by wrapping around the array when `l->last == l->size - 1`.

As a result, if `l->last == d->size - 1` and the array is not yet *too sparsed*, `l->last = l->capacity - 1`, wrapping around the index of into the other side of the array and then pushing the new value `v`. However, if `l->last != d->size - 1`, the index will just increment by 1.

Note that if the array is *too full*, it will run the operation `_expand_array(l)`. Moreover, if `l->reversed == true`, it will run the `push_left(l, v)` operation instead.

<hr>
</details>


<hr>
</details>


<hr>
<hr>

<details>
<summary>Skip List</summary>

### Summary

`Skip List` is a unique data structure that is a combination of a `List` and a `Linked-list`. With this, it benefits from the advantages of the both data structures. Namely, the `update` operation of a `Linked-List` with time complexity of $O(1)$ and a search operation of a `List` with $O(logn)$ . Its bottommost level or commonly known as **Level 0** is the `Normal Lane`. `Normal Lane` is simply a `Doubly-linked list` that has **all** of the elements in list of `size: n` while the levels above it is the `Express Lane` that contains **only the subset** of elements below it.\
`Express Lanes` offers faster travesal given that it skip nodes per level. Its level promotion is dependent on the probability `p` which in our case is `1/2`, flipping **heads** _promotes_ the current node's level, and flipping **tails** just _maintains_ its current level, hence its max_height. 

<hr>

<details>
<summary>Structs</summary>

#### Struct: `SkipList`
Represents the main overarching list for this ADT.
The **Level 0** of the `SkipList` is just a `Linked-list` where in our use-case is a `Doubly-linked list`. In addition, it contains all elements in the `SkipList` while the succeeding levels above it is the subset of the elements found in **Level 0** varying dependent on the probability `p`.

#### Struct: `SkipNode`
Represents the connected elements in the `SkipList`.
Each `SkipNode` has `left` pointer that references to the previous **existing node**, if there is no node in the left side then it will be connected to the `head_sentinel` instead.
Its `right` pointer points to the `next` **existing node**, if no node then it will be connected to the `tail_sentinel` instead.
It also has `below` pointer that points to the **existing node** beneath it otherwise, it connects to NULL.
Moreover, since the sentinels, `head_sentinel` and `tail_sentinel` are also `SkipNodes` there is an additional (`boolean`) field `is_sentinel` that is set to `false` if it is not pertaining to the sentinels and `true` otherwise.
Lastly, each `SkipNode` have `DATA` field that stores the value of the node, and (`LENGTH`) field `width` that acts as the offset from left to right.

#### Struct: `Levellist`
Represents a list that stores each `levels` present in the `SkipList`.\
To keep track of the **`HEADER`** (where the `SkipList` starts), it has two _(2)_ pointers that points to the `top` and `bottom`.\
`top` level pertains to the topmost level and `bottom` level always pertains to the **Level 0**.

#### Struct: `Level`
It has two _(2)_ pointers `up` and `down` that helps navigates the _succeeding_ and _preceeding_ levels in vertical direction.\
Each levels have `SkipNode` fields `head_sentinel` and `tail_sentinel` representing the leftmost sentinel and rightmost sentinel respectively.\
It also has field `cached_right_width` that stores the width of the right elements given that the _`Indexable Skip-list`_ reads offset of each nodes from left to right.

#### Struct: `LevelRecordsList`
It stores the `head` and `tail` of a `LevelRecord`.

#### Struct: `LevelRecord`
It is a doubly-linked list that keeps track of the level heights of each `SkipNode`.\
It has field `Level` named `topLevel` that stores the highest level achieved by a `SkipNode`. 

<hr>
</details>

<details>
<summary>Initializer</summary>

### Operation: `MAKE`

#### Helper: `_ceil_lg(n) -> LENGTH`
This function makes use of the built-in function `__builtin_clz` that returns the count of the leading zeroes.
`__builtin_clz(0LL)` returns the total number of bits in `LENGTH` and subtracts it to `__builtin_clz(n-1)` which is the number of leading zeroes of the passed number - 1. by subtracting 1 to n it will effectively calculate the ceil. 


#### Helper: `_cap_height(n) -> LENGTH`
The group decides to dynamically resize the `max_height` of the SkipList based on the `size: n`. `_cap_height` will always be called whenever you _insert_ or _delete_ a `SkipNode` from the `SkipList`. It calls the `_ceil_lg` that effectively gets the `log2` of the `n` and `ceils` it up. This operation takes $O(1)$.

> [!NOTE]  
> Recall that the height of a skip list is $$O(\log_2(n))$$.

#### Helper: `_make_node(v) -> SkipNode`
It initializes the newly created `SkipNode`.\
It allocates memory for the **newNode** and initially points its `right`, `left`, and `below` pointers to **_NULL_**.\
Make also sets the (`boolean`) `is_sentinel` default to **false**, while `width` to **0** and `val` to the passed argument `v`.

#### Helper: `_make_level() -> Level`
It initializes a new `Level` once called.\
It allocates memory for the **newLevel** and initially points its `up` and `down` pointers to **_NULL_**.\
This also creates new sentinels for the specific level hence, allocating memory to `head_sentinel` and `tail_sentinel` while initializing its respective pointers.

#### Helper: `_init() -> SkipList`
This initializes the `SkipList` proper.\
It allocates memory for the fields `LevelList` and `LevelRecordsList` while also setting its pointers to **_NULL_**.\
It initially sets the values of `leftmost`, `rightmost`, `curr_height`, and `size` to the `LENGTH: 0`. 

#### Main: `make(n, seq) -> SkipList`
This main operation aims to create a `SkipList` from a `sequence` with `size: n`.\
In order to make the `rand()` work in probability for every run, it first calls `srand(time(NULL))` controlling the choice of seed.\
It iterates over the sequence and makes use of the function `push_right()` to insert the `SkipNodes` to the `SkipList`.

<hr>
</details>

<details>
<summary>Flags/List Info</summary>

### Operation: `reverse(l)`
Reversing a `list` with `n` elements will take time complexity of $O(n)$. Hence, the group found an elegant way of achieving the `reverse`.\
Instead this function will **toggle** the (`boolean`) field in `SkipList` called `reversed`.\
It simply flips the `reversed` flag for the other operations to appropriately use, resulting in $O(1)$ worst case time complexity for this operation.

### Operation: `size(l) -> LENGTH`
It simply returns a `LENGTH` which is the current `size: n` of the `SkipList`.

### Operation: `empty(l) -> bool`
It simply returns `bool` that checks whether the `SkipList` has no elements or has `size == 0`.

<hr>
</details>

<details>
<summary>Getters/Setters</summary>

### Operation: `GET/SET/PEEK_*`

#### Helper: `_get_node(l, target, fromSet, v) -> SkipNode`
This helper aims to search for the specific `SkipNode` at target index `i`.\
It has two _(2)_ loop iteration:

1. **Sentinel Phase**
- This is the first while loop, it traverses the `SkipList` from `**HEADER**` until it gets out and lands on a non-sentinel `SkipNode`.

2. **Main Traversal Phase**
- This is the second while loop, once it gets out of the sentinel node, it will start traversing to the `SkipNodes` until the target `i` is found.

> [WHY?]  
> Q: _"Why do we need the `Sentinel Phase`?"_\
> A: In `Indexable SkipList` we only count the widths of the non-sentinel SkipNodes. Recall that sentinel nodes doesn't represent any meaningful data (can be `INT_MAX`, `INT_MIN`, `-1`, `+-inf`, etc.) and only acts as the "boundary" of a program.

#### Main: `get(l, i) -> DATA`
If $0 \leq i < n$ is not satisfied, then it simply returns $0$.\
It utilizes the `_get_node`, once the `SkipNode` at target `i` is found it would read the `DATA` from it and returns the `val`.

#### Main: `set(l, i, v)`
If $0 \leq i < n$ is not satisfied, then it simply returns $0$.\
It utilizes the `_get_node`, once the `SkipNode` at target `i` is found it would replace the `val` of `SkipNode` with `DATA: v`.

#### Main: `peek_left(l) -> DATA`
This simply returns the leftmost `DATA` of the `SkipNode`. If `reversed` is toggled **ON** it would return the rightmost `DATA` instead.

#### Main: `peek_right(l) -> DATA`
This simply returns the rightmost `DATA` of the `SkipNode`. If `reversed` is toggled **ON** it would return the leftmost `DATA` instead.

<hr>
</details>

<details>
<summary>Insertions/Deletions</summary>

### Operation: `PUSH_*`, `POP_*`

#### Helpers: `_flip_coin() -> bool`
This simulates the flipping of the coin given that it has a probability of `p = 1/2 `. It utilizes `rand()` function, the rand function will get random integer value from the max threshold `RAND_MAX = 2147483647`. the comparison is based off the following:

1. **Heads**
- If `rand() > RAND_MAX / 2` it will return `true`. 
2. **Tails**
- If `rand() <= RAND_MAX / 2` it will return `false`. 

#### Helpers: `_promote_level(l, from_right) -> Level`
Useful helper function that is called whenever inserting a `SkipNode` to a `SkipList`. 

It has a while loop that iterates as long as it satisfies the condition: `l->curr_height < l->max_height && _flip_coin() == true` since, the skipList aims to make a new Level above the current level whenever the flipping of coin lands `heads` otherwise it will just maintain its current level.\
Inside the iteration, it checks first if the current level of the `SkipNode` is the topmost, since you can only `_make_level` whenever your `SkipNode` is at the top. It cannot add between levels. 

Moreover, it has a (`boolean`) parameter **from_right** that is passed as **true** if it was called by the `push_right` and **false** if the argument was from `push_left`. If it's **from_right** then the promotion of newNode will happen on the inserted node at the right. If it's **!(from_right)** then the promotion will happen on the leftmost while also updating its `cached_right_width`. 

#### Helpers: `_demote_level(l, currLevel, from_right)`
Unlike the` _promote_level` it will do the reverse of promoting the level hence, by pruning the level instead.

Once called it starts from **`HEADER`** and traverse from up to down. It has a condition to check whether the current level is empty or not. If it is empty it will change the pointer of the current level and the next level before freeing the current topmost level, to ensure safety of updating the status of `SkipList`.

#### Helpers: `_push_left_base(l, v)`
This is the "true" `push_left` operation, unaffected by the `reversed` flag.\
Before inserting the newNode, it undergoes a series of check to update the variables relating to the `size` and `width` of the `SkipList`
It updates the max_height by calling `_cap_height` given that inserting an element increases the `size: n`.

Before inserting, we must update the `width` of the leftmost node from **level 0** to the topmost. 

It will restart to **level 0**, and allocate memory for the newNode. Recall that at **level 0** it contains all of the elements hence when inserting you always add first at the **level 0** and then progress up. After inserting newNode to the **level 0** it calls `_promote_level` to check whether the newly added node is applicable to increase level or not. After that it will also update the `LevelRecord` to track the height attained by the newNode. 

#### Helpers: `_push_right_base(l, v)`
A mirror of `_push_right_base`.\
This is the "true" `push_right` operation, unaffected by the `reversed` flag.\
Before inserting the newNode, it undergoes a series of checks to update the variables relating to the `size` and `width` of the `SkipList`\
It updates the max_height by calling `_cap_height` given that inserting an element increases the `size: n`.

Before inserting, we must update the `cached_right_width` since we are pushing to the right of `SkipList`.

It will restart to **level 0**, and allocate memory for the newNode. Recall that at **level 0** it contains all of the elements hence when inserting you always add first at the **level 0** and then progress up. After inserting newNode to the **level 0** it calls `_promote_level` to check whether the newly added node is applicable to increase level or not. After that it will also update the `LevelRecord` to track the height attained by the newNode. 


#### Helpers: `_pop_left_base(l) -> bool`
This is the "true" `pop_left` operation, unaffected by the `reversed` flag.\
if the `size == 0` or `SkipList` is `empty` then it would immediately return `false` since we cannot pop an empty list. 

Before removing a `SkipNode` from the `SkipList`, the operation checks first and updates necessary variables including `size`. Since, removing an element decreases the size of the list it also calls the `_cap_height` function to dynamically resize. 

After all the necessary updates, we will now call `_demote_level` to check whether we can prune the level or not.

Finally, we update the `width` for the leftmost node and return `true` if we successfully popped an element.


#### Helpers: `_pop_right_base(l) -> bool`
A mirror of `_pop_right_base`.\
This is the "true" `pop_right` operation, unaffected by the `reversed` flag.\
if the `size == 0` or `SkipList` is `empty` then it would immediately return `false` since we cannot pop an empty list. 

Before removing a `SkipNode` from the `SkipList`, the operation checks first and updates necessary variables including `size`.\
Since, removing an element decreases the size of the list it also calls the `_cap_height` function to dynamically resize. 

After all the necessary updates, we will now call `_demote_level` to check whether we can prune the level or not.

Finally, we update the `cached_right_width` for the rightmost node and return `true` if we successfully popped an element.


#### Main: `push_left(list, v)`
Calls `_push_left_base` (or `_push_right_base` if `reversed` flag is enabled.)

#### Main: `push_right(list, v)`
Calls `_push_right_base` (or `_push_left_base` if `reversed` flag is enabled.)

#### Main: `pop_left(list)`
Calls `_pop_left_base` (or `_pop_right_base` if `reversed` flag is enabled.)

#### Main: `pop_right(list)`
Calls `_pop_right_base` (or `_pop_left_base` if `reversed` flag is enabled.)

<hr>
</details>

<hr>
</details>


<hr>
<hr>

<details>
<summary>Sequence of Trees</summary>

## Sequence of Trees

### Summary

The Perfect Binary Trees are represented by the struct PTree, which is a modified Segment Tree-esque data structure with a special property of having implicit bounds. For this reason, I also like to call it a Phantom Segment Tree (Phantom Index-Segment Tree), or PTree for short, as the bounds are only revealed once the list and trees are traversed with get/set operations.

<hr>

<details>
<summary>Structs</summary>

#### Struct: `PTreeList`
Represents the main overarching list for this ADT.\
It is a doubly-linked-list that holds the Perfect Binary Trees, with the `head` and `tail` pointers to a `PTreeListNode`.\
It records the true length of the list `n` (which is also the total number of leaf nodes across all trees.)\
It has a `reversed` (`boolean`) flag which allows it to have $O(1)$ worst case for its `reverse` operation.\
It also holds the `DATA` values `leftmost` and `rightmost`, for $O(1)$ worst case for its `peek_left` and `peek_right` operation. These can be affected by the `set`, `push_*`, `pop_*` operations.

#### Struct: `PTreeListNode`
Represents a simple doubly-linked-list node for `PTreeList`.\
It has pointers to the previous and next `PTreeListNode`.\
It holds a single value `ptree` (`PTree`).

#### Struct: `PTree`
Represents a Perfect Binary Tree.\
It records its own `k` value (its type), and also the `l` for number of leaf nodes it has. Mathematically, $2^k = l$\
It has a pointer to its root `PTreeNode`.

#### Struct: `PTreeNode`
Represents a node of `PTree`.\
It has the discriminator flag `leaf` (`boolean`).\
With `leaf` it uses `union` to determine whether it holds a `DATA` value and nothing else, or only the `left` and `right` pointers to its children, for saving up memory.

<hr>
</details>


<details>
<summary>Initializer</summary>

### Operation: `MAKE`


#### Helper: `_get_greatest_power_of_two(number) -> k`
This helper function returns the exponent of the greatest power of 2 at most the given number.
Formally, it returns the largest $k$ where $2^k \le \text{number}$.
For example, with $n=22$, its greatest power of two is $2^4=16$, because $2^5=32$ which exceeds it.\
It utilizes bitshifting for quick exponents.

#### Helper: `_construct_ptree_nodes_from_range(sequence, offset, lowerBound, upperBound) -> PTreeNode`
A recursive function that constructs a Perfect Binary Tree from the root, with the leaves accurately representing a subsequence of the given sequence.
Once it reaches the leaves, it gets the appropriate value from the sequence, along with `offset` (if it's in a different part of the sequence).

For example, we have a sequence of $n=12$

$$\{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11\}$$

If we want to get a `PTree` of type $3$ with the leaves from indices $4$ to $11$, then we call the helper function with:

$$\text{lowerBound} = 0$$


$$\text{upperBound} = 7$$

(since the length is $2^3 = 8$). We also specify

$$\text{offset} = 4$$

since the subsequence starts from index $4$.
Then after the recursive calls, the resulting leaves would be

$$\{4, 5, 6, 7, 8, 9, 10, 11\}$$

Then, the caller receives the root node.\
The reason this is by design is that there's no need to have a separate driver code for this recursive function to call the appropriate bounds. Simply always provide $lowerBound = 0$, and then specify $upperBound = 2^k$, and it will immediately start shifting indices and constructing its children without more helper function bloat.

#### Helper: `_construct_ptrees(n, seq) -> head, tail`
The point of this helper is to represent `n` as distinct powers of two, from highest to lowest.\
It achieves this by repetitively using `_get_greatest_power_of_two` and constructing a `PTree` for each remaining subsequence using `_construct_ptree_nodes_from_range`, until it exhausts the entire sequence.

For example, we have a sequence of $n=7$

$${0, 1, 2, 3, 4, 5, 6}$$

We can represent n as the sum of distinct powers of two:

$$n = 2^2 + 2^1 + 2^0 = 4 + 2 + 1$$

So, the sequence of k's would be

$${2, 1, 0}$$

and there would be three PTrees. The leaves of these three PTrees would contain the numbers:

$${0, 1, 2, 3}, {4, 5}, {6}$$

So, it satisfies the increasing and then decreasing sequence of k's requirement.\
After everything, it gives the caller the appropriate doubly-linked-list from `head` to `tail`.

#### Main: `make(n, seq) -> list`
With these helper functions, `make` can finally construct the list.\
It initializes the members `n`, `reversed`, `leftmost`, and `rightmost` appropriately.\
Then, since the datatype of `LENGTH` is unsigned, it only calls `_construct_ptrees` if $n > 0$, and initializes the appropriate `head` and `tail` members.

<hr>
</details>

<details>
<summary>Flags/List Info</summary>


### Operation: `SIZE`
#### Main: `size(list) -> LENGTH`
It simply returns the `n` member of the list, representing its current size.

### Operation: `EMPTY`
#### Main: `empty(list) -> bool`
It returns whether `n` is zero or not.

### Operation: `REVERSE`
#### Main: `reverse(list)`
It simply flips the `reversed` flag for the other operations to appropriately use, resulting in $O(1)$ worst case time complexity for this operation.
<hr>
</details>

<details>
<summary>Getters/Setters</summary>

### Operation: `GET`/`SET`/`PEEK_*`

#### Helpers:
**`_get_leaf_node(list, i, lowerBound, upperBound) -> node`**
**`_get_leaf_node_at_index(list, i) -> node`**
The star of the show -- these helper functions' goal is to return the appropriate leaf node for a given index `i`.\
Because the nodes themselves don't store their index, the function must calculate for the **"phantom"** index. That is, the function has to manually calculate the "offsets" and bounds themselves. In particular, it keeps track for `lowerBound` and `upperBound`.\
The calculations of these offsets and bounds are only done on the needed nodes instead of every single node in every single tree, so search is brought down to $O(logn)$.

`_get_leaf_node_at_index` utilizes the `l` member of the `PTree` to calculate for offsets along the doubly-linked-list (horizontal) as it traverses from left to right.\
Then, `_get_leaf_node` is called for the correct `PTree` and and shifts the current recorded `lowerBound` and `upperBound` as it traverses down the tree (vertical).\
The appropriate bounds are:

$$\text{LeftChild} = [lowerBound, mid]$$


$$\text{RightChild} = [mid+1, upperBound]$$


So we check if the index is in either one, and update the bounds appropriately.\
Note that this will always either result in a leaf node holding the `DATA` value, or `NULL`.\
And voila, we have found the correct leaf.\
With this helper function, we can now make:

#### Main: `get(list, i) -> DATA`
If $0 \leq i < n$ is not satisfied, then it throws an error.\
It utilizes `_get_leaf_node_at_index` to get the appropriate node, and returns its `DATA` value.\
The index is affected by the `reversed` flag to correct for reversal.

#### Main: `set(list, i, v)`
If $0 \leq i < n$ is not satisfied, then it throws an error.\
It utilizes `_get_leaf_node_at_index` to get the appropriate node, and sets its `DATA` value to `v`.\
The index is affected by the `reversed` flag to correct for reversal.\
It also updates `leftmost` or `rightmost`, depending on whether the index rests on $0$ or $n-1$.

#### Main: `peek_left(list) -> DATA`
if $n = 0$, then it throws an error.\
Returns `leftmost`, or `rightmost` if `reversed` flag is enabled.

#### Main: `peek_right(list) -> DATA`
if $n = 0$, then it throws an error.\
Returns `rightmost`, or `leftmost` if `reversed` flag is enabled.

<hr>
</details>

<details>
<summary>Insertions/Deletions</summary>

### Operation: `PUSH_*`, `POP_*`

#### Helper: `_construct_zero_ptree(v) -> tree`
The purpose of this helper function is to construct a `PTree` of type $0$, with the given `DATA` value as its root (leaf)'s value.\
This is useful for pushing a new value into the list.

#### Helpers:
**`_merge_non_distinct_ptrees_to_right(list, start)`**\
**`_merge_non_distinct_ptrees_to_left(list, start)`**

Here we have helper functions for merging non-distinct `PTrees` together, split into two: one for merging to the left, and one for merging to the right.\
It is inspired by binomial heap's merge operation.

It aims to correct the list into satisfying the concatenation of a strictly increasing and strictly decreasing
sequence of $k$'s. After a push/pop operation, there may be some PTrees that are non-distinct (that is, their $k$'s (types) are the same.) This means that we can combine these two trees into one bigger tree, with type $k+1$.

We are given a sequence of PTrees as a doubly-linked-list. We start at some node in this list and proceed to the right (left). If our current PTree's type is the same as the next PTree, then we construct a third PTree containing their two roots as children. (We know that this is of type k+1, and contains l*2 leaf nodes.)\
By assumption, we know that all the PTrees further than the current and next PTree already have distinct $k$'s (distinct types) when this function was called.

By "merging" these two PTrees together, one of two conditions can happen:
- this new larger PTree has a different $k$ from the next one.
- this new larger PTree has the same $k$ as the next one, so we can combine them both into one even bigger PTree.

The function will keep merging to the right (left) until it falls into the first condition, which means it finished correcting the list's sequence of $k$'s, since any PTree further than the two being checked are already distinct by assumption.
(This helper function directly modifies the doubly-linked-list of the main list.)

#### Helpers:
**`_cascade_remove_left(list, start) -> subHead, subTail`**\
**`_cascade_remove_right(list, start) -> subHead, subTail`**

These are helper functions used by the `pop_*` operations.\
They are pretty much the opposite of the merge operations, split into two: one for cascading the right of the `PTree`, and one for cascading the left.\
We know that only the leftmost (rightmost) leaf of the target `PTree` is the one that needs to be removed.
So, its purpose is to "wrap open" the `PTree` back into a sequence of `PTrees`.\
It does this by snipping off the right (left) subtree and making it its own doubly-linked-list node sub-list.\
Then the other leftover left (right) child is deallocated and removed from the list entirely.
Each successive right (left) subtree is of type $$k-1$$ and $$l/2$$.

Note that we assume the given PTree for this helper is of type k > 0, since if the PTree is of type 0, then it can just be immediately removed from the list without any repercussions (it's a single leaf, which is already our target for removal.)\
After everything, it gives the caller the appropriate sublist of right-child (left-child) `PTrees`, with its own `subHead` and `subTail`.

#### Helper: `_peek_a_boo(list)`
A useful helper function for updating the `leftmost`/`rightmost` value after a modification of the list.\
A useful helper function for updating the `leftmost`/`rightmost` value after a modification of the list.\
It is useful for peek_left/peek_right operations.\
It does this by getting the leftmost (rightmost) `PTree` and traversing all the way to the leftmost (rightmost) leaf node, and finally updating the appropriate value.\
It does this for both `leftmost` and `rightmost` in each call.

#### Helper: `_push_left_base(list, v)`
This is the "true" `push_left` operation, unaffected by the `reversed` flag.\
It first calls `_construct_zero_ptree` on the `DATA` value `v`, and pushes it to the head (left) of the doubly-linked-list.\
Then, it calls `_merge_non_distinct_ptrees_to_right` on the new head of the list, to make sure it satisfies the required k-concatenation.\
Finally, it updates `leftmost` to the new value (and `rightmost` if the new `PTree` is also the tail).


#### Helper: `_push_right_base(list, v)`
A mirror of `_push_left_base`.\
This is the "true" `push_right` operation, unaffected by the `reversed` flag.\
It first calls `_construct_zero_ptree` on the `DATA` value `v`, and pushes it to the tail (right) of the doubly-linked-list.\
Then, it calls `_merge_non_distinct_ptrees_to_left` on the new tail of the list, to make sure it satisfies the required k-concatenation.\
Finally, it updates `rightmost` to the new value (and `leftmost` if the new `PTree` is also the head).

#### Helper: `_pop_left_base(list)`
This is the "true" `pop_left` operation, unaffected by the `reversed` flag.\
It first checks if the leftmost `PTree` is of type $0$. If it is, then it simply removes that doubly-linked-list node from the list.\
If not, then it calls `_cascade_remove_left` on the leftmost `PTree`, then pushes the new sublist to the head (left), and then finally calls `_merge_non_distinct_ptrees_to_right` to fix any non-distinct types.\
In both cases, it calls `_peek_a_boo(list)` to update `leftmost` and `rightmost` appropriately.

#### Helper: `_pop_right_base(list)`
A mirror of `_pop_left_base`.\
This is the "true" `pop_right` operation, unaffected by the `reversed` flag.\
It first checks if the rightmost `PTree` is of type $0$. If it is, then it simply removes that doubly-linked-list node from the list.\
If not, then it calls `_cascade_remove_right` on the rightmost `PTree`, then pushes the new sublist to the tail (right), and then finally calls `_merge_non_distinct_ptrees_to_left` to fix any non-distinct types.\
In both cases, it calls `_peek_a_boo(list)` to update `leftmost` and `rightmost` appropriately.


#### Main: `push_left(list, v)`
Calls `_push_left_base` (or `_push_right_base` if `reversed` flag is enabled.)

#### Main: `push_right(list, v)`
Calls `_push_right_base` (or `_push_left_base` if `reversed` flag is enabled.)

#### Main: `pop_left(list)`
Calls `_pop_left_base` (or `_pop_right_base` if `reversed` flag is enabled.)

#### Main: `pop_right(list)`
Calls `_pop_right_base` (or `_pop_left_base` if `reversed` flag is enabled.)

<hr>
</details>

</details>

<hr>
<hr>
<hr>

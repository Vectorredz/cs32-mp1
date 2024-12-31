

# Implementation Details

<details>
<summary>Global</summary>

### Global Implementation

Global types and functions were used to make certain conventions consistent across all the list types.

#### Type: `LENGTH` (`size_t`)
This type is used for values that represent the length of something, whether it be the size of the list, an index, or a counter based on the size/index.

#### Type: `DATA` (`int64_t`)
This type is used for values that represent the data that the list holds.


#### Function: `TEST_elements(list) -> n, seq`
This is used by the Unit Tester to check for correctness against the true raw sequence of `DATA` values of the list (unaffected by reversal flags), WITHOUT using `get` or any other operation.\
For this reason, the implementer must absolutely make sure that it works correctly for any `n` and `seq`, so that there is no confusion on if it's the executed operation that failed, or if it's `TEST_elements` that's the culprit.

#### Function: `TEST_internal(list) -> bool`
This is used by the Unit Tester to check if any internal testing was successful. The implementer of the list can put any test within this function, as long as it returns either `true` or `false` to determine if the test was successful or not.\
For example, the Sequence of Trees implementation tests for the required sequence of k's for each tree to be a concatenation of strictly increasing, and then strictly decreasing types. It will return `false` if it fails to satisfy this test at any point.

</details>

<hr>
<hr>

<details>
<summary>Doubly Linked List</summary>

## Doubly Linked List

### Summary

<hr>

</details>
<summary>Structs</summary>

#### Struct: `List`
A doubly-linked list which is an extension of the singly-linked list with the same pointer referencing to `head` but with additional `tail` which is the leftmost `ListNode` and rightmost `ListNode` respectively.\ 
The struct also have a field `size` that stores the size `n` of the list.\ Lastly, the struct also have a (`boolean`) field `reversed` that flags whether the `reverse` function is called.\ 
This property allows the implementation to have an $O(1)$ worst-case time complexity for its `reverse` operation.\

#### Struct: `ListNode`
This represents the doubly-linked list node for the `List`.\
It has pointer `left` that references to the previous `ListNode` and `right` which references to the next adjacent `ListNode`.\ Moreover, each `ListNode` has its corresponding `DATA` field `val` that stores the value of the said node.\

<hr>
</details>

</details>
<summary>Initializer</summary>

### Operation: `MAKE`

#### Helper: `initList()`
It allocates memory for the `List` `l` using `malloc()`.\ It initially points `head` and `tail` pointers  to `NULL`. Moreover, it sets `reversed` and `size` to `bool` false and `LENGTH` $0$ respectively.\ Lastly it returns the initialized `List` `L` to the caller.

#### Main: `*make(n, seq) -> list`
this is the main operation of `Make`


<hr>
</details>


<details>
<summary>Dynamic Array</summary>
TODO
</details>

<hr>
<hr>

<details>
<summary>Skip List</summary>
TODO

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


#### Helper: `_getGreatestPowerOfTwo(number) -> exponent, k`
This helper function aims to get the greatest power of two as well as its exponent, obtainable for a given number without exceeding it. For example, with $n=22$,
its greatest power of two is $2^4==16$, because $2^5=32$ which exceeds it.\
It utilizes bitshifting for quick exponents.

#### Helper: `_constructPTreeNodesFromRange(sequence, offset, lowerBound, upperBound) -> PTreeNode`
A recursive function that constructs a Perfect Binary Tree from the root, with the leaves accurately representing a subsequence of the given sequence.
Once it reaches the leaves, it gets the appropriate value from the sequence, along with `offset` (if it's in a different part of the sequence).\
For example, we have a sequence of

$$\{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11\}$$

which is of length $12$.
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

#### Helper: `_constructPTrees(n, seq) -> head, tail`
The point of this helper is to represent `n` as distinct powers of two, from highest to lowest.\
It achieves this by repetitively using `_getGreatestPowerOfTwo` and constructing a `PTree` for each remaining subsequence using `_constructPTreeNodesFromRange`, until it exhausts the entire sequence.
After everything, it gives the caller the appropriate doubly-linked-list from `head` to `tail`.

#### Main: `make(n, seq) -> list`
With these helper functions, `make` can finally construct the list.\
It initializes the members `n`, `reversed`, `leftmost`, and `rightmost` appropriately.\
Then, since the datatype of `LENGTH` is unsigned, it only calls `_constructPTrees` if $n > 0$, and initializes the appropriate `head` and `tail` members.

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

#### Helper: `_getLeafNodeAtIndex(i) -> node`
The star of the show -- this helper function's goal is to return the appropriate leaf node for a given index `i`.\
Because the nodes themselves don't store their index, the function must calculate for the **"phantom"** index. That is, the function has to manually calculate the "offsets" and bounds themselves. In particular, it keeps track for `lowerBound` and `upperBound`.\
The calculations of these offsets and bounds are only done on the needed nodes instead of every single node in every single tree, so search is brought down to $O(logn)$.\
It utilizes the `l` member of the `PTree` to calculate for offsets along the doubly-linked-list (horizontal) as it traverses from left to right. Then, it shifts the current recorded `lowerBound` and `upperBound` as it traverses down the tree (vertical).\
The appropriate bounds are:

$$\text{LeftChild} = [lowerBound, mid]$$


$$\text{RightChild} = [mid+1, upperBound]$$


So we check if the index is in either one, and update the bounds appropriately.\
Note that this will always either result in a leaf node holding the `DATA` value, or `NULL`.\
And voila, we have found the correct leaf.\
With this helper function, we can now make:

#### Main: `get(list, i) -> DATA`
If $0 \leq i < n$ is not satisfied, then it simply returns $0$.\
It utilizes `_getLeafNodeAtIndex` to get the appropriate node, and returns its `DATA` value.\
The index is affected by the `reversed` flag to correct for reversal.

#### Main: `set(list, i, v)`
If $0 \leq i < n$ is not satisfied, then it simply returns.\
It utilizes `_getLeafNodeAtIndex` to get the appropriate node, and sets its `DATA` value to `v`.\
The index is affected by the `reversed` flag to correct for reversal.\
It also updates `leftmost` or `rightmost`, depending on whether the index rests on $0$ or $n-1$.

#### Main: `peek_left(list) -> DATA`
Returns `leftmost`, or `rightmost` if `reversed` flag is enabled.

#### Main: `peek_right(list) -> DATA`
Returns `rightmost`, or `leftmost` if `reversed` flag is enabled.

<hr>
</details>

<details>
<summary>Insertions/Deletions</summary>

### Operation: `PUSH_*`, `POP_*`

#### Helper: `_constructZeroPTree(v) -> tree`
The purpose of this helper function is to construct a `PTree` of type $0$, with the given DATA value as its root (leaf)'s value.\
This is useful for pushing a new value into the list.

#### Helpers:
**`_mergeNonDistinctPTreesToRight(list, start)`**\
**`_mergeNonDistinctPTreesToLeft(list, start)`**

Here we have helper functions for merging non-distinct `PTrees` together, split into two: one for merging to the left, and one for merging to the right.\
`PTrees` are non-distinct if their type `k` is the same, which means they can be combined into one greater power of two, improving the running time of the get/set operations, satisfying the concatenation of a strictly increasing and strictly decreasing sequence of $k$'s, and overall preventing the degenerate representation of $n$ trees of type $0$.\
It is inspired by binomial heap's merge operation.
Given a starting doubly-linked-list node, we proceed to the right (left),
and if our current `PTree`'s type is the same as the next `PTree`, then we construct a third `PTree` containing their two roots as children.
We know that this is of type $k+1$, and contains $l*2$ leaf nodes.\
We immediately stop if the next tree's type is distinct from the current tree, as we know that trees further than it are already distinct and satisfy the required k-concatenation.\
This helper function directly modifies the doubly-linked-list of the main list.

#### Helpers:
**`_cascadeRemovalLeft(list, start) -> subHead, subTail`**\
**`_cascadeRemovalRight(list, start) -> subHead, subTail`**

These are helper functions used by the `pop_*` operations.\
They are pretty much the opposite of the merge operations, split into two: one for cascading the right of the `PTree`, and one for cascading the left.\
We know that only the leftmost (rightmost) leaf of the target `PTree` is the one that needs to be removed.
So, its purpose is to "wrap open" the `PTree` back into a sequence of `PTrees`.\
It does this by snipping off the right (left) subtree and making it its own doubly-linked-list node sub-list.\
Then the other leftover left (right) child is deallocated and removed from the list entirely.
Each successive right (left) subtree is of type k-1 and l/2.\
After everything, it gives the caller the appropriate sublist of right-child (left-child) `PTrees`, with its own `subHead` and `subTail`.

#### Helper: `_peekABoo(list)`
A useful helper function for updating the `leftmost`/rightmost value after a modification of the list.\
It is useful for peek_left/peek_right operations.\
It does this by getting the leftmost (rightmost) `PTree` and traversing all the way to the leftmost (rightmost) leaf node, and finally updating the appropriate value.\
It does this for both `leftmost` and `rightmost` in each call.

#### Helper: `_push_left_base(list, v)`
This is the "true" `push_left` operation, unaffected by the `reversed` flag.\
It first calls `_constructZeroPTree` on the `DATA` value `v`, and pushes it to the head (left) of the doubly-linked-list.\
Then, it calls `_mergeNonDistinctPTreesToRight` on the new head of the list, to make sure it satisfies the required k-concatenation.\
Finally, it updates `leftmost` to the new value (and `rightmost` if the new `PTree` is also the tail).


#### Helper: `_push_right_base(list, v)`
A mirror of `_push_left_base`.\
This is the "true" `push_left` operation, unaffected by the `reversed` flag.\
It first calls `_constructZeroPTree` on the `DATA` value `v`, and pushes it to the tail (right) of the doubly-linked-list.\
Then, it calls `_mergeNonDistinctPTreesToLeft` on the new tail of the list, to make sure it satisfies the required k-concatenation.\
Finally, it updates `rightmost` to the new value (and `leftmost` if the new `PTree` is also the head).

#### Helper: `_pop_left_base(list)`
This is the "true" `pop_left` operation, unaffected by the `reversed` flag.\
It first checks if the leftmost `PTree` is of type $0$. If it is, then it simply removes that doubly-linked-list node from the list.\
If not, then it calls `_cascadeRemovalLeft` on the leftmost `PTree`, then pushes the new sublist to the head (left), and then finally calls `_mergeNonDistinctPTreesToRight` to fix any non-distinct types.\
In both cases, it calls `_peekABoo(list)` to update `leftmost` and `rightmost` appropriately.

#### Helper: `_pop_right_base(list)`
A mirror of `_pop_left_base`.\
This is the "true" `pop_right` operation, unaffected by the `reversed` flag.\
It first checks if the rightmost `PTree` is of type $0$. If it is, then it simply removes that doubly-linked-list node from the list.\
If not, then it calls `_cascadeRemovalRight` on the rightmost `PTree`, then pushes the new sublist to the tail (right), and then finally calls `_mergeNonDistinctPTreesToLeft` to fix any non-distinct types.\
In both cases, it calls `_peekABoo(list)` to update `leftmost` and `rightmost` appropriately.


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
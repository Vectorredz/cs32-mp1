

### Doubly Linked Lists
***
#### Function: `make`
**Time Complexity: $O(\mathscr{l})$ worst case**<br>
<b>Proof:</b> Notice the implementation contains a singular for-loop. Now, if we look at the lines outside the for-loop, we can see that the operation involves a series of the following constant-time operations:
- variable assignment
- malloc
- if-else statement
- return statement

Each of the operations involved that were done outside of the for-loop run at constant time, $O(1)$. From this we can derive the recurrence relation of the combination of outer statements, $T_{outer} = c$, where c is the constant representing the total time taken by the combination of operations outside the for-loop.

Now, focusing on the for-loop, we can see that within the for-loop are a combination of the operations aforementioned. Moreover, due to the nature of the for-loop, the operations within the for-loop run for a total of $\mathscr{l}$ times, where $\mathscr{l}$ is the length of the given sequence. For clarity, let's solve for the recurrence relation of the for-loop $T_{loop}$ .

$$T(\mathscr{l}) = T(\mathscr{l}-1) + c $$
$$T(\mathscr{l}-1) = T(\mathscr{l}-2) + c$$$$ \vdots$$
$$T(1) = c$$


If we were to simplify the recurrence further, we would get,

$$T(\mathscr{l}) = T(1) + c \cdot (\mathscr{l} - 1)$$

Since $T(1)$ can equal to $c$, we can substitute and get the following,

$$T(\mathscr{l}) = c + c \cdot \mathscr{l} - c $$
$$ = c \cdot \mathscr{l}$$

By definition of big O notation, we can conclude that the recurrence relation of the loop runs at $T_{loop} = O(\mathscr{l})$ because there exists a constant $C$ and a value $l_0$ such that for all $l\ge l_0$ , the following inequality holds:

$$
c\mathscr{l}\le C\mathscr{l}
$$

That is, 

$$
C\ge c
$$


Finally, combining all the solved recurrence relations, we get, 

$$T(n) = T_{outer}+T_{loop} $$
$$= O(1) + O(\mathscr{l}) $$$$= O(\mathscr{l})$$

Thus proving that `make` runs at $O(\mathscr{l})$ worst case. 

#### Function: `push_left` \& `push_right`
**Time Complexity: $O(1)$ worst case**<br>
<b>Proof:</b> Notice the implementation involve a series of the following constant-time operations:
- variable assignment
- malloc operation
- if-else statement
- return statement
- arithmetic operations

For example, both push operations create a new node and link it either to the head or the tail of the list, saving the new node as the new head or tail of the list. Which side the node is added to depends on the specified push operation and whether the list is reversed or not.

As observed, the operations involved run in constant time $O(1)$. Therefore, we can derive the recurrence relation for the push operations to be $T_{push}=c$, where $c$ is the constant time it takes to run the combination of operations within either of the two functions.

Thus, we have proven that `push_left` and `push_right` runs at $O(1)$ worst case regardless of whether the list is reversed or not, because the operations done remain constant.

#### Function: `pop_left` \& `pop_right`
**Time Complexity: $O(1)$ worst case**<br>
<b>Proof:</b>  Notice the operations involve a series of the following constant-time operations:
- fprintf statement
- malloc operation
- if-else statement
- return statement
- arithmetic operations

For example, both operations free the existing tail or head node and set the node before or after the node as the new tail or head . Which side is popped depends on the specified pop operation and whether the list is reversed or not.

As observed, the operations involved run in constant time, $O(1)$. Therefore, we can derive the recurrence relation for the pop operations as $T_{pop}=c$, where $c$ is the constant time it takes to run the combination of operations within either of the two functions.

Thus, we have proven that `pop_left & pop_right` runs at $O(1)$ worst case regardless of whether the list is reversed or not, because the operations done remain constant.


#### Function: `peek` & `size` \& `empty`
**Time Complexity: $O(1)$ worst case**<br>
<b>Proof:</b>  Notice there is only one operation involved in the `size` operation:
- return statement

The implementation returns a variable of the doubly linked list data structure, `l->size`. Where $l$ is the doubly linked list passed to the `size` operation.

As observed, because the return statement runs at constant time, $O(1)$, and is the sole operation in `size`. By definition, `size` also runs at $O(1)$ worst case. 

Now, `empty` and `peek` is similar to `size` except that `empty` adds an if-else statement dependent on `l->size` and returns either `true` or `false` depending if `l->size == 0` or not; and `peek` returns the leftmost or rightmost that are saved in the data structure. Notice that both return and the if-else statement runs in constant time, $O(1)$. Therefore, `empty` and `peek` also runs at $O(1)$.

Thus, we have proven that `size` and `empty` and `peek` runs at $O(1)$ worst case.

#### Function: `get` \& `set` 

**Time Complexity: $O(n)$ worst case**  
  
**Proof:** Notice the both `get` and `set` each contain only 1 while-loop and a series of constant time operations. Now, if we look at the lines outside the while-loop, we can see that the operation involves a series of the following constant-time operations:
- variable assignment
- fprintf statement
- if-else statement
- return for `get`
- arithmetic operations

Both `get` and `set` are passed a list $l$ and index $i$ while `set` is also given a value $v$ aside from $l$ and $i$. The algorithm traverses through each node of the list one by one until it reaches node `i`. Depending on the specified operation, it will return the value of node $i$ or set a new value, $v$, for node $i$.

We can see that the operations done outside of the while-loop are a series of constant time operations. Thus, we can derive `get`and `set` to have recurrence relations of $T_{outer}=c$.

Now, looking within the while loop, it is observed that if $i$ = $n$, where $n$ is the length of the list, the while loop runs at worst-case $O(n)$. For clarity, we can derive the recurrence relation to be $T_{loop} = T(n+1)+O(1)$, where $O(1)$ represents the constant time operations within the while-loop and $T(n+1)$ represent the time complexity of the number of iterations of the while-loop (Note that if given a size $n$ inclusive, the while-loop runs $n+1$ times where the $n+1$ is the last iteration before exiting). If we were to solve the recurrence relation.

$$T_{loop} = T(n+1) + O(1)$$
$$T(n)= T(n) + O(1)  $$
$$T(n-1)= T(n-1) + O(1)$$
$$\vdots$$
$$T(0)=O(1)=c$$


Simplifying the recurrence relation further, 
$$T_{loop}=c(n+1)+c$$
$$=cn+c+c$$
$$=cn+2c$$


that is, where c is a constant.

By definition of the big O notation, $T_{loop}=O(n)$ because there exists a constant $C$ and a value $n_0$ such that for all $n\ge n_0$ the following inequality holds:

$$
cn+2c\le C\cdot n
$$

That is,

$$\begin{align*}
C\ge\frac{cn+2c}{n}
\end{align*}$$

Now, combining both recurrences, we get:

$$
T(n)=T_{outer}+T_{loop}$$
$$= O(1)+O(n)$$


Which is, by definition of Big O, $O(n)$.
Thus, we have proven that both `get` and `set` runs at $O(n)$ worst case.
#### Function: `reverse`
**Time Complexity: $O(1)$ worst case**<br>
<b>Proof:</b>  Notice there is only one operation involved in the `size` operation:
- negation statement: `reversed = !reversed`

The implementation lies on the boolean `l->reversed`, where $l$ is the list passed to the `reverse` function. The only thing it does is negates the current value of the boolean. This is possible because all the other operations adjust to the current state of `reversed`, that is, it will do the exact opposite if reversed.

As observed, because there is a singular statement, that is the negation. The time complexity of `reverse` lies on the time complexity of the negation. Since the negation is a constant time operation, it runs at $O(1)$. As a result, `reverse` also runs at $O(1)$. 

Thus, we have proven that `reverse` runs at $O(1)$ worst case.


***
### Dynamic Array
***
#### Function: `make`
**Time Complexity: $O(\mathscr{l})$ worst case**<br>
<b>Proof:</b> Notice the implementation contains only 1 for-loop. Now, if we look at the lines outside the for-loop, we can see that the operation involves a series of the following constant-time operations:
- variable assignment
- malloc
- if-else statement
- return statement

Each of the operations involved that were done outside of the for-loop run at constant time, $O(1)$. From this we can derive the recurrence relation of the combination of outer statements, $T_{outer} = c$, where c is the constant representing the total time taken by the combination of operations outside the for-loop.

Now, focusing on the for-loop, we can see that within the for-loop are a combination of the operations aforementioned. Moreover, due to the nature of the for-loop, the operations within the for-loop run for a total of $\mathscr{l}$ times, where $\mathscr{l}$ is the length of the given sequence. For clarity, let's solve for the recurrence relation of the for-loop $T_{loop}$ .

$$T(\mathscr{l}) = T(\mathscr{l}-1) + c $$
$$T(\mathscr{l}-1) = T(\mathscr{l}-2) + c$$$$ \vdots$$
$$T(1) = c$$


If we were to simplify the recurrence further, we would get,

$$T(\mathscr{l}) = T(1) + c \cdot (\mathscr{l} - 1)$$

Since $T(1)$ can equal to $c$, we can substitute and get the following,

$$T(\mathscr{l}) = c + c \cdot \mathscr{l} - c $$
$$ = c \cdot \mathscr{l}$$

By definition of big O notation, we can conclude that the recurrence relation of the loop runs at $T_{loop} = O(\mathscr{l})$ because there exists a constant $C$ and a value $l_0$ such that for all $l\ge l_0$ , the following inequality holds:

$$
c\mathscr{l}\le C\mathscr{l}
$$

That is, 

$$
C\ge c
$$


Finally, combining all the solved recurrence relations, we get, 

$$T(n) = T_{outer}+T_{loop} $$
$$= O(1) + O(\mathscr{l}) $$$$= O(\mathscr{l})$$

Thus proving that `make` runs at $O(\mathscr{l})$ worst case. 


#### Function: `push_left` \& `push_right`
**Time Complexity: $O(1)$ amortized**<br>
<b>Proof:</b> Notice the major part of the implementation involve a series of the following constant-time operations:
- variable assignment
- if-else statement
- arithmetic operations

For example, both push operations append a value to the leftmost or the rightmost, increasing the size of the list by one. Which side the node is added to depends on the specified push operation and whether the list is reversed or not. Moreover, the list used circular indexing to be able to push elements at $O(1)$.

However, it is $O(1)$ *amortized* and not *worst case* because of the nature of the dynamic array. Given that the list increases in size when the list is *too full*, we implemented a helper function called `expand_array` to aid in the expansion of the array which also sets the irregular indexes back to a zero-based numbering. To derive the recurrence relation we get the following.

$$T(n) = T_{push} + T_{expand}$$


As observed, the major part of the implementation's operations involved run in constant time, $O(1)$. Therefore, we can derive the recurrence relation for the push operations to be $T_{push}=c$, where $c$ is the constant time it takes to run the combination of operations within either of the two functions.

Now for $T_{expand}$ ,  notice that the implementation consists of exactly two for-loops and a series of constant time operations. For this, we can derive the recurrence relation to be:

$$
T_{expand} = T_{1} +T_{2}
$$

Where $T_1$ represents the first for-loop and $T_2$ represents the second for-loop. Notice that $T_1$ consists of a series of constant time operations, which run in $O(1)$, to copy the old list into a temporary one. However, because the for-loop iterates through these constant time operations for a series of $n$ times, the time complexity results in $T_1 = O(n)$. Here, $n$ represents the variable `d->elements` that takes into account the current number of elements the list has.

Similarly, $T_2$ also contains a series of constant time operations iterated for a series of $n$ times. We similarly get $T_2 = O(n)$.

From this, we can get the following:


$$
T_{decrease} = T_1 + T_2 
$$
$$O(n) + O(n) = O(2n)$$




Simplifying this further, By definition of the big O notation, $T_{expand}=O(n)$ because there exists a constant $C$ and a value $n_0$ such that for all $n\ge n_0$ the following inequality holds:

$$2n \le C\cdot n$$

That is, 

$$C \ge 2$$


From this, we simplify it to be $T_{expand} = O(n)$. However, it is important to note that $T_{expand}$ only runs when the list is *too full*. This is important to note because over a large number of `push` operations, most run at $T_{push} = O(1)$ and will only run $T_{expand} = O(n)$ when the list is *too full*. Since resizing happens infrequently, `push` runs at $O(1)$ amortized. 

Thus, we have proven that `push_left`  and `push_right` runs at $O(1)$ amortized regardless of whether the list is reversed or not, because the operations done remain constant.

#### Function: `pop_left` \& `pop_right`
**Time Complexity: $O(1)$ amortized**<br>
<b>Proof:</b> Notice the major part of the implementation involve a series of the following constant-time operations:
- variable assignment
- if-else statement
- arithmetic operations

For example, both push operations append a value to the leftmost or the rightmost, increasing the size of the list by one. Which side the node is added to depends on the specified push operation and whether the list is reversed or not. Moreover, the list used circular indexing to be able to push elements at $O(1)$.

However, it is $O(1)$ *amortized* and not *worst case* because of the nature of the dynamic array. Given that the list increases in size when the list is *too little*, we implemented a helper function called `decrease_array` to aid in the expansion of the array which also sets the irregular indexes back to a zero-based numbering. To derive the recurrence relation we get the following.

$$T(n) = T_{push} + T_{decrease}$$


As observed, the major part of the implementation's operations involved run in constant time, $O(1)$. Therefore, we can derive the recurrence relation for the push operations to be $T_{push}=c$, where $c$ is the constant time it takes to run the combination of operations within either of the two functions.

Now for $T_{decrease}$ ,  notice that the implementation consists of exactly two for-loops and a series of constant time operations. For this, we can derive the recurrence relation to be:

$$
T_{decrease} = T_{1} +T_{2}
$$

Where $T_1$ represents the first for-loop and $T_2$ represents the second for-loop. Notice that $T_1$ consists of a series of constant time operations, which run in $O(1)$, to copy the old list into a temporary one. However, because the for-loop iterates through these constant time operations for a series of $n$ times, the time complexity results in $T_1 = O(n)$. Here, $n$ represents the variable `d->elements` that takes into account the current number of elements the list has.

Similarly, $T_2$ also contains a series of constant time operations iterated for a series of $n$ times. We similarly get $T_2 = O(n)$.

From this, we can get the following:

$$
T_{decrease} = T_1 + T_2 
$$
$$O(n) + O(n) = O(2n)$$


Simplifying this further, By definition of the big O notation, $T_{expand}=O(n)$ because there exists a constant $C$ and a value $n_0$ such that for all $n\ge n_0$ the following inequality holds:

$$2n \le C\cdot n$$

That is, 

$$C \ge 2$$


From this, we simplify it to be $T_{decrease} = O(n)$. However, it is important to note that $T_{decrease}$ only runs when the list is *too little*. This is important to note because over a large number of `push` operations, most run at $T_{push} = O(1)$ and will only run $T_{decrease} = O(n)$ when the list is *too little*. Since resizing happens infrequently, `push` runs at $O(1)$ amortized. 

Thus, we have proven that `push_left`  and `push_right` runs at $O(1)$ amortized regardless of whether the list is reversed or not, because the operations done remain constant.

#### Function: `peek`\& `size` \& `empty`
**Time Complexity: $O(1)$ worst case**<br>
<b>Proof:</b>  Notice there is only one operation involved in the `size` operation:
- return statement

The implementation returns a variable of the doubly linked list data structure, `d->elements`. Where $d$ is the dynamic array passed to the `size` operation.

As observed, because the return statement runs at constant time, $O(1)$, and is the sole operation in `size`. By definition, `size` also runs at $O(1)$ worst case. 

Now, `empty` and `peek` is similar to `size` except that `empty` adds an if-else statement dependent on `d->size` and returns either `true` or `false` depending if `d->size == 0` or not; and `peek` returns the leftmost or rightmost value that are saved in the data structure. Notice that both return and the if-else statement runs in constant time, $O(1)$. Therefore, `empty` and `peek` also runs at $O(1)$.

Thus, we have proven that `size` and `empty` and `peek` runs at $O(1)$ worst case.

#### Function: `get` \& `set`
**Time Complexity: $O(1)$ worst case**<br>
<b>Proof:</b>  Notice the operations does not have any loops present and involve a series of the following constant-time operations:
- fprintf statement
- if-else statement
- return statement
- arithmetic operations
- array indexing operations

For example, both operations operations free find the index that is passed to the function relative to the current indexing of the list. That is because the starting index of the list may be a $-4$ or a $5$, so it is important to find the relative index of the list. For the `get` operation, the value at `d->array[relative_index]` is returned. In the case of the `set` operation, a new value $v$ is assigned to the element at the specified index, such that, `d->array[relative_index] = v`, where $v$ is the value passed to the `set` function.

As observed, all the operations involved run in constant time, $O(1)$. Therefore, we can derive the recurrence relation for the pop operations as $T_{get/set}=c$, where $c$ is the constant time it takes to run the combination of operations within either of the two functions.

Thus, we have proven that `get` and `set` runs at $O(1)$ worst case regardless of whether the list is reversed or not, because the operations done remain constant.

#### Function: `reverse`
**Time Complexity: $O(1)$ worst case**<br>
<b>Proof:</b>  Notice there is only one operation involved in the `size` operation:
- negation statement: `reversed = !reversed`

The implementation lies on the boolean `l->reversed`, where $l$ is the list passed to the `reverse` function. The only thing it does is negates the current value of the boolean. This is possible because all the other operations adjust to the current state of `reversed`, that is, it will do the exact opposite if reversed.

As observed, because there is a singular statement, that is the negation. The time complexity of `reverse` lies on the time complexity of the negation. Since the negation is a constant time operation, it runs at $O(1)$. As a result, `reverse` also runs at $O(1)$.

Thus, we have proven that `reverse` runs at $O(1)$ worst case.

***

### Skip List

#### Function: `make`\& `push_left`\& `push_right`
**Time Complexity: $O(\mathscr{l})$ expected**<br>
<b>Proof:</b> Notice the implementation involves two function calls, `_initSkipList` and `push_right`, a for-loop, and a series of the following constant-time operations:
- variable assignment
- malloc
- if-else statement
- return statement

Each of the operations involved that were done within the functions run in constant time, $O(1)$. From this we can derive the recurrence relation of the combination of all the constant time operations, $T_{outer} = c$, where c is the constant representing the total time taken by the combination of operations outside the recursive function.

**T<sub>Loop</sub>: for-loop**. The loop iterates a total of $\mathscr{l}$ times, where $\mathscr{l}$ is the length of the given sequence. That would mean that the loop itself would have a time complexity of $O(\mathscr{l})$ worst case. However, since the cost of each iteration would be $T_{push}$, we can leave $T_{loop}$ to be the following:

$$T_{loop} = O(\mathscr{l}) \cdot T_{push}$$
Where the final time complexity of the loop will depend on the time complexity of $T_{push}$.

**T<sub>init</sub>: \_initSkipList**.  The function contains a series of operations and another function, `_makeLevel`, that run in constant time, $O(1)$. Since`_makeLevel` runs in $O(1)$ because it only contains a series of constant time operations., that would mean `_initSkipList` would also run at $O(1)$. Thus, 

$$T_\text{init} = O(1)$$

**T<sub>push</sub>: push_\***. The`push` operation only calls a helper function`_push_*_base`. This means that the time complexity of `push` is equivalent to the time complexity of `_push_*_base`. Here, `_push_*_base` calls two more functions, `_capHeight` and `_promoteLevel`. 

For `_capHeight`,  it utilizes `ceiling` and `log` functions of `math.h` to calculate the maximum height of the skip list. Since these operations run at $O(1)$, we can expect that the time complexity for `_capHeight`, $T_{capHeight}$ , to be $O(1)$ worst case.

Moreover,  `_promoteLevel` utilizes a while-loop to iterate through the maximum height of the skip list which is calculated at `_capHeight` to be $\lceil \lg n\rceil$, where n is the number of elements in the list. This means the loop will iterate for a maximum of $\lceil \lg n\rceil$ times. Since each iteration of the loop will run in constant time, $O(1)$, that would mean that the time complexity of `_promoteLevel` would be $O(\lg n)$ worst case. 

However, it is important to note that the loops iteration is dependent on the probability that `_flipCoin`, a random number generator that generates either true or false. It will only continue iterating if `_flipCoin` returns true. This means that it would have a $50\%$ chance for the loop to terminate, making the expected number of iterations a constant number, $2$. For clarity, we calculate the number of expected iterations with probability $p$ to be the following:

$$\text{expected iterations} = \frac{1}{p} = \frac{1}{0.5}=2$$

As a result, since the expected iterations of `_flipCoin` to be in constant expected time, $O(1)$, `_promoteLevel` would end up having a time complexity of $O(1)$ *expected*. This is because the iteration relies on probabilistic means for it to continue iterating and the initial pushing action runs in $O(1)$ worst case. Thus,

$$T_{promote}=O(1) \text{ expected}$$

We can use the same logic for the while-loop inside `_push_*_base` as it traverses vertically. According to the calculated iterations, the while-loop is expected to iterate a constant number of times making the time complexity of this while-loop, $T_{while}$ , to be $O(1)$ *expected*.

Combining all these time complexities, we get the following time complexity for the `push` operation:

$$T_{push} =T_{pushBase}= T_{capHeight}+T_{promote}+T_{while}$$
$$=O(1)+O(1)+O(1)$$
$$T_{push} = O(1) \text{ expected}$$

Both push functions use similar algorithms, having almost the same functions and operations. Since there are no extra functions added and it only differs by a few constant time operations, they will have similar time complexities.

Since $T_{push}=O(1)$ expected, $T_{init} = O(1)$ worst case, and $T_{loop} =O(\mathscr{l})$ worst case, we can conclude the following:

$$T_{make} = T_{init} + T_{outer}+T_{loop}\cdot T_{push}$$
$$= O(1) + O(1) + O(\mathscr{l})\cdot O(1)$$
$$T_{make} = O(l) \text{ expected}$$

$T_{loop}$ is multiplied to $T_{push}$ because for every iteration of $T_{loop}$ , `push` is called, thus the cost for each iteration of the loop to be $T_{push}$.

Thus, we have proven that the time complexity of `make` is $O(\mathscr{l})$ expected.

Moreover, `push_left` and `push_right` will have a time complexity of $O(1)$ expected.

#### Function: `pop_left`\& `pop_right`
**Time Complexity: $O(1)$ expected**

**Proof**: The `pop` operation only involves calling a function, `_pop_left_base`. Thus, the time complexity of `pop` is equivalent to the time complexity of `_pop_left_base`. Here, `_pop_left_base` consists of a while-loop and calls two more functions, `_capHeight` and `_demoteLevel`. 

For `_capHeight`,  it utilizes `ceiling` and `log` functions of `math.h` to calculate the maximum height of the skip list. Since these operations run at $O(1)$, we can expect that the time complexity for `_capHeight`, $T_{capHeight}$ , to be $O(1)$ worst case.

Moreover, `_demoteLevel` utilizes a while-loop to iterate through the maximum height of the skip list which is calculated at `_capHeight` to be $\lceil \lg n\rceil$, where n is the number of elements in the list. This means the loop will iterate for a maximum of $\lceil \lg n\rceil$ times. Since each iteration of the loop will run in constant time, $O(1)$, that would mean that the time complexity of `_demoteLevel` would be $O(\lg n)$ worst case. 

However, it is important to note that the loops iteration is dependent on the probability that `_flipCoin`, a random number generator that generates either true or false. It will only continue iterating if `_flipCoin` returns true. This means that it would have a $50\%$ chance for the loop to terminate, making the expected number of iterations a constant number, $2$. For clarity, we calculate the number of expected iterations with probability $p$ to be the following:

$$\text{expected iterations} = \frac{1}{p} = \frac{1}{0.5}=2$$

As a result, since the expected iterations of `_flipCoin` to be a constant, `_demoteLevel` would end up having a time complexity of $O(1)$ *expected*. This is because the iteration relies on probabilistic means for it to continue iterating and the initial pushing action runs in $O(1)$ worst case. Thus,

$$T_{demote} = O(1) \text{ expected}$$

We can use the same logic for the while-loop inside `_pop_*_base` as it traverses vertically. According to the calculated iterations, the while-loop is expected to iterate a constant number of times making the time complexity of this while-loop, $T_{while}$ , to be $O(1)$ *expected*.

Combining all these time complexities, we get the following time complexity for the `push` operation:

$$T_{pop} =T_{popBase}= T_{capHeight}+T_{promote}+T_{while}$$
$$=O(1)+O(1)+O(1)$$
$$T_{pop} = O(1) \text{ expected}$$


Both push functions use similar algorithms, having almost the same functions and operations. Since there are no extra functions added and it only differs by a few constant time operations, they will have similar time complexities.

Thus, we have proven that both `pop` functions have time complexities $O(1)$ expected.

#### Function: `peek` \& `size` \& `empty`
**Time Complexity: $O(1)$ worst case**<br>
<b>Proof:</b>  Notice there is only one operation involved in the `size` operation:
- return statement

The implementation returns a variable of the doubly linked list data structure, `l->size`. Where $sl$ is the skip list passed to the `size` operation.

As observed, because the return statement runs at constant time, $O(1)$, and is the sole operation in `size`. By definition, `size` also runs at $O(1)$ worst case. 

Now, `empty` and `peek` is similar to `size` except that `empty` adds an if-else statement dependent on `l->size` and returns either `true` or `false` depending if `l->size == 0` or not; and `peek` returns the leftmost or rightmost value that are saved in the data structure. Notice that both return and the if-else statement runs in constant time, $O(1)$. Therefore, `empty` and `peek` also runs at $O(1)$.

Thus, we have proven that `size` and `empty` and `peek` runs at $O(1)$ worst case.

#### Function: `get` \& `set`
**Time Complexity: $O(\log n)$ expected**

**Proof**: Aside from constant time operations, the `get` and `set` operations involve calling a function, `_getNode`. Since all the operations outside the function call are constant time operations, by definition of Big O, the time complexity of `get` and `set` are dependent on `_getNode`. 

In `_getNode`, there are constant time operations alongside two while-loops. This would mean that the overall time complexity is dependent on the time complexity of the two while-loops combined. 

For the first while-loop, $T_{L1}$ , the loop iterates through the sentinel nodes, stopping when it encounters a sentinel node connected to a node whose width offset is less than or equal to the desired index. 

The amount of sentinel nodes present are determined by the `_capHeight` function. Since there are $\lceil \lg n \rceil$ sentinel nodes, the while loop would iterate at most $\lceil \lg n \rceil$ times. Thus, $T_{L1}$ would have the following time complexity:

$$T_{L1} = O(\lceil \lg n \rceil)$$

We can simplify the complexity by removing the ceiling function. To remove the ceiling function, we need to find a value that bounds the function from above. Thus, we can propose the following:

$$T_{L1} = O(\lceil \lg n\rceil)=O(\lg n + 1)$$
Here, $\lg n + 1 > \lg n$ allowing us to make it the "simplified" time complexity. Simplifying it further since logarithmic growth is much quicker than constant growth, there exists a constant $c$ and some sufficiently large $n$, such that the constant growth is negligible. Thus, we can remove the $+1$. We can also simplify $\lg n$ to $\log n$ further. Since $\log n$ and $\lg n$ grow asymptotically positive at the same rate, differing by a constant (base), there exists a constant $c$ such that for all sufficiently large $n$, $\lg n \le c \cdot \log n$. Therefore we can simplify it further into the following:

$$T_{L1}=O(\lg n+1) = O(\log n)$$

Now, for the second while-loop, $T_{L2}$ , the loop traverses through the list moving either rightwards or downwards until it finds the desired node. If we were to form a list and become *unlucky* such that no node beyond the lowest level, we would end up with a doubly-linked list. This would cause the loop to iterate an $n$ amount of times worst case, resulting in $T_{L2}=O(n)$ worst case. However, because we are playing into chance, the probability of the worst case happening such that $n$ is large enough to affect the running time is very low. 

We know that the loop depends on the total amount of times you move right or down. Since we have proved that in $T_{L1}$ ,  that the running time of downward movement is worst case $O(\log n)$. Now, all we need to prove rightward movement.

For rightward movement, we know that the number of nodes decreases geometrically by a power of two, $2^k$. We can get the recurrence relation of the number of rightward movements per level to be the following:

$$T(n) =T(n -2^{\lg n-1})+O(1)$$
Where $T(n -2^{\lg n})$ to represent the the amount times a level with the power of two will be formed and $O(1)$ represents the arithmetic work to increment a counter variable (for more information on this recurrence relation and its solution, see [constructPTrees](PROOFS.md#constructptrees)). Simplifying the recurrence relation we get the following:

$$T(n) = T\left(\frac{n}{2}\right)+O(1)$$

Solving the recurrence relation using *Master's Theorem*, we set $a=1$, $b=2$, critical point $p=0$, and $k=0$, where $O(n^k)$.  Since $p=k$, it falls into case 2 of the *Master's Theorem*. We get the following:

$$T(n) = O(n^k+\log_b n)$$
$$T(n)=O(n^0+\log_2n)$$
$$T(n)=O(\log_2n)$$

Simplifying this further by standardizing the logarithmic function using the definition of big O, we get:

$$T(n)=O(\log n)$$

Thus, we have proven that there are at most $O(\log n)$ nodes per level excluding the lowest level. 

Since we know that there are $O(\log n)$ possible nodes for rightward movement, excluding the lowest level's nodes, and because skip lists are constructed probabilistically, we expect the number of rightward movements to be much less than $n$. The chance of encountering nodes in the upper levels reduces the number of required rightward movements. Therefore, we can conclude that the total number of rightward movements is $O(\log n)$ in expectation. Given that each iteration of the loop performs constant work, $O(1)$, we can determine that the time complexity for $T_{L2}$ is:

$$T_{L2}=O(\log n) \text{ expected}$$

Thus, the total time complexity of `_getNode` is the following:

$$T(n)=T_{L1}+T_{L2}$$
$$=O(\log n) +O(\log n) $$

Simplifying this further, since $T_{L2}$ is somewhat of an upper bound of $T_{L1}$ due to its *expected* behavior, by definition of big O, we get the following:

$$T(n) = O(\log n) \text{ expected}$$

Since `_getNode` is $O(\log n)$ expected, that would mean both `get` and `set` would also be $O(\log n)$ expected because the operations outside of `_getNode` run in constant time, $O(1)$. 

Thus, we have proven that both `get` and `set` run in $O(\log n)$ expected.

#### Function: `reverse`
**Time Complexity: $O(1)$ worst case**<br>
<b>Proof:</b>  Notice there is only one operation involved in the `size` operation:
- negation statement: `reversed = !reversed`

The implementation lies on the boolean `l->reversed`, where $l$ is the list passed to the `reverse` function. The only thing it does is negates the current value of the boolean. This is possible because all the other operations adjust to the current state of `reversed`, that is, it will do the exact opposite if reversed.

As observed, because there is a singular statement, that is the negation. The time complexity of `reverse` lies on the time complexity of the negation. Since the negation is a constant time operation, it runs at $O(1)$. As a result, `reverse` also runs at $O(1)$.

Thus, we have proven that `reverse` runs at $O(1)$ worst case.
***
### Sequence of Trees
***
#### Function: `make`
**Time Complexity: $O(\mathscr{l})$ worst case**<br>
<b>Proof:</b> Notice the implementation contains a recursive function, `_constructPTreesFromRange`; and two looping functions, `_getGreatestPowerOfTwo` and `_constructPTrees` . Within each function, we can see that the operations involve a series of the following constant-time operations:
- variable assignment
- malloc
- if-else statement
- return statement

Each of the operations involved that were done within the functions run in constant time, $O(1)$. From this we can derive the recurrence relation of the combination of all the constant time operations, $T_{outer} = c$, where c is the constant representing the total time taken by the combination of operations outside the recursive function.

**T<sub>pow</sub>: \_getGreatestPowerOfTwo**. Analyzing this while-loop function, we can notice that the function uses a special technique called bit-shifting. Notice that each iteration of the loop, $\text{pow}$ exponentially grows in value. As a result, as it iterates through the loop, until $\text{pow} \ge \text{num}$, $pow=2^{k++}$. Thus, the iteration happens a total of $\lfloor\lg \mathscr{l}\rfloor$ times, where $\mathscr{l}$ is $\text{num}$ or the length of the given sequence.

By definition of big O notation, since $\lg \mathscr{l}$ grows at a logarithmic power, $\lg \mathscr{l}$ grows asymptotically positive at the same rate as $\log \mathscr{l}$. As a result, `_getGreatestPowerOfTwo` runs at $T_{pow} = O(\log \mathscr{l})$.

**T<sub>recursion</sub>:  \_constructPTreesFromRange**. Now, focusing on the recursive function, `_constructPTreesFromRange`, we can see that within `_constructPTreesFromRange` are a combination of the constant time operations aforementioned and recursive calls. Similar to a divide and conquer algorithm, the recursive call divides the list into two halves. To analyze the function, lets view it from somewhat of a mathematical induction's perspective.

**Base Case.** When the `upperBound` and the `lowerBound` end up equal to each other, then it would do a series of two constant time operations, `variable assignment` and `array indexing assignment`. As a result, the base case runs at $O(1)$.

**Inductive Step:** When the `upperBound` and the `lowerBound` are not equal to each other, the recursive call splits the list into two halves. This happens until it cannot be split anymore, which is when the `base case` is met. Both sizes of the left and right subtree (of when the list is split) results in size of $n/2$.  After splitting, the function then constructs the parent nodes and does other assignment. Since these are constant time operations, the cost for these are $O(1)$. From this, we can derive the following recurrence relation $T_{recursion}$:

$$T(n)=2T(n/2) + O(1)$$

Where $2T(n/2)$ represent the two initial subtrees created by the function and $O(1)$ represents the cost within the recursive calls as explained above.

Now, solving the recurrence relation using *Master's Theorem*. We let $a=2$, $b=2$, and $f(n) = O(1)$.
Since $f(n) = O(1)$, we let $k=0$. Calculating for the critical value $p$, we get the following:

$$p=\log_{b}{a}=\log_22=1$$

Since $p=1$ and $k=0$, resulting in $k<p$, by Case 1 of the *Master's Theorem*, the time complexity of the recurrence relation results in $T(n)=O(n^p) = O(n^1) = O(n)$. For this case, since $n$ represents the length of the given list, we can rewrite it as $\mathscr{l}$. We get, 

$$T_{recursion} = O(\mathscr{l})$$

**T<sub>main</sub>: \_constructPTrees**. This function is where it all began. Analyzing the function, it is observed that the function creates a series of operations, similar to the aforementioned, that run in constant time, $O(1)$. Aside from this, it contains two function calls, $T_{L_1}$ and $T_{recursion}$. 

Focusing on the while-loop, it runs until $\text{currentStart} > \text{upperBound}$, where $\text{currentStart}$ starts at $0$ and $\text{upperBound}$ starts at $\mathscr{l}-1$. Here, $\text{currentStart}$ increments by a power of two. We can construct a working sequence of the while-loop to be the following for better visualization, where $c$ is $\text{currentStart}$ and $\mathscr{l}$ is the length of the given sequence:

$${c_n}= c+2^{\lg\mathscr{l}}+2^{\mathscr{l}-2^{\lg\mathscr{l}}}+... < \mathscr{l}$$
$$c_n = c+\sum_{i=0}^{\lg{\mathscr{l}}}{2^{\lg(\mathscr{l}-2^i)}}$$

with the assumption that $\lg$ is equivalent to $\lfloor\log_2 \rfloor$. From this, and all the functions aforementioned, we can derive the a working recurrence to describe the time complexity of `_constructPTrees`.

$$T(\mathscr{l}) = T_{main} + O(pow+recursion+outer)$$

This is because, `_constructPTrees` runs a total of $T_{main}$ times. Moreover, for each iteration, it costs $O(pow+recursion+outer)$.

#### constructPTrees
$$T(\mathscr{l})=T(\mathscr{l}-2^{\lg\mathscr{l}}) + O(\lg\mathscr{l}+ \mathscr{l}+1)$$


Note that since  $\lg = \lfloor\log_{2}\rfloor$, we need to drop the floor function somehow. This is important because 

$$\mathscr{l}-2^{\lfloor\lg\mathscr{l}\rfloor}\ge\mathscr{l}-2^{\lg \mathscr{l}}$$

 Thus, making $\mathscr{l}-2^{\lg \mathscr{l}}$ the lower bound for $\mathscr{l}-2^{\lfloor\lg\mathscr{l}\rfloor}$. Since in big O, we are analyzing the upper bound of the time complexity, we can transform the floor function to something simpler. We get the following:

$$\mathscr{l}-2^{\lfloor\lg\mathscr{l}\rfloor} \rightarrow \mathscr{l}-2^{\lg\mathscr{l}-1}$$
This transformation satisfies the fact that $\mathscr{l}-2^{\lfloor\lg\mathscr{l}\rfloor} \le \mathscr{l}-2^{\lg\mathscr{l}-1}$ allowing us to use it instead for simpler calculations. 

Now, simplifying the recurrence further, we get the following:

$$T(\mathscr{l}) = T(\mathscr{l}-2^{\lg\mathscr{l}-1})+O(\lg\mathscr{l}+\mathscr{l}+1)$$
$$T(\mathscr{l}) = T\left(\mathscr{l}-\frac{2^{\lg\mathscr{l}}}{2}\right)+O(\lg\mathscr{l}+\mathscr{l}+1)$$
$$ T(\mathscr{l}) = T\left(\mathscr{l}-\frac{{\mathscr{l}}}{2}\right)+O(\lg\mathscr{l}+\mathscr{l}+1)$$

Thus, we get the following:

$$T(\mathscr{l})=  T(\mathscr{l}) = T\left(\frac{{\mathscr{l}}}{2}\right)+O(\lg\mathscr{l}+\mathscr{l}+1)$$

and by definition of big O, since polynomial grows much quicker than logarithmic and constant growth, at some very large $\mathscr{l}_0$, for all $\mathscr{l}>\mathscr{l}_0$, $\lg{\mathscr{l}}$ and $1$ become negligible. As a result, we can simplify it further to the following:

$$T(\mathscr{l}) = T\left(\frac{{\mathscr{l}}}{2}\right)+O(\mathscr{l})$$

Now, solving the recurrence relation using *Master's Theorem*. We let $a=1$, $b=2$, and $f(\mathscr{l}) = O(\mathscr{l})$.
Since $f(n) = O(\mathscr{l})$, we let $k=1$. Calculating for the critical value $p$, we get the following:

$$p=\log_{b}{a}=\log_21=0$$

Since $p=0$ and $k=1$, resulting in $k>p$, the recurrence relation falls into Case 3 of the *Master's Theorem*. As a result, we need to verify the regularity condition. The regularity condition $c<1$ where:

$$af\left(\frac{\mathscr{l}}{b}\right) \le c\cdot f(\mathscr{l})$$

We get the following:

$$f\left(\frac{\mathscr{l}}{2}\right)\le c\cdot f(\mathscr{l})$$

Now, simplifying $f\left(\frac{\mathscr{l}}{2}\right)$ for comparison and defining $f(n)=O(\mathscr{l})$ as stated above, we get:

$$f\left(\frac{\mathscr{l}}{2}\right)=O\left(\frac{\mathscr{l}}{2}\right)=\frac{1}{2}O(\mathscr{l})$$
$$\frac{1}{2}O(\mathscr{l})\le c\cdot O(\mathscr{l})$$

Here, we can see that $c=\frac{1}{2}$ by comparison and  since $c<1$, it satisfies the regularity condition. By case 3, $T(\mathscr{l})=O(f(\mathscr{l}))$, we get:

$$T(\mathscr{l})=O(\mathscr{l})$$

Since we have shown that `_constructPTrees` runs in $O(\mathscr{l})$, we may now conclude that `make` also runs in $O(\mathscr{l})$. Thus proving that `make` runs at $O(\mathscr{l})$ worst case. 

#### Function: `push_left` \& `push_right`
**Time Complexity: $O(\log{n})$ worst case**<br>
<b>Proof:</b> Notice the implementation calls a function, `_push_*_base`. Within the function, two other functions are called, `_constructZeroPTree` and `_mergeNonDistinctPTrees*` However, if we focus on the lines outside the function calls, we can see that the operation involves a series of the following constant-time operations:
- variable assignment
- if-else statement
- arithmetic operations

Each of the operations involved that were done outside of the for-loop run at constant time, $O(1)$. From this we can derive the recurrence relation of the combination of outer statements, $T_{outer} = c$, where c is the constant representing the total time taken by the combination of operations outside the recursive function.

**T<sub>zero</sub>: \_constructZeroPTree**. Focusing on this function, it is observed to have a series of constant time operations. There are no recursive calls or loops present. Thus, $T_{zero}$ has a time complexity of $O(1)$.

**T<sub>merge</sub>: \_mergeNonDistinctPTrees\***. Focusing on this function, it is observed to have a series of constant time operations along with a while-loop. Because a `PTree` has a height of $\lg n$, where $n$ is the number of nodes in the tree, the loop iterates a total of $\lg n$ times as it traverses each level of the tree. 

The running time of the loop is proportional to the amount of iterations multiplied by the cost of each iteration. Since the loop runs for a total of $\lg n$ iterations and each iteration takes constant time to run, then we can achieve it's time complexity to be $T_{merge}=O(\lg n)$. 

Rewriting the time complexity further in terms of standard logarithms, since $\lg n$ and $\log n$ grow asymptotically positive at the same rate, there exists a constant $c$ such that $\lg n \le c \cdot \log n$. Thus, by definition of big O, we can express: 

$$T_{merge}=O(\lg n) = O(\log n)$$

Now, getting the total time complexity of the function, we have the following:

$$T(n) = T_{outer} + T_{zero} + T_{merge}$$
$$T(n)=O(1)+O(1)+O(\log n)$$

By definition of big O, we can ignore $O(1)$ because logarithmic time grows quicker than constant time for all n and for some sufficiently large $n_0$ where $n\ge n_0$.

Thus proving that `push` runs at $O(\log n)$ worst case. 

#### Function: `pop_left` \& `pop_right`
**Time Complexity: $O(\log{n})$ worst case**<br>
<b>Proof:</b> Notice the implementation calls a function, `_pop_*_base`. Within the function, another function is called, `_peekABoo`. However, if we focus on the lines outside `_peekABoo` and the if-else statement used in`pop`, we can see that the operation involves a series of the following constant-time operations:
- variable assignment
- if-else statement
- arithmetic operations

Each of the operations involved that were done outside of the for-loop run at constant time, $O(1)$. From this we can derive the recurrence relation of the combination of outer statements, $T_{outer} = c$, where c is the constant representing the total time taken by the combination of operations outside the recursive function.

**T<sub>cascade</sub>: \_cascadeRemoval\***. The first loop traverses through each tree node in the sequence. Each tree is of size $2^k$ and that it is constructed by taking the largest power of two first, if possible. This approach is similar to the binary number system, where it prioritizes the largest power of two first, it can be used to represent the amount of trees a decimal number would have.  

Using the concept, since the maximum number of bits that any decimal number would have is $\lfloor \lg n \rfloor + 1$, that means that there are also the same amount of trees given n. Since we are searching for the upper bound, we can put down the floor function, giving us the following:

$$\text{amount of trees} = \lg n +1$$

Therefore, the while loop will iterate at most $\lg n + 1$ times. And since the operations used within the while loop are constant time operations, we get the following time complexity for this function to be $T_{cascade} = O(\lg n +1) = O(\lg n)$.

This can be simplified to $O(\log⁡ n)$, as $\lg n$ and $\log n$ grow asymptotically at the same rate, differing by a constant factor due to the differences in bases. By the definition of big O, we conclude the following:

$$T_{cascade}=O(\lg n) = O(\log n)$$

**T<sub>peek</sub>: \_peekABoo**. Focusing on this function, it is observed to have a series of constant time operations alongside two while-loop. After popping a node, the function would then traverse both the left and right of the tree until the end to set the last node as the new leftmost or rightmost. Because the loop traverses through a tree, it naturally iterates the same amount as the height of the tree. That is, since the height of the tree is $\log n$ (as proven in the `push` function) and the operations within the loop run in constant time, we get the following:

$$T_{peek} = O(2\log n) =O(\log n)$$

The time complexity is $O(2\log n)$ because there are two while-loop in the function. However, this can be simplified to $O(\log⁡ n)$, as $2\log n$ and $\log n$ grow asymptotically at the same rate, differing only by a constant factor. Specifically, there exists a constant $c \ge 2$ such that $2 \log n \le c \cdot \log n$. By the definition of big O, we conclude that $T_{peek}=O(\log ⁡n)$.

Now, combining all of it, we get:

$$T(n)=T_{outer} + T_{cascade} + T_{peek}$$
$$= O(1) + O(\log n) + O(\log n)$$
$$T(n) = O(2\log n + 1)$$

This can be simplified further using the same concepts as the aforementioned into:

$$T(n)=O(\log n)$$

Thus proving that `push` runs at $O(\log n)$ worst case. 

#### Function: `peek`\&`size` \& `empty`
**Time Complexity: $O(1)$ worst case**<br>
<b>Proof:</b>  Notice there is only one operation involved in the `size` operation:
- return statement

The implementation returns a variable of the doubly linked list data structure, `list->n`. Where $d$ is the dynamic array passed to the `size` operation.

As observed, because the return statement runs at constant time, $O(1)$, and is the sole operation in `size`. By definition, `size` also runs at $O(1)$ worst case. 

Now, `empty` and `peek` is similar to `size` except that `empty` adds an if-else statement dependent on `d->size` and returns either `true` or `false` depending if `list->n == 0` or not; and `peek` returns the leftmost or rightmost value. Notice that both return and the if-else statement runs in constant time, $O(1)$. Therefore, `empty` also runs at $O(1)$.

Thus, we have proven that `size` and `empty` runs at $O(1)$ worst case.

#### Function: `get` \& `set`
**Time Complexity: $O(\log{n})$ worst case**<br>
<b>Proof:</b> Notice the implementation calls a function, `_getLeafNodeAtIndex`. Within the function, we can see that the operation involves a series of the following constant-time operations alongside two while-loop:
- variable assignment
- if-else statement
- arithmetic operations
- return statement

Each of the operations involved that were done outside of the for-loop run at constant time, $O(1)$. From this we can derive the recurrence relation of the combination of outer statements, $T_{outer} = c$, where c is the constant representing the total time taken by the combination of operations outside the recursive function.

**T<sub>get</sub>: \_getLeafNodeatIndex**. The first loop traverses through each tree node in the sequence. Each tree is of size $2^k$ and that it is constructed by taking the largest power of two first, if possible. This approach is similar to the binary number system, where it prioritizes the largest power of two first, it can be used to represent the amount of trees a decimal number would have.  

Using the concept, since the maximum number of bits that any decimal number would have is $\lfloor \lg n \rfloor + 1$, that means that there are also the same amount of trees given n. Since we are searching for the upper bound, we can put down the floor function, giving us the following:

$$\text{amount of trees} = \lg n +1$$

Therefore, the while loop will iterate at most $\lg n + 1$ times. And since the operations used within the while loop are constant time operations, we get the following time complexity for this function to be the following:

$$T_{get} = O(\lg n +1) = O(\lg n)$$

This can be simplified to $O(\log⁡ n)$, as $\lg n$ and $\log n$ grow asymptotically at the same rate, differing by a constant factor due to the differences in bases. By the definition of big O, we conclude that 

$$T_{get}=O(\log ⁡n)$$

Focusing on the second loop, it is observed to have a series of constant time operations alongside two while-loop. After popping a node, the function would then traverse both the left and right of the tree until the end to set the last node as the new leftmost or rightmost. Because the loop traverses through a tree, it naturally iterates the same amount as the height of the tree. That is, since the height of the tree is $\log n$ (as proven in the `push` function) and the operations within the loop run in constant time, we get the following:

$$T(n) = O(2\log n) =O(\log n)$$

The time complexity is $O(2\log n)$ because there are two while-loop in the function. However, this can be simplified to $O(\log⁡ n)$, as $2\log n$ and $\log n$ grow asymptotically at the same rate, differing only by a constant factor. Specifically, there exists a constant $c \ge 2$ such that $2 \log n \le c \cdot \log n$. By the definition of big O, we conclude that $T(n)=O(\log ⁡n)$.

Combining the complexities we get:

$$T(n) = O(\log n)  + O(\log n) = O(2\log n)$$
Simplifying this further to be:

$$T(n) = O(\log n)$$

Thus proving that `get` and `set` runs at $O(\log n)$ worst case. 

#### Function: `reverse`
**Time Complexity: $O(1)$ worst case**<br>
<b>Proof:</b>  Notice there is only one operation involved in the `size` operation:
- negation statement: `reversed = !reversed`

The implementation lies on the boolean `list->reversed`, where $l$ is the list passed to the `reverse` function. The only thing it does is negates the current value of the boolean. This is possible because all the other operations adjust to the current state of `reversed`, that is, it will do the exact opposite if reversed.

As observed, because there is a singular statement, that is the negation. The time complexity of `reverse` lies on the time complexity of the negation. Since the negation is a constant time operation, it runs at $O(1)$. As a result, `reverse` also runs at $O(1)$.

Thus, we have proven that `reverse` runs at $O(1)$ worst case.

***

### Memory Complexity
***
#### Doubly Linked List
**Proof**: A doubly linked list has a memory complexity of $O(n)$ worst case because its memory usage grows linearly to the amount of nodes present. All operations aside from `make` take constant memory cost, $O(1)$; `make`, on the other hand, take $O(n)$ memory cost since it does the, `push` operation which is $O(1)$, an $n$ amount of times.

Thus, the doubly linked list has a memory complexity of $O(n)$ worst case.
***
#### Dynamic Array
**Proof**: A dynamic array has a memory complexity of $O(n)$ worst case because its memory usage grows linearly to the size of the array. All operations aside from `make` take constant memory cost, $O(1)$; `make`, on the other hand, take $O(n)$ memory cost since it does the `push` operation, which is $O(1)$, an $n$ amount of times. Moreover, resizing operations only differ by a constant of $2$ or $\frac{1}{4}$ as it resizes the array into either $2\cdot\text{size}$ or $\frac{1}{4} \cdot \text{size}$. 

Thus, the dynamic array has a memory complexity of $O(n)$ worst case.
***
#### Skip List
**Proof**: A skip list has a memory complexity of $O(n \log n)$ worst case. This is assuming each node is present until the highest possible level. However, because the probability of the skip list achieving that behavior is so low, its memory complexity only differs by a constant. This is calculated by the following:

$$\text{expected probability} = \frac{1}{p}= \frac{1}{0.5}=2$$
Where $p$ is the probability that a node would be promoted to a higher level. Because of this, we can simplify the skip list to have the following *expected* memory complexity:

$$S(n)=O(n \log n)=O(2n)=O(n)$$
Since the times of nodes present can be simplified to a constant due to its probabilistic behavior, it now has a memory complexity of $O(n)$ expected. Moreover, if operations were to be done, even if it were to visit all possible nodes, due to its *expected* behavior, all operations would at most have a memory complexity of $O(n)$ expected.

Thus, we have proven that a skip list has a memory complexity of $O(n)$ expected.

***
#### Sequence of Trees
**Proof**: The memory complexity required to store data in the sequence of trees data structure is $O(2^k)$ worst case. This is because each tree in the sequence is a perfect binary tree which is of the form $2^k$. We can visualize it to be the following, where $l$ is the total number of leaf nodes and $q$ is the total number of nodes.

$$\left{l\right}_{k}=2^{k_1}+2^{k_2} + \dots$$
$$\left{q\right}_{k}=(2^{k_1+1}-1)+(2^{k_2+1}-1) + \dots$$

Thus, we can write the total number of nodes, $n$, in terms of total number of leaf nodes, $l$, to have the following memory complexities:

$$\left{q\right}_l=(2l_1-1)+(2l_2-1)+...$$

So, if we were to write the memory complexity in terms of the given size of the sequence, $n$, it would have the following:

$$S(n)=(2l_1-1)+(2l_2-1)+\dots$$
$$=2(l_1+l_2+\dots)+(1+1+\dots)$$
$$S(n)=O(2l)=O(2n)$$


This is because in the sequence of trees, the leaf nodes represent all the elements present in the given sequence. Thus, $l=n$.

Simplifying the memory complexity further since there exists a constant $C\ge2c$ for all $n \ge 0$, by definition of big O, we can simplify it to the following:

$$S(n) = O(n)$$

Note that all operations either take $O(1)$ or $O(n)$ worst case space. This is because if we were to do the other operations that add new trees, it would still be accounted for in the sequences above.

Thus, we have proven that the sequence of trees has a memory complexity of $O(n)$ worst case.
***
<div style="text-align: center;"> End. </div>

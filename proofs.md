

### Doubly Linked Lists

#### Function: `make`
**Time Complexity: $O(\mathscr{l})$ worst case**<br>
<b>Proof:</b> Notice the code contains only 1 for-loop. Now, if we look at the lines outside the for-loop, we can see that the operation involves a series of the following constant-time operations:
- variable assignment
- malloc
- if-else statement
- return

Each of the operations involved that were done outside of the for-loop run at constant time, $O(1)$. From this we can derive the recurrence relation of the combination of outer statements, $T_{outer} = c$, where c is the constant representing the total time taken by the combination of operations outside the for-loop.

Now, focusing on the for-loop, we can see that within the for-loop are a combination of the operations aforementioned. Moreover, due to the nature of the for-loop, the operations within the for-loop run for a total of $\mathscr{l}$ times, where &#8467 is the length of the given sequence. For clarity, let's solve for the recurrence relation of the for-loop $T_{loop}$ .
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
<b>Proof:</b> Notice the operations involve a series of the following constant-time operations:
- variable assignment
- malloc operation
- if-else statement
- return statement
- arithmetic operations

For example, both push operations create a new node and link it either to the head or the tail of the list, saving the new node as the new head or tail of the list. Which side the node is added to depends on the specified push operation and whether the list is reversed or not.

As observed, the operations involved run in constant time $O(1)$. Therefore, we can derive the recurrence relation for the push operations to be $T_{push}=c$, where $c$ is the constant time it takes to run the combination of operations within either of the two functions.

Thus, we have proven that `push_left & push_right` runs at $O(1)$ worst case regardless of whether the list is reversed or not, because the operations done remain constant.

#### Function: `pop_left` \& `pop_right`
**Time Complexity: $O(1)$ worst case**<br>
<b>Proof:</b>  Notice the operations involve a series of the following constant-time operations:
- fprintf statement
- malloc operation
- if-else statement
- return statement
- arithmetic operations

For example, both operations operations free the existing tail or head node and set the node before or after the node as the new tail or head . Which side is popped depends on the specified pop operation and whether the list is reversed or not.

As observed, the operations involved run in constant time, $O(1)$. Therefore, we can derive the recurrence relation for the pop operations as $T_{pop}=c$, where $c$ is the constant time it takes to run the combination of operations within either of the two functions.

Thus, we have proven that `pop_left & pop_right` runs at $O(1)$ worst case regardless of whether the list is reversed or not, because the operations done remain constant.


#### Function: `size`
**Time Complexity: $O(1)$ worst case**<br>
<b>Proof:</b>  Notice there is only one operation involved in the `size` operation:
- return statement

The code returns a variable of the doubly linked list data structure, `l->size`. Where $l$ is the doubly linked list passed to the `size` operation.

As observed, because the return statement runs at constant time, $O(1)$, and is the sole operation in `size`. By definition, `size` also runs at $O(1)$ worst case. 

Thus, we have proven that `size` runs at $O(1)$ worst case.

#### Function: `get`\& `set` 

**Time Complexity: $O(n)$ worst case**  
  
**Proof:** Notice the both `get` and `set` each contain only 1 while-loop and a series of constant time operations. Now, if we look at the lines outside the while-loop, we can see that the operation involves a series of the following constant-time operations:
- variable assignment
- fprintf statement
- if-else statement
- return for `get`
- arithmetic operations

Both `get` and `set` are passed a list $l$ and index $i$ while `set` is also given a value $v$ aside from $l$ and $i$. The algorithm traverses through each node of the list one by one until it reaches node `i`. Depending on the specified operation, it will return the value of node $i$ or set a new value, $v$, for node $i$.

We can see that the operations done outside of the while-loop are a series of constant time operations. Thus, we can derive `get`and `set` to have recurrence relations of $T_{outer}=c$.

Now, looking within the while loop, it is observed that if $i$ = $n$, where$n$ is the length of the list, the while loop runs at worst-case $O(n)$. For clarity, we can derive the recurrence relation to be $T_{loop} = T(n+1)+O(1)$, where $O(1)$ represents the constant time operations within the while-loop and $T(n+1)$ represent the time complexity of the number of iterations of the while-loop (Note that if given a size $n$ inclusive, the while-loop runs $n+1$ times where the $n+1$ is the last iteration before exiting). If we were to solve the recurrence relation.

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

The code lies on the boolean `l->reversed`, where $l$ is the list passed to the `reverse` function. The only thing it does is negates the current value of the boolean.

As observed, because there is a singular statement, that is the negation. The time complexity of `reverse` lies on the time complexity of the negation. Since the negation is a constant time operation, it runs at $O(1)$. As a result, `reverse` also runs at $O(1)$.

Thus, we have proven that `reverse` runs at $O(1)$ worst case.

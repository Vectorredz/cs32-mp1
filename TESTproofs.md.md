### Doubly Linked Lists

#### Function: `make`
**Time Complexity: $O(\mathscr{l})$ worst case**<br>
**Proof:** Notice the code contains only 1 for-loop. Now, if we look at the lines outside the for-loop, we can see that the operation involves a series of the following constant-time operations:
- variable assignment
- malloc
- if-else statement
- return

Each of the operations involved that were done outside of the for-loop run at constant time, $O(1)$. From this we can derive the recurrence relation of the combination of outer statements, $T_{\text{outer}} = c$, where $c$ is the constant representing the total time taken by the combination of operations outside the for-loop.

Now, focusing on the for-loop, we can see that within the for-loop are a combination of the operations aforementioned. Moreover, due to the nature of the for-loop, the operations within the for-loop run for a total of $\mathscr{l}$ times, where $\mathscr{l}$ is the length of the given sequence. For clarity, let's solve for the recurrence relation of the for-loop $T_{\text{loop}}$.

$$T(\mathscr{l}) = T(\mathscr{l}-1) + c  $$
$$T(\mathscr{l}-1) = T(\mathscr{l}-2) + c$$$$\vdots$$
$$T(1) = c$$

If we simplify the recurrence further, we would get:

$$T(\mathscr{l}) = T(1) + c \cdot (\mathscr{l} - 1)$$

Since $T(1)$ can equal $c$, we can substitute and get the following:

$$T(\mathscr{l}) = c + c \cdot \mathscr{l} - c \\ = c \cdot \mathscr{l}
$$

By definition of big O notation, we can conclude that the recurrence relation of the loop runs at $T_{\text{loop}} = O(\mathscr{l})$ because there exists a constant $C$ and a value $\mathscr{l}_0$ such that for all $\mathscr{l} \geq \mathscr{l}_0$, the following inequality holds:

$$\mathscr{l} \leq C\mathscr{l}$$
That is, 

$$
C \geq c
$$

Finally, combining all the solved recurrence relations, we get:

$$
\begin{aligned}
T(n) &= T_{\text{outer}} + T_{\text{loop}} \\ 
&= O(1) + O(\mathscr{l}) \\ 
&= O(\mathscr{l})
\end{aligned}
$$
Thus proving that `make` runs at $O(\mathscr{l})$ worst case. 

#### Function: `push_left` & `push_right`
**Time Complexity: $O(1)$ worst case**<br>
**Proof:** Notice the operations involve a series of the following constant-time operations:
- variable assignment
- malloc operation
- if-else statement
- return statement
- arithmetic operations

For example, both push operations create a new node and link it either to the head or the tail of the list, saving the new node as the new head or tail of the list. Which side the node is added to depends on the specified push operation and whether the list is reversed or not.

As observed, the operations involved run in constant time $O(1)$. Therefore, we can derive the recurrence relation for the push operations to be $T_{\text{push}}=c$, where $c$ is the constant time it takes to run the combination of operations within either of the two functions.

Thus, we have proven that `push_left & push_right` runs at $O(1)$ worst case regardless of whether the list is reversed or not, because the operations done remain constant.

...

#### Function: `reverse`
**Time Complexity: $O(1)$ worst case**<br>
**Proof:**  Notice there is only one operation involved in the `reverse` operation:
- negation statement: `reversed = !reversed`

The code lies on the boolean `l->reversed`, where $l$ is the list passed to the `reverse` function. The only thing it does is negate the current value of the boolean.

As observed, because there is a singular statement, that is the negation, the time complexity of `reverse` lies on the time complexity of the negation. Since the negation is a constant time operation, it runs at $O(1)$. As a result, `reverse` also runs at $O(1)$.

Thus, we have proven that `reverse` runs at $O(1)$ worst case.

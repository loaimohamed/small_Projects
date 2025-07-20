The C code snippet you provided `snakeEntity` is intended to update the positions of a snake's body segments and mark them in a `list` (presumably a game board or grid).

-----

### Analysis of the Code's Logic

Let's break down why you might be seeing a reduction in displayed points when the snake moves, based on the provided code:

1.  **`se->points[0] = se->head;`**: This line correctly sets the first point in the `points` array to the snake's current `head` position. This is the new position of the snake's head.

2.  **`Point temp = se->head;`**: A temporary `Point` variable `temp` is initialized with the snake's head position. This `temp` variable is crucial for the "shifting" logic.

3.  **The `for` loop (`for (int i = 0; i < se->length; i++)`)**: This loop iterates through the snake's `points` array, which should represent its body segments.

      * **`Point current = se->points[i];`**: Inside the loop, `current` stores the original position of the *i*-th segment *before* it's updated.

      * **`list[getPos(current)] = 1;`**: This line marks the `current` (original) position of the segment in your `list` as occupied (by setting it to 1). This seems correct for marking the snake's presence on the board.

      * **`se->points[i] = temp;`**: This is the key line where the update happens. The *i*-th segment's new position is set to the value of `temp`.

      * **`temp = current;`**: Immediately after updating `se->points[i]`, `temp` is updated to the `current` (original) position of the segment that was *just* processed.

### Why Points Might Be Reducing (The Issue)

The problem lies in how `temp` is updated and used within the loop. Let's trace it for a snake of `length = 3` with initial points `A, B, C` and a new head `H`:

  * **Initial State**:

      * `se->head` = `H`
      * `se->points[0]` = `A`
      * `se->points[1]` = `B`
      * `se->points[2]` = `C`

  * **Before Loop**:

      * `se->points[0]` becomes `H` (from the line `se->points[0] = se->head;`)
      * `temp` = `H`

  * **Loop Iteration `i = 0`**:

      * `current` = `se->points[0]` (which is `H` from the pre-loop assignment)
      * `list[getPos(H)] = 1;` (Marks the new head position)
      * `se->points[0] = temp;` (This is `se->points[0] = H;` - essentially `H = H;` which does nothing useful here, as `se->points[0]` is already `H`)
      * `temp = current;` (This is `temp = H;`)

  * **Loop Iteration `i = 1`**:

      * `current` = `se->points[1]` (which is `B`)
      * `list[getPos(B)] = 1;` (Marks position `B`)
      * `se->points[1] = temp;` (This is `se->points[1] = H;`) - **Now `se->points[1]` is also `H`\!**
      * `temp = current;` (This is `temp = B;`)

  * **Loop Iteration `i = 2`**:

      * `current` = `se->points[2]` (which is `C`)
      * `list[getPos(C)] = 1;` (Marks position `C`)
      * `se->points[2] = temp;` (This is `se->points[2] = B;`)
      * `temp = current;` (This is `temp = C;`)

**Result after the loop**: `se->points` would be `[H, H, B]`. You've lost `C` and `B` has become `H`. The snake is effectively shrinking to a head and potentially some duplicated segments. The `list` will have the old positions marked, but the `se->points` array, which defines the snake's current state, is being incorrectly updated.

The fundamental issue is that `se->points[0]` is correctly set *outside* the loop, but then the loop's logic tries to "shift" points starting from `se->points[0]` itself using `temp`. This results in the new head position propagating to the first segment, and the subsequent segments not correctly inheriting the positions of the segments *ahead* of them.

-----

### How to Fix It

The standard way to update a snake's body is to shift each segment to the position of the segment directly in front of it, starting from the tail and moving towards the head. The head's new position is determined by the player's input or game logic.

Here's the corrected logic:

```c
void snakeEntity(int *list, SnakeProp *se) {
    // 1. Clear the old snake positions from the list (if not handled elsewhere)
    //    It's crucial to remove the snake's old trail from the board.
    //    If getPos() maps to an index in a 1D array representing the board,
    //    you need to clear the old positions before drawing new ones.
    //    A simple way is to iterate through the *old* points and set list[getPos(old_point)] = 0;
    //    However, often the clearing of the old tail happens as part of the
    //    game loop, where the last point is removed when the snake moves.
    //    For now, let's focus on updating se->points correctly.

    // Store the last point of the tail before it's overwritten, if you need to clear it.
    // Point oldTail = se->points[se->length - 1]; // Only if you manage tail clearing here

    // 2. Shift all body segments (from tail to head)
    //    Each segment takes the position of the segment in front of it.
    for (int i = se->length - 1; i > 0; i--) {
        se->points[i] = se->points[i - 1];
    }

    // 3. Update the head's position
    se->points[0] = se->head; // se->head should already contain the new desired head position

    // 4. Mark all current snake positions on the list
    for (int i = 0; i < se->length; i++) {
        list[getPos(se->points[i])] = 1;
    }
    // If you need to clear the old tail from the board:
    // list[getPos(oldTail)] = 0; // Only if you store oldTail above and it's no longer part of the snake.
}
```

**Explanation of the corrected logic:**

1.  **Shift Body (Backward Loop):** The loop `for (int i = se->length - 1; i > 0; i--)` starts from the *tail* of the snake (`se->length - 1`) and moves towards the head (stopping at index `1`). In each iteration, `se->points[i]` (the current segment) is assigned the position of `se->points[i - 1]` (the segment in front of it). This correctly propagates the positions down the snake's body.
2.  **Update Head:** After all body segments have shifted, `se->points[0]` (the head) is updated to the `se->head` position, which is assumed to be calculated elsewhere based on movement direction.
3.  **Mark on List:** Finally, iterate through the *updated* `se->points` array to mark all current snake positions on your `list`.

Make sure that `se->head` is updated **before** calling `snakeEntity` to reflect the new desired position of the snake's head based on user input or game mechanics.

-----

If you'd like to share more of your snake game's structure, especially how `se->head` is updated and how the `list` (game board) is managed, I can provide more targeted advice\!
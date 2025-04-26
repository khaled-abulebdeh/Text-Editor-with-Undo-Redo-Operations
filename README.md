
# Text Editor with Undo/Redo Operations (Stack & Queue Implementation)

## Overview

This project implements a simple **Text Editor** with **Undo** and **Redo** functionality using **Stacks** and **Queues** in C.  
The user can load text from a file, insert new strings, delete strings, undo/redo changes, and finally save the updated text to a new output file.

The project uses:
- **Undo Stack**: Tracks insertions and deletions.
- **Redo Stack**: Tracks undone actions for redo functionality.
- **Add Queue**: Temporarily stores words during insertions.

---

## Files

| File | Description |
|:-----|:------------|
| `main.c` | Main C source code implementing the text editor with undo/redo features. |
| `inputFile.txt` | Input file containing the initial text to load into memory. |
| `outputFile.txt` | Output file generated after saving the modified text (created at runtime). |
| `Project_Description.pdf` | Project assignment description and requirements document. |

---

## Features

- Load initial text from a file.
- Insert strings into the text.
- Remove strings from the text.
- Undo previous operations (insertion/removal).
- Redo previously undone operations.
- Print the current content of Undo and Redo stacks.
- Save the updated text into an output file.
- Exit the application gracefully.

---


## Requirements

- C Language (Standard C)
- Basic understanding of Stacks and Queues
- `inputFile.txt` must be available in the same directory for initial loading.



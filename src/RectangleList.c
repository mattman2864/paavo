#pragma once

#include "raylib.h"

typedef struct {
    Rectangle *items;
    int count;
    int capacity;
} RectangleList;

void InitRectangleList(RectangleList *list, int initialCapacity) {
    list->count = 0;
    list->capacity = initialCapacity;
    list->items = (Rectangle *)malloc(list->capacity * sizeof(Rectangle));
    if (list->items == NULL) {
        // Handle memory allocation failure
        exit(1);
    }
}

// Adds a new rectangle to the list.
void AddRectangle(RectangleList *list, Rectangle rect) {
    // If the list is full, double its capacity.
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->items = (Rectangle *)realloc(list->items, list->capacity * sizeof(Rectangle));
        if (list->items == NULL) {
            // Handle memory allocation failure
            exit(1);
        }
    }
    list->items[list->count] = rect;
    list->count++;
}

// Removes a rectangle at a specific index.
void RemoveRectangle(RectangleList *list, int index) {
    if (index < 0 || index >= list->count) {
        return; // Index is out of bounds, do nothing.
    }
    // Shift elements to the left to fill the gap.
    for (int i = index; i < list->count - 1; i++) {
        list->items[i] = list->items[i + 1];
    }
    list->count--;
}

// Frees the memory used by the list.
void ClearRectangleList(RectangleList *list) {
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

void DrawAllRectangles(RectangleList *list) {
    // You can iterate through the list and draw each rectangle.
    for (int i = 0; i < list->count; i++) {
        DrawRectangleRec(list->items[i], DARKGREEN);
    }
}

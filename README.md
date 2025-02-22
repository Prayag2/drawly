# Plan
Started: `2025-01-02 04:40PM`  
Development is divided into phases.  
The project will eventually be open sourced. However, I will work on the first few phases myself, to maximize learning.  
The following is a list of features I'll be planning to add to it:  

## Phase 1 (Basic Features)
- [x] A simple fixed size canvas to draw on using a black coloured stroke.
- [x] Different shapes like rectangle, ellipse, arrow, line and stroke.
- [x] An eraser to erase the strokes (deleting the strokes).
- [x] Testing.

## Phase 2
- [x] Refactor to try to adhere to SOLID principles and utilize useful design patterns
- [x] A custom Qt layout for toolbar and properties bar
- [x] Make canvas infinite and add ability to move the viewport
- [ ] Use caching techniques to optimize moving the canvas around
- [ ] A properties bar to change the following properties:
    - [ ] Colour of strokes
    - [ ] Stroke width
    - [ ] Eraser size
    - [ ] Stroke style (dashed, solid, etc)

## After Phase 2
- [ ] Selection tool
- [ ] Ability to delete selected items
- [ ] Ability to move (translate) selected items
- [ ] Ability to transform selected items
- [ ] Ability to rotate selected items
- [ ] Better freeform smoothing algorithms
- [ ] Undo/redo operations
- [ ] Pressure senstivity for drawing tablets
- [ ] Shortcut keys (including pen buttons)
- [ ] Allow snapping
- [ ] Image support
- [ ] Text support
- [ ] Text formatting like bold, underline, italics, etc.
- [ ] UI overhaul
- [ ] Filling shapes with a background colour

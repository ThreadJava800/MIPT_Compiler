# Issues
Here will be placed solutions, which I don't like in my code, in order to reimplement them in the (hopefully) nearest future.
- Main & print functions are now separate tokens and nodes. Consider merging them with the general function node.
- DEFINE_FRIENDS macro in ast nodes structures doesn't look nice.
- Implement better logging (tracing should be added).
- Functions has map does not look nice in ProgramNode_t structure. It could be moved to Driver_t for convenience, but it contradicts its purpose.
- Refactor code to fit the same code-style.
- I don't like templated ScopeTreeBuilder in my code: didn't have enough time to think more here. Hopefully, will fix it in the future.

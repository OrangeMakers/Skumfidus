# TODO: Display Changes

1. Update `Settings` class to use `MatrixDisplay` instead of `OMDisplay`:
   - Replace `OMDisplay` with `MatrixDisplay` in the constructor and member variables.
   - Update all display update calls to use `updateDisplay` method.

2. Remove `OMDisplay` class and its files from the project.

3. Update any other classes or files that might be using `OMDisplay`:
   - Search for any remaining `OMDisplay` includes or usages.
   - Replace them with `MatrixDisplay` and update the method calls accordingly.

4. Review and update any timing-related code:
   - The new `MatrixDisplay` doesn't have built-in duration for messages.
   - Implement any necessary timing logic in the main loop or relevant functions.

5. Update documentation:
   - Update README or other documentation to reflect the change from `OMDisplay` to `MatrixDisplay`.
   - Document the new display update mechanism and its thread-safe nature.

6. Testing:
   - Thoroughly test all display-related functionality to ensure it works correctly with the new `MatrixDisplay`.
   - Pay special attention to areas where message duration was previously used.

7. Performance review:
   - Monitor the performance of the new display update mechanism.
   - Adjust the update interval in the `MatrixDisplay` thread if necessary.

8. Code cleanup:
   - Remove any unused variables or functions related to the old display system.
   - Ensure consistent naming and coding style with the new display implementation.

9. Update any build scripts or project configuration files if necessary to reflect the removal of `OMDisplay` and addition of `MatrixDisplay`.

10. Consider implementing a simple message queue in the main loop if temporary messages are needed, as the new `MatrixDisplay` doesn't handle this internally.

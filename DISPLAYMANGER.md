I want to implement a state machine to handle the display, because its not handled efficient now, please make a generic state machine that handles all display actions it should be as described below

- Be non blocking, if a delay is neede use millis never use delay
- Keep track of what is on the display in a global variable, so it never has to clear the display
- Make a dedicated function to update the display
  - The function should keep track on the current shown value on the display and calculate what to update, it should take the row / column and lenght to know where to update the value
  - It should have a function to align left or right in dedicated functions that is called with arguments FunctionName(<Message>, <row>, <start column>, <optional end column>) if no endcolumn is specified it should just calulate that from the string, also use datatypes for row and colums that is between 1-16, easier to manage when writing code, an example could be WriteDisplayRight("Text",2,10,16) that should result in writing a string a string in row 2 which starts at position 10 and ends at 16 and it should "   Text", but the white space should only be written if the old text did not have white space, Align Left should just be WriteDisplay so thats default.
- Make a function to write a alert on the display, it should take 3 options (<row1 text>, <row2 text>, <display for miliseconds>) 

Please check if this is a good way of doing this and if you can suggest a better way of handling the display please do so
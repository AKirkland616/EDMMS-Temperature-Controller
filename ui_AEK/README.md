# Assignment Six: Stack Store

### Objective

For this assignment you are to implement a simple stack-based store on the MSP that can respond to the following commands through `minicom`/`screen`:

1. `push <name> <quantity> <price>`
1. `pop`
1. `sell`
1. `show`

I am providing you with a shell code file _and a header file_ with the appropriate struct definitions and global variables. Do not change this file unless you have a good reason.

Along with the above commands, pressing the onboard switch should swap the top two items of the stack and _preserve the command line_. I have made some changes to the shell loop that should help with this.

Also, you should use the onboard green and red LED to show the status of the stack:

1. if the stack is empty both lights are *OFF*
1. if the stack contains items, _but is not full_ the green light should be *ON*
1. if the stack is full, the only the red light should be *ON*

Make sure to check conditions of the stack when a command is called and return a descriptive error if the action cannot be completed.


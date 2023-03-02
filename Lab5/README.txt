2) Row: 1, Column: 3, how many times it polled key: 58, how many loops have been done: 228
3) Row: 1, Column: 3, how many times interrupted: 36, how many loops done: 7037

4) First checks if a column has been pressed, then it will check what specific column (C0,C1,C2) was pressed. If there was a specific column pressed, then it will check what row was pressed, by checking the output of every single one individually. The same is done on the columns

5)  The key difference between Polling and interrupts is that the interrupt waits for a interrupt flag, while polling just checks the key constantly. Even though in our key_pressed_runs counter was much larger than the polling, this is mainly because Polling has a 15ms delay while interrupt does not. 

6) Interrupts are a better way for user input, since it is not needed to check constantly if we have been given user input. Polling is a waste of resources.
7)
- Debouncing can vary over different buttons, also having interrupts from a person can lead to corruption to conditionals.
- Using interrupts to put to sleep and waking up is possible with an interrupt.

- Interrupts should be used to sleep and wake up, since having something like a poll would use resources when the device needs to sleep. So interrupts could have the ability to let it sleep
and wake it up when needed.

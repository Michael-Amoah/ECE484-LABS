4) First checks if a column has been pressed, then it will check what specific column (C0,C1,C2) was pressed. If there was a specific column pressed, then it will check what row was pressed, by checking the output of every single one individually. The same is done on the columns
5)
6)
7)
- Debouncing can vary over different buttons, also having interrupts from a person can lead to corruption to conditionals.
- Using interrupts to put to sleep and waking up is possible with an interrupt.

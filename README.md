# CriticalPath-Partitioning-KL-Algorithm
c++14 to has been used during working on this.

This code reads a Netlist file, then create a connectivity matrix for the netlist. After that, it performs a proper partitioning using KL Algorithm. The code also extracts the different paths, calculate the maximum propagation delay for each node, and then determine the suitable clock period.

The code is parameterized for the number of inputs per gate, the number of transistors per gate and delay per gate. I used structs and linked lists to avoid the limitations of the maximum size of arrays.

The code works with arranged netlist files only, it can not work with shuffled netlist files (the abnormal case). why?
I divide the netlist to mini netlists, between every two flip flops, I decided to created a netlist between every flip flop stage to extract the critical path and calculate its delay, then at the end the code performs a comparison to extract and calculate the critical path of the entire system and t calculate the minimum clock period.

KL Algorithm works with random initial starting points, works perfectly for every single pass, but when doing multiple passes to reach 
the end sometimes it fails, and other times it works perfectly (This needs to be reviewed).

# OS-Bankers-Algorithm-Project-2
 C++ Banker's Algorithm for Deadlock Avoidance for Linux

# Banker's Algorithm
 The banker's algorithm tests for safety by simulating a predetermined allocation of resources with the maximum alocations per process. This program was built to determine whether the system is in a safe state given the snapshot included in the 'snapshot.txt' file and if so, what the safe sequence of resolution would be?
 
# Solution Methodology
  The Banker's Algorithm verifies that a system is in a safe state by checking through each process to see how many resources each process needs, then checks to see that the needs can be met for each process by using the available resources to meet the process's needs and returning that processes allocated resources back to an available state. Any processes that cannot eventually have their needs met are left in an unfinished state, resulting in the system not being in a safe state. If all needs can be met, the system is in a safe state and the process that those needs were met is recorded.

# Instruction and Use
 Compile with g++, execute program with 'snapshot.txt' as an argument. 
 <br>
 <br>
 snapshot.txt is an example of the format to set up a the banker with a scenario.
 <br>
 The program will generate an 'output.txt' file to store output of last executed snapshot.

# Output Example
Output can be found in generated 'output.txt' file:<br><br>

"The system is in a safe state!<br>
The safe sequence is: P1, P3, P0, P2, P4"<br>
<br>
or
<br>
"The system is not in a safe state.<br>
Reason: Not enough resources for this request"
 
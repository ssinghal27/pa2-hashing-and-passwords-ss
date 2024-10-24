1. Two other password cracking techniques:
a. Dictionary Attack: This method uses a list of common passwords (like words from a dictionary). You would try each word from the list to see if it matches. To implement this, you'd read the dictionary file and run each word through the check password method.

b. Brute Force Attack: This method tries all possible combinations of characters. To implement it, you would generate every possible password and check each one using the check password method.

2. Memory vs. Speed:
The memory required is minimal, mostly storing small variables like passwords and hashes. However, the process is more limited by speed because it involves checking many potential passwords. The more variations you try, the longer the program takes to run. Therefore, password cracking is generally more limited by processing speed than memory.

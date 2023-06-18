# BufferOverflowAttack
Project using extensive buffer overflow attacks! :)

# Summary of the Project

## Part 1
In Part 1 of the project, I focused on exploiting a buffer overflow vulnerability in the `test` function. By carefully examining the code, I identified that the vulnerability arises from the `strcpy(test, input)` line. This line copies the user input into a buffer without proper bounds checking, leading to a potential buffer overflow.

To exploit this vulnerability, I disassembled the `main` function to understand its structure and identify the return address location. By analyzing the assembly code, I found that the `main` function calls the `test` function at address 0x08048e24. The following line, at address 0x08048f68, is the return address that we want to modify.

To calculate the distance between the buffer in the `test` function and the return address on the stack, I used GDB. I set a breakpoint at the start of the `test` function, ran the program with sample input, and printed the value of the `$ebp` register (0xffffd668) and the address of the `test` buffer (0xffffd64f). By subtracting the buffer address from `$ebp` and adding 4 (to account for the saved `$ebp` value), I determined that the distance between the buffer and the return address is 29 bytes.

Using this information, I crafted a payload in the form of `b"A" * 29 + b"\x72\x8e\x04\x08"`, where "A" is repeated 29 times to fill the buffer, and the following 4 bytes overwrite the return address with the desired address 0x8048e72 (location of `log_result()`). Executing the payload successfully overwrites the return address, diverting the program's flow to the desired function.

## Part 1 Bonus
In Part 1 Bonus, I encountered a segmentation fault issue caused by the `log_result` function's return address pointing to an arbitrary and inaccessible address. To overcome this problem, I modified the exploit to ensure that the return address of the `log_result` function points to a valid address.

By analyzing the assembly code and using GDB, I determined the address of the end of the `main` function, which is a suitable location for the modified return address. I updated the exploit payload to include the desired return address and executed it as `./try_me $(python3 -c 'import sys; sys.stdout.buffer.write(b"A" * 29 + b"\x72\x8e\x04\x08" + b"\x5a\x91\x04\x08")')`.

With this modified exploit, the program no longer crashes due to the segmentation fault, and the exploit successfully executes.

## Part 2
In Part 2, I continued with the exploitation, this time focusing on manipulating function parameters. By analyzing the assembly code and understanding the conditional statement in the code, I identified that the program compares the value of the first parameter with a specific value (0xefbeadde). To exploit this, I needed to gain control over the first parameter passed to the function.

To achieve this, I appended an additional `b"AAAA"` to the payload to ensure proper alignment and reached the first parameter's position on the stack. By overwriting the first argument with the desired value (0xefbeadde), I could pass the conditional check in the code.

I also found the memory address of the `log_result_advanced` function (0x8048ea0) using GDB's `info address` command. With this information, I crafted an exploit payload: `run $(python3 -c 'import sys; sys.stdout.buffer.write(b"A" * 29 + b"\xa0\x8e\x04\x08" + b"AAAA" + b"\xde\xad\xbe\xef")')`. This payload overwrites the return address with the address of `log_result_advanced`, ensures correct alignment, and modifies the first parameter with the desired value.

Executing the payload successfully triggers the exploitation, allowing me to write the `uid_1023_crack_advanced` file.

## Part 3
In Part 3, my goal was to trigger the `open(char *filename, flags)` function while manipulating its parameters. To achieve this, I utilized the buffer overflow vulnerability and carefully crafted the payload.

Similar to previous exploits, I filled the buffer with "A" repeated 29 times to populate it up to the exact point where further bytes would overwrite the function's return address. Instead of overwriting the return address with random data, I set it to the address of the `open` function (0x806cef0).

Manipulating the parameters required careful consideration. I used the technique of pushing the filename's pointer onto the stack, followed by pushing the flag option. To ensure proper alignment, I added `b"BBBB"` and `b"\x4c\xca\xff\xff"` to the payload.

One challenge in this exploit was finding the location of the "uid_1023_crack_super" string in memory. While I initially used GDB locally to identify the string's location, I had to employ a brute-force approach in the server environment, incrementally adding or subtracting various offsets until finding the correct location (0xffffca4c).

The final payload took the form of `./try_me $(python3 -c 'import sys; sys.stdout.buffer.write(b"A" * 29 + b"\xf0\xce\x06\x08" + b"BBBB" + b"\x4c\xca\xff\xff" + b"\x40\x04" + b"\x90\x90" + b"uid_1023_crack_super" + b"\x00")')`. This payload successfully overwrote the return address, manipulated the parameters, and created the `uid_1023_crack_super` file in the `/home/admin/` directory.

While the use of ROP chaining could have achieved similar results, the chosen buffer overflow approach proved simpler and more suitable for this specific scenario.


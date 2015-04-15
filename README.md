# Devlin
*Computer processor simulator in C*

This project was developed to simulate how CPU deal with processes in a basic level. Is a mono-processor that receives two parameters:
- **-n**: The number o processes to be simulated
- **-t**: The time to each cycle of CPU (in seconds)

### Compiling

```shell
➜ gcc devlin.c -o devlin
```

### Running

```shell
➜ ./devlin -n100 -t5
```

### Output

```
CPU Time: 21294
Running: 0
Ready: 1
Blocked: 0
Dealloced: 100
There's no process running. Let's get the first one from ReadyList (1).
There's 101 processes created.
We still have -1 processes to create.
Process 100 changed status to 3
Time running: 0.56 seconds.
Yeah, all process are done.
```

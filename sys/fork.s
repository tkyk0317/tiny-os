.global fork_sys_call
fork_sys_call:
    mov x8, #0
    svc #0
    ret

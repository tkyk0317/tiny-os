.global write_sys_call
write_sys_call:
    mov x8, #1
    svc #0
    ret

; io.s - I/O port operations
global outb
global inb

; outb - Write a byte to an I/O port
; Parameters: [esp+4] port address, [esp+8] data
outb:
    mov dx, [esp+4]    ; Port address
    mov al, [esp+8]    ; Data to write
    out dx, al
    ret

; inb - Read a byte from an I/O port
; Parameters: [esp+4] port address
; Returns: al - the read byte
inb:
    mov dx, [esp+4]    ; Port address
    in al, dx          ; Read byte into al
    ret

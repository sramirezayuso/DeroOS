GLOBAL  _read_msw,_lidt
GLOBAL  _int_08_hand
GLOBAL  _int_09_hand
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug
GLOBAL  _write, _read
GLOBAL	_outb, _inb, _outl, _inl
GLOBAL	_read_msr

EXTERN  int_08
EXTERN  int_09


SECTION .text


_Cli:
		cli			; limpia flag de interrupciones
		ret

_Sti:

		sti			; habilita interrupciones por flag
		ret

_mascaraPIC1:			; Escribe mascara del PIC 1
		push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	21h,al
        pop     ebp
        retn

_mascaraPIC2:			; Escribe mascara del PIC 2
		push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	0A1h,al
        pop     ebp
        retn

_read_msw:
        smsw    ax		; Obtiene la Machine Status Word
        retn


_lidt:				; Carga el IDTR
        push    ebp
        mov     ebp, esp
        push    ebx
        mov     ebx, [ss: ebp + 6] ; ds:bx = puntero a IDTR 
		rol	ebx,16		    	
		lidt    [ds: ebx]          ; carga IDTR
        pop     ebx
        pop     ebp
        retn


_int_08_hand:				; Handler de INT 8 ( Timer tick )
        push    ds
        push    es                      ; Se salvan los registros
        pusha                           ; Carga de DS y ES con el valor del selector
        mov     ax, 10h			; a utilizar.
        mov     ds, ax
        mov     es, ax                  
        call    int_08                 
        mov	al,20h			; Envio de EOI generico al PIC
		out	20h,al
		popa                            
        pop     es
        pop     ds
        iret


_int_09_hand:				; Handler de INT 9 ( Keyboard )
        push    ds
        push    es                      ; Se salvan los registros
        pusha                           ; Carga de DS y ES con el valor del selector
        mov     ax, 10h			; a utilizar.
        mov     ds, ax
        mov     es, ax
		mov	eax, 0h
		in	al, 60h			; Se obtiene el scan code del buffer
		push 	eax
        call    int_09                
		pop	eax
        mov	al,20h			; Envio de EOI generico al PIC
		out	20h,al
		popa                            
        pop     es
        pop     ds
        iret

_write:
        push	ebp
        mov		ebp, esp
		mov 	al, [ebp+8]		;Byte a escribir
		mov 	ebx, [ebp+12]	;Direccion de la pantalla
		mov		[ebx], al
		mov 	esp,ebp
		pop 	ebp   
		ret

_read:
		push	ebp
		mov	ebp, esp
		mov ebx, [ebp+8]
		mov eax, 0
		mov al, [ebx]
		mov 	esp,ebp
		pop 	ebp   
		ret
	
_outb:
		push    ebp
		mov     ebp, esp
		push	eax
		push	edx
		mov     eax, [ebp+12]
		mov     edx, [ebp+8]
		out     dx, al
		pop     edx
		pop     eax
		mov     esp, ebp
		pop     ebp
		ret

_inb:
		push	ebp
		mov     ebp, esp
		push	eax
		push	edx
		mov     edx, [ebp+8]
		in     	al, dx
		pop   	edx
		mov     esp, ebp
		pop   	ebp
		ret

_outl:
        	push    ebp
        	mov     ebp, esp
		push    eax
		push    edx
		mov     eax, [ebp+12]
		mov     dx, [ebp+8]
		out     dx, eax
		pop     edx
		pop 	eax
		mov     esp, ebp
		pop     ebp
		ret

_inl:
		push    ebp
		mov     ebp, esp
		push    edx
		mov     dx, [ebp+8]
		in 	eax, dx
		pop     edx
		mov     esp, ebp
		pop     ebp
		ret
		
_read_msr:
		push    ebp
		mov     ebp, esp
		push 	ecx
		mov 	ecx, [ebp+8]
		rdmsr
		pop		ecx
		mov     esp, ebp
		pop     ebp
		ret

; Debug para el BOCHS, detiene la ejecución; Para continuar colocar en el BOCHSDBG: set $eax=0
;


_debug:
        push    bp
        mov     bp, sp
        push	ax
vuelve:	mov     ax, 1
        cmp	ax, 0
		jne	vuelve
		pop	ax
		pop     bp
        retn

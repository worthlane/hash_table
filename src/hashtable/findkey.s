section .text

global _FindKey                                         ; predefined entry point name for ld
                                                        ; rdi = node_t* root_cell, rsi = const char* key
_FindKey:           mov     rdx, QWORD [rdi]            ; rdx = root_cell
                    vmovdqu    ymm0, YWORD [rsi]        ; ymm0 = key
                    mov     rax, rdi                    ;  |
                    test    rdx, rdx                    ;  | return root_cell if cell->key == 0, rax = cur_cell
                    jne     .COMPARE_KEYS               ;  |
                    ret
.LOOP_LIST:
                    mov     rax, QWORD [rax + 16]       ; cur_cell = cur_cell->next
                    mov     rdx, QWORD [rax]            ; rdx = cur_cell->key
                    test    rdx, rdx
                    je      .RETURN                     ; return (rax = cur_cell)
.COMPARE_KEYS:
                    xor        ecx, ecx                 ;  |
                    vptest     ymm0, YWORD [rdx]        ;  |    Inlined strcmp
                    setnc      cl                       ;  |

                    test    ecx, ecx                    ; return if strcmp == 0
                    jne     .LOOP_LIST
.RETURN:
                    vzeroupper
                    ret

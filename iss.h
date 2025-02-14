#ifndef ISS_H
#define ISS_H

#define IMM 0x0
#define ADD 0x1
#define ADDI 0x2
#define SUB 0x3
#define XOR 0x4
#define OR 0x5
#define AND 0x6
#define SR 0x7
#define ORI 0x8
#define ORIS 0x9
#define LD 0xA
#define ST 0xB
#define LT 0xC
#define EQ 0xD
#define IO 0xE
#define END 0xF

int cpu(char *buffer);

#endif // ISS_H

#include "iss.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint16_t mem[(1 << 16) - 1];
uint16_t regs[4];

void show_reg(uint16_t i) {
  char reg_name[4][3] = {"RA", "RB", "RC", "PC"};
  printf("%s : %d\n", reg_name[i], regs[i]);
}

void input(uint16_t rs, uint16_t *where_to_write) {
  if (rs == 3)
    printf("\n");
  else {
    printf("Entree attendue de type uint16_t : ");
    scanf("%hu", where_to_write);
  }
}

uint16_t sext(uint8_t imm) {
  return (uint16_t)(imm + (imm >> 7) * (0xFF << 8));
  // return (uint16_t) ((int16_t) ((int8_t) imm));
}

void load(char *buffer) {
  char *token = strtok(buffer, "\n ");
  token = strtok(NULL, "\n ");
  token = strtok(NULL, "\n ");
  uint16_t addr = 0;
  while (token != NULL) {
    printf("token: %s\n", token);
    int n;
    int res = sscanf(token, "%x", &n);
    if (res != 1) {
      fprintf(stderr, "Error parsing input file.\n");
      exit(1);
    }
    mem[addr++] = (uint16_t)n;
    token = strtok(NULL, "\n ");
  }
  printf("\n");
}

int cpu(char *buffer) {
  load(buffer);
  uint16_t instruction, *pc = &(regs[3]);
  uint8_t opcode, rd, rs, imm;
  int loop = 1;
  while (loop) {
    // TODO
    instruction = mem[*pc];

    opcode = instruction >> 12;
    instruction = instruction - ((uint16_t)opcode << 12);
    rd = instruction >> 10;
    instruction = instruction - ((uint16_t)rd << 10);
    rs = instruction >> 8;
    instruction = instruction - ((uint16_t)rs << 8);
    imm = instruction;

    switch (opcode) {
    case IMM:
      regs[rd] = sext(imm);
      (*pc)++;
      break;

    case ADD:
      regs[rd] = regs[rd] + regs[rs];
      (*pc)++;
      break;

    case ADDI:
      regs[rd] = regs[rs] + sext(imm);
      (*pc)++;
      break;

    case SUB:
      regs[rd] = regs[rd] - regs[rs];
      (*pc)++;
      break;

    case XOR:
      regs[rd] = regs[rd] ^ regs[rs];
      (*pc)++;
      break;

    case OR:
      regs[rd] = regs[rd] | regs[rs];
      (*pc)++;
      break;

    case AND:
      regs[rd] = regs[rd] & regs[rs];
      (*pc)++;
      break;

    case SR:
      regs[rd] = regs[rd] >> 1;
      (*pc)++;
      break;

    case ORI:
      regs[rd] = regs[rd] | (uint16_t)imm;
      (*pc)++;
      break;

    case ORIS:
      regs[rd] = regs[rd] | ((uint16_t)imm << 8);
      (*pc)++;
      break;

    case LD:
      regs[rd] = mem[regs[rs]];
      (*pc)++;
      break;

    case ST:
      mem[regs[rs]] = regs[rd];
      (*pc)++;
      break;

    case LT:
      if (regs[rd] < regs[rs])
        (*pc += sext(imm));
      else
        (*pc)++;
      break;

    case EQ:
      if (regs[rd] == regs[rs])
        (*pc += sext(imm));
      else
        (*pc)++;
      break;

    case IO:
      switch (rd) {
      case 0:
        input(rs, &regs[rs]);
        break;
      case 1:
        input(rs, &mem[regs[rs]]);
        break;
      case 2:
        show_reg(rs);
        break;
      case 3:
        printf("mem[%d] = %d\n", regs[rs], mem[regs[rs]]);
        break;
      }
      (*pc)++;
      break;
    case END:
      loop = 0;
      break;
    }
  }
  return 0;
}

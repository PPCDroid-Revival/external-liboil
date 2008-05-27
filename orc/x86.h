
#ifndef _ORC_X86_H_
#define _ORC_X86_H_

#include <unistd.h>

extern int x86_64;
extern int x86_exec_ptr;

void x86_emit_push (OrcProgram *program, int size, int reg);
void x86_emit_pop (OrcProgram *program, int size, int reg);
void x86_emit_mov_memoffset_reg (OrcProgram *program, int size, int offset, int reg1, int reg2);
void x86_emit_mov_memoffset_mmx (OrcProgram *program, int size, int offset,
    int reg1, int reg2);
void x86_emit_mov_memoffset_sse (OrcProgram *program, int size, int offset,
    int reg1, int reg2);
void x86_emit_mov_reg_memoffset (OrcProgram *program, int size, int reg1, int offset, int reg2);
void x86_emit_mov_mmx_memoffset (OrcProgram *program, int size, int reg1, int offset,
    int reg2);
void x86_emit_mov_sse_memoffset (OrcProgram *program, int size, int reg1, int offset,
    int reg2);
void x86_emit_mov_imm_reg (OrcProgram *program, int size, int value, int reg1);
void x86_emit_mov_reg_reg (OrcProgram *program, int size, int reg1, int reg2);
void x86_emit_mov_reg_mmx (OrcProgram *program, int reg1, int reg2);
void x86_emit_mov_mmx_reg (OrcProgram *program, int reg1, int reg2);
void x86_emit_mov_reg_sse (OrcProgram *program, int reg1, int reg2);
void x86_emit_mov_sse_reg (OrcProgram *program, int reg1, int reg2);
void x86_emit_test_reg_reg (OrcProgram *program, int size, int reg1, int reg2);
void x86_emit_sar_imm_reg (OrcProgram *program, int size, int value, int reg);
void x86_emit_dec_memoffset (OrcProgram *program, int size, int offset, int reg);
void x86_emit_add_imm_memoffset (OrcProgram *program, int size, int value, int offset, int reg);
void x86_emit_and_imm_memoffset (OrcProgram *program, int size, int value, int offset, int reg);
void x86_emit_add_imm_reg (OrcProgram *program, int size, int value, int reg);
void x86_emit_and_imm_reg (OrcProgram *program, int size, int value, int reg);
void x86_emit_cmp_imm_memoffset (OrcProgram *program, int size, int value,
    int offset, int reg);
void x86_emit_emms (OrcProgram *program);
void x86_emit_ret (OrcProgram *program);
void x86_emit_je (OrcProgram *program, int label);
void x86_emit_jne (OrcProgram *program, int label);
void x86_emit_label (OrcProgram *program, int label);

void x86_emit_modrm_memoffset (OrcProgram *program, int reg1, int offset, int reg2);
void x86_emit_modrm_reg (OrcProgram *program, int reg1, int reg2);
void x86_test (OrcProgram *program);

void mmx_emit_loadi_s16 (OrcProgram *p, int reg, int value);

enum {
  X86_EAX = ORC_GP_REG_BASE,
  X86_ECX,
  X86_EDX,
  X86_EBX,
  X86_ESP,
  X86_EBP,
  X86_ESI,
  X86_EDI,
  X86_R8,
  X86_R9,
  X86_R10,
  X86_R11,
  X86_R12,
  X86_R13,
  X86_R14,
  X86_R15,
  X86_MM0 = ORC_VEC1_REG_BASE,
  X86_MM1,
  X86_MM2,
  X86_MM3,
  X86_MM4,
  X86_MM5,
  X86_MM6,
  X86_MM7,
  X86_XMM0 = ORC_VEC2_REG_BASE,
  X86_XMM1,
  X86_XMM2,
  X86_XMM3,
  X86_XMM4,
  X86_XMM5,
  X86_XMM6,
  X86_XMM7,
  X86_XMM8,
  X86_XMM9,
  X86_XMM10,
  X86_XMM11,
  X86_XMM12,
  X86_XMM13,
  X86_XMM14,
  X86_XMM15
};

const char * x86_get_regname(int i);
int x86_get_regnum(int i);
const char * x86_get_regname_16(int i);
const char * x86_get_regname_64(int i);
const char * x86_get_regname_ptr(int i);
const char * x86_get_regname_mmx(int i);
const char * x86_get_regname_sse(int i);

#endif



#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <orc/orcprogram.h>

void orc_program_assign_rules (OrcProgram *program);
void orc_program_global_reg_alloc (OrcProgram *program);
void orc_program_rewrite_vars (OrcProgram *program);
void orc_program_rewrite_vars2 (OrcProgram *program);
void orc_program_do_regs (OrcProgram *program);

OrcProgram *
orc_program_new (void)
{
  OrcProgram *p;

  p = malloc(sizeof(OrcProgram));
  memset (p, 0, sizeof(OrcProgram));

  p->rule_set = ORC_RULE_MMX_4;
  p->n_per_loop = 4;

  return p;
}

void
orc_program_free (OrcProgram *program)
{
  int i;
  for(i=0;i<program->n_vars;i++){
    free (program->vars[i].name);
  }
  free (program);
}

int
orc_program_add_temporary (OrcProgram *program, const char *type, const char *name)
{
  int i = program->n_vars;

  program->vars[i].vartype = ORC_VAR_TYPE_TEMP;
  program->vars[i].type = orc_type_get(type);
  program->vars[i].name = strdup(name);
  program->n_vars++;

  return i;
}

int
orc_program_dup_temporary (OrcProgram *program, int var, int j)
{
  int i = program->n_vars;

  program->vars[i].vartype = ORC_VAR_TYPE_TEMP;
  program->vars[i].type = program->vars[var].type;
  program->vars[i].name = g_strdup_printf("%s.dup%d",
      program->vars[var].name, j);
  program->n_vars++;

  return i;
}

int
orc_program_add_source (OrcProgram *program, const char *type, const char *name)
{
  int i = program->n_vars;

  program->vars[i].vartype = ORC_VAR_TYPE_SRC;
  program->vars[i].type = orc_type_get(type);
  program->vars[i].name = strdup(name);
  program->n_vars++;

  return i;
}

int
orc_program_add_destination (OrcProgram *program, const char *type, const char *name)
{
  int i = program->n_vars;

  program->vars[i].vartype = ORC_VAR_TYPE_DEST;
  program->vars[i].type = orc_type_get(type);
  program->vars[i].name = strdup(name);
  program->n_vars++;

  return i;
}

int
orc_program_add_constant (OrcProgram *program, const char *type, int value, const char *name)
{
  int i = program->n_vars;

  program->vars[i].vartype = ORC_VAR_TYPE_CONST;
  program->vars[i].type = orc_type_get(type);
  program->vars[i].s16 = value;
  program->vars[i].name = strdup(name);
  program->n_vars++;

  return i;
}

int
orc_program_add_parameter (OrcProgram *program, OrcType *type, int value, const char *name)
{

  return 0;
}

void
orc_program_append (OrcProgram *program, const char *name, int arg0,
    int arg1, int arg2)
{
  OrcInstruction *insn;

  insn = program->insns + program->n_insns;

  insn->opcode = orc_opcode_find_by_name (name);
  if (!insn->opcode) {
    printf("unknown opcode: %s\n", name);
  }
  insn->args[0] = arg0;
  insn->args[1] = arg1;
  insn->args[2] = arg2;
  
  program->n_insns++;
}

int
orc_program_allocate_register (OrcProgram *program, int data_reg)
{
  int i;

  if (program->rule_set == ORC_RULE_SCALAR_1) {
    data_reg = FALSE;
  }

  if (!data_reg) {
    for(i=ORC_GP_REG_BASE;i<ORC_GP_REG_BASE+8;i++){
      if (program->alloc_regs[i] == 0) {
        program->alloc_regs[i]++;
        program->used_regs[i] = 1;
        return i;
      }
    }
  } else {
    for(i=ORC_GP_REG_BASE+8;i<ORC_GP_REG_BASE+16;i++){
      if (program->alloc_regs[i] == 0) {
        program->alloc_regs[i]++;
        program->used_regs[i] = 1;
        return i;
      }
    }
  }
  g_print("register overflow\n");
  return 0;
}

void
orc_program_compile (OrcProgram *program)
{
  orc_program_assign_rules (program);
  orc_program_rewrite_vars (program);

  orc_program_reset_alloc (program);
  orc_program_global_reg_alloc (program);

  orc_program_do_regs (program);

  orc_program_rewrite_vars2 (program);

  orc_program_allocate_codemem (program);
  orc_program_assemble_x86 (program);

  orc_program_dump_code (program);
}

void
orc_program_assign_rules (OrcProgram *program)
{
  int i;

  for(i=0;i<program->n_insns;i++) {
    OrcInstruction *insn = program->insns + i;
    unsigned int flags;

    insn->rule = insn->opcode->rules + program->rule_set;

    flags = insn->rule->flags;
    if (flags & ORC_RULE_REG_IMM &&
        program->vars[insn->args[2]].vartype == ORC_VAR_TYPE_CONST) {
      program->insns[i].rule_flag = ORC_RULE_REG_IMM;
    } else {
      program->insns[i].rule_flag = ORC_RULE_REG_REG;
    }
  }
}

void
orc_program_rewrite_vars (OrcProgram *program)
{
  int j;
  int k;
  OrcInstruction *insn;
  OrcOpcode *opcode;
  int var;
  int actual_var;

  for(j=0;j<program->n_insns;j++){
    insn = program->insns + j;
    opcode = insn->opcode;

    /* set up args */
    for(k=opcode->n_dest;k<opcode->n_src + opcode->n_dest;k++){
      var = insn->args[k];
      if (program->vars[var].vartype == ORC_VAR_TYPE_DEST) {
        g_print("ERROR: using dest var as source\n");
      }

      actual_var = var;
      if (program->vars[var].replaced) {
        actual_var = program->vars[var].replacement;
        insn->args[k] = actual_var;
      }

      if (!program->vars[var].used) {
        if (program->vars[var].vartype == ORC_VAR_TYPE_TEMP) {
          g_print("ERROR: using uninitialized temp var\n");
        }
        program->vars[var].used = TRUE;
        program->vars[var].first_use = j;
      }
      program->vars[actual_var].last_use = j;
    }

    for(k=0;k<opcode->n_dest;k++){
      var = insn->args[k];

      if (program->vars[var].vartype == ORC_VAR_TYPE_SRC) {
        g_print("ERROR: using src var as dest\n");
      }
      if (program->vars[var].vartype == ORC_VAR_TYPE_CONST) {
        g_print("ERROR: using const var as dest\n");
      }
      if (program->vars[var].vartype == ORC_VAR_TYPE_PARAM) {
        g_print("ERROR: using param var as dest\n");
      }

      actual_var = var;
      if (program->vars[var].replaced) {
        actual_var = program->vars[var].replacement;
        insn->args[k] = actual_var;
      }

      if (!program->vars[var].used) {
        program->vars[actual_var].used = TRUE;
        program->vars[actual_var].first_use = j;
      } else {
        if (program->vars[var].vartype == ORC_VAR_TYPE_DEST) {
          g_print("ERROR: writing dest more than once\n");
        }
        if (program->vars[var].vartype == ORC_VAR_TYPE_TEMP) {
          actual_var = orc_program_dup_temporary (program, var, j);
          program->vars[var].replaced = TRUE;
          program->vars[var].replacement = actual_var;
          insn->args[k] = actual_var;
          program->vars[actual_var].used = TRUE;
          program->vars[actual_var].first_use = j;
        }
      }
      program->vars[actual_var].last_use = j;
    }
  }
}

void
orc_program_global_reg_alloc (OrcProgram *program)
{
  int i;
  OrcVariable *var;


  for(i=0;i<program->n_vars;i++){
    var = program->vars + i;
    switch (var->vartype) {
      case ORC_VAR_TYPE_CONST:
        var->first_use = -1;
        var->last_use = -1;
        var->alloc = orc_program_allocate_register (program, TRUE);
        break;
      case ORC_VAR_TYPE_SRC:
      case ORC_VAR_TYPE_DEST:
        var->ptr_register = orc_program_allocate_register (program, FALSE);
        break;
      default:
        break;
    }
  }
}

void
orc_program_do_regs (OrcProgram *program)
{
  int i;
  int k;
  int var;
  OrcInstruction *insn;
  OrcOpcode *opcode;

  for(i=0;i<program->n_insns;i++){
    insn = program->insns + i;
    opcode = insn->opcode;

    for(k=opcode->n_dest;k<opcode->n_src + opcode->n_dest;k++){
      var = insn->args[k];


    }

    for(k=0;k<opcode->n_dest;k++){
      var = insn->args[k];
    }
  }
}

void
orc_program_rewrite_vars2 (OrcProgram *program)
{
  int i;
  int j;
  int k;

  for(j=0;j<program->n_insns;j++){
#if 1
    /* must be true to chain src1 to dest:
     *  - rule must handle it
     *  - src1 must be last_use
     */
    if (1 || program->insns[j].rule->flags & ORC_RULE_REG_REG) {
      int src1 = program->insns[j].args[1];
      int dest = program->insns[j].args[0];
      if (program->vars[src1].last_use == j) {
        if (program->vars[src1].first_use == j) {
          k = orc_program_allocate_register (program, TRUE);
          program->vars[src1].alloc = k;
        }
        program->alloc_regs[program->vars[src1].alloc]++;
        program->vars[dest].alloc = program->vars[src1].alloc;
      }
    }
#endif

    if (program->insns[j].rule_flag == ORC_RULE_REG_IMM) {
      /* immediate operand, don't load */
      int src2 = program->insns[j].args[2];
      program->vars[src2].alloc = 1;
    } else {
      int src2 = program->insns[j].args[2];
      if (program->vars[src2].alloc == 1) {
        program->vars[src2].alloc = 0;
      }
    }

    for(i=0;i<program->n_vars;i++){
      if (program->vars[i].first_use == j) {
        if (program->vars[i].alloc) continue;
        k = orc_program_allocate_register (program, TRUE);
        program->vars[i].alloc = k;
      }
    }
    for(i=0;i<program->n_vars;i++){
      if (program->vars[i].last_use == j) {
        program->alloc_regs[program->vars[i].alloc]--;
      }
    }
  }

#if 0
  for(i=0;i<program->n_vars;i++){
    g_print("# %2d: %2d %2d %d\n",
        i,
        program->vars[i].first_use,
        program->vars[i].last_use,
        program->vars[i].alloc);
  }
#endif

}

void
orc_program_dump_code (OrcProgram *program)
{
  FILE *file;

  file = fopen("dump","w");

  fwrite (program->code, 1, program->codeptr - program->code, file);
  fclose (file);
}

void
orc_program_dump (OrcProgram *program)
{
  int i;
  int j;
  OrcOpcode *opcode;
  OrcInstruction *insn;

  for(i=0;i<program->n_insns;i++){
    insn = program->insns + i;
    opcode = insn->opcode;

    g_print("insn: %d\n", i);
    g_print("  opcode: %s\n", opcode->name);

    for(j=0;j<opcode->n_dest;j++){
      g_print("  dest%d: %d %s\n", j, insn->args[j],
          program->vars[insn->args[j]].name);
    }
    for(j=0;j<opcode->n_src;j++){
      g_print("  src%d: %d %s\n", j, insn->args[opcode->n_dest + j],
          program->vars[insn->args[opcode->n_dest + j]].name);
    }

    g_print("\n");
  }

  for(i=0;i<program->n_vars;i++){
    g_print("var: %d %s\n", i, program->vars[i].name);
    g_print("first_use: %d\n", program->vars[i].first_use);
    g_print("last_use: %d\n", program->vars[i].last_use);

    g_print("\n");
  }

}

int
orc_variable_get_size (OrcVariable *var)
{
  return 2;
}


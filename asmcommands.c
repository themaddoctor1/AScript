#include "asmcommands.h"

int comparisons = 0;

void writeAsmBlock(FILE* file, char* line) {
    int i = -1;
    while(line[++i]);
    
    //printf("%s", line);

    fwrite(line, 1, i, file);
}
void writeComment(FILE* file, char* comment) {
    writeAsmBlock(file, "; ");
    writeAsmBlock(file, comment);
    writeAsmBlock(file, "\n");
}

void branchEQ(FILE* execfile, int regA, int regB, char* br) {
    char scratch[64];
    //First, compare
    sprintf(scratch, "cp 0x%x, 0x%x\n", regA, regB);
    writeAsmBlock(execfile, scratch);
    //Then, branch if
    sprintf(scratch, "breq %s\n", br);
    writeAsmBlock(execfile, scratch);

}

void branchNE(FILE* execfile, int regA, int regB, char* br) {
    char scratch[64];
    //First, compare
    sprintf(scratch, "cp 0x%x, 0x%x\n", regA, regB);
    writeAsmBlock(execfile, scratch);
    //Then, branch if
    sprintf(scratch, "brne %s\n", br);
    writeAsmBlock(execfile, scratch);

}

void branchGE(FILE* execfile, int regA, int regB, char* br) {
    char scratch[64];
    //First, compare
    sprintf(scratch, "cp 0x%x, 0x%x\n", regA, regB);
    writeAsmBlock(execfile, scratch);
    //Then, branch if
    sprintf(scratch, "brge %s\n", br);
    writeAsmBlock(execfile, scratch);

}

void branchGT(FILE* execfile, int regA, int regB, char* br) {
    char scratch[64];
    //First, compare
    sprintf(scratch, "cp 0x%x, 0x%x\n", regA, regB);
    writeAsmBlock(execfile, scratch);
    //Then, branch if
    sprintf(scratch, "brgt %s\n", br);
    writeAsmBlock(execfile, scratch);

}


void branchLE(FILE* execfile, int regA, int regB, char* br) {
    char scratch[64];
    //First, compare
    sprintf(scratch, "cp 0x%x, 0x%x\n", regA, regB);
    writeAsmBlock(execfile, scratch);
    //Then, branch if
    sprintf(scratch, "brle %s\n", br);
    writeAsmBlock(execfile, scratch);

}

void branchLT(FILE* execfile, int regA, int regB, char* br) {
    char scratch[64];
    //First, compare
    sprintf(scratch, "cp 0x%x, 0x%x\n", regA, regB);
    writeAsmBlock(execfile, scratch);
    //Then, branch if
    sprintf(scratch, "brlt %s\n", br);
    writeAsmBlock(execfile, scratch);

}

void jumpToLabel(FILE* execfile, char* label) {
    char scratch[64];
    
    //Jump to label
    sprintf(scratch, "jmp %s\n", label);
    writeAsmBlock(execfile, scratch);

}

void copyReg(FILE* execfile, int dst, int src) {
    char cmdline[64];
    sprintf(cmdline, "mov $%x, $%x\n", dst, src);
    
    writeAsmBlock(execfile, cmdline);
}

void copyRegFromMem(FILE* execfile, int dst, int src) {
    int zh = src / 0x100, zl = src % 0x100;
    char scratch[64];

    sprintf(scratch, "ldi zh, $%x\n", zh);
    writeAsmBlock(execfile, scratch);
    
    sprintf(scratch, "ldi zl, $%x\n", zl);
    writeAsmBlock(execfile, scratch);
    
    sprintf(scratch, "ld 0x%x, z\n", dst);
    writeAsmBlock(execfile, scratch);
}

void copyRegToMem(FILE* execfile, int dst, int src) {
    char scratch[64];

    sprintf(scratch, "sts 0x%x, 0x%x\n", dst, src);
    writeAsmBlock(execfile, scratch);
}
void loadReg(FILE* execfile, int r, char* v) {
    char cmdline[64];
    sprintf(cmdline, "ldi 0x%x, %s\n", r, v);

    writeAsmBlock(execfile, cmdline);
}

void addReg(FILE* execfile, int a, int b) {
    char cmdline[64];
    sprintf(cmdline, "add 0x%x, 0x%x\n", a, b);
    
    writeAsmBlock(execfile, cmdline);
}

void subReg(FILE* execfile, int a, int b) {
    char cmdline[64];
    sprintf(cmdline, "sub 0x%x, 0x%x\n", a, b);
    
    writeAsmBlock(execfile, cmdline);
}

void mulRegs(FILE* execfile, int a, int b) {
    char cmdline[64];
    sprintf(cmdline, "sub 0x%x, 0x%x\n", a, b);

    writeAsmBlock(execfile, cmdline);
}

void andReg(FILE* execfile, int a, int b) {
    char cmdline[64];
    sprintf(cmdline, "and 0x%x, 0x%x\n", a, b);
    
    writeAsmBlock(execfile, cmdline);
}

void orReg(FILE* execfile, int a, int b) {
    char cmdline[64];
    sprintf(cmdline, "or 0x%x, 0x%x\n", a, b);

    writeAsmBlock(execfile, cmdline);
}

void xorReg(FILE* execfile, int a, int b) {
    char cmdline[64];
    sprintf(cmdline, "eor 0x%x, 0x%x\n", a, b);

    writeAsmBlock(execfile, cmdline);
}

void eqBool(FILE* execfile, int a, int b, int dst) {
    char label[64];
    sprintf(label, "comp%i", comparisons++);

    //Sets the initial value for if false, then compares
    loadReg(execfile, dst, "$1"); 

    //Branch if false
    branchEQ(execfile, a, b, label);
    loadReg(execfile, dst, "$0");
    writeAsmBlock(execfile, label);
    writeAsmBlock(execfile, ":\n");

}

void neBool(FILE* execfile, int a, int b, int dst) {
    char label[64];
    sprintf(label, "comp%i", comparisons++);

    //Sets the initial value for if false, then compares
    loadReg(execfile, dst, "$1"); 

    //Branch if false
    branchNE(execfile, a, b, label);
    loadReg(execfile, dst, "$0");
    writeAsmBlock(execfile, label);
    writeAsmBlock(execfile, ":\n");

}

void geBool(FILE* execfile, int a, int b, int dst) {
    char label[64];
    sprintf(label, "comp%i", comparisons++);

    //Sets the initial value for if false, then compares
    loadReg(execfile, dst, "$1"); 

    //Branch if false
    branchGE(execfile, a, b, label);
    loadReg(execfile, dst, "$0");
    writeAsmBlock(execfile, label);
    writeAsmBlock(execfile, ":\n");

}

void gtBool(FILE* execfile, int a, int b, int dst) {
    char label[64];
    sprintf(label, "comp%i", comparisons++);

    //Sets the initial value for if false, then compares
    loadReg(execfile, dst, "$1"); 

    //Branch if false
    branchGT(execfile, a, b, label);
    loadReg(execfile, dst, "$0");
    writeAsmBlock(execfile, label);
    writeAsmBlock(execfile, ":\n");

}

void leBool(FILE* execfile, int a, int b, int dst) {
    char label[64];
    sprintf(label, "comp%i", comparisons++);

    //Sets the initial value for if false, then compares
    loadReg(execfile, dst, "$1"); 

    //Branch if false
    branchLE(execfile, a, b, label);
    loadReg(execfile, dst, "$0");
    writeAsmBlock(execfile, label);
    writeAsmBlock(execfile, ":\n");

}

void ltBool(FILE* execfile, int a, int b, int dst) {
    char label[64];
    sprintf(label, "comp%i", comparisons++);

    //Sets the initial value for if false, then compares
    loadReg(execfile, dst, "$1"); 

    //Branch if false
    branchLT(execfile, a, b, label);
    loadReg(execfile, dst, "$0");
    writeAsmBlock(execfile, label);
    writeAsmBlock(execfile, ":\n");

}


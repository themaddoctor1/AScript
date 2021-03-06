#include "functioncall.h"

#include "asmcommands.h"
#include "mathconvert.h"
#include "strmanip.h"

#include <string.h>

int buildStkFrame(FILE* execfile, char* params, CMP_TOK type) {
    int size = 0;
    
    writeComment(execfile, "Building stack frame for function call");
    writeAsmBlock(execfile, "; Return type is ");
    writeAsmBlock(execfile, type);
    writeAsmBlock(execfile, "\n");

    //Places the return value
    if(type && strcmp(type, "void")) {
        writeComment(execfile, "Making space for return value");
        loadRegV(execfile, 16, 0);
        
        int i = -1, size = sizeOfType(type);
        while(++i < size)
            stackPush(execfile, 16);

        //The size of the stack should not include the return value.
        size = 0;
    }

    char* str = params;
    while(*str) {
        if(*str == ' ') {
            str = &str[1];
            continue;
        }
        
        //Finds and adds the next parameter.
        char* val = contentToOperator(str, ',', '(', ')');

        char buff[32 + strlen(val)];
        sprintf(buff, "Calculating parameter '%s'", val);
        writeComment(execfile, buff);

        pemdas(execfile, val, 2);

        str = &str[strlen(val)];
        if(*str) str = &str[1];

        size += 2;

        free(val);
    }

    //Finalize the stack frame by pushing x_old and then storing sp in x.
    writeComment(execfile, "Saves old stack pointer for later");
    writeAsmBlock(execfile, "push xh\npush xl\nin xh, sph\nin xl, spl\n");

    return size;

}

void performFunctionCall(FILE* execfile, char* params, CMP_TOK type, char* name) {
    
    char buff[256];

    sprintf(buff, "Calling function %s %s(%s)", type, name, params);
    writeComment(execfile, buff);


    //First, the stack frame needs to be setup.
    int size = buildStkFrame(execfile, params, type);
    
    //Jumps to the function. The program counter will be pushed to the stack.
    sprintf(buff, "call function_%s\n", name);
    writeAsmBlock(execfile, buff);
    
    //Performs the final instructions that grab the return value and restore the stack
    finalizeReturn(execfile, size, type);

    writeComment(execfile, "Finished function call");

}

void performFunctionReturn(FILE* execfile) {

    //Change the stack pointer to point back to the beginning
    int size = ((VarFrame)getFromList(getVars(), listSize(getVars()) - 1))->addr + 1;
    
    writeComment(execfile, "Deep end return computations");
    if(size > 0) { //If the size is positive, there are values that need skimming

        //Get the current stack pointer
        writeAsmBlock(execfile, "in r16, spl\nin r17,sph\n");
        
        //Subtract the size of the frame stack from the pointer
        loadRegV(execfile, 18, size % 256);
        loadRegV(execfile, 19, size / 256);
        writeAsmBlock(execfile, "add r16, r18\nadc r17, r19\n");

        //Set new stack pointer
        writeAsmBlock(execfile, "out spl, r16\nout sph, r17\n");
        
    }

    //Performs the return.
    writeAsmBlock(execfile, "ret\n");

}

void finalizeReturn(FILE* execfile, int size, CMP_TOK type) {
    //At this point, the stack pointer should point to the high bit of x_old on the stack.
     
    //Restore x
    writeComment(execfile, "Restoring old x");
    writeAsmBlock(execfile, "pop xl\npop xh\n");
    
    //Pops the values from the stack.
    writeAsmBlock(execfile, "in r16, spl\nin r17, sph\n");
    
    //Loads the size of the stack into memory.
    loadRegV(execfile, 18, size % 256);
    loadRegV(execfile, 19, size / 256);

    //Subtract the stack size, and then drop the part of the frame.
    writeAsmBlock(execfile, "add r16, r18\nadc r17, r19\n");

    writeAsmBlock(execfile, "out spl, r16\nout sph, r17\n");

    //All that should be left is the return value, which should be at the top of the stack.

}



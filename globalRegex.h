#ifndef GLOBALREGEX_H
#define GLOBALREGEX_H
#include <QRegExp>
#include <QStringList>
//Regex will be moved here.



QString registerRegex = "\\$((?:[12]?[\\d])|(?:3[012])|(?:zero)|(?:at)|(?:v[01])|(?:a[0-3])|(?:t\\d)|(?:s[0-7])|(?:k[01])|gp|fp|ra|sp)";
QString commentRegex = "#.+";
QString labelRegex = "\\b[a-zA-z_]\\w*:\\b";


//  instruction $register, $register, $register
//    add
//    addu
//    sub
//    subu
//    and
//    or
//    nor
//    xor
//    srlv
//    sllv
//    srav
//    slt
//    sltu

QString registerInstructions = "(add|addu|sub|subu|and|or|nor|xor|srlv|sllv|srav|slt|sltu)";
QString registerFormat = registerInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, imm($register)
//      sb
//      lb
//      lbu
//      sh
//      lh
//      lhu
//      sw
//      lw
//      lwl
//      lwr
//      swl
//      swr
//      ll
//      sc

QString memoryInstructions = "(sb|lb|lbu|sh|lh|lhu|sw|lw|lwl|lwr|swl|swr|ll|sc)";
QString memoryFormat = memoryInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)[ \\t]*\\([ \\t]*" + registerRegex + "[ \\t]*\\)";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register, immediate
//    addi
//    addiu
//    andi
//    ori
//    nori
//    xori
//    srl
//    sll
//    sra
//    slti
//    sltiu
//    beq
//    bne

QString immInstructions = "(addi|addiu|andi|ori|nori|xori|srl|sll|sra|slti|sltiu|beq|bne)";
QString immFormat = immInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register, label
//      beq
//      bne

QString labelInstructions = "(beq|bne)";
QString labelFormat = labelInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*[a-zA-z_]\\w*";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, immediate
//      lui

QString singleimmInstructions = "(lui)";
QString singleimmFormat = singleimmInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register
//      jr
//      jalr
//      mfhi
//      mflo
//      mtlo
//      mthi

QString singleRegisterInstructions = "(jr|jalr|mfhi|mflo|mtlo|mthi)";
QString singleRegisterFormat = singleRegisterInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register
//      mult
//      multu
//      div
//      divu

QString doubleRegisterInstructions = "(mult|multu|div|divu)";
QString doubleRegisterFormat = doubleRegisterInstructions + "[ \\t]+" + registerRegex;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction label
//      j
//      jal

QString jumpInstructions = "(j|jal)";
QString jumpFormat = jumpInstructions + "[ \\t]+[a-zA-z_]\\w*";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction
//      syscall
//      nop

QString standaloneInstructions = "(syscall|nop)";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // GLOBALREGEX_H

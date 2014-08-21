#include "Assembler.h"
#include <QDebug>
#include <iostream>


// matches valid register names
QString registerRegex = "\\$((?:[12]?[\\d])|(?:3[012])|(?:zero)|(?:at)|(?:v[01])|(?:a[0-3])|(?:t\\d)|(?:s[0-7])|(?:k[01])|gp|fp|ra|sp)";
// invalid register name: matches $str where the str is not a valid register name
QString InvalidRegisterRegex = "\\$(?!(?:[12]?[\\d])|(?:3[012])|(?:zero)|(?:at)|(?:v[01])|(?:a[0-3])|(?:t\\d)|(?:s[0-7])|(?:k[01])|gp|fp|ra|sp)";
// Matches comments
QString commentRegex = "#.+";
// Matches labels
QString labelRegex = "[a-zA-Z_]\\w*";
QString labelRegexCapture = "(" + labelRegex +"):";
// Matches invalid labels (start with a number or an invalid character)
QString invalidLabelRegex = "\\b[^a-zA-Z_]\\w*:\\b";
// Matches valid directives' names
QString directivesRegex = "\\.(align|asciiz?|byte|data|double|float|globl|half|include|kdata|ktext|space|text|word)";
// Matches invalid directives
QString invalidDirectivesRegex = "\\.(?!align|asciiz?|byte|data|double|float|globl|half|include|kdata|ktext|space|text|word)";
// Matches strings
QString cstringsRegex = "\".*?[^\\\\]\"";
// Matches strings
QString invalidCstringsRegex = "\"(?:.*[^\\\\][^\"])$";

QString numberRegex = "(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)";
QRegExp registerRegExp(registerRegex, Qt::CaseInsensitive);
QRegExp numberRegExp(numberRegex, Qt::CaseInsensitive);
QRegExp threeArgsInstruction = QRegExp("(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + "([a-z]+)" + "[ \\t]+" + "([\\w$]+)" + "[ \\t]*,[ \\t]*" + "([\\w$]+)" + "[ \\t]*,[ \\t]*" + "([\\w$]+)" + "(?:[ \\t]+" + commentRegex + ")?$");
QRegExp twoArgsInstruction = QRegExp("(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + "([a-z]+)" + "[ \\t]+" + "([\\w$]+)" + "[ \\t]*,[ \\t]*" + "([\\w$]+)" + "(?:[ \\t]+" + commentRegex + ")?$");
QRegExp oneArgInstruction = QRegExp("(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + "([a-z]+)" + "[ \\t]+" + "([\\w$]+)" + "(?:[ \\t]+" + commentRegex + ")?$");
QRegExp memoryArgsInstruction = QRegExp("(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + "([a-z]+)" + "[ \\t]+" + "([\\w$]+)" + "[ \\t]*,[ \\t]*" + "([\\w$]+)" + "[ \\t]*\\([ \\t]*" + "([\\w$]+)" + "[ \\t]*\\)(?:[ \\t]+" + commentRegex + ")?$");

//  instruction $register, $register, $register
//    add
//    addu
//    and
//    or
//    nor
//    xor
//    sub
//    subu
//    srlv
//    sllv
//    srav
//    slt
//    sltu

QString registerInstructions = "(add|addu|sub|subu|and|or|nor|xor|srlv|sllv|srav|slt|sltu)";
QString registerFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + registerInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?$";
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
QString memoryFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + memoryInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)[ \\t]*\\([ \\t]*" + registerRegex + "[ \\t]*\\)(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register, immediate
//    addi
//    addiu
//    andi
//    ori
//    nori
//    xori
//    slti
//    sltiu
//    srl
//    sll
//    sra
//    beq
//    bne

QString immInstructions = "(addi|addiu|andi|ori|nori|xori|srl|sll|sra|slti|sltiu|beq|bne)";
QString immFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + immInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register, label
//      beq
//      bne

QString labelInstructions = "(beq|bne)";
QString labelFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + labelInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*([a-zA-Z_]\\w*)"+ "(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, immediate
//      lui

QString singleimmInstructions = "(lui)";
QString singleimmFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + singleimmInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)" + "(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register
//      jr
//      jalr
//      mfhi
//      mflo
//      mtlo
//      mthi

QString singleRegisterInstructions = "(jr|jalr|mfhi|mflo|mtlo|mthi)";
QString singleRegisterFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + singleRegisterInstructions + "[ \\t]+" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register
//      mult
//      multu
//      div
//      divu

QString doubleRegisterInstructions = "(mult|multu|div|divu)";
QString doubleRegisterFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + doubleRegisterInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction label
//      j
//      jal
QString jumpInstructions = "(j|jal)";
QString jumpFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + jumpInstructions + "[ \\t]+([a-zA-Z_]\\w*)" + "(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction
//      syscall
//      nop

QString standaloneInstructions = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + "(syscall|nop)"+ "(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register, label
//  blt
//  bgt
//  ble
//  bge
//  bltu
//  bgtu
//  bleu
//  bgeu

QString pLabelInstructions = "(blt|bgt|ble|bge|bltu|bgtu|bleu|bgeu)";
QString pLabelFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pLabelInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*([a-zA-Z_]\\w*)"+ "(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, immediate, label
//  blti
//  bgti
//  blei
//  bgei
//  bltiu
//  bgtiu
//  bleiu
//  bgeiu

QString pRILInstructions = "(blti|bgti|blei|bgei|bltiu|bgtiu|bleiu|bgeiu)";
QString pRILFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pRILInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)+[ \\t]*,[ \\t]*([a-zA-Z_]\\w*)"+ "(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, label
//  beqz
//  bnez
//  bltz
//  bgtz
//  blez
//  bgez
//  la

QString pZlabelInstructions = "(beqz|bnez|bltz|bgtz|blez|bgez)";
QString pZlabelFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pZlabelInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*([a-zA-Z_]\\w*)"+ "(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, immediate
//  li
//  ror
//  rol

QString pSingleimmInstructions = "(li|ror|rol)";
QString pSingleimmFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pSingleimmInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)" + "(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register
//  not
//  neg
//  move
//  abs
QString pDoubleRegisterInstructions = "(not|neg|move|abs)";
QString pDoubleRegisterFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pDoubleRegisterInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register, $register
//  mul
//  div
//  rem

QString pRegisterInstructions = "(mul|div|rem)";
QString pRegisterFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pRegisterInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register
//  clear
QString pSingleRegisterInstructions = "(clear)";
QString pSingleRegisterFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pSingleRegisterInstructions + "[ \\t]+" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register, immediate
//  subi

QString pImmInstructions = "(subi)";
QString pImmFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pImmInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Errors!!

//    addi
//    addiu
//    andi
//    ori
//    nori
//    xori
//    slti
//    sltiu
QString invalidRegisterInstructions = "(add|addu|and|or|nor|xor|slt|sltu)";
QString invalidRegisterFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + invalidRegisterInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)(?:[ \\t]+" + commentRegex + ")?$";

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
QString invalidMemoryFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + memoryInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*"+ registerRegex +"[ \\t]*\\([ \\t]*" + registerRegex + "[ \\t]*\\)(?:[ \\t]+" + commentRegex + ")?$";

QString invalidImmediateInstructions = "(addi|addiu|andi|ori|nori|xori|slti|sltiu)";
QString invalidImmediateFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + invalidImmediateInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?$";

//    srl
//    sll
//    sra
QString invalidShiftInstructions = "(srl|sll|sra)";
QString invalidShiftFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + invalidShiftInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?$";

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

QRegExp R(registerFormat, Qt::CaseInsensitive), M(memoryFormat, Qt::CaseInsensitive), I(immFormat, Qt::CaseInsensitive), L(labelFormat, Qt::CaseInsensitive), SR(singleRegisterFormat, Qt::CaseInsensitive), SI(singleimmFormat, Qt::CaseInsensitive), DR(doubleRegisterFormat, Qt::CaseInsensitive), J(jumpFormat, Qt::CaseInsensitive), SA(standaloneInstructions, Qt::CaseInsensitive), LBL(labelRegexCapture, Qt::CaseInsensitive), CMT(commentRegex, Qt::CaseInsensitive), WHITSPACE("^[ \\t]+$", Qt::CaseInsensitive);
QRegExp PR(pRegisterFormat, Qt::CaseInsensitive), PRIL(pRILFormat, Qt::CaseInsensitive), PL(pLabelFormat, Qt::CaseInsensitive), PZ(pZlabelFormat, Qt::CaseInsensitive), PSI(pSingleimmFormat, Qt::CaseInsensitive), PDR(pDoubleRegisterFormat, Qt::CaseInsensitive), PSR(pSingleRegisterFormat, Qt::CaseInsensitive), PI(pImmFormat, Qt::CaseInsensitive);
QRegExp invalidR(invalidRegisterFormat, Qt::CaseInsensitive), invalidM(invalidMemoryFormat, Qt::CaseInsensitive), invalidI(invalidImmediateFormat, Qt::CaseInsensitive), invalidSh(invalidShiftFormat, Qt::CaseInsensitive);

Assembler::Assembler(QStringList* stringList, Memory *memory, QVector<int> * mRegisters)
{
    this->mem = memory;
    this->registers = mRegisters;
    initializeRegisters();
    initializeFunctions();
    address = lineNumber = 0;

    connect(this,SIGNAL(buttonPressed(int,int,bool)),mem, SLOT(updateKey(int, int, bool)));

    foreach (QString line, *stringList)
    {
        if((R.indexIn(line, 0)) != -1)
        {
            instructions.push_back(Instruction(R.cap(2),registers,opcode[R.cap(2)],registerIndex[R.cap(4)],registerIndex[R.cap(5)],registerIndex[R.cap(3)],0,0,RFormat));
            //qDebug()<<R.cap(1)<<" "<<R.cap(2)<<" "<<R.cap(3)<<" "<<R.cap(4)<<" "<<R.cap(5)<<"\n";
            if(R.cap(1).size()) labels[R.cap(1)] = address;
        }
        else if((M.indexIn(line, 0)) != -1)
        {
            instructions.push_back(Instruction(M.cap(2),registers,opcode[M.cap(2)],registerIndex[M.cap(5)],registerIndex[M.cap(3)],0,getNumber(M.cap(4)),0,IFormat));
            if(M.cap(1).size()) labels[M.cap(1)] = address;
        }
        else if((I.indexIn(line, 0)) != -1)
        {
            if(I.cap(2) == "sll" || I.cap(2) == "srl" || I.cap(2) == "sra")
                instructions.push_back(Instruction(I.cap(2),registers,opcode[I.cap(2)],0,registerIndex[I.cap(4)],registerIndex[I.cap(3)],0,getNumber(I.cap(5)),RFormat));
            else
                instructions.push_back(Instruction(I.cap(2),registers,opcode[I.cap(2)],registerIndex[I.cap(4)],registerIndex[I.cap(3)],0,getNumber(I.cap(5)),0,IFormat));
            if(I.cap(1).size() > 0) labels[I.cap(1)] = address;
        }
        else if((L.indexIn(line, 0)) != -1)
        {
            if(labels.contains(L.cap(5))){
                instructions.push_back(Instruction(L.cap(2),registers,opcode[L.cap(2)],registerIndex[I.cap(3)],registerIndex[I.cap(4)],0,labels[L.cap(5)]-address-1,0,IFormat));
            }
            else{
                missingBranchLabels.push_back(qMakePair(qMakePair(address,lineNumber),L.cap(5)));
                instructions.push_back(Instruction(L.cap(2),registers,opcode[L.cap(2)],registerIndex[I.cap(3)],registerIndex[I.cap(4)],0,0,0,IFormat));
            }
            if(L.cap(1).size()) labels[L.cap(1)] = address;
        }
        else if((SR.indexIn(line, 0)) != -1)
        {
            if(SR.cap(2) == "j" || SR.cap(2) == "jal")
                instructions.push_back(Instruction(SR.cap(2),registers,opcode[SR.cap(2)],registerIndex[SR.cap(3)],0,0,0,0,RFormat));
            else
                instructions.push_back(Instruction(SR.cap(2),registers,opcode[SR.cap(2)],0,0,registerIndex[SR.cap(3)],0,0,RFormat));
            if(SR.cap(1).size()) labels[SR.cap(1)] = address;
        }
        else if((SI.indexIn(line, 0)) != -1)
        {
            instructions.push_back(Instruction(SI.cap(2),registers,opcode[SI.cap(2)],0,registerIndex[SI.cap(3)],0,getNumber(SI.cap(4)),0,IFormat));
            if(SI.cap(1).size()) labels[SI.cap(1)] = address;
        }
        else if((DR.indexIn(line, 0)) != -1)
        {
            instructions.push_back(Instruction(DR.cap(2),registers,opcode[DR.cap(2)],registerIndex[DR.cap(3)],registerIndex[DR.cap(4)],0,0,0,RFormat));
            if(DR.cap(1).size()) labels[DR.cap(1)] = address;
        }
        else if((J.indexIn(line, 0)) != -1)
        {
            if(labels.contains(J.cap(3))){
                qDebug()<<"Yes!";
                instructions.push_back(Instruction(J.cap(2),registers,opcode[J.cap(2)],0,0,0,labels[J.cap(3)],0,JFormat));
            }
            else{
                missingJumpLabels.push_back(qMakePair(qMakePair(address,lineNumber),J.cap(3)));
                instructions.push_back(Instruction(J.cap(2),registers,opcode[J.cap(2)],0,0,0,0,0,JFormat));
            }
            if(J.cap(1).size()) labels[J.cap(1)] = address;
        }
        else if((SA.indexIn(line, 0)) != -1)
        {
            instructions.push_back(Instruction(SA.cap(2),registers,opcode[SA.cap(2)],0,0,0,0,0,RFormat));
            if(SA.cap(1).size()) labels[SA.cap(1)] = address;
        }
        else if(PR.indexIn(line, 0) != -1)
        {
            handlePR(PR,line);
        }
        else if(PRIL.indexIn(line, 0) != -1)
        {
            handlePRIL(PRIL,line);
        }
        else if(PL.indexIn(line, 0) != -1)
        {
            handlePL(PL,line);
        }
        else if(PZ.indexIn(line, 0) != -1)
        {
            handlePZ(PZ,line);
        }
        else if(PSI.indexIn(line, 0) != -1)
        {
            handlePSI(PSI,line);
        }
        else if(PDR.indexIn(line, 0) != -1)
        {
            handlePDR(PDR,line);
        }
        else if(PSR.indexIn(line, 0) != -1)
        {
            handlePSR(PSR,line);
        }
        else if(PI.indexIn(line, 0) != -1)
        {
            handlePI(PI,line);
        }
        else if((LBL.indexIn(line, 0)) != -1)
        {
            labels[LBL.cap(1)] = address;
            address--;
        }
        else if((CMT.indexIn(line, 0)) != -1 || line.size()==0)
        {
            address--;
        }
        else
        {
            address--;
            if((threeArgsInstruction.indexIn(line, 0)) != -1){
                QString instructionName = threeArgsInstruction.cap(2).toLower();
                if(
                        instructionName == "add"    ||
                        instructionName == "addu"   ||
                        instructionName == "and"    ||
                        instructionName == "or"     ||
                        instructionName == "nor"    ||
                        instructionName == "xor"    ||
                        instructionName == "slt"    ||
                        instructionName == "sltu"   ||
                        instructionName == "sub"    ||
                        instructionName == "subu"   ||
                        instructionName == "srlv"   ||
                        instructionName == "sllv"   ||
                        instructionName == "srav"   ||
                        instructionName == "mul"    ||
                        instructionName == "div"    ||
                        instructionName == "rem"
                        ){
                    if(registerRegExp.indexIn(threeArgsInstruction.cap(3)) == -1){
                        if(threeArgsInstruction.cap(3)[0] == '$'){
                            errorList.push_back(Error("Invalid register \""+threeArgsInstruction.cap(3)+"\"",lineNumber));
                        }else{
                            errorList.push_back(Error("First argument must be a register",lineNumber));
                        }
                    }else if(registerRegExp.indexIn(threeArgsInstruction.cap(4)) == -1){
                        if(threeArgsInstruction.cap(4)[0] == '$'){
                            errorList.push_back(Error("Invalid register \""+threeArgsInstruction.cap(4)+"\"",lineNumber));
                        }else{
                            errorList.push_back(Error("Second argument must be a register",lineNumber));
                        }
                    }else if(registerRegExp.indexIn(threeArgsInstruction.cap(5)) == -1){
                        if(threeArgsInstruction.cap(5)[0] == '$'){
                            errorList.push_back(Error("Invalid register \""+threeArgsInstruction.cap(5)+"\"",lineNumber));
                        }else if(numberRegExp.indexIn(threeArgsInstruction.cap(5)) != -1 &&
                                 (
                                     instructionName == "add"    ||
                                     instructionName == "sub"    ||
                                     instructionName == "addu"   ||
                                     instructionName == "and"    ||
                                     instructionName == "or"     ||
                                     instructionName == "nor"    ||
                                     instructionName == "xor"    ||
                                     instructionName == "slt"    ||
                                     instructionName == "sltu"
                                     )){
                            bool uFlag = false;
                            if(instructionName.endsWith("u")){
                                instructionName = instructionName.mid(0,instructionName.size()-1);
                                uFlag = true;
                            }
                            errorList.push_back(Error("Third operand must be a register, use \""+instructionName+"i"+(uFlag? "u":"")+"\" instead of \""+instructionName+(uFlag? "u":"")+"\"",lineNumber));

                        }else if(numberRegExp.indexIn(threeArgsInstruction.cap(5)) != -1 &&
                                 (
                                     instructionName == "sllv"    ||
                                     instructionName == "srlv"   ||
                                     instructionName == "srav"
                                     )){
                            errorList.push_back(Error("Third operand must be a register, use \""+instructionName.remove(instructionName.size()-1,1)+"\" instead of \""+instructionName+"v\"",lineNumber));

                        }else{
                            errorList.push_back(Error("Third argument must be a register",lineNumber));
                        }
                    }else{
                        errorList.push_back(Error("Syntax Error",lineNumber));
                    }
                }else if(
                         instructionName == "addi"    ||
                         instructionName == "subi"    ||
                         instructionName == "addiu"   ||
                         instructionName == "andi"    ||
                         instructionName == "ori"     ||
                         instructionName == "nori"    ||
                         instructionName == "xori"    ||
                         instructionName == "slti"    ||
                         instructionName == "sltiu"   ||
                         instructionName == "srl"     ||
                         instructionName == "sra"     ||
                         instructionName == "sll"     ||
                         instructionName == "beq"     ||
                         instructionName == "bne"     ||
                         instructionName == "blt"     ||
                         instructionName == "bgt"     ||
                         instructionName == "ble"     ||
                         instructionName == "bge"     ||
                         instructionName == "bltu"    ||
                         instructionName == "bgtu"    ||
                         instructionName == "bleu"    ||
                         instructionName == "bgeu"
                         ){
                    if(registerRegExp.indexIn(threeArgsInstruction.cap(3)) == -1){
                        if(threeArgsInstruction.cap(3)[0] == '$'){
                            errorList.push_back(Error("Invalid register \""+threeArgsInstruction.cap(3)+"\"",lineNumber));
                        }else{
                            errorList.push_back(Error("First argument must be a register",lineNumber));
                        }
                    }else if(registerRegExp.indexIn(threeArgsInstruction.cap(4)) == -1){
                        if(threeArgsInstruction.cap(4)[0] == '$'){
                            errorList.push_back(Error("Invalid register \""+threeArgsInstruction.cap(4)+"\"",lineNumber));
                        }else{
                            errorList.push_back(Error("Second argument must be a register",lineNumber));
                        }
                    }else if(numberRegExp.indexIn(threeArgsInstruction.cap(5)) != 0){
                        if((registerRegExp.indexIn(threeArgsInstruction.cap(5)) != -1) &&
                                (
                                    instructionName == "addi"    ||
                                    instructionName == "subi"    ||
                                    instructionName == "addiu"   ||
                                    instructionName == "andi"    ||
                                    instructionName == "ori"     ||
                                    instructionName == "nori"    ||
                                    instructionName == "xori"    ||
                                    instructionName == "slti"    ||
                                    instructionName == "sltiu"
                                    )
                                ){
                            QString instructionName2 = instructionName;
                            instructionName.remove(instructionName.indexOf("i"),1);
                            errorList.push_back(Error("Third operand must be a number, use \""+instructionName+"\" instead of \""+instructionName2+"\"",lineNumber));
                        }else if((registerRegExp.indexIn(threeArgsInstruction.cap(5)) != -1) &&
                                 (
                                     instructionName == "sll"    ||
                                     instructionName == "srl"   ||
                                     instructionName == "sra"
                                     )
                                 ){
                            errorList.push_back(Error("Third operand must be a number, use \""+instructionName+"v\" instead of \""+instructionName+"\"",lineNumber));
                        }else if((registerRegExp.indexIn(threeArgsInstruction.cap(5)) != -1) &&
                                 (
                                     instructionName == "beq"     ||
                                     instructionName == "bne"     ||
                                     instructionName == "blt"     ||
                                     instructionName == "bgt"     ||
                                     instructionName == "ble"     ||
                                     instructionName == "bge"     ||
                                     instructionName == "bltu"    ||
                                     instructionName == "bgtu"    ||
                                     instructionName == "bleu"    ||
                                     instructionName == "bgeu"
                                     )
                                 ){
                            errorList.push_back(Error("Branch value cannot be a register, use a label or a number",lineNumber));
                        }else if(
                                 instructionName == "beq"     ||
                                 instructionName == "bne"     ||
                                 instructionName == "blt"     ||
                                 instructionName == "bgt"     ||
                                 instructionName == "ble"     ||
                                 instructionName == "bge"     ||
                                 instructionName == "bltu"    ||
                                 instructionName == "bgtu"    ||
                                 instructionName == "bleu"    ||
                                 instructionName == "bgeu"
                                 ){
                            errorList.push_back(Error("Invalid label \""+threeArgsInstruction.cap(5)+"\", labels must start with a latin character and can only contain characters, digits and underscores",lineNumber));
                        }
                    }
                }else if(instructionSet.contains(instructionName)){
                    errorList.push_back(Error("Instruction \""+instructionName+"\" does not take three arguments",lineNumber));
                }else{
                    int i;
                    for(i=0; i<instructionList.size(); i++){
                        if(stringDistance(instructionName.toStdString(), instructionList[i].toStdString()) == 1)
                            break;
                    }
                    if(i == instructionList.size())
                        errorList.push_back(Error("Unknown instruction \""+instructionName+"\"",lineNumber));
                    else
                        errorList.push_back(Error("Unknown instruction \""+instructionName+"\", did you mean \""+instructionList[i]+"\"?",lineNumber));
                }
            }else if((twoArgsInstruction.indexIn(line, 0)) != -1){
                QString instructionName = twoArgsInstruction.cap(2).toLower();
                if(
                        instructionName == "mult"   ||
                        instructionName == "multu"  ||
                        instructionName == "div"    ||
                        instructionName == "divu"   ||
                        instructionName == "not"    ||
                        instructionName == "neg"    ||
                        instructionName == "move"   ||
                        instructionName == "abs"
                        ){
                    if(registerRegExp.indexIn(twoArgsInstruction.cap(3)) == -1){
                        if(twoArgsInstruction.cap(3)[0] == '$'){
                            errorList.push_back(Error("Invalid register \""+twoArgsInstruction.cap(3)+"\"",lineNumber));
                        }else{
                            errorList.push_back(Error("First argument must be a register",lineNumber));
                        }
                    }else if(registerRegExp.indexIn(twoArgsInstruction.cap(4)) == -1){
                        if(twoArgsInstruction.cap(4)[0] == '$'){
                            errorList.push_back(Error("Invalid register \""+twoArgsInstruction.cap(4)+"\"",lineNumber));
                        }else{
                            errorList.push_back(Error("Second argument must be a register",lineNumber));
                        }
                    }else{
                        errorList.push_back(Error("Syntax Error",lineNumber));
                    }
                }else if(
                         instructionName == "lui"   ||
                         instructionName == "li"    ||
                         instructionName == "ror"   ||
                         instructionName == "rol"
                         ){
                    if(registerRegExp.indexIn(twoArgsInstruction.cap(3)) == -1){
                        if(twoArgsInstruction.cap(3)[0] == '$'){
                            errorList.push_back(Error("Invalid register \""+twoArgsInstruction.cap(3)+"\"",lineNumber));
                        }else{
                            errorList.push_back(Error("First argument must be a register",lineNumber));
                        }
                    }else if(numberRegExp.indexIn(twoArgsInstruction.cap(4)) != 0){
                        if(registerRegExp.indexIn(twoArgsInstruction.cap(4)) != -1){
                            if(instructionName == "rol" || instructionName == "ror"){
                                errorList.push_back(Error("Second Argument must be a number, cannot rotate by a register",lineNumber));
                            }else{
                                errorList.push_back(Error("Second argument must be a number, use \"move\" instead of \""+instructionName+"\"",lineNumber));
                            }
                        }else{
                            errorList.push_back(Error("Second argument must be a number",lineNumber));
                        }
                    }else{
                        errorList.push_back(Error("Syntax Error",lineNumber));
                    }
                }else if(
                         instructionName == "beqz"  ||
                         instructionName == "bnez"  ||
                         instructionName == "bltz"  ||
                         instructionName == "bgtz"  ||
                         instructionName == "blez"  ||
                         instructionName == "bgez"  ||
                         instructionName == "la"
                         ){
                    if(registerRegExp.indexIn(twoArgsInstruction.cap(3)) == -1){
                        if(twoArgsInstruction.cap(3)[0] == '$'){
                            errorList.push_back(Error("Invalid register \""+twoArgsInstruction.cap(3)+"\"",lineNumber));
                        }else{
                            errorList.push_back(Error("First argument must be a register",lineNumber));
                        }
                    }else if(registerRegExp.indexIn(twoArgsInstruction.cap(4)) != -1){
                        errorList.push_back(Error("Branch value cannot be a register, use a label instead",lineNumber));
                    }else{
                        errorList.push_back(Error("Invalid label \""+twoArgsInstruction.cap(4)+"\", labels must start with a latin character and can only contain characters, digits and underscores",lineNumber));
                    }
                }else if(instructionSet.contains(instructionName)){
                    errorList.push_back(Error("Instruction \""+instructionName+"\" does not take two arguments",lineNumber));
                }else{
                    int i;
                    for(i=0; i<instructionList.size(); i++){
                        if(stringDistance(instructionName.toStdString(), instructionList[i].toStdString()) == 1)
                            break;
                    }
                    if(i == instructionList.size())
                        errorList.push_back(Error("Unknown instruction \""+instructionName+"\"",lineNumber));
                    else
                        errorList.push_back(Error("Unknown instruction \""+instructionName+"\", did you mean \""+instructionList[i]+"\"?",lineNumber));

                }

            }else if((oneArgInstruction.indexIn(line, 0)) != -1){
                QString instructionName = oneArgInstruction.cap(2).toLower();
                if(
                        instructionName == "jr"     ||
                        instructionName == "jalr"   ||
                        instructionName == "mfhi"   ||
                        instructionName == "mflo"   ||
                        instructionName == "mtlo"   ||
                        instructionName == "mthi"   ||
                        instructionName == "clear"
                        ){
                    if(registerRegExp.indexIn(oneArgInstruction.cap(3)) == -1){
                        if(oneArgInstruction.cap(3)[0] == '$'){
                            errorList.push_back(Error("Invalid register \""+oneArgInstruction.cap(3)+"\"",lineNumber));
                        }else{
                            errorList.push_back(Error("First argument must be a register",lineNumber));
                        }
                    }else{
                        errorList.push_back(Error("Syntax Error",lineNumber));
                    }
                }else if(
                         instructionName == "j"     ||
                         instructionName == "jal"
                         ){
                    errorList.push_back(Error("Invalid label \""+oneArgInstruction.cap(3)+"\", labels must start with a latin character and can only contain characters, digits and underscores",lineNumber));
                }else if(instructionSet.contains(instructionName)){
                    errorList.push_back(Error("Instruction \""+instructionName+"\" does not take one argument",lineNumber));
                }else{
                    int i;
                    for(i=0; i<instructionList.size(); i++){
                        if(stringDistance(instructionName.toStdString(), instructionList[i].toStdString()) == 1)
                            break;
                    }
                    if(i == instructionList.size())
                        errorList.push_back(Error("Unknown instruction \""+instructionName+"\"",lineNumber));
                    else
                        errorList.push_back(Error("Unknown instruction \""+instructionName+"\", did you mean \""+instructionList[i]+"\"?",lineNumber));

                }
            }else if((memoryArgsInstruction.indexIn(line, 0)) != -1){
                QString instructionName = memoryArgsInstruction.cap(2).toLower();
                qDebug() << memoryArgsInstruction.cap(2) << " " << memoryArgsInstruction.cap(3) << " " << memoryArgsInstruction.cap(4) << " " << memoryArgsInstruction.cap(5);
                if(QRegExp(memoryInstructions, Qt::CaseInsensitive).indexIn(instructionName,0) != -1){
                    if(registerRegExp.indexIn(memoryArgsInstruction.cap(3)) == -1){
                        if(memoryArgsInstruction.cap(3)[0] == '$'){
                            errorList.push_back(Error("Invalid register \""+memoryArgsInstruction.cap(3)+"\"",lineNumber));
                        }else{
                            errorList.push_back(Error("First argument must be a register",lineNumber));
                        }
                    }else if(numberRegExp.indexIn(memoryArgsInstruction.cap(4)) != 0){
                        if(registerRegExp.indexIn(memoryArgsInstruction.cap(4)) != -1){
                            errorList.push_back(Error("Memory offset cannot be a register, use an immediate value instead",lineNumber));
                        }else{
                            errorList.push_back(Error("Memory offset must be an immediate value",lineNumber));
                        }
                    }else if(registerRegExp.indexIn(memoryArgsInstruction.cap(5)) == -1){
                        if(memoryArgsInstruction.cap(5)[0] == '$'){
                            errorList.push_back(Error("Invalid register \""+memoryArgsInstruction.cap(5)+"\"",lineNumber));
                        }else{
                            errorList.push_back(Error("Memory base address must be stored in a register",lineNumber));
                        }
                    }else{
                        qDebug() << "shit";
                        errorList.push_back(Error("Syntax Error",lineNumber));
                    }
                }else if(instructionSet.contains(instructionName)){
                    errorList.push_back(Error("Instruction \""+instructionName+"\" is not a memory instruction",lineNumber));
                }else{
                    int i;
                    for(i=0; i<instructionList.size(); i++){
                        if(stringDistance(instructionName.toStdString(), instructionList[i].toStdString()) == 1)
                            break;
                    }
                    if(i == instructionList.size())
                        errorList.push_back(Error("Unknown instruction \""+instructionName+"\"",lineNumber));
                    else
                        errorList.push_back(Error("Unknown instruction \""+instructionName+"\", did you mean \""+instructionList[i]+"\"?",lineNumber));
                }
            }else{
                errorList.push_back(Error("Syntax Error",lineNumber));
            }
            address++;
            lineNumber++;
        }
    }

    for (int i=0; i<missingBranchLabels.size(); i++)
    {
        QPair<QPair<int,int>,QString> lbl = missingBranchLabels[i];
        if(labels.contains(lbl.second)){
            instructions[lbl.first.first].setImm(labels[lbl.second]-lbl.first.first-1);
        } else {
            errorList.push_back(Error("Label \""+lbl.second+"\" was not found",lbl.first.second));
        }
    }
    for (int i=0; i<missingJumpLabels.size(); i++)
    {
        QPair<QPair<int,int>,QString> lbl = missingJumpLabels[i];
        if(labels.contains(lbl.second)){
            instructions[lbl.first.first].setImm(labels[lbl.second]);
        } else {
            errorList.push_back(Error("Label \""+lbl.second+"\" was not found",lbl.first.second));
        }
    }


    unsigned int addr = mem->textSegmentBaseAddress;
    for (int i = 0; i < instructions.size(); i++){
        instructions[i].setMem(mem);
        mem->storeWord(addr,instructions[i].getWord());
        addr += 4;
        //QObject::connect(&ins, SIGNAL(raiseException(int)), this, SLOT(exceptionHandler(int)));
    }


    for (int i = 0; i < instructions.size(); i++)
    {
        qDebug() << "********* instruction " + QString::number(i) + " ********";
        qDebug() << "Name: " << instructions[i].getName();
        qDebug() << "Rd: " << instructions[i].getRd();
        qDebug() << "Rs: " << instructions[i].getRs();
        qDebug() << "Rt: " << instructions[i].getRt();
        qDebug() << "Imm: " << instructions[i].getImm();
        qDebug() << "Shamt: " << instructions[i].getShamt();
        qDebug() << "**********************************************";

    }


    for (int i = 0; i<errorList.size(); i++)
    {
        qDebug() << errorList[i].lineNumber << " " << errorList[i].description;
    }


}

Assembler::~Assembler()
{
    //delete mem;
    //delete engine;
    //delete registers;
}


void Assembler::initializeFunctions()
{
    functionsMap["add"] = &add;
    functionsMap["addu"] = &addu;
    functionsMap["sub"] = &sub;
    functionsMap["subu"] = &subu;
    functionsMap["and"] = &and_;
    functionsMap["or"] = &or_;
    functionsMap["xor"] = &xor_;
    functionsMap["nor"] = &nor_;
    functionsMap["srlv"] = &srlv;
    functionsMap["sllv"] = &sllv;
    functionsMap["srav"] = &srav;
    functionsMap["slt"] = &slt;
    functionsMap["sltu"] = &sltu;

    functionsMap["sb"] = &sb;
    functionsMap["lb"] = &lb;
    functionsMap["lbu"] = &lbu;
    functionsMap["sh"] = &sh;
    functionsMap["lh"] = &lh;
    functionsMap["lhu"] = &lhu;
    functionsMap["sw"] = &sw;
    functionsMap["lw"] = &lw;
    functionsMap["lwl"] = &lwl;
    functionsMap["lwr"] = &lwr;
    functionsMap["swl"] = &swl;
    functionsMap["swr"] = &swr;
    functionsMap["ll"] = &ll;
    functionsMap["sc"] = &sc;


    functionsMap["addi"] = &addi;
    functionsMap["addiu"] = &addiu;
    functionsMap["andi"] = &andi;
    functionsMap["ori"] = &ori;
    functionsMap["nori"] = &nori;
    functionsMap["xori"] = &xori;
    functionsMap["srl"] = &srl;
    functionsMap["sll"] = &sll;
    functionsMap["sra"] = &sra;
    functionsMap["slti"] = &slti;
    functionsMap["sltiu"] = &sltiu;
    functionsMap["beq"] = &beq;
    functionsMap["bne"] = &bne;
    functionsMap["lui"] = &lui;

    functionsMap["jr"] = &jr;
    functionsMap["jalr"] = &jalr;
    functionsMap["mfhi"] = &mfhi;
    functionsMap["mflo"] = &mflo;
    functionsMap["mthi"] = &mthi;
    functionsMap["mtlo"] = &mtlo;
    functionsMap["mult"] = &mult;
    functionsMap["multu"] = &multu;
    functionsMap["div"] = &div;
    functionsMap["divu"] = &divu;
    functionsMap["j"] = &j_;
    functionsMap["jal"] = &jal;
    functionsMap["syscall"] = &syscall;
    functionsMap["nop"] = &nop;

    instructionSet.insert("add");
    instructionSet.insert("addu");
    instructionSet.insert("sub");
    instructionSet.insert("subu");
    instructionSet.insert("and");
    instructionSet.insert("or");
    instructionSet.insert("nor");
    instructionSet.insert("xor");
    instructionSet.insert("srlv");
    instructionSet.insert("sllv");
    instructionSet.insert("srav");
    instructionSet.insert("slt");
    instructionSet.insert("sltu");
    instructionSet.insert("sb");
    instructionSet.insert("lb");
    instructionSet.insert("lbu");
    instructionSet.insert("sh");
    instructionSet.insert("lh");
    instructionSet.insert("lhu");
    instructionSet.insert("sw");
    instructionSet.insert("lw");
    instructionSet.insert("lwl");
    instructionSet.insert("lwr");
    instructionSet.insert("swl");
    instructionSet.insert("swr");
    instructionSet.insert("ll");
    instructionSet.insert("sc");
    instructionSet.insert("addi");
    instructionSet.insert("addiu");
    instructionSet.insert("andi");
    instructionSet.insert("ori");
    instructionSet.insert("nori");
    instructionSet.insert("xori");
    instructionSet.insert("srl");
    instructionSet.insert("sll");
    instructionSet.insert("sra");
    instructionSet.insert("slti");
    instructionSet.insert("sltiu");
    instructionSet.insert("beq");
    instructionSet.insert("bne");
    instructionSet.insert("lui");
    instructionSet.insert("jr");
    instructionSet.insert("jalr");
    instructionSet.insert("mfhi");
    instructionSet.insert("mflo");
    instructionSet.insert("mtlo");
    instructionSet.insert("mthi");
    instructionSet.insert("mult");
    instructionSet.insert("multu");
    instructionSet.insert("div");
    instructionSet.insert("divu");
    instructionSet.insert("j");
    instructionSet.insert("jal");
    instructionSet.insert("syscall");
    instructionSet.insert("nop");

    instructionSet.insert("blt");
    instructionSet.insert("bgt");
    instructionSet.insert("ble");
    instructionSet.insert("bge");
    instructionSet.insert("bltu");
    instructionSet.insert("bgtu");
    instructionSet.insert("bleu");
    instructionSet.insert("bgeu");
    instructionSet.insert("blti");
    instructionSet.insert("bgti");
    instructionSet.insert("blei");
    instructionSet.insert("bgei");
    instructionSet.insert("bltiu");
    instructionSet.insert("bgtiu");
    instructionSet.insert("bleiu");
    instructionSet.insert("bgeiu");
    instructionSet.insert("beqz");
    instructionSet.insert("bnez");
    instructionSet.insert("bltz");
    instructionSet.insert("bgtz");
    instructionSet.insert("blez");
    instructionSet.insert("bgez");
    instructionSet.insert("li");
    instructionSet.insert("ror");
    instructionSet.insert("rol");
    instructionSet.insert("not");
    instructionSet.insert("neg");
    instructionSet.insert("move");
    instructionSet.insert("abs");
    instructionSet.insert("mul");
    instructionSet.insert("div");
    instructionSet.insert("rem");
    instructionSet.insert("clear");
    instructionSet.insert("subi");


    instructionList.append("addiu");
    instructionList.append("addu");
    instructionList.append("addi");
    instructionList.append("add");
    instructionList.append("andi");
    instructionList.append("ori");
    instructionList.append("nori");
    instructionList.append("xori");
    instructionList.append("sub");
    instructionList.append("subu");
    instructionList.append("and");
    instructionList.append("or");
    instructionList.append("nor");
    instructionList.append("xor");
    instructionList.append("srlv");
    instructionList.append("sllv");
    instructionList.append("srav");
    instructionList.append("slt");
    instructionList.append("sltu");
    instructionList.append("sw");
    instructionList.append("lw");
    instructionList.append("sb");
    instructionList.append("lb");
    instructionList.append("lbu");
    instructionList.append("sh");
    instructionList.append("lh");
    instructionList.append("lhu");
    instructionList.append("srl");
    instructionList.append("sll");
    instructionList.append("sra");
    instructionList.append("slti");
    instructionList.append("sltiu");
    instructionList.append("beq");
    instructionList.append("bne");
    instructionList.append("lui");
    instructionList.append("jalr");
    instructionList.append("mfhi");
    instructionList.append("mflo");
    instructionList.append("mtlo");
    instructionList.append("mthi");
    instructionList.append("multu");
    instructionList.append("mult");
    instructionList.append("div");
    instructionList.append("divu");
    instructionList.append("jr");
    instructionList.append("jal");
    instructionList.append("j");
    instructionList.append("syscall");
    instructionList.append("blt");
    instructionList.append("bgt");
    instructionList.append("ble");
    instructionList.append("bge");
    instructionList.append("bltu");
    instructionList.append("bgtu");
    instructionList.append("bleu");
    instructionList.append("bgeu");
    instructionList.append("blti");
    instructionList.append("bgti");
    instructionList.append("blei");
    instructionList.append("bgei");
    instructionList.append("bltiu");
    instructionList.append("bgtiu");
    instructionList.append("bleiu");
    instructionList.append("bgeiu");
    instructionList.append("beqz");
    instructionList.append("bnez");
    instructionList.append("bltz");
    instructionList.append("bgtz");
    instructionList.append("blez");
    instructionList.append("bgez");
    instructionList.append("li");
    instructionList.append("ror");
    instructionList.append("rol");
    instructionList.append("not");
    instructionList.append("neg");
    instructionList.append("move");
    instructionList.append("abs");
    instructionList.append("mul");
    instructionList.append("div");
    instructionList.append("rem");
    instructionList.append("clear");
    instructionList.append("subi");
    instructionList.append("nop");
    instructionList.append("lwl");
    instructionList.append("lwr");
    instructionList.append("swl");
    instructionList.append("swr");
    instructionList.append("ll");
    instructionList.append("sc");
}


void Assembler::handlePR(QRegExp m, QString line)
{
    if(m.cap(2) == "mul")
    {
        instructions.push_back(Instruction("mult",registers,opcode["mult"],registerIndex[m.cap(4)],registerIndex[m.cap(5)],0,0,0,RFormat));
        instructions.push_back(Instruction("mflo",registers,opcode["mflo"],0,0,registerIndex[SR.cap(3)],0,0,RFormat));
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address++;
    }
    else if(m.cap(2) == "div")
    {
        instructions.push_back(Instruction("div",registers,opcode["div"],registerIndex[m.cap(4)],registerIndex[m.cap(5)],0,0,0,RFormat));
        instructions.push_back(Instruction("mflo",registers,opcode["mflo"],0,0,registerIndex[SR.cap(3)],0,0,RFormat));
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address++;
    }
    else if(m.cap(2) == "rem")
    {
        instructions.push_back(Instruction("div",registers,opcode["div"],registerIndex[m.cap(4)],registerIndex[m.cap(5)],0,0,0,RFormat));
        instructions.push_back(Instruction("mfhi",registers,opcode["mfhi"],0,0,registerIndex[SR.cap(3)],0,0,RFormat));
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address++;
    }
}

void Assembler::handlePRIL(QRegExp m, QString line)
{
    if(m.cap(2) == "blti")
    {
        instructions.push_back(Instruction("slti",registers,opcode["slti"],registerIndex[m.cap(3)],registerIndex["at"],0,getNumber(m.cap(4)),0,IFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address++;
    }
    else if(m.cap(2) == "bgti")
    {
        instructions.push_back(Instruction("addi",registers,opcode["addi"],registerIndex["0"],registerIndex["at"],0,getNumber(m.cap(4)),0,IFormat));
        instructions.push_back(Instruction("slt",registers,opcode["slt"],registerIndex["at"],registerIndex[m.cap(3)],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-3,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+2,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address += 2;
    }
    else if(m.cap(2) == "blei")
    {
        instructions.push_back(Instruction("addi",registers,opcode["addi"],registerIndex["0"],registerIndex["at"],0,getNumber(m.cap(4)),0,IFormat));
        instructions.push_back(Instruction("slt",registers,opcode["slt"],registerIndex["at"],registerIndex[m.cap(3)],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-3,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+2,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address += 2;
    }
    else if(m.cap(2) == "bgei")
    {
        instructions.push_back(Instruction("slti",registers,opcode["slti"],registerIndex[m.cap(3)],registerIndex["at"],0,getNumber(m.cap(4)),0,IFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address++;
    }
    else if(m.cap(2) == "bltiu")
    {
        instructions.push_back(Instruction("sltiu",registers,opcode["sltiu"],registerIndex[m.cap(3)],registerIndex["at"],0,getNumber(m.cap(4)),0,IFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address++;
    }
    else if(m.cap(2) == "bgtiu")
    {
        instructions.push_back(Instruction("addi",registers,opcode["addi"],registerIndex["0"],registerIndex["at"],0,getNumber(m.cap(4)),0,IFormat));
        instructions.push_back(Instruction("sltu",registers,opcode["sltu"],registerIndex["at"],registerIndex[m.cap(3)],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-3,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+2,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address += 2;
    }
    else if(m.cap(2) == "bleiu")
    {
        instructions.push_back(Instruction("addi",registers,opcode["addi"],registerIndex["0"],registerIndex["at"],0,getNumber(m.cap(4)),0,IFormat));
        instructions.push_back(Instruction("sltu",registers,opcode["sltu"],registerIndex["at"],registerIndex[m.cap(3)],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-3,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+2,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address += 2;
    }
    else if(m.cap(2) == "bgeiu")
    {
        instructions.push_back(Instruction("sltiu",registers,opcode["sltiu"],registerIndex[m.cap(3)],registerIndex["at"],0,getNumber(m.cap(4)),0,IFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address++;
    }
}

void Assembler::handlePL(QRegExp m, QString line)
{
    if(m.cap(2) == "blt")
    {
        instructions.push_back(Instruction("slt",registers,opcode["slt"],registerIndex[m.cap(3)],registerIndex[m.cap(4)],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
    }
    else if(m.cap(2) == "bgt")
    {
        instructions.push_back(Instruction("slt",registers,opcode["slt"],registerIndex[m.cap(4)],registerIndex[m.cap(3)],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;

    }
    else if(m.cap(2) == "ble")
    {
        instructions.push_back(Instruction("slt",registers,opcode["slt"],registerIndex[m.cap(4)],registerIndex[m.cap(3)],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
    }
    else if(m.cap(2) == "bge")
    {
        instructions.push_back(Instruction("slt",registers,opcode["slt"],registerIndex[m.cap(3)],registerIndex[m.cap(4)],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
    }
    else if(m.cap(2) == "bltu")
    {
        instructions.push_back(Instruction("sltu",registers,opcode["sltu"],registerIndex[m.cap(3)],registerIndex[m.cap(4)],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
    }
    else if(m.cap(2) == "bgtu")
    {
        instructions.push_back(Instruction("sltu",registers,opcode["sltu"],registerIndex[m.cap(4)],registerIndex[m.cap(3)],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
    }
    else if(m.cap(2) == "bleu")
    {
        instructions.push_back(Instruction("sltu",registers,opcode["sltu"],registerIndex[m.cap(4)],registerIndex[m.cap(3)],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
    }
    else if(m.cap(2) == "bgeu")
    {
        instructions.push_back(Instruction("sltu",registers,opcode["sltu"],registerIndex[m.cap(3)],registerIndex[m.cap(4)],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(5))){
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(5)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(5)));
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
    }
    address++;
}

void Assembler::handlePZ(QRegExp m, QString line)
{
    if(m.cap(2) == "beqz")
    {
        if(labels.contains(m.cap(4))){
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex[m.cap(3)],0,labels[m.cap(4)]-address-1,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address,lineNumber),m.cap(4)));
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex[m.cap(3)],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
    }
    else if(m.cap(2) == "bnez")
    {
        if(labels.contains(m.cap(4))){
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex[m.cap(3)],0,labels[m.cap(4)]-address-1,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address,lineNumber),m.cap(4)));
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex[m.cap(3)],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
    }
    else if(m.cap(2) == "bltz")
    {
        instructions.push_back(Instruction("slt",registers,opcode["slt"],registerIndex[m.cap(3)],registerIndex["0"],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(4))){
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(4)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(4)));
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address++;
    }
    else if(m.cap(2) == "bgez")
    {
        instructions.push_back(Instruction("slt",registers,opcode["slt"],registerIndex[m.cap(3)],registerIndex["0"],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(4))){
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(4)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(4)));
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address++;
    }
    else if(m.cap(2) == "bgtz")
    {
        instructions.push_back(Instruction("slt",registers,opcode["slt"],registerIndex["0"],registerIndex[m.cap(3)],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(4))){
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(4)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(4)));
            instructions.push_back(Instruction("bne",registers,opcode["bne"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address++;
    }
    else if(m.cap(2) == "blez")
    {
        instructions.push_back(Instruction("slt",registers,opcode["slt"],registerIndex["0"],registerIndex[m.cap(3)],registerIndex["at"],0,0,RFormat));
        if(labels.contains(m.cap(4))){
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,labels[m.cap(4)]-address-2,0,IFormat));
        }
        else{
            missingBranchLabels.push_back(qMakePair(qMakePair(address+1,lineNumber),m.cap(4)));
            instructions.push_back(Instruction("beq",registers,opcode["beq"],registerIndex["0"],registerIndex["at"],0,0,0,IFormat));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address++;
    }
    else if(m.cap(2) == "la")
    {

    }
}

void Assembler::handlePSI(QRegExp m, QString line)
{
    if(m.cap(2) == "li")
    {
        int numberToLoad = getNumber(m.cap(3));
        if(numberToLoad > 0xffff){
            instructions.push_back(Instruction("lui",registers,opcode["lui"],0,registerIndex[m.cap(3)],0,numberToLoad >> 16,0,IFormat));
            instructions.push_back(Instruction("ori",registers,opcode["ori"],registerIndex[m.cap(3)],registerIndex[m.cap(3)],0,numberToLoad & 0xffff,0,IFormat));
            if(m.cap(1).size()) labels[m.cap(1)] = address;
            address++;
        }
        else{
            instructions.push_back(Instruction("addi",registers,opcode["addi"],registerIndex["0"],registerIndex[m.cap(3)],0,numberToLoad,0,IFormat));
            if(m.cap(1).size()) labels[m.cap(1)] = address;
        }
    }
    else if(m.cap(2) == "ror")
    {
        instructions.push_back(Instruction("srl",registers,opcode["srl"],0,registerIndex[m.cap(3)],registerIndex["at"],0,getNumber(m.cap(4)),RFormat));
        instructions.push_back(Instruction("sll",registers,opcode["sll"],0,registerIndex[m.cap(3)],registerIndex[m.cap(3)],0,32 - getNumber(m.cap(4)),RFormat));
        instructions.push_back(Instruction("or" ,registers,opcode["or" ],registerIndex[m.cap(3)],registerIndex["at"],registerIndex[m.cap(3)],0,0,RFormat));
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address += 2;
    }
    else if(m.cap(2) == "rol")
    {
        instructions.push_back(Instruction("sll",registers,opcode["sll"],0,registerIndex[m.cap(3)],registerIndex["at"],0,getNumber(m.cap(4)),RFormat));
        instructions.push_back(Instruction("srl",registers,opcode["srl"],0,registerIndex[m.cap(3)],registerIndex[m.cap(3)],0,32 - getNumber(m.cap(4)),RFormat));
        instructions.push_back(Instruction("or" ,registers,opcode["or" ],registerIndex[m.cap(3)],registerIndex["at"],registerIndex[m.cap(3)],0,0,RFormat));
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address += 2;
    }
}

void Assembler::handlePDR(QRegExp m, QString line)
{
    if(m.cap(2) == "not")
    {
        instructions.push_back(Instruction("nor",registers,opcode["nor"],registerIndex[m.cap(4)],registerIndex["0"],registerIndex[m.cap(3)],0,0,RFormat));
        if(m.cap(1).size()) labels[m.cap(1)] = address;
    }
    else if(m.cap(2) == "neg")
    {
        instructions.push_back(Instruction("sub",registers,opcode["sub"],registerIndex["0"],registerIndex[m.cap(4)],registerIndex[m.cap(3)],0,0,RFormat));
        if(m.cap(1).size() > 0) labels[m.cap(1)] = address;
    }
    else if(m.cap(2) == "move")
    {
        instructions.push_back(Instruction("addu",registers,opcode["addu"],registerIndex[m.cap(4)],registerIndex["0"],registerIndex[m.cap(3)],0,0,RFormat));
        if(m.cap(1).size() > 0) labels[m.cap(1)] = address;
    }
    else if(m.cap(2) == "abs")
    {
        instructions.push_back(Instruction("sra",registers,opcode["sra"],registerIndex[m.cap(4)],registerIndex["at"],0,0,31,IFormat));
        instructions.push_back(Instruction("xor",registers,opcode["xor"],registerIndex["at"],registerIndex[m.cap(4)],registerIndex[m.cap(3)],0,0,RFormat));
        instructions.push_back(Instruction("subu",registers,opcode["subu"],registerIndex[m.cap(3)],registerIndex["at"],registerIndex[m.cap(3)],0,0,RFormat));
        address += 2;
    }
}

void Assembler::handlePSR(QRegExp m, QString line)
{
    if(m.cap(2) == "clear")
    {
        instructions.push_back(Instruction("add",registers,opcode["add"],registerIndex["0"],registerIndex["0"],registerIndex[m.cap(3)],0,0,RFormat));
        if(m.cap(1).size() > 0) labels[m.cap(1)] = address;
    }
}

void Assembler::handlePI(QRegExp m, QString line)
{
    if(m.cap(2) == "subi")
    {
        instructions.push_back(Instruction("addi",registers,opcode["addi"],registerIndex[m.cap(4)],registerIndex[m.cap(3)],0,-getNumber(m.cap(5)),getNumber(m.cap(5)),IFormat));
        if(m.cap(1).size() > 0) labels[m.cap(1)] = address;
    }
}


Assembler::Assembler(){

}

void Assembler::simulate()
{
    for (int i = 0; i < registers->size(); i++)
        (*registers)[i] = 0;
    (*registers)[28] = 0x10008000;
    (*registers)[29] = 0x7FFFEFFC;

    PC = 1;
    int i = 0;
    int activePC = (PC - 1)/4;
    while (PC != -1 && ((PC - 1)/4) < instructions.size() && i < 100)
    {
        sf::Event event;
        while(mem->getTileEngine()->pollEvent(event))
        {
            if(event.type == sf::Event::KeyPressed){
                qDebug() << event.key.code << " pressed";
                switch(event.key.code){
                case Keyboard::Down:
                    emit Assembler::buttonPressed(DOWN_KEY_INDEX,0, true);
                    break;
                case Keyboard::Left:
                    emit Assembler::buttonPressed(LEFT_KEY_INDEX,0, true);
                    break;
                case Keyboard::Right:
                    emit Assembler::buttonPressed(RIGHT_KEY_INDEX,0, true);
                    break;
                case Keyboard::Up:
                    emit Assembler::buttonPressed(UP_KEY_INDEX,0, true);
                    break;
                case Keyboard::Z:
                    emit Assembler::buttonPressed(A_KEY_INDEX,0, true);
                    break;
                case Keyboard::X:
                    emit Assembler::buttonPressed(B_KEY_INDEX,0, true);
                    break;
                case Keyboard::D:
                    emit Assembler::buttonPressed(R_KEY_INDEX,0, true);
                    break;
                case Keyboard::A:
                    emit Assembler::buttonPressed(L_KEY_INDEX,0, true);
                    break;
                case Keyboard::Return:
                    emit Assembler::buttonPressed(START_KEY_INDEX,0, true);
                    break;
                case Keyboard::BackSpace:
                    emit Assembler::buttonPressed(SELECT_KEY_INDEX,0, true);
                    break;
                default:
                    qDebug() << "Another button was pressed";
                }
            } else if(event.type == sf::Event::KeyReleased){
                qDebug() << event.key.code << " released";
                switch(event.key.code){
                case Keyboard::Down:
                    emit Assembler::buttonPressed(DOWN_KEY_INDEX,0, false);
                    break;
                case Keyboard::Left:
                    emit Assembler::buttonPressed(LEFT_KEY_INDEX,0, false);
                    break;
                case Keyboard::Right:
                    emit Assembler::buttonPressed(RIGHT_KEY_INDEX,0, false);
                    break;
                case Keyboard::Up:
                    emit Assembler::buttonPressed(UP_KEY_INDEX,0, false);
                    break;
                case Keyboard::Z:
                    emit Assembler::buttonPressed(A_KEY_INDEX,0, false);
                    break;
                case Keyboard::X:
                    emit Assembler::buttonPressed(B_KEY_INDEX,0, false);
                    break;
                case Keyboard::D:
                    emit Assembler::buttonPressed(R_KEY_INDEX,0, false);
                    break;
                case Keyboard::A:
                    emit Assembler::buttonPressed(L_KEY_INDEX,0, false);
                    break;
                case Keyboard::Return:
                    emit Assembler::buttonPressed(START_KEY_INDEX,0, false);
                    break;
                case Keyboard::BackSpace:
                    emit Assembler::buttonPressed(SELECT_KEY_INDEX,0, false);
                    break;
                default:
                    qDebug() << "Another button was released";
                }
            } else if(event.type == sf::Event::JoystickButtonPressed){
                qDebug() << event.joystickButton.button << " pressed";
            } else if(event.type == sf::Event::JoystickButtonReleased){
                qDebug() << event.joystickButton.button << " released";
            }
        }

        activePC = ((PC - 1)/4);
        instructions[activePC].setFunc(functionsMap[instructions[activePC].getName().trimmed()]);
        instructions[activePC].execute(PC);
        i++;
    }
}





int Assembler::getNumber(QString s)
{
    bool f;
    s = s.toLower();
    if(s.startsWith("0x"))
        return s.mid(2).toUInt(&f,16);
    else if(s.startsWith("0b"))
        return s.mid(2).toUInt(&f,2);
    else return s.toInt();
}



void Assembler::initializeRegisters()
{
    registerIndex["zero"] = 0;
    registerIndex["at"] = 1;
    registerIndex["v0"] = 2;
    registerIndex["v1"] = 3;
    registerIndex["a0"] = 4;
    registerIndex["a1"] = 5;
    registerIndex["a2"] = 6;
    registerIndex["a3"] = 7;
    registerIndex["t0"] = 8;
    registerIndex["t1"] = 9;
    registerIndex["t2"] = 10;
    registerIndex["t3"] = 11;
    registerIndex["t4"] = 12;
    registerIndex["t5"] = 13;
    registerIndex["t6"] = 14;
    registerIndex["t7"] = 15;
    registerIndex["s0"] = 16;
    registerIndex["s1"] = 17;
    registerIndex["s2"] = 18;
    registerIndex["s3"] = 19;
    registerIndex["s4"] = 20;
    registerIndex["s5"] = 21;
    registerIndex["s6"] = 22;
    registerIndex["s7"] = 23;
    registerIndex["t8"] = 24;
    registerIndex["t9"] = 25;
    registerIndex["k0"] = 26;
    registerIndex["k1"] = 27;
    registerIndex["gp"] = 28;
    registerIndex["sp"] = 29;
    registerIndex["fp"] = 30;
    registerIndex["ra"] = 31;
    registerIndex["0"] = 0;
    registerIndex["1"] = 1;
    registerIndex["2"] = 2;
    registerIndex["3"] = 3;
    registerIndex["4"] = 4;
    registerIndex["5"] = 5;
    registerIndex["6"] = 6;
    registerIndex["7"] = 7;
    registerIndex["8"] = 8;
    registerIndex["9"] = 9;
    registerIndex["10"] = 10;
    registerIndex["11"] = 11;
    registerIndex["12"] = 12;
    registerIndex["13"] = 13;
    registerIndex["14"] = 14;
    registerIndex["15"] = 15;
    registerIndex["16"] = 16;
    registerIndex["17"] = 17;
    registerIndex["18"] = 18;
    registerIndex["19"] = 19;
    registerIndex["20"] = 20;
    registerIndex["21"] = 21;
    registerIndex["22"] = 22;
    registerIndex["23"] = 23;
    registerIndex["24"] = 24;
    registerIndex["25"] = 25;
    registerIndex["26"] = 26;
    registerIndex["27"] = 27;
    registerIndex["28"] = 28;
    registerIndex["29"] = 29;
    registerIndex["30"] = 30;
    registerIndex["31"] = 31;
    opcode["j"] = 2;
    opcode["jal"] = 3;
    opcode["beq"] = 4;
    opcode["bne"] = 5;
    opcode["addi"] = 8;
    opcode["addiu"] = 9;
    opcode["slti"] = 10;
    opcode["sltiu"] = 11;
    opcode["andi"] = 12;
    opcode["ori"] = 13;
    opcode["xori"] = 14;
    opcode["lui"] = 15;
    opcode["lb"] = 32;
    opcode["lh"] = 33;
    opcode["lwl"] = 34;
    opcode["lw"] = 35;
    opcode["lbu"] = 36;
    opcode["lhu"] = 37;
    opcode["lwr"] = 38;
    opcode["sb"] = 40;
    opcode["sh"] = 41;
    opcode["swl"] = 42;
    opcode["sw"] = 43;
    opcode["swr"] = 46;
    opcode["ll"] = 48;
    opcode["sc"] = 56;

    //registers = new QVector<int>(32,0);

}



int Assembler::minimum4(int n1, int n2, int n3, int n4)
{
    int min1 = std::min(n1, n2);
    int min2 = std::min(n3, n4);
    return std::min(min1, min2);
}

int Assembler::stringDistance(std::string s, std::string t){
    int i, j, cost, k, i1,j1,DB;
    int INFINITY = s.length() + t.length();
    QVector<int> DA(256);
    QVector< QVector<int> > dpMatrix(s.length()+2, QVector<int>(t.length()+2));
    for(i = 0; i < s.length()+1; i++) {
        dpMatrix[i+1][1] = i ;
        dpMatrix[i+1][0] = INFINITY;
    }
    for(j = 0; j<t.length()+1; j++) {
        dpMatrix[1][j+1] = j ;
        dpMatrix[0][j+1]= INFINITY;
    }
    for(k = 0; k < DA.size(); k++) DA[k] = 0;
    for(i = 1; i< s.length()+1; i++) {
        DB = 0;
        for(j = 1; j< t.length()+1; j++) {
            i1 = DA[t[j-1]];
            j1 = DB;
            cost = (s[i-1]!=t[j-1]);
            if(cost==0) DB = j;
            dpMatrix[i+1][j+1] =
                    minimum4(
                        dpMatrix[i][j]+cost,
                        dpMatrix[i+1][j] + 1,
                        dpMatrix[i][j+1]+1,
                        dpMatrix[i1][j1] + (i-i1-1) + 1 + (j-j1-1)
                    );
        }
        DA[s[i-1]] = i;
    }
    cost = dpMatrix[s.length()+1][t.length()+1];
    return cost;
}

void Assembler::exceptionHandler(int exceptionNumber)
{

}

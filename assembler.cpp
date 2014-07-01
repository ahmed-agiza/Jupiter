#include "Assembler.h"



// matches valid register names
QString registerRegex = "\\$((?:[12]?[\\d])|(?:3[012])|(?:zero)|(?:at)|(?:v[01])|(?:a[0-3])|(?:t\\d)|(?:s[0-7])|(?:k[01])|gp|fp|ra|sp)";
// invalid register name: matches $str where the str is not a valid register name
QString InvalidRegisterRegex = "\\$(?!(?:[12]?[\\d])|(?:3[012])|(?:zero)|(?:at)|(?:v[01])|(?:a[0-3])|(?:t\\d)|(?:s[0-7])|(?:k[01])|gp|fp|ra|sp)";
// Matches comments
QString commentRegex = "#.+";
// Matches labels
QString labelRegex = "\\b([a-zA-Z_]\\w*):";
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
QString registerFormat = "(" + labelRegex + "[ \t]*)?" + registerInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?";
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
QString memoryFormat = "(" + labelRegex + "[ \t]*)?" + memoryInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)[ \\t]*\\([ \\t]*" + registerRegex + "[ \\t]*\\)(?:[ \\t]+" + commentRegex + ")?";
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
QString immFormat = "(" + labelRegex + "[ \t]*)?" + immInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register, label
//      beq
//      bne

QString labelInstructions = "(beq|bne)";
QString labelFormat = "(" + labelRegex + "[ \t]*)?" + labelInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*([a-zA-Z_]\\w*)"+ "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, immediate
//      lui

QString singleimmInstructions = "(lui)";
QString singleimmFormat = "(" + labelRegex + "[ \t]*)?" + singleimmInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)" + "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register
//      jr
//      jalr
//      mfhi
//      mflo
//      mtlo
//      mthi

QString singleRegisterInstructions = "(jr|jalr|mfhi|mflo|mtlo|mthi)";
QString singleRegisterFormat = "(" + labelRegex + "[ \t]*)?" + singleRegisterInstructions + "[ \\t]+" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register
//      mult
//      multu
//      div
//      divu

QString doubleRegisterInstructions = "(mult|multu|div|divu)";
QString doubleRegisterFormat = "(" + labelRegex + "[ \t]*)?" + doubleRegisterInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction label
//      j
//      jal

QString jumpInstructions = "(j|jal)";
QString jumpFormat = "(" + labelRegex + "[ \t]*)?" + jumpInstructions + "[ \\t]+([a-zA-Z_]\\w*)" + "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction
//      syscall
//      nop

QString standaloneInstructions = "(" + labelRegex + "[ \t]*)?" + "(syscall|nop)"+ "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~





Assembler::Assembler(QStringList* stringList)
{
    QVector< QPair<int,QString> > missingBranchLabels;
    QVector< QPair<int,QString> > missingJumpLabels;
    initializeRegisters();
    int address = 0;
    //QStringList stringList = Text.document()->toPlainText().split('\n');
    QRegExp R(registerFormat), M(memoryFormat), I(immFormat), L(labelFormat), SR(singleRegisterFormat), SI(singleimmFormat), DR(doubleRegisterFormat), J(jumpFormat), SA(standaloneInstructions), LBL(labelRegex), CMT(commentRegex);
    foreach (QString line, *stringList) {
        if((R.indexIn(line, 0)) != -1){
            instructions.push_back(instruction(R.cap(2),&registers,opcode[R.cap(2)],registerIndex[R.cap(4)],registerIndex[R.cap(5)],registerIndex[R.cap(3)],0,0,RFormat));
            if(R.cap(1).size()) labels[R.cap(1)] = address;
        }else if((M.indexIn(line, 0)) != -1){
            instructions.push_back(instruction(M.cap(2),&registers,opcode[M.cap(2)],registerIndex[M.cap(5)],registerIndex[M.cap(3)],0,getNumber(M.cap(4)),0,IFormat));
            if(M.cap(1).size()) labels[M.cap(1)] = address;
        }else if((I.indexIn(line, 0)) != -1){
            instructions.push_back(instruction(I.cap(2),&registers,opcode[I.cap(2)],registerIndex[I.cap(4)],registerIndex[I.cap(3)],0,getNumber(I.cap(5)),getNumber(I.cap(5)),IFormat));
            if(I.cap(1).size()) labels[I.cap(1)] = address;
        }else if((L.indexIn(line, 0)) != -1){
            if(labels.contains(L.cap(5))){
                instructions.push_back(instruction(L.cap(2),&registers,opcode[L.cap(2)],registerIndex[I.cap(3)],registerIndex[I.cap(4)],0,labels[L.cap(5)]-address-1,0,IFormat));
            }else{
                missingBranchLabels.push_back(qMakePair(address,L.cap(5)));
                instructions.push_back(instruction(L.cap(2),&registers,opcode[L.cap(2)],registerIndex[I.cap(3)],registerIndex[I.cap(4)],0,0,0,IFormat));
            }if(L.cap(1).size()) labels[L.cap(1)] = address;
        }else if((SR.indexIn(line, 0)) != -1){
            instructions.push_back(instruction(SR.cap(2),&registers,opcode[SR.cap(2)],registerIndex[SR.cap(3)],0,registerIndex[SR.cap(3)],0,0,RFormat));
            if(SR.cap(1).size()) labels[SR.cap(1)] = address;
        }else if((SI.indexIn(line, 0)) != -1){
            instructions.push_back(instruction(SI.cap(2),&registers,opcode[SI.cap(2)],0,registerIndex[SI.cap(3)],0,getNumber(SI.cap(4)),0,IFormat));
            if(SI.cap(1).size()) labels[SI.cap(1)] = address;
        }else if((DR.indexIn(line, 0)) != -1){
            instructions.push_back(instruction(DR.cap(2),&registers,opcode[DR.cap(2)],registerIndex[DR.cap(3)],registerIndex[DR.cap(4)],0,0,0,RFormat));
            if(DR.cap(1).size()) labels[DR.cap(1)] = address;
        }else if((J.indexIn(line, 0)) != -1){
            if(labels.contains(J.cap(3))){
                instructions.push_back(instruction(J.cap(2),&registers,opcode[J.cap(2)],0,0,0,labels[L.cap(5)],0,JFormat));
            }else{
                missingJumpLabels.push_back(qMakePair(address,J.cap(3)));
                instructions.push_back(instruction(J.cap(2),&registers,opcode[J.cap(2)],0,0,0,0,0,JFormat));
            }if(J.cap(1).size()) labels[J.cap(1)] = address;
        }else if((SA.indexIn(line, 0)) != -1){
            instructions.push_back(instruction(SA.cap(2),&registers,opcode[SA.cap(2)],0,0,0,0,0,RFormat));
            if(SA.cap(1).size()) labels[SA.cap(1)] = address;
        }else if((LBL.indexIn(line, 0)) != -1){
            labels[LBL.cap(1)] = address;
            address--;
        }else if((CMT.indexIn(line, 0)) != -1){
            address--;
        }
        address++;
    }
    for (int i=0; i<missingBranchLabels.size(); i++){
        QPair<int,QString> lbl = missingBranchLabels[i];
        if(labels.contains(lbl.second)){
            instructions[lbl.first].setImm(labels[lbl.second]-lbl.first-1);
        } else {
            // missing label!!
        }
    }
    for (int i=0; i<missingJumpLabels.size(); i++){
        QPair<int,QString> lbl = missingJumpLabels[i];
        if(labels.contains(lbl.second)){
            instructions[lbl.first].setImm(labels[lbl.second]);
        } else {
            // missing label!!
        }
    }
}
Assembler::~Assembler(){}

Assembler::Assembler(){}


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
}


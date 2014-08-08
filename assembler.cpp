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
QString registerFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + registerInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?";
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
QString memoryFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + memoryInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)[ \\t]*\\([ \\t]*" + registerRegex + "[ \\t]*\\)(?:[ \\t]+" + commentRegex + ")?";
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
QString immFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + immInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register, label
//      beq
//      bne

QString labelInstructions = "(beq|bne)";
QString labelFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + labelInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*([a-zA-Z_]\\w*)"+ "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, immediate
//      lui

QString singleimmInstructions = "(lui)";
QString singleimmFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + singleimmInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)" + "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register
//      jr
//      jalr
//      mfhi
//      mflo
//      mtlo
//      mthi

QString singleRegisterInstructions = "(jr|jalr|mfhi|mflo|mtlo|mthi)";
QString singleRegisterFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + singleRegisterInstructions + "[ \\t]+" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register
//      mult
//      multu
//      div
//      divu

QString doubleRegisterInstructions = "(mult|multu|div|divu)";
QString doubleRegisterFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + doubleRegisterInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction label
//      j
//      jal

QString jumpInstructions = "(j|jal)";
QString jumpFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + jumpInstructions + "[ \\t]+([a-zA-Z_]\\w*)" + "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction
//      syscall
//      nop

QString standaloneInstructions = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + "(syscall|nop)"+ "(?:[ \\t]+" + commentRegex + ")?";
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
QString pLabelFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pLabelInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*([a-zA-Z_]\\w*)"+ "(?:[ \\t]+" + commentRegex + ")?";
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

QString pRILInstructions = "(blt|bgt|ble|bge|bltu|bgtu|bleu|bgeu)";
QString pRILFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pRILInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01][ \\t]*,[ \\t]*([a-zA-Z_]\\w*)"+ "(?:[ \\t]+" + commentRegex + ")?";
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
QString pZlabelFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pZlabelInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*([a-zA-Z_]\\w*)"+ "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, immediate
//  li
//  ror
//  rol

QString pSingleimmInstructions = "(li|ror|rol)";
QString pSingleimmFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pSingleimmInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)" + "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register
//  not
//  neg
//  move
//  abs

QString pDoubleRegisterInstructions = "(not|neg|move|abs)";
QString pDoubleRegisterFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pDoubleRegisterInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register, $register
//  mul
//  div
//  rem

QString pRegisterInstructions = "(mul|div|rem)";
QString pRegisterFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pRegisterInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register
//  clear

QString pSingleRegisterInstructions = "(clear)";
QString pSingleRegisterFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pSingleRegisterInstructions + "[ \\t]+" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register, immediate
//  subi

QString pImmInstructions = "(subi)";
QString pImmFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + pImmInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)(?:[ \\t]+" + commentRegex + ")?";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

QRegExp R(registerFormat, Qt::CaseInsensitive), M(memoryFormat, Qt::CaseInsensitive), I(immFormat, Qt::CaseInsensitive), L(labelFormat, Qt::CaseInsensitive), SR(singleRegisterFormat, Qt::CaseInsensitive), SI(singleimmFormat, Qt::CaseInsensitive), DR(doubleRegisterFormat, Qt::CaseInsensitive), J(jumpFormat, Qt::CaseInsensitive), SA(standaloneInstructions, Qt::CaseInsensitive), LBL(labelRegex, Qt::CaseInsensitive), CMT(commentRegex, Qt::CaseInsensitive), WHITSPACE("^[ \\t]+$", Qt::CaseInsensitive);
QRegExp PR(pRegisterFormat, Qt::CaseInsensitive), PRIL(pRILFormat, Qt::CaseInsensitive), PL(pLabelFormat, Qt::CaseInsensitive), PZ(pZlabelFormat, Qt::CaseInsensitive), PSI(pSingleimmFormat, Qt::CaseInsensitive), PDR(pDoubleRegisterFormat, Qt::CaseInsensitive), PSR(pSingleRegisterFormat, Qt::CaseInsensitive), PI(pImmFormat, Qt::CaseInsensitive);


Assembler::Assembler(QStringList* stringList)
{
    QVector< QPair<QPair<int,int>,QString> > missingBranchLabels;
    QVector< QPair<QPair<int,int>,QString> > missingJumpLabels;
    initializeRegisters();
    initializeFunctions();
    address = lineNumber = 0;
    //QStringList stringList = Text.document()->toPlainText().split('\n');
    foreach (QString line, *stringList)
    {
        if((R.indexIn(line, 0)) != -1)
        {
            instructions.push_back(instruction(R.cap(2),&registers,opcode[R.cap(2)],registerIndex[R.cap(4)],registerIndex[R.cap(5)],registerIndex[R.cap(3)],0,0,RFormat));
            //qDebug()<<R.cap(1)<<" "<<R.cap(2)<<" "<<R.cap(3)<<" "<<R.cap(4)<<" "<<R.cap(5)<<"\n";
            if(R.cap(1).size()) labels[R.cap(1)] = address;
        }
        else if((M.indexIn(line, 0)) != -1)
        {
            instructions.push_back(instruction(M.cap(2),&registers,opcode[M.cap(2)],registerIndex[M.cap(5)],registerIndex[M.cap(3)],0,getNumber(M.cap(4)),0,IFormat));
            if(M.cap(1).size()) labels[M.cap(1)] = address;
        }
        else if((I.indexIn(line, 0)) != -1)
        {
            instructions.push_back(instruction(I.cap(2),&registers,opcode[I.cap(2)],registerIndex[I.cap(4)],registerIndex[I.cap(3)],0,getNumber(I.cap(5)),getNumber(I.cap(5)),IFormat));
            if(I.cap(1).size() > 0) labels[I.cap(1)] = address;
        }
        else if((L.indexIn(line, 0)) != -1)
        {
            if(labels.contains(L.cap(5))){
                instructions.push_back(instruction(L.cap(2),&registers,opcode[L.cap(2)],registerIndex[I.cap(3)],registerIndex[I.cap(4)],0,labels[L.cap(5)]-address-1,0,IFormat));
            }
            else{
                missingBranchLabels.push_back(qMakePair(qMakePair(address,lineNumber),L.cap(5)));
                instructions.push_back(instruction(L.cap(2),&registers,opcode[L.cap(2)],registerIndex[I.cap(3)],registerIndex[I.cap(4)],0,0,0,IFormat));
            }
            if(L.cap(1).size()) labels[L.cap(1)] = address;
        }
        else if((SR.indexIn(line, 0)) != -1)
        {
            instructions.push_back(instruction(SR.cap(2),&registers,opcode[SR.cap(2)],registerIndex[SR.cap(3)],0,registerIndex[SR.cap(3)],0,0,RFormat));
            if(SR.cap(1).size()) labels[SR.cap(1)] = address;
        }
        else if((SI.indexIn(line, 0)) != -1)
        {
            instructions.push_back(instruction(SI.cap(2),&registers,opcode[SI.cap(2)],0,registerIndex[SI.cap(3)],0,getNumber(SI.cap(4)),0,IFormat));
            if(SI.cap(1).size()) labels[SI.cap(1)] = address;
        }
        else if((DR.indexIn(line, 0)) != -1)
        {
            instructions.push_back(instruction(DR.cap(2),&registers,opcode[DR.cap(2)],registerIndex[DR.cap(3)],registerIndex[DR.cap(4)],0,0,0,RFormat));
            if(DR.cap(1).size()) labels[DR.cap(1)] = address;
        }
        else if((J.indexIn(line, 0)) != -1)
        {
            if(labels.contains(J.cap(3))){
                qDebug()<<"Yes!";
                instructions.push_back(instruction(J.cap(2),&registers,opcode[J.cap(2)],0,0,0,labels[J.cap(3)],0,JFormat));
            }
            else{
                missingJumpLabels.push_back(qMakePair(qMakePair(address,lineNumber),J.cap(3)));
                instructions.push_back(instruction(J.cap(2),&registers,opcode[J.cap(2)],0,0,0,0,0,JFormat));
            }
            if(J.cap(1).size()) labels[J.cap(1)] = address;
        }
        else if((SA.indexIn(line, 0)) != -1)
        {
            instructions.push_back(instruction(SA.cap(2),&registers,opcode[SA.cap(2)],0,0,0,0,0,RFormat));
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
            errorList.push_back(Error("Invalid Syntax",lineNumber));
        }
        address++;
        lineNumber++;
    }
    for (int i=0; i<missingBranchLabels.size(); i++)
    {
        QPair<QPair<int,int>,QString> lbl = missingBranchLabels[i];
        if(labels.contains(lbl.second)){
            instructions[lbl.first.first].setImm(labels[lbl.second]-lbl.first.first-1);
        } else {
            errorList.push_back(Error("Label not found",lbl.first.second));
        }
    }
    for (int i=0; i<missingJumpLabels.size(); i++)
    {
        QPair<QPair<int,int>,QString> lbl = missingJumpLabels[i];
        if(labels.contains(lbl.second)){
            instructions[lbl.first.first].setImm(labels[lbl.second]);
        } else {
            errorList.push_back(Error("Label not found",lbl.first.second));
        }
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

    foreach(instruction ins,  instructions)
    {
        ins.setMem(mem);

        ins.setFunc(functionsMap[ins.getName()]);
        QObject::connect(&ins, SIGNAL(raiseException(int)), this, SLOT(exceptionHandler(int)));
    }
    //engine = new TileEngine(QWidget* parent, const QPoint& position, const QSize& size);
}
Assembler::~Assembler()
{
    delete mem;
    delete engine;
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

}

void Assembler::handlePR(QRegExp m, QString line)
{
    if(m.cap(2) == "mul")
    {

    }
    else if(m.cap(2) == "div")
    {

    }
    else if(m.cap(2) == "rem")
    {

    }
}
void Assembler::handlePRIL(QRegExp m, QString line)
{
    if(m.cap(2) == "blti")
    {

    }
    else if(m.cap(2) == "bgti")
    {

    }
    else if(m.cap(2) == "blei")
    {

    }
    else if(m.cap(2) == "bgei")
    {

    }
    else if(m.cap(2) == "bltiu")
    {

    }
    else if(m.cap(2) == "bgtiu")
    {

    }
    else if(m.cap(2) == "bleiu")
    {

    }
    else if(m.cap(2) == "bgeiu")
    {

    }
}
void Assembler::handlePL(QRegExp m, QString line)
{
    if(m.cap(2) == "blt")
    {

    }
    else if(m.cap(2) == "bgt")
    {

    }
    else if(m.cap(2) == "ble")
    {

    }
    else if(m.cap(2) == "bge")
    {

    }
    else if(m.cap(2) == "bltu")
    {

    }
    else if(m.cap(2) == "bgtu")
    {

    }
    else if(m.cap(2) == "bleu")
    {

    }
    else if(m.cap(2) == "bgeu")
    {

    }
}
void Assembler::handlePZ(QRegExp m, QString line)
{
    /*else*/ if(m.cap(2) == "beqz")
    {

    }
    else if(m.cap(2) == "bnez")
    {

    }
    else if(m.cap(2) == "bltz")
    {

    }
    else if(m.cap(2) == "bgez")
    {

    }
    else if(m.cap(2) == "bgtz")
    {

    }
    else if(m.cap(2) == "blez")
    {

    }
    else if(m.cap(2) == "la")
    {

    }
}
void Assembler::handlePSI(QRegExp m, QString line)
{
    if(m.cap(2) == "li")
    {

    }
    else if(m.cap(2) == "ror")
    {

    }
    else if(m.cap(2) == "rol")
    {

    }
}
void Assembler::handlePDR(QRegExp m, QString line)
{
    if(m.cap(2) == "not")
    {

    }
    else if(m.cap(2) == "neg")
    {

    }
    else if(m.cap(2) == "move")
    {

    }
    else if(m.cap(2) == "abs")
    {

    }
}
void Assembler::handlePSR(QRegExp m, QString line)
{
    if(m.cap(2) == "clear")
    {

    }
}
void Assembler::handlePI(QRegExp m, QString line)
{
    if(m.cap(2) == "subi")
    {

    }
}

//Assembler::~Assembler(){}
//>>>>>>> origin/master

Assembler::Assembler(){}


void Assembler::simulate()
{
    PC = 1;

    while (PC != -1 && PC <= instructions.size())
    {
        instructions[PC - 1].execute(PC);
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

    foreach (int r, registers) r = 0;
    registers[28] = 0x10008000;
    registers[29] = 0x7FFFEFFC;
}


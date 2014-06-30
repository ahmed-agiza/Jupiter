#include "assembler.h"

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
}

Assembler::Assembler(QTextEdit& Text)
{
    initializeRegisters();
    int address = 0;
    QStringList stringList = Text.document()->toPlainText().split('\n');
    QRegExp R(registerFormat), M(memoryFormat), I(immFormat), L(labelFormat), SR(singleRegisterFormat), SI(singleimmFormat), DR(doubleRegisterFormat), J(jumpFormat), SA(standaloneInstructions), LBL(labelRegex), CMT(commentRegex);
    foreach (QString line, stringList) {
        if((R.indexIn(line, 0)) != -1){
            instructions.push_back(instruction(R.cap(2),registerIndex[R.cap(4)],registerIndex[R.cap(5)],registerIndex[R.cap(3)],0,0));
            if(R.cap(1).size()) labels[R.cap(1)] = address;
        }else if((M.indexIn(line, 0)) != -1){
            instructions.push_back(instruction(M.cap(2),registerIndex[M.cap(5)],registerIndex[M.cap(3)],0,getNumber(M.cap(4)),0));
            if(M.cap(1).size()) labels[M.cap(1)] = address;
        }else if((I.indexIn(line, 0)) != -1){
            instructions.push_back(instruction(I.cap(2),registerIndex[I.cap(4)],registerIndex[I.cap(3)],0,getNumber(I.cap(5)),getNumber(I.cap(5))));
            if(I.cap(1).size()) labels[I.cap(1)] = address;
        }else if((L.indexIn(line, 0)) != -1){
            if(L.cap(1).size()) labels[L.cap(1)] = address;
        }else if((SR.indexIn(line, 0)) != -1){
            instructions.push_back(instruction(SR.cap(2),registerIndex[SR.cap(3)],0,registerIndex[SR.cap(3)],0,0));
            if(SR.cap(1).size()) labels[SR.cap(1)] = address;
        }else if((SI.indexIn(line, 0)) != -1){
            instructions.push_back(instruction(SI.cap(2),0,registerIndex[SI.cap(3)],0,getNumber(SI.cap(4)),0));
            if(SI.cap(1).size()) labels[SI.cap(1)] = address;
        }else if((DR.indexIn(line, 0)) != -1){
            instructions.push_back(instruction(DR.cap(2),registerIndex[DR.cap(3)],registerIndex(DR.cap(4)),0,0,0));
            if(DR.cap(1).size()) labels[DR.cap(1)] = address;
        }else if((J.indexIn(line, 0)) != -1){
            if(J.cap(1).size()) labels[J.cap(1)] = address;
        }else if((SA.indexIn(line, 0)) != -1){
            instructions.push_back(instruction(SA.cap(2),0,0,0,0,0));
        }else if((LBL.indexIn(line, 0)) != -1){
            labels[LBL.cap(1)] = address;
            address--;
        }else if((CMT.indexIn(line, 0)) != -1){
            address--;
        }
        address++;
    }
}

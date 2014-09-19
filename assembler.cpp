#include "Assembler.h"
#include "mainwindow.h"
#include <QDebug>
#include <iostream>
#include <QByteArray>
#include <math.h>

#ifndef SYSCALL_FUNCTIONS
#define SYSCALL_FUNCTIONS
#define PRINT_INTEGER 1
#define PRINT_FLOAT	2
#define PRINT_DOUBLE 3
#define PRINT_STRING 4
#define READ_INTEGER 5
#define READ_FLOAT 6
#define READ_DOUBLE 7
#define READ_STRING 8
#define MEMORY_ALLOCATION 9
#define EXIT 10
#define PRINT_CHARACTER	11
#define READ_CHARACTER 12
#endif


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
QString dataSegmentDirectives = "\\.(align|asciiz?|byte|double|float|half|space|word)";
// Matches invalid directives
QString invalidDirectivesRegex = "\\.(?!align|asciiz?|byte|data|double|float|globl|half|include|kdata|ktext|space|text|word)";
// Matches strings
QString cstringsRegex = "^\"[^\"\\\\]*(?:\\\\.[^\"\\\\]*)*\""; // "\".*[^\\\\]\"$";
QRegExp characterRegex ("^'((?:[^'\\\\\\n\\r]|(?:\\\\['nrtb0])))'$"); //
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
QString memoryFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + memoryInstructions + "[ \\t]+" + registerRegex + "[ \\t]*,[ \\t]*(0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+)?[ \\t]*\\([ \\t]*" + registerRegex + "[ \\t]*\\)(?:[ \\t]+" + commentRegex + ")?$";
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
//      mfhi
//      mflo
//      mtlo
//      mthi

QString singleRegisterInstructions = "(jr|mfhi|mflo|mtlo|mthi)";
QString singleRegisterFormat = "(?:(" + labelRegex + ")[ \\t]*:[ \\t]*)?" + singleRegisterInstructions + "[ \\t]+" + registerRegex + "(?:[ \\t]+" + commentRegex + ")?$";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//  instruction $register, $register
//      jalr
//      mult
//      multu
//      div
//      divu

QString doubleRegisterInstructions = "(mult|multu|div|divu|jalr)";
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

QString pZlabelInstructions = "(beqz|bnez|bltz|bgtz|blez|bgez|la)";
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

Assembler::Assembler(Memory *memory, QVector<int> * mRegisters, MainWindow * mainW)
{
    this->mainW = mainW;

    this->mem = memory;

    this->registers = mRegisters;
    totalCount = 0;
    currentProgress = 0;
    simulationSpeed = 0;

}

void Assembler::parseDataSegment(QStringList* stringList)
{
    "(align|asciiz?|byte|double|float|half|space|word)";
    QSet<QString> validDirectives;
    validDirectives.insert("align");
    validDirectives.insert("ascii");
    validDirectives.insert("asciiz");
    validDirectives.insert("byte");
    validDirectives.insert("double");
    validDirectives.insert("float");
    validDirectives.insert("half");
    validDirectives.insert("space");
    validDirectives.insert("word");
    lineNumber = 0;
    address = 0;

    QRegExp directivesRegex( "(?:[\\t ]*(" + labelRegex + "):[\\t ]*)?\\.([a-zA-Z]+)[ \\t]+([^\\r\\n]+)(?:#.+)?", Qt::CaseInsensitive);
    directivesRegex.setMinimal(false);
    foreach (QString line, *stringList)
    {
        emit progressUpdate(((float)currentProgress++/totalCount)*100);
        if(directivesRegex.indexIn(line, 0) != -1){
            QString directiveName = directivesRegex.cap(2);
            QString labelName = directivesRegex.cap(1);
            QString parameters = directivesRegex.cap(3);
            if(validDirectives.contains(directiveName)){
                if(directiveName == "align"){
                    int alignNumber = getNumber(parameters);
                    if(numberRegExp.indexIn(parameters) == 0 && alignNumber >= 0 && alignNumber <= 4){
                        int factor = std::pow(2, alignNumber);
                        address = ((address + factor - 1) / factor ) * factor;
                        if(labelName.size()){
                            if(dataLabels.contains(labelName)){
                                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
                            }
                            dataLabels[labelName] = address;
                        }
                    }else{
                        errorList.append(Error(".align directive must be followed by a number between 0 and 4", lineNumber));
                    }
                }else if(directiveName == "ascii" || directiveName == "asciiz"){
                    QRegExp cStrReg(cstringsRegex);
                    cStrReg.setMinimal(1);
                    if(cStrReg.indexIn(parameters) == 0){
                        parameters.replace("\\\"","\"");
                        parameters.replace("\\t","\t");
                        parameters.replace("\\n","\n");
                        parameters.replace("\\r","\r");
                        parameters.replace("\\b","\b");
                        parameters.replace("\\0","\0");
                        std::string actualString = parameters.mid(1,parameters.length()-2).toStdString();
                        int i;
                        for(i=address; i<actualString.size() + address; i++){
                            mem->storeByte(i + mem->dataSegmentBaseAddress , actualString[i]);
                        }
                        if(labelName.size()){

                            if(dataLabels.contains(labelName)){
                                errorList.append(Error("Label \"" + labelName + "\" is already defined.", lineNumber));
                            }
                            dataLabels[labelName] = address;

                        }
                        address = i;
                        if(directiveName.endsWith("z")) mem->storeByte(address++ + mem->dataSegmentBaseAddress , NULL);
                    }else if(QRegExp(invalidCstringsRegex).indexIn(parameters) == 0){
                        errorList.append(Error("Invalid string, missing closing quote", lineNumber));
                    }else{
                        errorList.append(Error("Invalid string", lineNumber));
                    }
                }else if(directiveName == "half" || directiveName == "word"){
                    parameters.remove(' ');
                    int alignNumber = ((directiveName == "half")? 1:2);
                    int factor = std::pow(2, alignNumber);
                    address = ((address + factor - 1) / factor ) * factor;
                    if(labelName.size()){
                        if(dataLabels.contains(labelName)){
                            errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
                        }
                        dataLabels[labelName] = address;
                    }
                    if(parameters.contains(':')){
                        int start = getNumber(parameters.mid(0,parameters.indexOf(':')));
                        int end = getNumber(parameters.mid(parameters.indexOf(':')+1));
                        if(end > 0xffff && directiveName == "half"){
                            errorList.append(Error("Half word value out of range. use .word instead of .half", lineNumber));
                        }
                        for(int i=0; i<(end-start+1); i++){
                            if (directiveName == "half")
                                mem->storeHWord(mem->dataSegmentBaseAddress + address, start + i);
                            else
                                mem->storeWord(mem->dataSegmentBaseAddress + address, start + i);
                            address += ((directiveName == "half")? 2:4);
                        }
                    }else if(parameters.contains(',')){
                        QStringList variables = parameters.remove(' ').remove('\t').split(',', QString::SkipEmptyParts);
                        foreach (QString immediateNumber, variables) {
                            if (directiveName == "half")
                                mem->storeHWord(mem->dataSegmentBaseAddress + address, getNumber(immediateNumber));
                            else
                                mem->storeWord(mem->dataSegmentBaseAddress + address, getNumber(immediateNumber));
                            address += ((directiveName == "half")? 2:4);
                            if(getNumber(immediateNumber) > 0xffff && directiveName == "half"){
                                errorList.append(Error("Half word value out of range. use .word instead of .half", lineNumber));
                            }
                        }
                    }else if(numberRegExp.indexIn(parameters) == 0){
                        if(getNumber(parameters) > 0xffff && directiveName == "half"){
                            errorList.append(Error("Half word value out of range. use .word instead of .half", lineNumber));
                        }
                        if (directiveName == "half")
                            mem->storeHWord(mem->dataSegmentBaseAddress + address, getNumber(parameters));
                        else
                            mem->storeWord(mem->dataSegmentBaseAddress + address, getNumber(parameters));
                        address += ((directiveName == "half")? 2:4);
                    }else{
                        errorList.append(Error("Syntax error", lineNumber));
                    }
                }else if(directiveName == "byte"){
                    if(labelName.size()){
                        if(dataLabels.contains(labelName)){
                            errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
                        }
                        dataLabels[labelName] = address;
                    }
                    if(parameters.contains(':')){
                        int start = getNumber(parameters.mid(0,parameters.indexOf(':')));
                        int end = getNumber(parameters.mid(parameters.indexOf(':')+1));
                        if(end > 0xff){
                            errorList.append(Error("Byte value out of range. use .half or .word instead of .byte", lineNumber));
                        }
                        for(int i=0; i<=(end-start+1); i++){
                            mem->storeByte(mem->dataSegmentBaseAddress + address, start + i);
                            address++;
                        }
                    }else if(parameters.contains(',')){
                        QStringList variables = parameters.remove(' ').remove('\t').split(',', QString::SkipEmptyParts);
                        foreach (QString immediateNumber, variables) {
                            if(characterRegex.indexIn(immediateNumber) == 0){
                                QString matchedChar = characterRegex.cap(1);
                                matchedChar.replace("\\'","'");
                                matchedChar.replace("\\t","\t");
                                matchedChar.replace("\\n","\n");
                                matchedChar.replace("\\r","\r");
                                matchedChar.replace("\\b","\b");
                                matchedChar.replace("\\0","\0");
                                mem->storeByte(mem->dataSegmentBaseAddress + address, matchedChar.toStdString()[0]);
                            }else if(numberRegExp.indexIn(immediateNumber) == 0)
                                mem->storeByte(mem->dataSegmentBaseAddress + address, getNumber(immediateNumber));
                            else
                                errorList.append(Error("Syntax error", lineNumber));
                            address++;
                            if(getNumber(immediateNumber) > 0xff){
                                errorList.append(Error("Byte value out of range. use .half or .word instead of .byte", lineNumber));
                            }
                        }
                    }else if(numberRegExp.indexIn(parameters) == 0){
                        if(getNumber(parameters) > 0xff){
                            errorList.append(Error("Byte value out of range. use .half or .word instead of .byte", lineNumber));
                        }
                        mem->storeByte(mem->dataSegmentBaseAddress + address, getNumber(parameters));
                        address++;
                    }else if(characterRegex.indexIn(parameters) == 0){
                        parameters.replace("\\'","'");
                        parameters.replace("\\t","\t");
                        parameters.replace("\\n","\n");
                        parameters.replace("\\r","\r");
                        parameters.replace("\\b","\b");
                        parameters.replace("\\0","\0");
                        mem->storeByte(mem->dataSegmentBaseAddress + address, parameters.toStdString()[1]);
                        address++;
                    }else{
                        errorList.append(Error("Syntax error", lineNumber));
                    }
                }else if(directiveName == "space"){
                    int spaceNumber = getNumber(parameters);
                    if(numberRegExp.indexIn(parameters) == 0){
                        if(labelName.size()){
                            if(dataLabels.contains(labelName)){
                                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
                            }
                            dataLabels[labelName] = address;
                        }
                        address += spaceNumber;
                    }else{
                        errorList.append(Error(".space directive must be followed by the number of bytes to reserve", lineNumber));
                    }
                }else if(directiveName == "float" || directiveName == "double"){
                    qDebug() << "FPU not yet implemented";
                }
            }else{
                errorList.push_back(Error("Invalid type specifier \""+directiveName+"\"", lineNumber));
            }
        }else if((CMT.indexIn(line, 0)) == -1 && WHITSPACE.indexIn(line, 0) == -1 && line.size() != 0){

        }
        lineNumber++;
    }

    for (int i = 0; i<errorList.size(); i++)
    {
        emit sendErrorMessage(errorList.at(i).lineNumber, errorList.at(i).description);
        //mainW->appendErrorMessage(QString::number(errorList.at(i).lineNumber) + " " + errorList.at(i).description);
        //qDebug() << errorList[i].lineNumber << " " << errorList[i].description;
    }

}

void Assembler::parseTextSegment(QStringList* stringList)
{
    address = lineNumber = 0;
    foreach (QString line, *stringList)
    {
        emit progressUpdate(((float)currentProgress++/totalCount)*100);
        if((R.indexIn(line, 0)) != -1)
        {
            instructions.push_back(Instruction(R.cap(2),registers,opcode[R.cap(2)],registerIndex[R.cap(4)],registerIndex[R.cap(5)],registerIndex[R.cap(3)],0,0,RFormat));
            //qDebug()<<R.cap(1)<<" "<<R.cap(2)<<" "<<R.cap(3)<<" "<<R.cap(4)<<" "<<R.cap(5)<<"\n";
            if(R.cap(1).size()){
                if(labels.contains(R.cap(1))){
                    errorList.append(Error("Label \""+R.cap(1)+"\" is already defined.", lineNumber));
                }
                labels[R.cap(1)] = address;
            }
        }
        else if((M.indexIn(line, 0)) != -1)
        {
            instructions.push_back(Instruction(M.cap(2),registers,opcode[M.cap(2)],registerIndex[M.cap(5)],registerIndex[M.cap(3)],0,(M.cap(4).size()? getNumber(M.cap(4)):0),0,IFormat));
            QString labelName = M.cap(1);
            if(M.cap(1).size()){
                if(labels.contains(labelName)){
                    errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
                }
                labels[labelName] = address;
            }
        }
        else if((I.indexIn(line, 0)) != -1)
        {
            if(I.cap(2) == "sll" || I.cap(2) == "srl" || I.cap(2) == "sra")
                instructions.push_back(Instruction(I.cap(2),registers,opcode[I.cap(2)],0,registerIndex[I.cap(4)],registerIndex[I.cap(3)],0,getNumber(I.cap(5)),RFormat));
            else
                instructions.push_back(Instruction(I.cap(2),registers,opcode[I.cap(2)],registerIndex[I.cap(4)],registerIndex[I.cap(3)],0,getNumber(I.cap(5)),0,IFormat));
            QString labelName = I.cap(1);
            if(labelName.size()){
                if(labels.contains(labelName)){
                    errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
                }
                labels[labelName] = address;
            }
        }
        else if((L.indexIn(line, 0)) != -1)
        {
            if(labels.contains(L.cap(5))){
                instructions.push_back(Instruction(L.cap(2),registers,opcode[L.cap(2)],registerIndex[L.cap(3)],registerIndex[L.cap(4)],0,labels[L.cap(5)]-address-1,0,IFormat));
            }
            else{
                missingBranchLabels.push_back(qMakePair(qMakePair(address,lineNumber),L.cap(5)));
                instructions.push_back(Instruction(L.cap(2),registers,opcode[L.cap(2)],registerIndex[L.cap(3)],registerIndex[L.cap(4)],0,0,0,IFormat));
            }
            QString labelName = L.cap(1);
            if(labelName.size()){
                if(labels.contains(labelName)){
                    errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
                }
                labels[labelName] = address;
            }
        }
        else if((SR.indexIn(line, 0)) != -1)
        {
            if(SR.cap(2) == "jr")
                instructions.push_back(Instruction(SR.cap(2),registers,opcode[SR.cap(2)],registerIndex[SR.cap(3)],0,0,0,0,RFormat));
            else
                instructions.push_back(Instruction(SR.cap(2),registers,opcode[SR.cap(2)],0,0,registerIndex[SR.cap(3)],0,0,RFormat));
            QString labelName = SR.cap(1);
            if(labelName.size()){
                if(labels.contains(labelName)){
                    errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
                }
                labels[labelName] = address;
            }
        }
        else if((SI.indexIn(line, 0)) != -1)
        {
            instructions.push_back(Instruction(SI.cap(2),registers,opcode[SI.cap(2)],0,registerIndex[SI.cap(3)],0,getNumber(SI.cap(4)),0,IFormat));
            QString labelName = SI.cap(1);
            if(labelName.size()){
                if(labels.contains(labelName)){
                    errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
                }
                labels[labelName] = address;
            }
        }
        else if((DR.indexIn(line, 0)) != -1)
        {
            if(DR.cap(2) == "jalr")
                instructions.push_back(Instruction(DR.cap(2),registers,opcode[DR.cap(2)],registerIndex[DR.cap(5)],0,registerIndex[DR.cap(3)],0,0,RFormat));
            else
                instructions.push_back(Instruction(DR.cap(2),registers,opcode[DR.cap(2)],registerIndex[DR.cap(3)],registerIndex[DR.cap(4)],0,0,0,RFormat));
            QString labelName = DR.cap(1);
            if(labelName.size()){
                if(labels.contains(labelName)){
                    errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
                }
                labels[labelName] = address;
            }
        }
        else if((J.indexIn(line, 0)) != -1)
        {
            if(labels.contains(J.cap(3))){
                instructions.push_back(Instruction(J.cap(2),registers,opcode[J.cap(2)],0,0,0,(labels[J.cap(3)]&0xfffffff),0,JFormat));
            }
            else{
                missingJumpLabels.push_back(qMakePair(qMakePair(address,lineNumber),J.cap(3)));
                instructions.push_back(Instruction(J.cap(2),registers,opcode[J.cap(2)],0,0,0,0,0,JFormat));
            }
            QString labelName = J.cap(1);
            if(labelName.size()){
                if(labels.contains(labelName)){
                    errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
                }
                labels[labelName] = address;
            }
        }
        else if((SA.indexIn(line, 0)) != -1)
        {
            instructions.push_back(Instruction(SA.cap(2),registers,opcode[SA.cap(2)],0,0,0,0,0,RFormat));
            QString labelName = SA.cap(1);
            if(labelName.size()){
                if(labels.contains(labelName)){
                    errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
                }
                labels[labelName] = address;
            }
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
        else if((CMT.indexIn(line, 0)) != -1 || WHITSPACE.indexIn(line, 0) != -1 || line.size()==0)
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
                }else if(instructionSet.search(instructionName.toStdString())){
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
                }else if(instructionSet.search(instructionName.toStdString())){
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
                }else if(instructionSet.search(instructionName.toStdString())){
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
                }else if(instructionSet.search(instructionName.toStdString())){
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
            errorList.push_back(Error("Label \""+lbl.second+"\" was not found",lbl.first.second));
        }
    }
    for (int i=0; i<missingJumpLabels.size(); i++)
    {
        QPair<QPair<int,int>,QString> lbl = missingJumpLabels[i];
        if(labels.contains(lbl.second)){
            instructions[lbl.first.first].setImm((labels[lbl.second]&0xfffffff));
        } else {
            errorList.push_back(Error("Label \""+lbl.second+"\" was not found",lbl.first.second));
        }
    }


    unsigned int addr = mem->textSegmentBaseAddress;
    for (int i = 0; i < instructions.size(); i++){
        instructions[i].setMem(mem);
        mem->storeWord(addr,instructions[i].getWord());
        addr += 4;
        QObject::connect(&(instructions[i]), SIGNAL(raiseException(int)), this, SLOT(exceptionHandler(int)));
    }


    for (int i = 0; i<errorList.size(); i++)
    {
        emit sendErrorMessage(errorList.at(i).lineNumber, errorList.at(i).description);
        qDebug() << errorList[i].lineNumber << " " << errorList[i].description;
    }

    getLineMapping();

    emit assemblyComplete();
}

Assembler::~Assembler()
{}


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
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
        address++;
        instructions[instructions.size()-1].setFromAssembler(true);
    }
    else if(m.cap(2) == "div")
    {
        instructions.push_back(Instruction("div",registers,opcode["div"],registerIndex[m.cap(4)],registerIndex[m.cap(5)],0,0,0,RFormat));
        instructions.push_back(Instruction("mflo",registers,opcode["mflo"],0,0,registerIndex[SR.cap(3)],0,0,RFormat));
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
        address++;
        instructions[instructions.size()-1].setFromAssembler(true);
    }
    else if(m.cap(2) == "rem")
    {
        instructions.push_back(Instruction("div",registers,opcode["div"],registerIndex[m.cap(4)],registerIndex[m.cap(5)],0,0,0,RFormat));
        instructions.push_back(Instruction("mfhi",registers,opcode["mfhi"],0,0,registerIndex[SR.cap(3)],0,0,RFormat));
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
        address++;
        instructions[instructions.size()-1].setFromAssembler(true);
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
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
        address++;
        instructions[instructions.size()-1].setFromAssembler(true);
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
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
        address += 2;
        instructions[instructions.size()-2].setFromAssembler(true);
        instructions[instructions.size()-1].setFromAssembler(true);
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
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
        address += 2;
        instructions[instructions.size()-2].setFromAssembler(true);
        instructions[instructions.size()-1].setFromAssembler(true);
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
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
        address++;
        instructions[instructions.size()-1].setFromAssembler(true);
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
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
        address++;
        instructions[instructions.size()-1].setFromAssembler(true);
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
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
        address += 2;
        instructions[instructions.size()-2].setFromAssembler(true);
        instructions[instructions.size()-1].setFromAssembler(true);
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
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
        address += 2;
        instructions[instructions.size()-2].setFromAssembler(true);
        instructions[instructions.size()-1].setFromAssembler(true);
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
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
        address++;
        instructions[instructions.size()-1].setFromAssembler(true);
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
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
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
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
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
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
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
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
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
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
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
        QString labelName = m.cap(1);
        if(labelName.size()){
            if(labels.contains(labelName)){
                errorList.append(Error("Label \""+labelName+"\" is already defined.", lineNumber));
            }
            labels[labelName] = address;
        }
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
    instructions[instructions.size()-1].setFromAssembler(true);
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
        instructions[instructions.size()-1].setFromAssembler(true);
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
        instructions[instructions.size()-1].setFromAssembler(true);
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
        instructions[instructions.size()-1].setFromAssembler(true);
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
        instructions[instructions.size()-1].setFromAssembler(true);
    }
    else if(m.cap(2) == "la")
    {
        QString datalbl = m.cap(4);
        if(dataLabels.contains(datalbl)){
            int addrrr = mem->dataSegmentBaseAddress+(labels[datalbl]<<2);
            instructions.push_back(Instruction("lui",registers,opcode["lui"],0,registerIndex[m.cap(3)],0, addrrr >> 16,0,IFormat));
            instructions.push_back(Instruction("ori",registers,opcode["ori"],registerIndex[m.cap(3)],registerIndex[m.cap(3)],0,addrrr & 0xffff,0,IFormat));
        }else if(labels.contains(datalbl)){
            int addrrr = mem->textSegmentBaseAddress+(labels[datalbl]<<2);
            instructions.push_back(Instruction("lui",registers,opcode["lui"],0,registerIndex[m.cap(3)],0, addrrr >> 16,0,IFormat));
            instructions.push_back(Instruction("ori",registers,opcode["ori"],registerIndex[m.cap(3)],registerIndex[m.cap(3)],0,addrrr & 0xffff,0,IFormat));
        }else{
            errorList.push_back(Error("Invalid label \""+ datalbl +"\".", lineNumber));
        }
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address++;
        instructions[instructions.size()-1].setFromAssembler(true);
    }
}

void Assembler::handlePSI(QRegExp m, QString line)
{
    if(m.cap(2) == "li")
    {
        int numberToLoad = getNumber(m.cap(4));
        if(numberToLoad > 0xffff){
            instructions.push_back(Instruction("lui",registers,opcode["lui"],0,registerIndex[m.cap(3)],0,numberToLoad >> 16,0,IFormat));
            instructions.push_back(Instruction("ori",registers,opcode["ori"],registerIndex[m.cap(3)],registerIndex[m.cap(3)],0,numberToLoad & 0xffff,0,IFormat));
            if(m.cap(1).size()) labels[m.cap(1)] = address;
            address++;
            instructions[instructions.size()-1].setFromAssembler(true);
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
        instructions[instructions.size()-2].setFromAssembler(true);
        instructions[instructions.size()-1].setFromAssembler(true);
    }
    else if(m.cap(2) == "rol")
    {
        instructions.push_back(Instruction("sll",registers,opcode["sll"],0,registerIndex[m.cap(3)],registerIndex["at"],0,getNumber(m.cap(4)),RFormat));
        instructions.push_back(Instruction("srl",registers,opcode["srl"],0,registerIndex[m.cap(3)],registerIndex[m.cap(3)],0,32 - getNumber(m.cap(4)),RFormat));
        instructions.push_back(Instruction("or" ,registers,opcode["or" ],registerIndex[m.cap(3)],registerIndex["at"],registerIndex[m.cap(3)],0,0,RFormat));
        if(m.cap(1).size()) labels[m.cap(1)] = address;
        address += 2;
        instructions[instructions.size()-2].setFromAssembler(true);
        instructions[instructions.size()-1].setFromAssembler(true);
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
        instructions[instructions.size()-2].setFromAssembler(true);
        instructions[instructions.size()-1].setFromAssembler(true);
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

void Assembler::handlePI(QRegExp m, QString line){
    if(m.cap(2) == "subi")
    {
        instructions.push_back(Instruction("addi",registers,opcode["addi"],registerIndex[m.cap(4)],registerIndex[m.cap(3)],0,-getNumber(m.cap(5)),getNumber(m.cap(5)),IFormat));
        if(m.cap(1).size() > 0) labels[m.cap(1)] = address;
    }
}

void Assembler::setSimulationSpeed(int speed){
    simulationSpeed = speed;
}

void Assembler::setLineMapping(QMap<int, int> mapping){
    lineMapping = mapping;
}

void Assembler::setRawList(QStringList list){
    rawLines = list;
}

void Assembler::getLineMapping(){
    if (rawLines.isEmpty() || strippedInstrs.isEmpty()){
        lineMapping.clear();
        return;
    }
    lineMapping.clear();

    int strippedIterator = 0;
    int instrIterator = 0;
    int rawIterator = 0;

    for (strippedIterator = 0; strippedIterator < strippedInstrs.size(); strippedIterator++){
        if (instrIterator >= instructions.size()){
            qDebug() << "Out of " << instructions.size() << " range " << instrIterator << "  " << strippedIterator
                        << "\n" << "Line: " << strippedInstrs.at(strippedIterator) << "  " << instructions.at(instrIterator - 1).getName();
            break;
        }
        if (instructions.at(instrIterator).isFromAssembler()){
            instrIterator++;
            strippedIterator--;
            continue;
        }
        for (rawIterator = strippedIterator; rawIterator < rawLines.size(); rawIterator++){
            if (strippedInstrs.at(strippedIterator) == rawLines.at(rawIterator).trimmed()){
                lineMapping[instrIterator] = rawIterator;
                break;
            }
        }
        instrIterator++;
    }
}


Assembler::Assembler(){

}

int Assembler::numberOfErrors(){
    return errorList.size();
}

inline void Assembler::executeFunction()
{
    if (waiting)
        return;

    if (simulationSpeed > 0)
        thread()->msleep(simulationSpeed);

    activePC = PC/4;
    emit executingInstruction(activePC);
    if (lineMapping.contains(activePC)){
        if (!instructions.at(activePC).isFromAssembler())
          emit executingLine(lineMapping[activePC]);
    }

    if (instructions[activePC].getName() == "syscall"){
        int functionNumber = (*registers)[2];
        QString msg;
        QString charac;
        QByteArray strArray;
        QByteArray strByte(1, ' ');
        int offset;
        int baseAddress;
        switch(functionNumber){
        case 0:
            qDebug() << "Invalid function value in $v0 for the syscall";
            PC += 4;
            break;
        case PRINT_INTEGER:
            msg = QString::number((*registers)[4]);
            emit printToConsole(msg);
            PC += 4;
            break;
        case PRINT_FLOAT:
            qDebug() << "FPU instructions are not implemented";
            PC += 4;
            break;
        case PRINT_DOUBLE:
            qDebug() << "FPU instructions are not implemented";
            PC += 4;
            break;
        case PRINT_STRING:
            //Printing String.
            msg = "";
            offset = 1;
            baseAddress = (*registers)[4];
            strArray.resize(1);
            strArray[0] = (char) mem->loadByte(baseAddress);
            while (strArray[0] != '\0' && offset < 100){
                charac = QString::fromLatin1(strArray);
                strArray [0] = (char) mem->loadByte(baseAddress + offset++);
                msg.append(charac);
            }
            emit printToConsole(msg);
            PC += 4;
            break;
        case READ_INTEGER:
            waiting = true;
            emit inputRequired(functionNumber);
            PC += 4;
            break;
        case READ_FLOAT:
            qDebug() << "FPU instructions are not implemented";
            PC += 4;
            break;
        case READ_DOUBLE:
            qDebug() << "FPU instructions are not implemented";
            PC += 4;
            break;
        case READ_STRING:
            waiting = true;
            emit setReadingLimit((*registers)[5]);
            emit inputRequired(functionNumber);
            PC += 4;
            break;
        case MEMORY_ALLOCATION:
            qDebug() << "Allocate heap memory";
            //Allocate Heap Memory
            PC += 4;
            break;
        case EXIT:
            exitExec = true;
            break;
        case PRINT_CHARACTER:
            qDebug() << "Print character";
            //Print character

            strByte[0] = (char) mem->loadByte((*registers)[4]);
            msg = QString::fromLatin1(strByte);
            emit printToConsole(msg);
            PC += 4;
            break;
        case READ_CHARACTER:
            qDebug() << "Read character";
            //Read character
            emit inputRequired(functionNumber);
            waiting = true;
            PC += 4;
            break;
        default:
            qDebug() << "Unhandled syscall function number " << (*registers)[1];
            PC += 4;
            break;
        }

    }
    instructions[activePC].setFunc(functionsMap[instructions[activePC].getName().trimmed()]);
    instructions[activePC].execute(PC);
    (*registers)[0] = 0;
    (*registers)[34] = PC;
    emit instructionExecuted();

}

void Assembler::simulate()
{

    QStringList logData;
    if (!resumeFlag){
        PC = 0;
        activePC = 0;
        exitExec = false;
        emit setReadingLimit(-1);
    }
    int i = 0;
    while (PC != -1 && ((PC/4) < instructions.size() && !exitExec /*&& i < 150*/))
    {
        if(mainW->isGFXEnabled()){
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
                        Assembler::buttonPressed(R_KEY_INDEX,0, true);
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
        }

        if (waiting || mainW->isSimulationPaused())
            break;

        executeFunction();

        i++;

    }
    if (!waiting){
        emit simulationComplete();
        emit logDataSignal(logData);
    }

}

void Assembler::resumeSimulation(){
    resumeFlag = true;
    simulate();
}

void Assembler::readInt(int value){
    qDebug() << "Received " << value;
    (*registers)[2] = value;
    waiting = false;
    resumeFlag = true;
    simulate();
}

void Assembler::readString(QString input){
    int baseAdress = (*registers)[4];
    for (int i = 0; i < input.length(); i++){
        mem->storeByte(baseAdress + i, input.at(i).toLatin1());
    }
    mem->storeByte(input.length(), '\0');
    waiting = false;
    resumeFlag = true;
    simulate();

}

void Assembler::readCharacter(QString charc){
    qDebug() << "Got char" << charc;
    if (charc.length() >= 1)
        (*registers)[2] = (int) charc.at(0).toLatin1();
    else
        (*registers)[2] = 0;
    waiting = false;
    resumeFlag = true;
    simulate();
}







int Assembler::getNumber(QString s){
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

    for (int i = 0; i < registers->size(); i++)
        (*registers)[i] = 0;
    (*registers)[28] = 0x10008000;
    (*registers)[29] = 0x7FFFEFFC;


}



int Assembler::minimum4(int n1, int n2, int n3, int n4)
{
    int min1 = std::min(n1, n2);
    int min2 = std::min(n3, n4);
    return std::min(min1, min2);
}

void Assembler::reset(){
    totalCount = 0;
    currentProgress = 0;
    simulationSpeed = 0;
    initializeRegisters();
    initializeFunctions();

    totalCount = 0;
    currentProgress = 0;
    PC = 0;
    activePC = 0;

    waiting = false;
    resumeFlag = false;
    exitExec = false;

    strippedInstrs.clear();
    lineMapping.clear();
    rawLines.clear();
    instructions.clear();
    errorList.clear();
    labels.clear();
    dataLabels.clear();

    mem->clearAll();

}

int Assembler::stringDistance(std::string s, std::string t){
    int i, j, cost, k, i1,j1,DB;
    int _INFINITY = s.length() + t.length();
    QVector<int> DA(256);
    QVector< QVector<int> > dpMatrix(s.length()+2, QVector<int>(t.length()+2));
    for(i = 0; i < s.length()+1; i++) {
        dpMatrix[i+1][1] = i ;
        dpMatrix[i+1][0] = _INFINITY;
    }
    for(j = 0; j<t.length()+1; j++) {
        dpMatrix[1][j+1] = j ;
        dpMatrix[0][j+1]= _INFINITY;
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

void Assembler::exceptionHandler(int exceptionNumber){
    qDebug() << "Exception: " << exceptionNumber;

}

void Assembler::assemble(QStringList dataFileStringList, QStringList textFileStringList){
    initializeRegisters();
    initializeFunctions();

    connect(this,SIGNAL(buttonPressed(int,int,bool)),mem, SLOT(updateKey(int, int, bool)));
    totalCount = dataFileStringList.size() + textFileStringList.size();
    currentProgress = 0;
    PC = 0;
    activePC = 0;

    waiting = false;
    resumeFlag = false;

    strippedInstrs = textFileStringList;

    mem->clearAll();

    parseDataSegment(&dataFileStringList);
    parseTextSegment(&textFileStringList);
}

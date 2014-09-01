#include "fileloader.h"
#include "ui_fileloader.h"
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
FileLoader::FileLoader(MainWindow *parent, FileDialogType type) :
    QDialog(parent),
    ui(new Ui::FileLoader)
{
    ui->setupUi(this);
    dialogType = type;
    this->setModal(true);
    this->setFixedSize(this->width(), this->height());
    p = parent;
    /*
    QHBoxLayout *layout = new QHBoxLayout(this);

    QToolButton* btnText = new QToolButton(this);
    btnText->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnText->setIcon(QIcon(":/icons/explorer/icons/explorer/textFileIcon.png"));
    btnText->setText("Text Segment File");
    btnText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QToolButton* btnData = new QToolButton(this);
    btnData->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnData->setIcon(QIcon(":/icons/explorer/icons/explorer/dataFileIcon.png"));
    btnData->setText(" Data Segment File");
    btnData->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QToolButton* btnRes = new QToolButton(this);
    btnRes->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnRes->setIcon(QIcon(":/icons/explorer/icons/explorer/binaryIcon.png"));
    btnRes->setText("Resource File");
    btnRes->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->addWidget(btnText);
    layout->addWidget(btnData);
    layout->addWidget(btnRes);
    QWidget *firstTempWidget = new QWidget(this);
    firstTempWidget->setLayout(layout);

    /*QHBoxLayout *hlayout = new QHBoxLayout(this);

    QToolButton *btnCancel = new QToolButton(this);
    btnCancel->setText("Cancel");
    hlayout->addWidget(btnCancel);
    QWidget *secondTempWidget = new QWidget(this);
    secondTempWidget->setLayout(hlayout);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addWidget(firstTempWidget);
    vlayout->addWidget(ui->btnCancel);
    this->setLayout(vlayout);*/
    //this->setFixedWidth(btnRes->width() + btnText->width() + btnData->width());
    //this->setFixedHeight(btnText->height() + btnText->iconSize().width() + 20);*/

    if(dialogType == CREATE_FILE){
        this->setWindowTitle("Creat New File");
        ui->btnRes->setEnabled(false);
    }else
        setWindowTitle("Add Existing File");



    QRect screenGeo = QApplication::desktop()->screenGeometry(0);
    QPoint centerPoint = screenGeo.center();
    centerPoint.setX(centerPoint.x() - this->width()/2);
    centerPoint.setY(centerPoint.y() - this->height()/2);
    move(centerPoint);

}

FileLoader::~FileLoader()
{
    delete ui;
}

void FileLoader::on_btnText_clicked()
{

}

void FileLoader::on_btnData_clicked()
{

}

void FileLoader::on_btnRes_clicked()
{

}

#include "reconfigureprojectdialog.h"
#include "ui_reconfigureprojectdialog.h"

ReconfigureProjectDialog::ReconfigureProjectDialog(MainWindow *parent, QString title, bool enableGF, bool littleEndian, int w, int h) :
    QDialog(parent),
    ui(new Ui::ReconfigureProjectDialog), p(parent)
{
    ui->setupUi(this);
    ui->lblTitle->setBuddy(ui->txtTitle);
    ui->lblTileHeight->setBuddy(ui->cmbTileHeight);
    ui->lblTileWidth->setBuddy(ui->cmbTileWidth);

    ui->txtTitle->setText(title);
    ui->chkEnableGFX->setChecked(enableGF);
    ui->radLEndian->setChecked(littleEndian);
    ui->radBEndian->setChecked(!littleEndian);
    ui->txtTitle->setText(title);
    ui->cmbTileHeight->setCurrentText(QString::number(h));
    ui->cmbTileWidth->setCurrentText(QString::number(w));

    QObject::connect(ui->chkEnableGFX, SIGNAL(toggled(bool)), this, SLOT(enableGFXWidgets()));
}

void ReconfigureProjectDialog::setParams(QString title, bool enableGF, bool littleEndian, int w, int h){
    ui->txtTitle->setText(title);
    ui->chkEnableGFX->setChecked(enableGF);
    ui->radLEndian->setChecked(littleEndian);
    ui->radBEndian->setChecked(!littleEndian);
    ui->txtTitle->setText(title);
    ui->cmbTileHeight->setCurrentText(QString::number(h));
    ui->cmbTileWidth->setCurrentText(QString::number(w));
}

QString ReconfigureProjectDialog::getTitle() const{
    return ui->txtTitle->text().trimmed();
}

bool ReconfigureProjectDialog::getLEndianState() const{
    return ui->radLEndian->isChecked();
}

bool ReconfigureProjectDialog::getGFXState() const{
    return ui->chkEnableGFX->isChecked();
}

int ReconfigureProjectDialog::getTileWidth() const{
    return ui->cmbTileWidth->currentText().trimmed().toInt();
}

int ReconfigureProjectDialog::getTileHeight() const{
    return ui->cmbTileHeight->currentText().trimmed().toInt();
}

void ReconfigureProjectDialog::enableGFXWidgets(){
    ui->cmbTileHeight->setEnabled(ui->chkEnableGFX->isChecked());
    ui->cmbTileWidth->setEnabled(ui->chkEnableGFX->isChecked());
}


ReconfigureProjectDialog::~ReconfigureProjectDialog()
{
    delete ui;
}


void ReconfigureProjectDialog::on_btnCancel_clicked(){
    hide();
}

void ReconfigureProjectDialog::on_btnSave_clicked(){
    QString projectTitle = ui->txtTitle->text().trimmed();
    if (projectTitle == ""){
        QMessageBox::critical(this, "Error", "Project title cannot be empty");
        return;
    }

    QRegExp fileValidator("^[A-Za-z0-9_\\@\\$.\\s]*$");
    if (fileValidator.indexIn(projectTitle) == -1){
        QMessageBox::critical(this, "Invalid Project Title", "The project title contains invalid characters");
        return;
    }
    if (ui->radBEndian->isChecked() == false && ui->radLEndian->isChecked() == false){
        QMessageBox::critical(this, "Error", "Select the endiannesss");
        return;
    }

    setResult(QDialog::Accepted);

    hide();


}


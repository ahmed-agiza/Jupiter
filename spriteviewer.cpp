#include "spriteviewer.h"
#include "ui_spriteviewer.h"

SpriteViewer::SpriteViewer(QWidget *parent, Memory *memory) :
    QDialog(parent),
    ui(new Ui::SpriteViewer)
{
    ui->setupUi(this);
    this->memory = memory;
    this->setWindowTitle("Sprite Editor");

    spriteRenderWindow = new SpriteRenderWindow(this,this->memory);
    ui->scrollArea->setWidget(spriteRenderWindow);
    connect(this, SIGNAL(renderScreen()), spriteRenderWindow, SLOT(repaint()));

    updateDisplay();
}

SpriteViewer::~SpriteViewer()
{
    delete ui;
}

void SpriteViewer::on_sizeTilesPushButton_clicked()
{
    if(ui->sizeTilesPushButton->text() == "1"){
        ui->sizeTilesPushButton->setText("2");
        memory->spriteRam[ui->spriteIndex->text().toUInt()].setSpriteSize(1);
    }else{
        ui->sizeTilesPushButton->setText("1");
        memory->spriteRam[ui->spriteIndex->text().toUInt()].setSpriteSize(0);
    }
    emit renderScreen();
}

void SpriteViewer::setViewOnly(bool viewOnly)
{
    ui->attachementOrientationPushButton->setDisabled(viewOnly);
    ui->layerSpinBox->setDisabled(viewOnly);
    ui->positionXlineEdit->setDisabled(viewOnly);
    ui->positionYlineEdit->setDisabled(viewOnly);
    ui->renderingEnabledCheckbox->setDisabled(viewOnly);
    ui->scaleXPushButton->setDisabled(viewOnly);
    ui->scaleYPushButton->setDisabled(viewOnly);
    ui->sizeTilesPushButton->setDisabled(viewOnly);
    ui->tile0FlippingCheckbox->setDisabled(viewOnly);
    ui->tile0FlippingYCheckbox->setDisabled(viewOnly);
    ui->tile1FlippingCheckbox->setDisabled(viewOnly);
    ui->tile1FlippingYCheckbox->setDisabled(viewOnly);
    ui->tile0Index->setDisabled(viewOnly);
    ui->tile0RotateLeft_PushButton->setDisabled(viewOnly);
    ui->tile0RotateRight_PushButton->setDisabled(viewOnly);
    ui->tile1Index->setDisabled(viewOnly);
    ui->tile1RotateLeft_PushButton->setDisabled(viewOnly);
    ui->tile1RotateRight_PushButton->setDisabled(viewOnly);
}

void SpriteViewer::on_attachementOrientationPushButton_clicked()
{
    if(ui->attachementOrientationPushButton->text() == "Vertical"){
        memory->spriteRam[ui->spriteIndex->text().toUInt()].setAttachementOrientation(1);
        ui->attachementOrientationPushButton->setText("Horizontal");
    }else{
        memory->spriteRam[ui->spriteIndex->text().toUInt()].setAttachementOrientation(0);
        ui->attachementOrientationPushButton->setText("Vertical");
    }
    emit renderScreen();
}

void SpriteViewer::on_renderingEnabledCheckbox_clicked(bool value)
{
    memory->spriteRam[ui->spriteIndex->text().toUInt()].setRenderingMode(value);
}

void SpriteViewer::on_spriteIndex_valueChanged(int arg1)
{
    spriteRenderWindow->setSpriteIndex(arg1);
    updateDisplay();
    emit renderScreen();
}

void SpriteViewer::on_tile0Index_valueChanged(int arg1)
{
    memory->spriteRam[ui->spriteIndex->value()].setTileIndex(0, arg1);
    emit renderScreen();
}

void SpriteViewer::on_tile1Index_valueChanged(int arg1)
{
    memory->spriteRam[ui->spriteIndex->value()].setTileIndex(1, arg1);
    emit renderScreen();
}

void SpriteViewer::updateDisplay()
{
    ui->sizeTilesPushButton->setText(QString::number(memory->spriteRam[ui->spriteIndex->value()].isTwoTiles()+1));
    QString attOrient[2] = {"Vertical", "Horizontal"};
    ui->attachementOrientationPushButton->setText(attOrient[memory->spriteRam[ui->spriteIndex->value()].isAttachedHorizontally()]);
    ui->tile0Index->setValue(memory->spriteRam[ui->spriteIndex->value()].getTileIndex(0));
    ui->tile1Index->setValue(memory->spriteRam[ui->spriteIndex->value()].getTileIndex(1));

    ui->tile0FlippingCheckbox->setChecked(memory->spriteRam[ui->spriteIndex->value()].getTile0FlipX());
    ui->tile0FlippingYCheckbox->setChecked(memory->spriteRam[ui->spriteIndex->value()].getTile0FlipY());
    ui->tile1FlippingCheckbox->setChecked(memory->spriteRam[ui->spriteIndex->value()].getTile1FlipX());
    ui->tile0FlippingYCheckbox->setChecked(memory->spriteRam[ui->spriteIndex->value()].getTile1FlipY());

    ui->layerSpinBox->setValue(memory->spriteRam[ui->spriteIndex->value()].getLayer());
    ui->renderingEnabledCheckbox->setChecked(memory->spriteRam[ui->spriteIndex->value()].shouldRender());
    ui->scaleXPushButton->setText(memory->spriteRam[ui->spriteIndex->value()].getScaleX()? "2":"1");
    ui->scaleYPushButton->setText(memory->spriteRam[ui->spriteIndex->value()].getScaleY()? "2":"1");

    ui->positionXlineEdit->setText(QString::number(memory->spriteRam[ui->spriteIndex->value()].getPositionX()));
    ui->positionYlineEdit->setText(QString::number(memory->spriteRam[ui->spriteIndex->value()].getPositionY()));

    QString rot[4] = {"0","270","90","180"};
    ui->tile0RotatonLabel->setText(rot[memory->spriteRam[ui->spriteIndex->value()].getTile0Rotation()]);
    ui->tile1RotatonLabel->setText(rot[memory->spriteRam[ui->spriteIndex->value()].getTile1Rotation()]);
}

void SpriteViewer::on_tile0FlippingCheckbox_clicked(bool checked)
{
    memory->spriteRam[ui->spriteIndex->value()].setTile0FlipX(checked);
    emit renderScreen();
}

void SpriteViewer::on_tile1FlippingCheckbox_clicked(bool checked)
{
    memory->spriteRam[ui->spriteIndex->value()].setTile1FlipX(checked);
    emit renderScreen();
}

void SpriteViewer::on_tile0FlippingYCheckbox_clicked(bool checked)
{
    memory->spriteRam[ui->spriteIndex->value()].setTile0FlipY(checked);
    emit renderScreen();
}

void SpriteViewer::on_tile1FlippingYCheckbox_clicked(bool checked)
{
    memory->spriteRam[ui->spriteIndex->value()].setTile1FlipY(checked);
    emit renderScreen();
}

void SpriteViewer::on_layerSpinBox_valueChanged(int arg1)
{
    memory->spriteRam[ui->spriteIndex->value()].setLayer(arg1);
}

void SpriteViewer::on_scaleXPushButton_clicked()
{
    memory->spriteRam[ui->spriteIndex->value()].setScaleX(ui->scaleXPushButton->text()=="2");
    ui->scaleXPushButton->setText(ui->scaleXPushButton->text()=="2"? "1":"2");
}

void SpriteViewer::on_scaleYPushButton_clicked()
{
    memory->spriteRam[ui->spriteIndex->value()].setScaleY(ui->scaleYPushButton->text()=="2");
    ui->scaleXPushButton->setText(ui->scaleYPushButton->text()=="2"? "1":"2");
}

void SpriteViewer::on_positionXlineEdit_editingFinished()
{
    memory->spriteRam[ui->spriteIndex->value()].setPositionX(ui->positionXlineEdit->text().toInt());
}

void SpriteViewer::on_positionYlineEdit_editingFinished()
{
    memory->spriteRam[ui->spriteIndex->value()].setPositionY(ui->positionYlineEdit->text().toInt());
}

void SpriteViewer::on_tile0RotateLeft_PushButton_clicked()
{
    GameSprite& gameSprite = memory->spriteRam[ui->spriteIndex->value()];
    switch(gameSprite.getTile0Rotation()){
    case 0:
        // 1
        gameSprite.setTile0Rotation(1);
        ui->tile0RotatonLabel->setText("270");
        break;
    case 1:
        // 3
        gameSprite.setTile0Rotation(3);
        ui->tile0RotatonLabel->setText("180");
        break;
    case 2:
        // 0
        gameSprite.setTile0Rotation(0);
        ui->tile0RotatonLabel->setText("0");
        break;
    case 3:
        // 2
        gameSprite.setTile0Rotation(2);
        ui->tile0RotatonLabel->setText("90");
        break;
    }

    emit renderScreen();
}

void SpriteViewer::on_tile0RotateRight_PushButton_clicked()
{
    GameSprite& gameSprite = memory->spriteRam[ui->spriteIndex->value()];
    switch(gameSprite.getTile0Rotation()){
    case 0:
        gameSprite.setTile0Rotation(2);
        ui->tile0RotatonLabel->setText("90");
        break;
    case 1:
        gameSprite.setTile0Rotation(0);
        ui->tile0RotatonLabel->setText("0");
        break;
    case 2:
        gameSprite.setTile0Rotation(3);
        ui->tile0RotatonLabel->setText("180");
        break;
    case 3:
        gameSprite.setTile0Rotation(1);
        ui->tile0RotatonLabel->setText("270");
        break;
    }
    emit renderScreen();
}

void SpriteViewer::on_tile1RotateLeft_PushButton_clicked()
{
    GameSprite& gameSprite = memory->spriteRam[ui->spriteIndex->value()];
    switch(gameSprite.getTile1Rotation()){
    case 0:
        // 1
        gameSprite.setTile1Rotation(1);
        ui->tile1RotatonLabel->setText("270");
        break;
    case 1:
        // 3
        gameSprite.setTile1Rotation(3);
        ui->tile1RotatonLabel->setText("180");
        break;
    case 2:
        // 0
        gameSprite.setTile1Rotation(0);
        ui->tile1RotatonLabel->setText("0");
        break;
    case 3:
        // 2
        gameSprite.setTile1Rotation(2);
        ui->tile1RotatonLabel->setText("90");
        break;
    }
    emit renderScreen();
}

void SpriteViewer::on_tile1RotateRight_PushButton_clicked()
{
    GameSprite& gameSprite = memory->spriteRam[ui->spriteIndex->value()];
    switch(gameSprite.getTile1Rotation()){
    case 0:
        gameSprite.setTile1Rotation(2);
        ui->tile1RotatonLabel->setText("90");
        break;
    case 1:
        gameSprite.setTile1Rotation(0);
        ui->tile1RotatonLabel->setText("0");
        break;
    case 2:
        gameSprite.setTile1Rotation(3);
        ui->tile1RotatonLabel->setText("180");
        break;
    case 3:
        gameSprite.setTile1Rotation(1);
        ui->tile1RotatonLabel->setText("270");
        break;
    }
    emit renderScreen();
}

void SpriteViewer::on_verticalSlider_valueChanged(int value)
{
    spriteRenderWindow->setScaleFactor(value);
    emit renderScreen();
}

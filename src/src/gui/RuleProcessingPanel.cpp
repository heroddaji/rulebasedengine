#include "RuleProcessingPanel.hpp"
#include "ui_RuleProcessingPanel.h"

RuleProcessingPanel::RuleProcessingPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RuleProcessingPanel)
{
    ui->setupUi(this);
}

RuleProcessingPanel::~RuleProcessingPanel()
{
    delete ui;
}

#ifndef RULEPROCESSINGPANEL_HPP
#define RULEPROCESSINGPANEL_HPP

#include <QDialog>

namespace Ui {
    class RuleProcessingPanel;
}

class RuleProcessingPanel : public QDialog
{
    Q_OBJECT

public:
    explicit RuleProcessingPanel(QWidget *parent = 0);
    ~RuleProcessingPanel();

private:
    Ui::RuleProcessingPanel *ui;
};

#endif // RULEPROCESSINGPANEL_HPP

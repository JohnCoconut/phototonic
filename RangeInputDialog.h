#pragma once

#include <QDialog>

namespace Ui {
class RangeInputDialog;
}

class RangeInputDialog : public QDialog {
    Q_OBJECT

public:
    explicit RangeInputDialog(QWidget *parent = nullptr);
    ~RangeInputDialog() override;

    qreal minimumValue();
    qreal maximumValue();

private:
    Ui::RangeInputDialog *ui;
};

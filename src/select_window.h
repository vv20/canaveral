#pragma	once

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QString>
#include <QMutex>

class SelectWindow : public QWidget {
  Q_OBJECT

  public:
    SelectWindow (QWidget* parent = 0);
    QString getSelectedSample();
  
  private slots:
    void onCancel();
    void onSelect();
    void onBack();
    void toggleSelect();

  private:
    void displayDir(QString dirName);

    QPushButton* backBtn;
    QPushButton* cancelBtn;
    QPushButton* selectBtn;
    QLineEdit* curDir;
    QListWidget* dirList;

    QString selectedFile;
    QMutex* selectedFileMutex;
};


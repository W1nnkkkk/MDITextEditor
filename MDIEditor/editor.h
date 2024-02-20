#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QAction>
#include <QCloseEvent>
#include <QApplication>

class Editor : public QTextEdit
{
    Q_OBJECT
public:
    Editor(QWidget *parent = nullptr);

    void newFile();
    bool save();
    bool saveAs();
    QSize sizeHint() const;
    QAction* windowMenuAction() const { return action; }
    Editor *open(QWidget *parent = nullptr);
    static Editor *openFile(const QString &fileName, QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void documentWasModified();

private:
    bool okToContinue();
    bool saveFile(const QString& fileName);
    bool readFile(const QString &fileName);
    bool writeFile(const QString &fileName);
    void setCurrentFile(const QString& fileName);
    QString strippedName(const QString fullFileName);

    QAction* action;
    bool isUntitled = true;
    QString curFile;
};

#endif // EDITOR_H

#include <QtGui>
#include <QMessageBox>
#include "editor.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mdiArea = new QMdiArea;
    setCentralWidget(mdiArea);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateActions()));

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    setWindowTitle(tr("MDI Editor"));
    QTimer::singleShot(0,this, SLOT(loadFiles())); // приложение показывается даже тогда когда загрузились не все файлы
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    Editor* edit = new Editor;
    edit->newFile();
    addEditor(edit);
}

void MainWindow::openFile(const QString &fileName)
{
    Editor* edit = Editor::openFile(fileName, this);
    if(edit)
        addEditor(edit);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();
    if(!mdiArea->subWindowList().isEmpty())
    {
        event->ignore();
    }
    else event->accept();
}

void MainWindow::open()
{
    Editor *edit = new Editor;
    edit->open(this);
    if (edit)
        addEditor(edit);
    else delete edit;
}

void MainWindow::save()
{
    if (activeEditor())
        activeEditor()->save();
}

void MainWindow::saveAs()
{
    if(activeEditor())
        activeEditor()->saveAs();
}

void MainWindow::cut()
{
    if(activeEditor())
        activeEditor()->cut();
}

void MainWindow::copy()
{
    if(activeEditor())
        activeEditor()->copy();
}

void MainWindow::paste()
{
    if(activeEditor())
        activeEditor()->paste();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About MDI Editor"),
                       tr("<h2>Editor 1.0</h2>"
                          "<p>Copyright &copy; 2024 Software Inc. "
                          "<p>MDI Editor is a small app that demonstrates "
                          "QMDIArea."));
}

void MainWindow::updateActions()
{
    bool hasEditor = (activeEditor() != 0);
    bool hasSelection = activeEditor()
                        && activeEditor()->textCursor().hasSelection();

    saveAction->setEnabled(hasEditor);
    saveAsAction->setEnabled(hasEditor);
    cutAction->setEnabled(hasEditor);
    copyAction->setEnabled(hasEditor);
    pasteAction->setEnabled(hasEditor);
    closeAction->setEnabled(hasEditor);
    closeAllAction->setEnabled(hasEditor);
    titleAction->setEnabled(hasEditor);
    cascadeAction->setEnabled(hasEditor);
    nextAction->setEnabled(hasEditor);
    previousAction->setEnabled(hasEditor);
    separatorAction->setEnabled(hasEditor);

    if(activeEditor())
        activeEditor()->windowMenuAction()->setChecked(true);
}

void MainWindow::loadFiles()
{
    QStringList args = QApplication::arguments();
    args.removeFirst();
    if(!args.isEmpty())
    {
        foreach (QString arg, args)
            openFile(arg);
        mdiArea->cascadeSubWindows();
    }
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new File"));
    connect(newAction, SIGNAL(triggered(bool)), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open"), this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open File"));
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(open()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save File"));
    connect(saveAction, SIGNAL(triggered(bool)), this, SLOT(save()));

    saveAsAction = new QAction(tr("&New"), this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save File"));
    connect(saveAsAction, SIGNAL(triggered(bool)), this, SLOT(save()));

    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the app"));
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));


    cutAction = new QAction(tr("&Cut"), this);
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("Cut the current selection"));
    connect(cutAction, SIGNAL(triggered(bool)), this, SLOT(cut()));


    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("Copy current selection"));
    connect(copyAction, SIGNAL(triggered(bool)), this, SLOT(copy()));


    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Paste the clipboard"));
    connect(pasteAction, SIGNAL(triggered(bool)), this, SLOT(paste()));

    closeAction = new QAction(tr("&Close"), this);
    closeAction->setShortcut(QKeySequence::Close);
    closeAction->setStatusTip(tr("Close the active window"));
    connect(closeAction, SIGNAL(triggered(bool)), mdiArea, SLOT(closeActiveSubWindow()));


    closeAllAction = new QAction(tr("&Close All"), this);
    closeAllAction->setShortcut(QKeySequence::New);
    connect(closeAllAction, SIGNAL(triggered(bool)), this, SLOT(close()));


    titleAction = new QAction(tr("&Title"), this);
    titleAction->setStatusTip(tr("Title this window"));
    connect(titleAction, SIGNAL(triggered(bool)), mdiArea, SLOT(titleSubWindows()));

    cascadeAction = new QAction(tr("&Cascade"), this);
    cascadeAction->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAction, SIGNAL(triggered(bool)), mdiArea, SLOT(cascadeSubWindows()));

    nextAction = new QAction(tr("&Next"), this);
    nextAction->setShortcut(QKeySequence::NextChild);
    nextAction->setStatusTip(tr("Move the focus"));
    connect(nextAction, SIGNAL(triggered(bool)), mdiArea, SLOT(activateNextSubWindow()));

    previousAction = new QAction(tr("&Previous"), this);
    previousAction->setShortcut(QKeySequence::PreviousChild);
    previousAction->setStatusTip(tr("Move the focus"));
    connect(previousAction, SIGNAL(triggered(bool)), mdiArea, SLOT(activatePreviousSubWindow()));

    separatorAction = new QAction(this);
    separatorAction->setSeparator(true);

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show about message"));
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));

    aboutQtAction = new QAction(tr("&About Qt"), this);
    aboutQtAction->setStatusTip(tr("Show about qt message"));
    connect(aboutQtAction, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));

    windowActionGroup = new QActionGroup(this);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);

    windowMenu = menuBar()->addMenu(tr("&Window"));
    windowMenu->addAction(closeAction);
    windowMenu->addAction(closeAllAction);
    windowMenu->addSeparator();
    windowMenu->addAction(titleAction);
    windowMenu->addAction(cascadeAction);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAction);
    windowMenu->addAction(previousAction);
    windowMenu->addAction(separatorAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
}

void MainWindow::createStatusBar()
{
    readyLabel = new QLabel(tr("Ready"));
    statusBar()->addWidget(readyLabel, 1);
}

void MainWindow::addEditor(Editor *editor)
{
    connect(editor, SIGNAL(copyAvailable(bool)),
            cutAction, SLOT(setEnabled(bool)));
    connect(editor, SIGNAL(copyAvailable(bool)),
            copyAction, SLOT(setEnabled(bool)));

    QMdiSubWindow *subWindow = mdiArea->addSubWindow(editor);
    windowMenu->addAction(editor->windowMenuAction());
    windowActionGroup->addAction(editor->windowMenuAction());
    subWindow->show();
}

Editor *MainWindow::activeEditor()
{
    QMdiSubWindow *subWindow = mdiArea->activeSubWindow();
    if(subWindow)
        return qobject_cast<Editor *>(subWindow->widget());
    return 0;
}

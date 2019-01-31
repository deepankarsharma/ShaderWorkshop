#ifndef EDITORPAGE_H
#define EDITORPAGE_H

#include <QWidget>

namespace Ui {
class EditorPage;
}

class GLSLHighlighter;

class EditorPage : public QWidget
{
    Q_OBJECT

public:
    explicit EditorPage(QWidget *parent = 0);
    ~EditorPage();

private:
    Ui::EditorPage *ui;
    GLSLHighlighter *highlighter;
};

#endif // EDITORPAGE_H
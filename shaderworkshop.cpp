#include "shaderworkshop.h"
#include "editorpage.h"
#include "renderer.h"
#include "ui_shaderworkshop.h"

ShaderWorkshop::ShaderWorkshop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShaderWorkshop),
    imagePage(new EditorPage),
    defaultItemName("Add buffer"),
    maxBufferPages(4)
{
    ui->setupUi(this);

    renderer = ui->openGLWidget;

    setupWidgets();
}

ShaderWorkshop::~ShaderWorkshop()
{
    qDeleteAll(bufferPages);
    delete imagePage;
    delete ui;
}

void ShaderWorkshop::newBufferRequested(const QString &name)
{
    // default combo box item activations should not create new buffers
    if (name == defaultItemName) {
        return;
    }

    Q_ASSERT(bufferPages.contains(name));

    QWidget *widget = bufferPages.value(name);

    tab->insertTab(tab->count(), widget, name);
    comboBox->removeItem(comboBox->findText(name));
    // always show default combo box item
    comboBox->setCurrentText(defaultItemName);
}

void ShaderWorkshop::bufferCloseRequested(int index)
{
    QString name = tab->tabText(index);

    Q_ASSERT(bufferPages.contains(name));

    tab->removeTab(index);
    comboBox->addItem(name);
}

void ShaderWorkshop::setupWidgets()
{
    tab = ui->tabWidget;
    tab->setTabsClosable(true);

    Q_ASSERT(tab->count() == 0);

    imagePage->setShaderSource(renderer->defaultFragmentShader());

    int index = tab->insertTab(tab->count(), imagePage, tr("Image"));
    // user should not be able to close main image page
    tab->tabBar()->setTabButton(index, QTabBar::LeftSide, Q_NULLPTR);

    comboBox = ui->comboBox;
    comboBox->addItem(defaultItemName);

    for (int i = 0; i < maxBufferPages; i++) {
        EditorPage *page = new EditorPage;
        page->setShaderSource(renderer->defaultFragmentShader());

        QString name = bufferName(i);
        bufferPages[name] = page;
        comboBox->addItem(name);
    }

    connect(comboBox, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::activated),
            this, &ShaderWorkshop::newBufferRequested);

    connect(tab, SIGNAL(tabCloseRequested(int)),
            this, SLOT(bufferCloseRequested(int)));
}

QString ShaderWorkshop::bufferName(int index) const
{
    char letter = 'A' + index;

    return QString("Buffer %1").arg(letter);
}

/********************************************************************************
** Form generated from reading UI file 'opengl_model_displayer.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENGL_MODEL_DISPLAYER_H
#define UI_OPENGL_MODEL_DISPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OpenGLModelDisplayerClass
{
public:
    QAction *action_open_obj_;
    QAction *action_view_obj_;
    QAction *action_view_robot_;
    QAction *action_view_basic_demo_;
    QAction *action_load_texture;
    QWidget *widget_central_;
    QFrame *frame_gl_canvas_;
    QPushButton *button_switch_rotate_mode_;
    QPushButton *button_reset_eye_;
    QLabel *label_hint_;
    QMenuBar *menu_bar_;
    QMenu *menu_file_;
    QMenu *menu_mode_;
    QToolBar *tool_bar_main_;
    QStatusBar *status_bar_;

    void setupUi(QMainWindow *OpenGLModelDisplayerClass)
    {
        if (OpenGLModelDisplayerClass->objectName().isEmpty())
            OpenGLModelDisplayerClass->setObjectName(QStringLiteral("OpenGLModelDisplayerClass"));
        OpenGLModelDisplayerClass->resize(1024, 768);
        OpenGLModelDisplayerClass->setMouseTracking(false);
        OpenGLModelDisplayerClass->setFocusPolicy(Qt::NoFocus);
        action_open_obj_ = new QAction(OpenGLModelDisplayerClass);
        action_open_obj_->setObjectName(QStringLiteral("action_open_obj_"));
        action_view_obj_ = new QAction(OpenGLModelDisplayerClass);
        action_view_obj_->setObjectName(QStringLiteral("action_view_obj_"));
        action_view_robot_ = new QAction(OpenGLModelDisplayerClass);
        action_view_robot_->setObjectName(QStringLiteral("action_view_robot_"));
        action_view_basic_demo_ = new QAction(OpenGLModelDisplayerClass);
        action_view_basic_demo_->setObjectName(QStringLiteral("action_view_basic_demo_"));
        action_load_texture = new QAction(OpenGLModelDisplayerClass);
        action_load_texture->setObjectName(QStringLiteral("action_load_texture"));
        widget_central_ = new QWidget(OpenGLModelDisplayerClass);
        widget_central_->setObjectName(QStringLiteral("widget_central_"));
        widget_central_->setEnabled(true);
        widget_central_->setMouseTracking(true);
        frame_gl_canvas_ = new QFrame(widget_central_);
        frame_gl_canvas_->setObjectName(QStringLiteral("frame_gl_canvas_"));
        frame_gl_canvas_->setGeometry(QRect(10, 10, 800, 600));
        frame_gl_canvas_->setMouseTracking(true);
        frame_gl_canvas_->setFocusPolicy(Qt::ClickFocus);
        frame_gl_canvas_->setFrameShape(QFrame::StyledPanel);
        frame_gl_canvas_->setFrameShadow(QFrame::Raised);
        button_switch_rotate_mode_ = new QPushButton(widget_central_);
        button_switch_rotate_mode_->setObjectName(QStringLiteral("button_switch_rotate_mode_"));
        button_switch_rotate_mode_->setGeometry(QRect(820, 400, 150, 25));
        button_reset_eye_ = new QPushButton(widget_central_);
        button_reset_eye_->setObjectName(QStringLiteral("button_reset_eye_"));
        button_reset_eye_->setGeometry(QRect(820, 430, 100, 25));
        label_hint_ = new QLabel(widget_central_);
        label_hint_->setObjectName(QStringLiteral("label_hint_"));
        label_hint_->setGeometry(QRect(820, 10, 180, 150));
        label_hint_->setAcceptDrops(false);
        label_hint_->setWordWrap(true);
        OpenGLModelDisplayerClass->setCentralWidget(widget_central_);
        menu_bar_ = new QMenuBar(OpenGLModelDisplayerClass);
        menu_bar_->setObjectName(QStringLiteral("menu_bar_"));
        menu_bar_->setGeometry(QRect(0, 0, 1024, 21));
        menu_file_ = new QMenu(menu_bar_);
        menu_file_->setObjectName(QStringLiteral("menu_file_"));
        menu_mode_ = new QMenu(menu_bar_);
        menu_mode_->setObjectName(QStringLiteral("menu_mode_"));
        OpenGLModelDisplayerClass->setMenuBar(menu_bar_);
        tool_bar_main_ = new QToolBar(OpenGLModelDisplayerClass);
        tool_bar_main_->setObjectName(QStringLiteral("tool_bar_main_"));
        OpenGLModelDisplayerClass->addToolBar(Qt::TopToolBarArea, tool_bar_main_);
        status_bar_ = new QStatusBar(OpenGLModelDisplayerClass);
        status_bar_->setObjectName(QStringLiteral("status_bar_"));
        OpenGLModelDisplayerClass->setStatusBar(status_bar_);

        menu_bar_->addAction(menu_file_->menuAction());
        menu_bar_->addAction(menu_mode_->menuAction());
        menu_file_->addAction(action_open_obj_);
        menu_file_->addAction(action_load_texture);
        menu_mode_->addAction(action_view_basic_demo_);
        menu_mode_->addAction(action_view_obj_);
        menu_mode_->addAction(action_view_robot_);

        retranslateUi(OpenGLModelDisplayerClass);

        QMetaObject::connectSlotsByName(OpenGLModelDisplayerClass);
    } // setupUi

    void retranslateUi(QMainWindow *OpenGLModelDisplayerClass)
    {
        OpenGLModelDisplayerClass->setWindowTitle(QApplication::translate("OpenGLModelDisplayerClass", "OpenGLModelDisplayer", 0));
        action_open_obj_->setText(QApplication::translate("OpenGLModelDisplayerClass", "Open obj", 0));
        action_view_obj_->setText(QApplication::translate("OpenGLModelDisplayerClass", "View obj", 0));
        action_view_robot_->setText(QApplication::translate("OpenGLModelDisplayerClass", "View robot", 0));
        action_view_basic_demo_->setText(QApplication::translate("OpenGLModelDisplayerClass", "View basic demo", 0));
        action_load_texture->setText(QApplication::translate("OpenGLModelDisplayerClass", "Load texture", 0));
        button_switch_rotate_mode_->setText(QApplication::translate("OpenGLModelDisplayerClass", "Switch rotate mode", 0));
        button_reset_eye_->setText(QApplication::translate("OpenGLModelDisplayerClass", "Reset eye", 0));
        label_hint_->setText(QApplication::translate("OpenGLModelDisplayerClass", "Press mouse : adjust angle\n"
"Scroll mouse : Change eyes distance", 0));
        menu_file_->setTitle(QApplication::translate("OpenGLModelDisplayerClass", "File", 0));
        menu_mode_->setTitle(QApplication::translate("OpenGLModelDisplayerClass", "Mode", 0));
    } // retranslateUi

};

namespace Ui {
    class OpenGLModelDisplayerClass: public Ui_OpenGLModelDisplayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENGL_MODEL_DISPLAYER_H

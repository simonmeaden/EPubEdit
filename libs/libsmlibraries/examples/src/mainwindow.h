/*
   Copyright 2020 Simon Meaden

   Permission is hereby granted, free of charge, to any person obtaining a copy of this
   software and associated documentation files (the "Software"), to deal in the Software
   without restriction, including without limitation the rights to use, copy, modify, merge,
   publish, distribute, sublicense, and/or sell copies of the Software, and to permit
                                                                         persons to whom the Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all copies or
   substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
   INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
   OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
      SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "qyamlcpp/qyamlcpp.h"
#include "sm_widgets/labelledwidgets.h"

/*!
   \file mainwindow.h mainwindow.cpp
   \class MainWindow mainwindow.h
   \brief Main test application for sm_widgets.
   \since 5.7.0
   \license The MIT License
   \copyright © 2019 - 2020 Simon Meaden. All rights reserved.
*/
class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   //! Constructs MainWindow test application.
   explicit MainWindow(QWidget* parent = nullptr);
   ~MainWindow();

private:
   QTabWidget* m_tabs;

   QGroupBox* m_labelVerticalBox, *m_labelHorizontalBox, *m_widgetVerticalBox, *m_widgetHorizontalBox;
   QButtonGroup* m_labelVerticalGrp, *m_labelHorizontalGrp, *m_widgetVerticalGrp, *m_widgetHorizontalGrp;

   LabelledComboBox* m_comboBox;
   LabelledSpinBox* m_spinBox;
   LabelledDoubleSpinBox* m_doubleSpinBox;
   LabelledExSpinBox* m_exSpinBox;
   LabelledTextField* m_textField;
   LabelledLineEdit* m_lineEdit;

   LabelledComboBox* m_widgetChoiceBox;
   LabelledLineEdit* m_labelText, *m_widgetText;
   AbstractLabelledWidget* m_currentWidget;
   QPlainTextEdit* m_labelStylesheet, *m_widgetStylesheet, *m_overallStylesheet;
   QPushButton* m_setLabelStylesheet, *m_setWidgetStylesheet;
   LabelledComboBox* m_positionBox, *m_exBox;
   LabelledComboBox* m_labelHPoliciesBox, *m_labelVPoliciesBox;
   LabelledComboBox* m_widgetHPoliciesBox, *m_widgetVPoliciesBox;
   LabelledSpinBox* m_left, *m_right, *m_top, *m_bottom, *m_spacing;
   QStackedWidget* m_spinStack;
   int m_spinIntStack, m_spinDblStack;
   QPushButton* m_setSpinBtn;
   LabelledSpinBox* m_minIntSpin, *m_maxIntSpin, *m_stepIntSpin;
   LabelledDoubleSpinBox* m_minDblSpin, *m_maxDblSpin, *m_stepDblSpin;
   LabelledLineEdit* m_exPrefixEdit;
   QCheckBox* m_exNegBeforePrefixBox;
   QColor m_text, m_back, m_tempText, m_tempBack;

//   ExTabWidget* m_exTabWidget;
   QCheckBox* m_showClockBox, *m_showSecondsBox/*, *m_showClockFrame*/;
   LabelledComboBox* m_clockFrameStyleBox;
   QPlainTextEdit* m_clockStylesheetEdit;
   QPushButton* m_clockStylesheetBtn;

   QCheckBox* m_showLoginBox;
   LabelledComboBox* m_loginStyleBox;
   LabelledLineEdit* m_loginSimplePasswordEdit, *m_loginStandardPasswordEdit,
                     *m_loginStandardUsernameEdit;
   QPushButton* m_simpleLoginBtn, *m_stdPasswordBtn, *m_clearStdPasswordsBtn;

   QCheckBox* m_showMessageBox, *m_enableMarqueeBox;
   QDoubleSpinBox* m_marqueeSpeedBox;
   LabelledDoubleSpinBox* m_timeoutBox;
   LabelledLineEdit* m_messageEdit, *m_tempMessageEdit;
   QPushButton* m_messageBtn, *m_tempMessageBtn;
   QPushButton* m_textColorBtn, *m_backColorBtn, *m_tempTextColorBtn, *m_tempBackColorBtn;

   void setLabelStylesheet();
   void setWidgetStylesheet();
   void setStylesheet();

   void setLabelPosition(const QString& pos);
   void setLeft();
   void setRight();
   void setTop();
   void setBottom();
   void setSpacing();

   void enableWidgets(bool enable);

   void lineEditChanged(const QString& text);
   void comboBoxChanged(const QString& text);
   void spinBoxChanged(int value);
   void doubleSpinBoxChanged(double value);
   void exSpinBoxChanged(int value);
   void setClockStylesheet();
   void setCurrentWidgetAlignment();
   void exBoxTypeChanged(int index);
   void setMessage();
   void setTempMessage();

   void chooseWidget(const QString& text);
   void labelTextHasChanged(const QString& text);
   void widgetTextHasChanged(const QString& text);
   void labelSizePolicyHasChanged(const QString&);
   void widgetSizePolicyHasChanged(const QString&);
   void labelTextFormatHasChanged(const QString&);
   void widgetTextFormatHasChanged(const QString&);
   void spinWidgetChanged();
   void clockFrameStyleChanged(int);
   void loginStyleChanged(int);

   void setSimpleLogin();
   void setStandardLogin();

   void setAlignmentStatus();
   void setStylesheetStatus();
   void setLabelPositionStatus();
   void setLayoutStatus();
   void setSizePolicyStatus();
   void setSpinBoxStatus();

   QString sizePolicyToString(QSizePolicy::Policy policy);
   void setFormattedText(Qt::TextFormat format);

   void showClock(bool enable);
   void showSeconds(bool enable);
   void showMessages(bool enable);
   void setMarqueeMoving(bool enable);
   void setMarqueeSpeed(qreal charPerSec);

   void showLogin(bool enable);


   void initGui();
   QWidget* initLabelledWidgets();
   QWidget* initChooseWidgetBox();
   QWidget* initAlignmentBox();
   QWidget* initVAlignBox(const QString& type, QGroupBox* box, QButtonGroup* grp);
   QWidget* initHAlignBox(const QString& direction, QGroupBox* box, QButtonGroup* grp);
   QWidget* initStylesheetBox();
   QWidget* initSpinBoxRangeBox();
   QWidget* initPolicyBox();
   QWidget* initLabelledWidgetFrame();
   QWidget* initExTabWidget();
   QWidget* initClockControls();
   QWidget* initLoginControls();
   QWidget* initMessageControls();
};

#endif // MAINWINDOW_H

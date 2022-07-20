#include "common.h"
#include "headerwidget.h"
#include <QPlainTextEdit>

QWidget* createEditor(QWidget*parent) {

  auto textEdit = new QPlainTextEdit(parent);
  textEdit->setContentsMargins(0, 0, 0, 0);
  textEdit->setPlainText(
    ""
    "/*************************************************************************"
    "***\n"
    "**\n"
    "** Copyright (C) 2016 The Qt Company Ltd.\n"
    "** Contact: https://www.qt.io/licensing/\n"
    "**\n"
    "** This file is part of the QtWidgets module of the Qt Toolkit.\n"
    "**\n"
    "** $QT_BEGIN_LICENSE:LGPL$\n"
    "** Commercial License Usage\n"
    "** Licensees holding valid commercial Qt licenses may use this file in\n "
    "** accordance with the commercial license agreement provided with the\n"
    "** Software or, alternatively, in accordance with the terms contained in\n"
    "** a written agreement between you and The Qt Company. For licensing "
    "terms\n"
    "** and conditions see https://www.qt.io/terms-conditions. For further\n"
    "** information use the contact form at https://www.qt.io/contact-us.\n"
    "**\n"
    "** GNU Lesser General Public License Usage\n"
    "** Alternatively, this file may be used under the terms of the GNU "
    "Lesser\n"
    "** General Public License version 3 as published by the FreeSoftware\n"
    "** Foundation and appearing in the file LICENSE.LGPL3 included in the\n "
    "** packaging of this file. Please review the following information to\n "
    "** ensure the GNU Lesser General Public License version 3 requirements\n"
    "** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.\n"
    "**\n"
    "** GNU General Public License Usage\n"
    "** Alternatively, this file may be used under the terms of the GNU\n"
    "** General Public License version 2.0 or (at your option) the GNU "
    "General\n"
    "** Public license version 3 or any later version approved by the KDE "
    "Free\n"
    "** Qt Foundation. The licenses are as published by the Free Software\n"
    "** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3\n "
    "** included in the packaging of this file. Please review the following\n"
    "** information to ensure the GNU General Public License requirements "
    "will\n"
    "** be met: https://www.gnu.org/licenses/gpl-2.0.html and\n"
    "** https://www.gnu.org/licenses/gpl-3.0.html.\n"
    "**\n"
    "** $QT_END_LICENSE$\n"
    "**\n"
    "**************************************************************************"
    "**/");
  return textEdit;
}
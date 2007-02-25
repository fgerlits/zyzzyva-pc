//---------------------------------------------------------------------------
// CardboxForm.cpp
//
// A form for querying and editing the contents of the cardbox system.
//
// Copyright 2007 Michael W Thelen <mthelen@gmail.com>.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//---------------------------------------------------------------------------

#include "CardboxForm.h"
#include "QuizDatabase.h"
#include "WordEngine.h"
#include "ZPushButton.h"
#include "Auxil.h"
#include "Defs.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

using namespace Defs;

const QString TITLE_PREFIX = "Cardbox";

//---------------------------------------------------------------------------
//  CardboxForm
//
//! Constructor.
//
//! @param parent the parent widget
//! @param f widget flags
//---------------------------------------------------------------------------
CardboxForm::CardboxForm (WordEngine* e, QWidget* parent, Qt::WFlags f)
    : ActionForm (CardboxFormType, parent, f), wordEngine (e),
    quizDatabase (0), cardboxModel (0)
{
    QVBoxLayout* mainVlay = new QVBoxLayout (this);
    mainVlay->setMargin (MARGIN);
    mainVlay->setSpacing (SPACING);
    Q_CHECK_PTR (mainVlay);

    QLabel* label = new QLabel
        ("Shift cardbox questions so the backlog contains a certain number "
         "of questions.  Currently this is hard-coded to reschedule all "
         "questions in the Anagrams cardbox.");
    Q_CHECK_PTR (label);
    label->setWordWrap (true);
    mainVlay->addWidget (label);

    // Current Backlog area
    QHBoxLayout* currentHlay = new QHBoxLayout;
    Q_CHECK_PTR (currentHlay);
    currentHlay->setSpacing (SPACING);
    mainVlay->addLayout (currentHlay);

    QLabel* currentLabel = new QLabel ("Current backlog size:");
    Q_CHECK_PTR (currentLabel);
    currentHlay->addWidget (currentLabel);

    backlogLabel = new QLabel;
    Q_CHECK_PTR (backlogLabel);
    currentHlay->addWidget (backlogLabel);

    ZPushButton* refreshBacklogButton = new ZPushButton;
    Q_CHECK_PTR (refreshBacklogButton);
    refreshBacklogButton->setText ("&Refresh");
    refreshBacklogButton->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect (refreshBacklogButton, SIGNAL (clicked()),
             SLOT (refreshBacklogClicked()));
    currentHlay->addWidget (refreshBacklogButton);

    // Shift area
    QHBoxLayout* shiftHlay = new QHBoxLayout;
    Q_CHECK_PTR (shiftHlay);
    shiftHlay->setSpacing (SPACING);
    mainVlay->addLayout (shiftHlay);

    QLabel* shiftLabel = new QLabel ("Desired backlog size:");
    Q_CHECK_PTR (shiftLabel);
    shiftHlay->addWidget (shiftLabel);

    backlogSbox = new QSpinBox;
    Q_CHECK_PTR (backlogSbox);
    backlogSbox->setMinimum (1);
    backlogSbox->setMaximum (999999);
    shiftHlay->addWidget (backlogSbox);

    ZPushButton* shiftButton = new ZPushButton;
    Q_CHECK_PTR (shiftButton);
    shiftButton->setText ("&Shift");
    shiftButton->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect (shiftButton, SIGNAL (clicked()), SLOT (shiftClicked()));
    shiftHlay->addWidget (shiftButton);

    cardboxView = new QTreeView;
    Q_CHECK_PTR (cardboxView);
    mainVlay->addWidget (cardboxView);

    ZPushButton* refreshCardboxButton = new ZPushButton;
    Q_CHECK_PTR (refreshCardboxButton);
    refreshCardboxButton->setText ("&Refresh");
    refreshCardboxButton->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect (refreshCardboxButton, SIGNAL (clicked()),
             SLOT (refreshCardboxClicked()));
    mainVlay->addWidget (refreshCardboxButton);

    mainVlay->addStretch (0);

    quizSpecChanged();
}

//---------------------------------------------------------------------------
//  getIcon
//
//! Returns the current icon.
//
//! @return the current icon
//---------------------------------------------------------------------------
QIcon
CardboxForm::getIcon() const
{
    return QIcon (":/cardbox-icon");
}

//---------------------------------------------------------------------------
//  getTitle
//
//! Returns the current title string.
//
//! @return the current title string
//---------------------------------------------------------------------------
QString
CardboxForm::getTitle() const
{
    return TITLE_PREFIX;
}

//---------------------------------------------------------------------------
//  getStatusString
//
//! Returns the current status string.
//
//! @return the current status string
//---------------------------------------------------------------------------
QString
CardboxForm::getStatusString() const
{
    return QString::null;
}

//---------------------------------------------------------------------------
//  shiftClicked
//
//! Called when the Delay button is clicked.  Delay the cardbox contents.
//---------------------------------------------------------------------------
void
CardboxForm::shiftClicked()
{
    qDebug ("CardboxForm::shiftClicked");

    int desiredBacklog = backlogSbox->value();
    QString lexicon = wordEngine->getLexiconName();
    QString quizType = "Anagrams";
    // do some searching to get a limiting list of questions
    QStringList questions;

    QuizDatabase db (lexicon, quizType);
    db.shiftCardbox (questions, desiredBacklog);
}

//---------------------------------------------------------------------------
//  refreshBacklogClicked
//
//! Called when the Refresh button is clicked.
//---------------------------------------------------------------------------
void
CardboxForm::refreshBacklogClicked()
{
    qDebug ("CardboxForm::refreshBacklogClicked");

    QString lexicon = wordEngine->getLexiconName();
    QString quizType = "Anagrams";

    QuizDatabase db (lexicon, quizType);
    QStringList readyQuestions = db.getAllReadyQuestions();
    backlogLabel->setText (QString::number (readyQuestions.count()));
}

//---------------------------------------------------------------------------
//  refreshCardboxClicked
//
//! Called when the Refresh button is clicked.
//---------------------------------------------------------------------------
void
CardboxForm::refreshCardboxClicked()
{
    qDebug ("CardboxForm::refreshCardboxClicked");

    if (!cardboxModel || !quizDatabase)
        return;

    QString cardboxQueryStr = "SELECT cardbox, count(*) from questions "
        "WHERE cardbox NOT NULL GROUP BY cardbox"; 
    cardboxModel->setQuery (cardboxQueryStr, *(quizDatabase->getDatabase()));
    cardboxModel->setHeaderData (0, Qt::Horizontal, "Cardbox");
    cardboxModel->setHeaderData (1, Qt::Horizontal, "Count");
}

//---------------------------------------------------------------------------
//  quizSpecChanged
//
//! Called when the Lexicon or Quiz Type is changed by the user.
//---------------------------------------------------------------------------
void
CardboxForm::quizSpecChanged()
{
    qDebug ("CardboxForm::quizSpecChanged");

    QString lexicon = wordEngine->getLexiconName();
    QString quizType = "Anagrams";

    delete quizDatabase;
    quizDatabase = new QuizDatabase (lexicon, quizType);
    Q_CHECK_PTR (quizDatabase);

    delete cardboxModel;
    cardboxModel = new QSqlQueryModel;
    Q_CHECK_PTR (cardboxModel);
    cardboxView->setModel (cardboxModel);

    refreshCardboxClicked();
}
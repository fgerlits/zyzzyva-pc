//---------------------------------------------------------------------------
// QuizResponse.h
//
// A class to represent a quiz response.
//
// Copyright 2012 Boshvark Software, LLC.
//
// This file is part of Zyzzyva.
//
// Zyzzyva is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Zyzzyva is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//---------------------------------------------------------------------------

#ifndef ZYZZYVA_QUIZ_RESPONSE_H
#define ZYZZYVA_QUIZ_RESPONSE_H

class QuizResponse
{
    public:
    enum Status {
        Incomplete = 0,
        Correct = 1,
        Missed = 2,
        Incorrect = 3
    };

    public:
    QuizResponse() : questionIndex(-1), status(Incomplete) { }
    ~QuizResponse() { }

    bool isValid() const;

    void setQuestionIndex(const int i) { questionIndex = i; }
    void setStatus(const Status s) { status = s; }
    void setName(const QString& n) { name = n; }

    int getQuestionIndex() const { return questionIndex; }
    Status getStatus() const { return status; }
    QString getName() const { return name; }

    private:
    int questionIndex;
    Status status;
    QString name;
};

#endif // ZYZZYVA_QUIZ_RESPONSE_H
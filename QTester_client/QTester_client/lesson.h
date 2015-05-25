#pragma once
#ifndef LESSON_H
#define LESSON_H

#include <QString>
#include <QVector>
#include <QLocale>
#include "theme.h"

class Lesson
{
private:
    QString _id;
    QString _title;
    QLocale::Language _lang;
    QVector<Theme> _themes;

    Theme selectTheme( const int themeId, 
                       const SQLMgr &sqlManager, 
                       const qint64 questionsCount,
                       const int answersCount )const;

public:
    Lesson(){};
    Lesson(const QString &title);

    QString getTitle() const;
    QLocale::Language getLang() const;
    QVector<Theme> getThemes() const;

	void setTitle(const QString &title);
	void setLang(const QLocale::Language &lang);
    void pushTheme(const Theme &theme);

	void selectThemesFromDataBase(const SQLMgr &sqlManager, 
		                         const QVector<int> &themIds, 
								 const int themesCount, 
                                 const qint64 questionsCount,
                                 const int answersCount);

    
};

namespace Table
{
	namespace Lesson
	{
		const QString TABLE_NAME = "Lessons";

		namespace Fields
		{
			const QString LESSON_ID = "lesson_id";
			const QString NAME = "name";
			const QString TITLE = "title";
			const QString PROFESTION_ID = "profestion_id";
		}
	}
}
#endif
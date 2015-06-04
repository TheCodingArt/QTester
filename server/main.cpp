#include <QCoreApplication>
#include <iostream>

#include "sqlitemgr.h"
#include "profession.h"
#include "testgenerator.h"
#include "jsonformat.h"

#ifdef QT_DEBUG
#define _DEBUG
#endif

int main( int argc, char *argv[] )
{
    QCoreApplication a( argc, argv );

    SQLMgr *db = new SQLiteMgr( "", "", "", "" );
    setlocale( LC_ALL, "Russian" );

    IdTitleMap lessons = Lesson::getLessList( *db ),
        themes = Theme::getThemeList( *db );

    idThemeIdsMap lessons_themes_ids;

    lessons_themes_ids[lessons.key( "Сетевые технологии" )] = QStringList( { themes.key( "Топология и конфигурация сетей" ), themes.key( "JavaScript" ) } );
    lessons_themes_ids[lessons.key( "ОС и ПО ВК" )] = QStringList( { themes.key( "UNIX-системы" ), themes.key( "Norton Comander" ) } );

    ParamsForCollection params;
    params.professionId = "1";
    params.lessIds = lessons_themes_ids.keys().first();
    params.themsIds = lessons_themes_ids.values().first();
    params.questionsCount = 5;
    params.answersCount = 5;

    auto prof = TestGenerator::collectTestVariant( *db, params );
    QJsonDocument doc;
    doc.setObject( JsonFormat::professionToJsonObj( prof ) );
    qDebug() << doc.toJson( QJsonDocument::Indented );

    return  a.exec();
}


/*
 TODO:
* ConnectionManager
* UserControl
* AccessManager
* iApiManager
* Api1_0

*/
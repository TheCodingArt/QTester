#include "databasedatamanager.h"

DatabaseDataManager::DatabaseDataManager()
{
    _db = SQLiteMgr::instance();
}

bool DatabaseDataManager::authentification(const QString &login, const QString &password)
{
    return _db->auth( login, password );
}

User DatabaseDataManager::getUser(const QString &login, const QString &password)
{

    SqlWhere where(     Tables::Users::Fields::LOGIN    + " = '" + login    + "'" );
             where.AND( Tables::Users::Fields::PASSWORD + " = '" + password + "'" );
    QStringList fields( { "firstName ||' '|| secondName AS fullName", Tables::Users::Fields::USERGROUP_ID } );

    QSqlQuery query = _db->select(Tables::Users::TABLE_NAME, fields, where);
    query.first();

    QString fullUserName = query.value( "fullName" ).toString();
    QString userGroup    = query.value( Tables::Users::Fields::USERGROUP_ID ).toString();

    User user;
    user.setLogin( login );
    user.setGroup( userGroup );
    user.setFullName( fullUserName );

    return user;
}

User DatabaseDataManager::getUser(const QString &id)
{
    SqlWhere where( Tables::Users::Fields::USER_ID + " = '" + id + "'" );
    QString fullNameQuery = Tables::Users::Fields::FIRSTNAME + " || ' ' || " + Tables::Users::Fields::SECONDNAME;
    QStringList fields( { fullNameQuery +" AS fullName", Tables::Users::Fields::USERGROUP_ID, Tables::Users::Fields::LOGIN } );

    QSqlQuery query = _db->select(Tables::Users::TABLE_NAME, fields, where);
    query.first();

    QString fullUserName = query.value( "fullName" ).toString();
    QString userGroup    = query.value( Tables::Users::Fields::USERGROUP_ID ).toString();
    QString login        = query.value( Tables::Users::Fields::LOGIN ).toString();

    User user;
    user.setLogin( login );
    user.setGroup( userGroup );
    user.setFullName( fullUserName );

    return user;
}

IdTitleMap DatabaseDataManager::getProfessionsList(const qint64 limit)
{
    IdTitleMap professionsList;

    QStringList fields;
                fields << Tables::Professtions::Fields::PROFESSION_ID
                       << Tables::Professtions::Fields::TITLE;

    QSqlQuery query = _db->select( Tables::Professtions::TABLE_NAME, fields, limit);

    while ( query.next() ) {
        QString id    = query.value( Tables::Professtions::Fields::PROFESSION_ID ).toString();
        QString title = query.value( Tables::Professtions::Fields::TITLE         ).toString();

        professionsList.insert( id, title );
    }

    return professionsList;
}

IdTitleMap DatabaseDataManager::getLessonsList(const QString &profession_id, const qint64 limit)
{
    using namespace Tables::Lessons;

    QStringList fields( { Fields::LESSON_ID, Fields::TITLE } );
    SqlWhere where( Fields::PROFESSION_ID + " = '" + profession_id + "'" );

    IdTitleMap lessonsList;

    QSqlQuery query = _db->select( TABLE_NAME, fields, where, limit );

    while( query.next() )
    {
        QString id    = query.value( Fields::LESSON_ID ).toString();
        QString title = query.value( Fields::TITLE     ).toString();

        lessonsList.insert( id, title );
    }

    return lessonsList;
}

IdTitleMap DatabaseDataManager::getThemesList(const QString &lesson_id, const qint64 limit)
{
    using namespace Tables::Themes;
    QStringList fields( { Fields::THEME_ID, Fields::TITLE } );
    SqlWhere where( Fields::LESSON_ID + " = '" + lesson_id + "'" );


    QSqlQuery query = _db->select( TABLE_NAME, fields, where, limit );

    IdTitleMap themesList;

    while( query.next() )
    {
        QString id    = query.value( Fields::THEME_ID ).toString();
        QString title = query.value( Fields::TITLE    ).toString();

        themesList.insert( id, title );
    }

    return themesList;
}

IdTitleMap DatabaseDataManager::getQuestionsList(const QString &theme_id, const qint64 limit)
{
    using namespace Tables::Questions;
    QStringList fields( { Fields::QUESTION_ID, Fields::TEXT } );
    SqlWhere where( Fields::THEME_ID + " = '" + theme_id + "'" );


    QSqlQuery query = _db->select( TABLE_NAME, fields, where, limit );

    IdTitleMap questionsList;

    while( query.next() )
    {
        QString id    = query.value( Fields::QUESTION_ID ).toString();
        QString title = query.value( Fields::TEXT        ).toString();

        questionsList.insert( id, title );
    }

    return questionsList;
}

IdTitleMap DatabaseDataManager::getAnswersList(const QString &question_id, const qint64 limit)
{
    using namespace Tables::Answers;
    QStringList fields( { Fields::ANSWER_ID, Fields::TEXT } );
    SqlWhere where( Fields::QUESTION_ID + " = '" + question_id + "'" );


    QSqlQuery query = _db->select( TABLE_NAME, fields, where, limit );

    IdTitleMap answersList;

    while( query.next() )
    {
        QString id    = query.value( Fields::ANSWER_ID ).toString();
        QString title = query.value( Fields::TEXT      ).toString();

        answersList.insert( id, title );
    }

    return answersList;
}

Profession DatabaseDataManager::getProfession(const QString &profession_id)
{
    using namespace Tables::Professtions;
    QStringList fields( { Fields::NAME, Fields::TITLE } );
    SqlWhere where( Fields::PROFESSION_ID + " = '" + profession_id + "'" );


    QSqlQuery query = _db->select( TABLE_NAME, fields, where );

    QString name  = query.value( Fields::NAME  ).toString();
    QString title = query.value( Fields::TITLE ).toString();

    Profession profession;
    profession.setId( profession_id );
    profession.setName( name );
    profession.setTitle( title );

    return profession;
}

Lesson DatabaseDataManager::getLesson(const QString &lesson_id)
{
    using namespace Tables::Lessons;
    QStringList fields( { Fields::NAME, Fields::TITLE, Fields::LANGUAGE } );
    SqlWhere where( Fields::LESSON_ID + " = '" + lesson_id + "'" );


    QSqlQuery query = _db->select( TABLE_NAME, fields, where );

    QString name     = query.value( Fields::NAME     ).toString();
    QString title    = query.value( Fields::TITLE    ).toString();
    QString language = query.value( Fields::LANGUAGE ).toString();

    Lesson lesson;
    lesson.setId( lesson_id );
    lesson.setLang( QLocale( language ).language() );
    lesson.setName( name );
    lesson.setTitle( title );

    return lesson;
}

Theme DatabaseDataManager::getTheme(const QString &theme_id)
{
    using namespace Tables::Themes;
    QStringList fields;
        fields << Fields::NAME
               << Fields::TITLE
               << Fields::DIFFICULTY;

    SqlWhere where( Fields::THEME_ID + " = '" + theme_id + "'" );


    QSqlQuery query = _db->select( TABLE_NAME, fields, where );

    QString name       = query.value( Fields::NAME       ).toString();
    QString title      = query.value( Fields::TITLE      ).toString();
    int     difficulty = query.value( Fields::DIFFICULTY ).toInt();

    Theme theme;
    theme.setId( theme_id );
    theme.setName( name );
    theme.setTitle( title );
    theme.setDifficulty( difficulty );

    return theme;
}

Question DatabaseDataManager::getQuestion(const QString &question_id)
{
    using namespace Tables::Questions;
    QStringList fields;
        fields << Fields::CASE_SENS
               << Fields::QUESTION_TYPE
               << Fields::RECOMENDED_TIME
               << Fields::STRIP_SPACE
               << Fields::TEXT;

    SqlWhere where( Fields::QUESTION_ID + " = '" + question_id + "'" );

    QSqlQuery query = _db->select( TABLE_NAME, fields, where );

    bool    caseSens    = query.value( Fields::CASE_SENS     ).toBool();
    bool    stripSpaces = query.value( Fields::STRIP_SPACE   ).toBool();
    QString text        = query.value( Fields::TEXT          ).toString();
    QString type        = query.value( Fields::QUESTION_TYPE ).toString();
    QTime   time        = query.value( Fields::RECOMENDED_TIME ).toTime();

    Question question;
    question.setId( question_id );
    question.setCaseSensitivity(  caseSens);
    question.setStripSpaces( stripSpaces );
    question.setText( text );
    question.setTime( time );
    question.setType( type );

    return question;
}

Answer DatabaseDataManager::getAnswer(const QString &answer_id)
{
    using namespace Tables::Answers;
    QStringList fields;
        fields << Fields::TEXT
               << Fields::VALID;

    SqlWhere where( Fields::ANSWER_ID + " = '" + answer_id + "'" );


    QSqlQuery query = _db->select( TABLE_NAME, fields, where );

    QString text  = query.value( Fields::TEXT  ).toString();
    bool    valid = query.value( Fields::VALID ).toBool();

    Answer answer;
    answer.setId( answer_id );
    answer.setValue( text );
    answer.setValid( valid );

    return answer;
}

bool DatabaseDataManager::addProfession(const Profession &profession)
{
    /// \todo add profession
    return false;
}

bool DatabaseDataManager::addLesson(const QString &profession_id, const Lesson &lesson)
{
    /// \todo add lesson
    return false;
}

bool DatabaseDataManager::addTheme(const QString &lesson_id, const Theme &theme)
{
    /// \todo add theme
    return false;
}

bool DatabaseDataManager::addQuestion(const QString &theme_id, const Question &question)
{
    /// \todo add question
    return false;
}

bool DatabaseDataManager::addAnswer(const QString &question_id, const Answer &answer)
{
    /// \todo add answer
    return false;
}

bool DatabaseDataManager::addUser(const User &user)
{
    /// \todo add user
    return false;
}


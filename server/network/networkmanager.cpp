#include "networkmanager.h"


void NetworkManager::connectSlots()
{
    connect( _server, SIGNAL( dataRecieved(QString, QTcpSocket*) ),
             this,    SLOT  ( inputQuery( QString, QTcpSocket* ) )
             );
    connect( _server, SIGNAL( newConnection( QTcpSocket* ) ),
             this,    SLOT  ( newClient( QTcpSocket* )   ) );
}

NetworkManager::NetworkManager(SQLMgr &sqlMgr, const int port)
    : QObject(0)
{
    _sqlMgr = &sqlMgr;

    _server = new TcpServer( port );

    connectSlots();
}

void NetworkManager::newClient( QTcpSocket* client )
{
    emit newClientConnected(client);
    qDebug() << "[NetworkManager::newClient]\n  "
             << client->localAddress().toString() << " " << client->localPort();
}

NetworkManager::~NetworkManager()
{
    delete _server;
}

void NetworkManager::inputQuery(QString request, QTcpSocket* client )
{
    qDebug() << "[NetworkManager::inputQuery]\n  "
             << "Request: " << request;

    QString outputData = RequestsManager::request( request, *_sqlMgr ).toJson();

    qDebug() << outputData;

    _server->sendToClient( client, outputData );
}

// file : ProtocolManager.h
// name : eric

#ifndef PROTOCOLMANAGER
#define PROTOCOLMANAGER

#include <vector>
#include <QString>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>

/****
  @brief This class standardizes message transfer between server and client.

  This class provides static methods to format messages into QByteArrays,
  classify messages, and extract the contents of a message.
****/
namespace oom
{
    class ProtocolError{};
    
    class ProtocolManager 
    {
    private:
        ProtocolManager() {};
        
    public:
        enum Protocol {
            START = '0',
            LoginRequest, /// A client is trying to log in to an account
            LoginAccept, /// A server accepted a login request
            LoginDenied, /// A server denied a login request
            CreateAccountRequest, /// A client is trying to create an account
            CreateAccountAccept, /// A server created an account
            CreateAccountDenied, /// A server refused to create an account
            MessageRequest, /// A client is trying to message a user
            MessageAccept, /// A server allowed a user to message a user
            MessageDenied, /// A server didnt allow a user to message a user
            MessageForward, /// A server is forwarding the message to a client
            SIZE
        };

        /*******************************************************************
           @brief returns a message's protocol
           @param msg : the message to be classified
           @exception ProtocolError if message contains invalid protocol
        ********************************************************************/
        static Protocol classify(const QString& msg);
        
        /********************************************************************
           @brief returns a message's message contents
           @param msg : the message to have contents returned
        ********************************************************************/
        static QString contents(const QString& msg);

        /********************************************************************
           @brief formats t and arg into a message
           @param t    : the protocol for the message
           argv : the arguments to include in the message
           @exception ProtocolError if t is an invalid protocol
        ********************************************************************/
        static QByteArray constructMsg(Protocol t, const QStringList& argv);

        
        // static QByteArray serialize(Protocol t, const QStringList& argv);
        // static QJsonObject deserialize(const QByteArray& msg);
    };
};

#endif

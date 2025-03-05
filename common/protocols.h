#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

class ProtocolError{};

enum class Protocol
{
    LoginRequest=0, 
    LoginAccept, 
    LoginDenied, //2
    CreateAccountRequest, 
    CreateAccountAccept, //4 
    CreateAccountDenied,  
    AuthCodeSubmit, //6
    AuthCodeAccept,
    AuthCodeDenied, //8
    AnnounceOffline,
    PrivateMessage, //10
    DiscoveryRequest,
    DiscoveryFail, //12
    DiscoveryAccept
};

class Serializer
{
public:
    virtual QByteArray operator()(const QList<QJsonValue>& argv) = 0;
protected:
    Serializer() {}
    virtual ~Serializer() {};
    
    QByteArray serializeUtil(Protocol t, const QList<QJsonValue>& argv,
                             const QStringList& argn);
};

class LoginRequest : public Serializer
{
private:
    static LoginRequest * instance;
public:
    static LoginRequest * getInstance()
    {
        if (instance == NULL)
            instance = new LoginRequest;
        return instance;
    }
    static void destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
    
    
    QByteArray operator()(const QList<QJsonValue>& argv)
    {
        return serializeUtil(Protocol::LoginRequest,
                             argv, {"Username", "Password"});
    }
};

class LoginAccept : public Serializer
{
private:
    static LoginAccept * instance;
public:
    static LoginAccept * getInstance()
    {
        if (instance == NULL)
            instance = new LoginAccept;
        return instance;
    }
    static void destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
    
    QByteArray operator()(const QList<QJsonValue>& argv)
    {
        return serializeUtil(Protocol::LoginAccept, argv, {"Username"});
    }
};

class LoginDenied : public Serializer
{
private:
    static LoginDenied * instance;
public:
    static LoginDenied * getInstance()
    {
        if (instance == NULL)
            instance = new LoginDenied;
        return instance;
    }
    static void destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
    
    QByteArray operator()(const QList<QJsonValue>& argv)
    {
        return serializeUtil(Protocol::LoginDenied, argv, {"Error Message"});
    }
};

class CreateAccountRequest : public Serializer
{
private:
    static CreateAccountRequest * instance;
public:
    static CreateAccountRequest * getInstance()
    {
        if (instance == NULL)
            instance = new CreateAccountRequest;
        return instance;
    }
    static void destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
    QByteArray operator()(const QList<QJsonValue>& argv)
    {
        return serializeUtil(Protocol::CreateAccountRequest,
                             argv, {"Username", "Password", "Email"});
    }
};

class CreateAccountAccept : public Serializer
{
private:
    static CreateAccountAccept * instance;
public:
    static CreateAccountAccept * getInstance()
    {
        if (instance == NULL)
            instance = new CreateAccountAccept;
        return instance;
    }
    
    static void destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
    
    QByteArray operator()(const QList<QJsonValue>& argv)
    {
        return serializeUtil(Protocol::CreateAccountAccept,
                             argv, {"Username", "Password"});
    }
};

class CreateAccountDenied : public Serializer
{
    private:
    static CreateAccountDenied * instance;
public:
    static CreateAccountDenied * getInstance()
    {
        if (instance == NULL)
            instance = new CreateAccountDenied;
        return instance;
    }
    static void destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
    QByteArray operator()(const QList<QJsonValue>& argv)
    {
        return serializeUtil(Protocol::CreateAccountDenied,
                             argv, {"Error Message"});
    }
};

class AuthCodeSubmit : public Serializer
{
private:
    static AuthCodeSubmit * instance;
public:
    static AuthCodeSubmit * getInstance()
    {
        if (instance == NULL)
            instance = new AuthCodeSubmit;
        return instance;
    }
    static void destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
    QByteArray operator()(const QList<QJsonValue>& argv)
    {
        return serializeUtil(Protocol::AuthCodeSubmit,
                             argv, {"Username","Password","Code"});
    }
};

class AuthCodeAccept : public Serializer
{
private:
    static AuthCodeAccept * instance;
public:
    static AuthCodeAccept * getInstance()
    {
        if (instance == NULL)
            instance = new AuthCodeAccept;
        return instance;
    }
    static void destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
    QByteArray operator()(const QList<QJsonValue>& argv)
    {
        return serializeUtil(Protocol::AuthCodeAccept, argv, {});
    }
};

class AuthCodeDenied : public Serializer
{
private:
    static AuthCodeDenied * instance;
public:
    static AuthCodeDenied * getInstance()
    {
        if (instance == NULL)
            instance = new AuthCodeDenied;
        return instance;
    }
    static void destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
    QByteArray operator()(const QList<QJsonValue>& argv)
    {
        return serializeUtil(Protocol::AuthCodeDenied, argv,
                             {"Error Message"});
    }
};

class AnnounceOffline : public Serializer
{
private:
    static AnnounceOffline * instance;
public:
    static AnnounceOffline * getInstance()
    {
        if (instance == NULL)
            instance = new AnnounceOffline;
        return instance;
    }
    static void destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
    QByteArray operator()(const QList<QJsonValue>& argv)
    {
        return serializeUtil(Protocol::AnnounceOffline, argv, {"Username"});
    }
};

class PrivateMessage : public Serializer
{
private:
    static PrivateMessage * instance;
public:
    static PrivateMessage * getInstance()
    {
        if (instance == NULL)
            instance = new PrivateMessage;
        return instance;
    }
    static void destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
    QByteArray operator()(const QList<QJsonValue>& argv)
    {
        return serializeUtil(Protocol::PrivateMessage,
                             argv, {"To","From","Message"});
    }
};

class DiscoveryRequest : public Serializer
{
private:
    static DiscoveryRequest * instance;
public:
    static DiscoveryRequest * getInstance()
    {
        if (instance == NULL)
            instance = new DiscoveryRequest;
        return instance;
    }
    static void destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
    QByteArray operator()(const QList<QJsonValue>& argv)
    {
        return serializeUtil(Protocol::DiscoveryRequest,
                             argv, {"CurrUser", "Username"});
    }
};

class DiscoveryFail : public Serializer
{
private:
    static DiscoveryFail * instance;
public:
    static DiscoveryFail * getInstance()
    {
        if (instance == NULL)
            instance = new DiscoveryFail;
        return instance;
    }
    static void destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
    QByteArray operator()(const QList<QJsonValue>& argv)
    {
        return serializeUtil(Protocol::DiscoveryFail, argv, {"Username"});
    }
};

class DiscoveryAccept : public Serializer
{
private:
    static DiscoveryAccept * instance;
public:
    static DiscoveryAccept * getInstance()
    {
        if (instance == NULL)
            instance = new DiscoveryAccept;
        return instance;
    }
    static void destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
    QByteArray operator()(const QList<QJsonValue>& argv)
    {
        return serializeUtil(Protocol::DiscoveryAccept, argv,
                             {"Username", "Messages"});
    }
};



#endif 

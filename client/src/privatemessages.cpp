// Name: Alex Franke
// File: privateMessages.cpp
// OOM Project
//
// Description: This file defines the PrivateMessages class, which manages the private messaging
//              functionality in a Discord-like Qt application. It interacts with the Client class
//              for network communication (sending/receiving messages), displays ongoing
//              conversations, and handles user interactions (searching for users, sending friend
//              requests, etc.).

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextBrowser>

#include "OOMTextBrowser.h"
#include "privatemessages.h"
#include "ui_privatemessages.h"
#include "User.h"
#include "message.h"

// Constructor for PrivateMessages. Sets up UI elements, signals, and slots.
PrivateMessages::PrivateMessages(QWidget *parent)
    : OOMWidget(parent), ui(new Ui::PrivateMessages),
      currentlyMessaging("","",""),
      messagingList(new DMListModel(this))
{
    ui->setupUi(this);                  // Sets up the UI from the .ui file
    ui->friendNameLabel->clear();       // Clears label that shows who the user is currently messaging

    // Model to display the list of users the client is messaging
    ui->friendView->setModel(messagingList);

    // Adds a placeholder user to test the messaging list
    messagingList->addUserToDMList(User("Test"));

    // Sets up a custom event filter to detect the Enter key in the text-edit area
    enterFilter = new EnterKeyFilter(this);
    ui->textEdit->installEventFilter(enterFilter);

    ui->friendView->viewport()->installEventFilter(this);

    // Connect the custom Enter key press signal to the onEnterKeyPressed slot
    connect(enterFilter, &EnterKeyFilter::enterPressed, this, &PrivateMessages::onEnterKeyPressed);

    // Detect Enter key press for the search user line-edit
    connect(ui->searchUserTextbox, &QLineEdit::returnPressed, this, &PrivateMessages::searchUser);

    // Handle incoming direct messages (sent from the Client object)
    connect(client, &Client::recievedDM, this, &PrivateMessages::receivedMessage);

    // If searching for a user fails, display an error message
    connect(client, &Client::discoverUserFail, this, [=](const QString& username){
        ui->userNotFoundLabel->setText("User " + username +  " not found!");
        ui->friendNameLabel->clear();
    });

    // If searching for a user succeeds, load the conversation with that user
    connect(client, &Client::discoverUserSucceed, this, [=](const QString& username, const QJsonArray & messageJsonList){
        ui->userNotFoundLabel->clear();
        ui->friendNameLabel->setText("Now messaging: " + username);

        // Update the user we are currently messaging
        currentlyMessaging = User(username);

        // Add the found user to the messaging list
        messagingList->addUserToDMList(currentlyMessaging);

        // Populate the text browser with the returned messages (server history)
        for (auto obj : messageJsonList)
        {
            QString to   = obj["To"].toString();
            QString from = obj["From"].toString();
            QString msg  = obj["Message"].toString();

            // Append each message to the text browser
            ui->textBrowser->appendMessage(Message(from, to, msg), 1);
            // Also update the model with the new message
            messagingList->messageReceived(currentlyMessaging, Message(from, to, msg));
        }

        // Show or hide the Add Friend button depending on whether the user is already a friend
        showAddFriendButton();
    });


    // Handle friend request lists and actual friend lists
    connect(client, &Client::sendFriendRequestList, this, [=](const QString usr, const QStringList& list)
    {
        // If we receive a list for the user we are currently messaging, update them
        if (usr == currentlyMessaging.get_username())
            currentlyMessaging.setFriendRequestList(list);
        // If we receive a list for our own user, update the client's friend request list
        else if (usr == client->getUser().get_username())
            client->getUser().setFriendRequestList(list);
    });

    // Handle receiving an updated friend list
    connect(client, &Client::sendFriendList, this, [=](const QString& usr, const QStringList& list) {
        if (usr == currentlyMessaging.get_username())
        {
            // Update the friend list of the user we are currently messaging
            currentlyMessaging.setFriendList(list);
        }
        else if (usr == client->getUser().get_username())
        {
            // Update the friend list of our own user object
            qDebug() << "Setting client friendlist as: " << list;
            client->getUser().setFriendList(list);
        }
    });

    //DENY BUTTON:
    //Remove friend request from combobox and tell client to remove friend request
    connect(ui->denyButton, &QPushButton::clicked, this, [=](){
        client->denyFriend(User(ui->friendRequestComboBox->currentText()));
        ui->friendRequestComboBox->removeItem(ui->friendRequestComboBox->currentIndex());
    });

    //ADD FRIEND BUTTON:
    // Connect the Add Friend button to the function that sends a friend request
    connect(ui->addFriendButton, &QPushButton::clicked, this, &PrivateMessages::sendFriendRequest);

    //ACCEPT FRIEND BUTTON:
    // Accept button to accept a friend request from the friendRequestComboBox
    connect(ui->acceptButton, &QPushButton::clicked, this, [=](){
        client->acceptFriend(User(ui->friendRequestComboBox->currentText()));
        ui->friendCombobox->addItem(ui->friendRequestComboBox->currentText());
        ui->friendRequestComboBox->removeItem(ui->friendRequestComboBox->currentIndex());
        showAddFriendButton();
    });

    //DM LIST:
    // Handle selecting a user from the friendView (on the left list) to open the DM window
    connect(ui->friendView, &QListView::clicked, this, &PrivateMessages::openDM);


    // Handle receiving a friend request, adds that request to the friendRequestComboBox
    connect(client, &Client::recievedFriendRequest, this, [=](const QString& from)
    {
        client->getUser().addFriendRequest(from);
        ui->friendRequestComboBox->addItem(from);
    });

    connect(client, &Client::friendAccepted, this, [=](const QString& from)
    {
        ui->friendCombobox->addItem(from);
    });
}


//When private messages is visible, this runs
void PrivateMessages::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    qDebug() << "Showing privatemessages";

    // Display the current user's username
    ui->currentUser->setText(client->getUser().get_username());

    QTimer::singleShot(1000, this, [this]() {
        if (!client) return;

        // Populate friend requests
        for (const QString& usr : client->getUser().getFriendRequestList())
            ui->friendRequestComboBox->addItem(usr);

        // Populate friend list
        for (const QString& usr : client->getUser().getFriendsList())
        {
            qDebug() << "in populate combobox... inserting " << usr;
            ui->friendCombobox->addItem(usr);
        }
    });
}

// Destructor: Cleans up the UI.
PrivateMessages::~PrivateMessages()
{
    delete ui;
}

// Checks if the currentlyMessaging user is already a friend. Shows or hides the Add Friend button accordingly.
void PrivateMessages::showAddFriendButton()
{
    if (client->getUser().getFriendsList().contains(currentlyMessaging.get_username()))
        ui->addFriendButton->hide();
    else
        ui->addFriendButton->show();
}

// Initiates a server request to search for another user by username.
// Clears the current conversation text and sets up for the discovered user's messages.
void PrivateMessages::searchUser()
{
    ui->textBrowser->clearHistory();
    ui->textBrowser->clear();

    User u = User(ui->searchUserTextbox->text());

    // Prevent searching for yourself
    if (u.get_username() == client->getUser().get_username()) return;
    // If this user is the one we are already messaging, do nothing
    if (u.get_username() == currentlyMessaging.get_username())
        return;

    // Indicate that we are searching
    ui->userNotFoundLabel->setText("Currently searching for: " + u.get_username());
    client->discover(u);
}

// Helper to format messages from other users, not fully implemented here
QString PrivateMessages::formatOtherMessage()
{
    QString otherMsg = "";
    QString user = currentlyMessaging.get_username();
    return "<b>" + user + "</b>:<br>" + otherMsg;
}

// Helper to format messages from the client user before sending
QString PrivateMessages::formatClientMessage()
{
    QString msg  = ui->textEdit->toMarkdown();
    QString user = client->getUser().get_username();
    return "<b>" + user + "</b>:<br>" + msg;
}

// Called when the user presses Enter in the text-edit box.
// Sends the typed message to the server and updates the UI.
void PrivateMessages::onEnterKeyPressed()
{
    // Build and display the client-formatted message
    QString fullMsg    = formatClientMessage();
    QString msgContent = ui->textEdit->toPlainText();

    // Create a Message object to store from/to content
    Message msg(client->getUser().get_username(), currentlyMessaging.get_username(), msgContent);

    // Append the new message to the text browser
    ui->textBrowser->appendMessage(msg, 0);

    // Clear the typed text and record the message in the DM list
    ui->textEdit->clear();
    messagingList->messageReceived(currentlyMessaging.get_username(), msg);

    // Send the message to the server
    client->privateMessage(currentlyMessaging.get_username(), msg.get_msg());
}

// Called when a message arrives from the server. If the user is currently being messaged,
// display it immediately; otherwise just record it in the DM list.
void PrivateMessages::receivedMessage(QString from, QString amsg)
{
    Message msg(from, client->getUser().get_username(), amsg);

    // If the incoming message is from the user we are currently chatting with, display it
    if (from == currentlyMessaging.get_username())
    {
        ui->textBrowser->appendMessage(msg, 0);
        messagingList->messageReceived(User(from), msg);
    }
    else
    {
        // Otherwise, store it in the message list so it can be viewed later
        messagingList->messageReceived(User(from), msg);
    }
}

// Called when the user clicks on someone in the friendView. Opens the DM session with that user
// and populates the text browser with existing message history.
void PrivateMessages::openDM(const QModelIndex &index)
{
    if (!index.isValid())
    {
        qDebug() << "Invalid index in openDM.";
        return;
    }

    QVariant userData = index.data(Qt::UserRole);
    if (!userData.isValid())
    {
        qDebug() << "Invalid user data in openDM.";
        return;
    }

    // Retrieve the User object from the model
    currentlyMessaging = userData.value<User>();

    ui->friendNameLabel->setText("Now messaging: " + currentlyMessaging.get_username());
    showAddFriendButton();

    // Load all past messages from the messaging list
    QList<Message> history = messagingList->getMessageHistory(currentlyMessaging);
    ui->textBrowser->clear();
    for (const Message &msg : history)
    {
        ui->textBrowser->appendMessage(msg, 1);
    }
}

// Sends a friend request to the user if they are not already a friend and
// if there are no pending friend requests in either direction.
void PrivateMessages::sendFriendRequest()
{
    qDebug() << "Friend request button clicked.";
    // Condition to ensure we only send a friend request if it is appropriate
    if (   !client->getUser().getFriendsList().contains(currentlyMessaging.get_username())
        && !client->getUser().getFriendRequestList().contains(currentlyMessaging.get_username())
        && !currentlyMessaging.getFriendRequestList().contains(client->getUser().get_username()))
    {
        qDebug() << "Sending friend request.";
        client->friendRequest(currentlyMessaging);
    }
}

// Filter to listen for rightclicking the friends list
bool PrivateMessages::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->friendView->viewport() && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::RightButton)
        {
            QModelIndex index = ui->friendView->indexAt(mouseEvent->pos());
            if (index.isValid())
            {
                qDebug() << "Right clicked on item: " << index.data().toString();
            }
            return true;
        }
    }
    return QObject::eventFilter(watched, event);
}

// Example of how future implementations might load messages from a file or other source.
// Currently not used for servers, but left here for reference.
void PrivateMessages::loadPage()
{
    QFile jsonFile("test.json");

    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file:" << "test.json";
        return;
    }

    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);

    if (!doc.isArray())
    {
        qDebug() << "JSON format is incorrect!";
        return;
    }

    QJsonArray jsonArray = doc.array();

    // Example of iterating through JSON data
    for (const QJsonValue &value : jsonArray)
    {
        if (!value.isObject()) continue;

        QJsonObject obj = value.toObject();
        QString to   = obj["To"].toString();
        QString from = obj["From"].toString();
        QString msg  = obj["Message"].toString();

        qDebug() << "To:" << to << "| From:" << from <<"| Msg:" << msg;
    }
}

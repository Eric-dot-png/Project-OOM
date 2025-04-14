// Name: Alex Franke, Eric Garcia
// File: privateMessages.cpp
// OOM Project
//
// Description: This file defines the PrivateMessages class, which manages the private messaging
//              functionality in a Discord-like Qt application. It interacts with the Client class
//              for network communication (sending/receiving messages), displays ongoing
//              conversations, and handles user interactions (searching for users, sending friend
//              requests, etc.).

// QT includes
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextBrowser>
#include <QScrollBar>
#include <QMovie>

// OOM includes
#include "OOMTextBrowser.h"
#include "privatemessages.h"
#include "ui_privatemessages.h"
#include "User.h"
#include "message.h"
#include "ChatObject.h"

// Constructor for PrivateMessages. Sets up UI elements, signals, and slots.
PrivateMessages::PrivateMessages(QWidget *parent)
    : OOMWidget(parent), ui(new Ui::PrivateMessages),
      currentlyMessaging(GUEST),
      messagingList(new DMListModel(this))
{
    ui->setupUi(this);                  // Sets up the UI from the .ui file
    ui->friendNameLabel->clear();       // Clears label that shows who the user is currently messaging

    // Model to display the list of users the client is messaging
    ui->friendView->setModel(messagingList);

    // Adds a placeholder user to test the messaging list
    // messagingList->addUserToDMList(User("Test"));

    // Sets up a custom event filter to detect the Enter key in the text-edit area
    enterFilter = new EnterKeyFilter(this);
    ui->textEdit->installEventFilter(enterFilter);

    ui->friendView->viewport()->installEventFilter(this);

    // hide the addFriend button by default
    ui->addFriendButton->hide();

    // Setup Client and UI
    connectClient();
    connectUI();

    loadMoreMessagesFlag = false;
}

// Destructor: Cleans up the UI.
PrivateMessages::~PrivateMessages()
{
    delete ui;
}

// All connect() functions for client/ui connections
void PrivateMessages::connectClient()
{
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
        
        
        // Update the user we are currently messaging
        currentlyMessaging = User(username);
        ui->friendNameLabel->setText("Now messaging: " + currentlyMessaging.get_username());

        // Add the found user to the messaging list
        messagingList->addUserToDMList(currentlyMessaging);
        QScrollBar* sb = ui->textBrowser->verticalScrollBar();

        // Populate the text browser with the returned messages (server history)
        for (int i = messageJsonList.size() - 1, j = 0; i >= 0; --i, j++)
        {
            QJsonObject obj = messageJsonList[i].toObject();
            QString to = obj["To"].toString();
            QString from = obj["From"].toString();
            QString msg = obj["Message"].toString();

            ui->textBrowser->append(QString("[%1]: %2").arg(from, msg));
            messagingList->messageReceived(currentlyMessaging, Message(from, to, msg));

        }

        sb->setValue(sb->maximum());
        loadMoreMessagesFlag = true;
        // Show or hide the Add Friend button depending on whether the user is already a friend
        showAddFriendButton();
    });


    // Handle receiving a friend request, adds that request to the friendRequestComboBox
    connect(client, &Client::recievedFriendRequest, this, [=](const QString& from)
    {
        ui->friendRequestComboBox->addItem(from);
    });


    connect(client, &Client::friendAccepted, this, [=](const QString& from)
    {
        ui->friendCombobox->addItem(from);
    });

    // setup friendslist
    connect(client, &Client::loginSuccess, this,
            &PrivateMessages::setupFriends);

    // set current user as the client's current user
    connect(client, &Client::loginSuccess, this, [=]() {
        ui->currentUser->setText(client->getUser().get_username());
    });
}

// All connect() functions for user/ui interaction
void PrivateMessages::connectUI()
{
    // Connect the custom Enter key press signal to the onEnterKeyPressed slot
    connect(enterFilter, &EnterKeyFilter::enterPressed, this, &PrivateMessages::onEnterKeyPressed);

    // TEXTBOX: searchUser
    // Detect Enter key press for the search user line-edit
    connect(ui->searchUserTextbox, &QLineEdit::returnPressed, this, &PrivateMessages::searchUser);

    //BUTTON: denyButton
    //Remove friend request from combobox and tell client to remove friend request
    connect(ui->denyButton, &QPushButton::clicked, this, [=](){
        client->denyFriend(User(ui->friendRequestComboBox->currentText()));
        ui->friendRequestComboBox->removeItem(ui->friendRequestComboBox->currentIndex());
    });

    //BUTTON: addFriendButton
    // Connect the Add Friend button to the function that sends a friend request
    connect(ui->addFriendButton, &QPushButton::clicked, this, &PrivateMessages::sendFriendRequest);

    //BUTTON: acceptButton
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

    ui->styleCheckbox->setChecked(false);

    //CHECKBOX
    connect(ui->styleCheckbox, &QCheckBox::toggled, this, [=](bool checked) {
        if (checked)
            qApp->setStyleSheet(loadStyleSheet(styles["DarkMode"]));
        else
            qApp->setStyleSheet(loadStyleSheet(styles["LightMode"]));
    });

    //SCROLLBAR/TEXTBROWSER

    // TODO fix this mf.
    // rn it for some reason forces the protocol state into a random one.
    // idk how tf its doing it

    connect(ui->textBrowser->verticalScrollBar(), &QScrollBar::valueChanged, this, [=](int value){
        QScrollBar* sb = ui->textBrowser->verticalScrollBar();
        quint32 currentSize = static_cast<quint32>(ui->textBrowser->document()->blockCount());
        //qDebug() << "currentSize: " << currentSize;

        if (value > sb->minimum() + 1 && value < sb->minimum() + 10 && loadMoreMessagesFlag) {

            const ChatObject* dm = client->getDMsWith(currentlyMessaging.get_username());
            if (dm)
            {
                quint32 size = dm->allMessages().size();
                //qDebug() << "size: " << size;

                //extendMessageHistory is bugged
                client->extendMessageHistory(currentlyMessaging, size);

                //Then connect function below should call for
                // &Client::recievedMoreMsgs to show messages instantly
            }
            else
            {
                qDebug() << "No dm found with " + currentlyMessaging.get_username();
            }
        }
    });

    //SCROLLBAR EXTEND
    // uncomment the signal in client.cpp
    connect(client, &Client::recievedMoreMsgs, this, [=](const QList<Message> list)
    {
        QScrollBar* sb = ui->textBrowser->verticalScrollBar();
        int oldValue = sb->value();
        int oldMax = sb->maximum();

        for (const Message &msg : list)
        {
            QString newMsg = QString("[%1]: %2").arg(msg.get_sender(), msg.get_msg());
            QString currentText = ui->textBrowser->toPlainText();

            ui->textBrowser->setPlainText(newMsg + '\n' + currentText);
        }

        int newMax = sb->maximum();
        int d = newMax - oldMax;
        sb->setValue(oldValue + d);
    });

    //COMBOBOX ---- TODO
    connect(ui->friendCombobox, QOverload<int>::of(&QComboBox::activated), this, [=](int index){
        QString u = ui->friendCombobox->currentText();
        QAbstractItemModel *model = ui->friendView->model();

        for (int row = 0; row < model->rowCount(); row++)
        {
            QModelIndex index = model->index(row, 0);
            QString itemText = model->data(index, Qt::DisplayRole).toString();

            if (itemText == u) {
                qDebug() << "Found at row: " << index.row();
                QVariant userData = index.data(Qt::UserRole);

                currentlyMessaging = userData.value<User>();

                ui->friendNameLabel->setText("Now messaging: " + currentlyMessaging.get_username());
                showAddFriendButton();

                ui->textBrowser->clear();
                for (const Message &msg : client->getDMsWith(currentlyMessaging.get_username())->allMessages())
                    ui->textBrowser->appendMessage(msg, 1);
                break;
            }
        }
    });
}


// Checks if the currentlyMessaging user is already a friend. Shows or hides the Add Friend button accordingly.
void PrivateMessages::showAddFriendButton()
{
    if (client->getFriendList().isFriend(currentlyMessaging.get_username()))
        ui->addFriendButton->hide();
    else
        ui->addFriendButton->show();
}

// Initiates a server request to search for another user by username.
// Clears the current conversation text and sets up for the discovered user's messages.
void PrivateMessages::searchUser()
{
    User u = User(ui->searchUserTextbox->text());

    // Prevent searching for yourself
    if (u.get_username() == client->getUser().get_username())
    {
        ui->userNotFoundLabel->setText("You can't search for yourself!");
        return;
    }
    // If this user is the one we are already messaging, do nothing
    if (u.get_username() == currentlyMessaging.get_username())
    {
        ui->userNotFoundLabel->setText("You are already messaging "  + currentlyMessaging.get_username() + '!');
        return;
    }


    QAbstractItemModel *model = ui->friendView->model();

    for (int row = 0; row < model->rowCount(); row++)
    {
        QModelIndex index = model->index(row, 0);
        QString itemText = model->data(index, Qt::DisplayRole).toString().section(':', 0, 0);

        if (itemText == u.get_username())
        {
            ui->userNotFoundLabel->setText(itemText + " is already in your dmlist! Click their name instead!");
            return;
        }
    }

    ui->textBrowser->clearHistory();
    ui->textBrowser->clear();

    // Indicate that we are searching
    ui->userNotFoundLabel->setText("Currently searching for: " + u.get_username());
    client->discover(u);
    loadMoreMessagesFlag = false;
    ui->searchUserTextbox->clear();
}

// Called when the user presses Enter in the text-edit box.
// Sends the typed message to the server and updates the UI.
void PrivateMessages::onEnterKeyPressed()
{
    if (currentlyMessaging.get_username() == "") return;
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
    ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
}

void PrivateMessages::setupFriends()
{
    const FriendList * fl = &client->getFriendList();
    for (const QString& usr : fl->friends())
    {
        ui->friendCombobox->addItem(usr);
    }
    for (const QString& usr : fl->incoming())
    {
        ui->friendRequestComboBox->addItem(usr);
    }
    for (const QString& usr : fl->outgoing())
    {
        // do something with outgoing here...
    }
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
    loadMoreMessagesFlag = false;
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

    ui->textBrowser->clear();
    // for (const Message &msg : client->getDMsWith(currentlyMessaging.get_username())->allMessages())
    //     ui->textBrowser->appendMessage(msg, 1);

    auto messages = client->getDMsWith(currentlyMessaging.get_username())->allMessages();
    for (int i = messages.size() - 1; i >= 0; --i)
    {
        const Message &msg = messages[i];
        ui->textBrowser->appendMessage(msg, 1);
    }
    loadMoreMessagesFlag = true;
}

// Sends a friend request to the user if they are not already a friend and
// if there are no pending friend requests in either direction.
void PrivateMessages::sendFriendRequest()
{
    qDebug() << "Friend request button clicked.";
    const FriendList * fl = &client->getFriendList();
    // Condition to ensure we only send a friend request if it is appropriate
    QString other = currentlyMessaging.get_username();

    if (!fl->isFriend(other) && !fl->isRequesting(other) &&
        !fl->isRequested(other))
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

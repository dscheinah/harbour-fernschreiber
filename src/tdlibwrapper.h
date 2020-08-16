/*
    Copyright (C) 2020 Sebastian J. Wolf

    This file is part of Fernschreiber.

    Fernschreiber is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Fernschreiber is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Fernschreiber. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef TDLIBWRAPPER_H
#define TDLIBWRAPPER_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QStandardPaths>
#include <td/telegram/td_json_client.h>
#include "tdlibreceiver.h"

class TDLibWrapper : public QObject
{
    Q_OBJECT
public:
    explicit TDLibWrapper(QObject *parent = nullptr);
    ~TDLibWrapper();

    enum AuthorizationState {
        Closed,
        Closing,
        LoggingOut,
        AuthorizationReady,
        WaitCode,
        WaitEncryptionKey,
        WaitOtherDeviceConfirmation,
        WaitPassword,
        WaitPhoneNumber,
        WaitRegistration,
        WaitTdlibParameters
    };
    Q_ENUM(AuthorizationState)

    enum ConnectionState {
        Connecting,
        ConnectingToProxy,
        ConnectionReady,
        Updating,
        WaitingForNetwork
    };
    Q_ENUM(ConnectionState)

    Q_INVOKABLE QString getVersion();
    Q_INVOKABLE TDLibWrapper::AuthorizationState getAuthorizationState();
    Q_INVOKABLE TDLibWrapper::ConnectionState getConnectionState();
    Q_INVOKABLE QVariantMap getUserInformation();
    Q_INVOKABLE QVariantMap getUserInformation(const QString &userId);
    Q_INVOKABLE QVariantMap getUnreadMessageInformation();
    Q_INVOKABLE QVariantMap getUnreadChatInformation();

    // Direct TDLib functions
    Q_INVOKABLE void sendRequest(const QVariantMap &requestObject);
    Q_INVOKABLE void setAuthenticationPhoneNumber(const QString &phoneNumber);
    Q_INVOKABLE void setAuthenticationCode(const QString &authenticationCode);
    Q_INVOKABLE void getChats();
    Q_INVOKABLE void downloadFile(const QString &fileId);

signals:
    void versionDetected(const QString &version);
    void authorizationStateChanged(const TDLibWrapper::AuthorizationState &authorizationState);
    void optionUpdated(const QString &optionName, const QVariant &optionValue);
    void connectionStateChanged(const TDLibWrapper::ConnectionState &connectionState);
    void fileUpdated(const int fileId, const QVariantMap &fileInformation);
    void newChatDiscovered(const QString chatId, const QVariantMap &chatInformation);
    void unreadMessageCountUpdated(const QVariantMap &messageCountInformation);
    void unreadChatCountUpdated(const QVariantMap &chatCountInformation);

public slots:
    void handleVersionDetected(const QString &version);
    void handleAuthorizationStateChanged(const QString &authorizationState);
    void handleOptionUpdated(const QString &optionName, const QVariant &optionValue);
    void handleConnectionStateChanged(const QString &connectionState);
    void handleUserUpdated(const QVariantMap &userInformation);
    void handleFileUpdated(const QVariantMap &fileInformation);
    void handleNewChatDiscovered(const QVariantMap &chatInformation);
    void handleUnreadMessageCountUpdated(const QVariantMap &messageCountInformation);
    void handleUnreadChatCountUpdated(const QVariantMap &chatCountInformation);

private:
    void *tdLibClient;
    TDLibReceiver *tdLibReceiver;
    QString version;
    TDLibWrapper::AuthorizationState authorizationState;
    TDLibWrapper::ConnectionState connectionState;
    QVariantMap options;
    QVariantMap userInformation;
    QVariantMap otherUsers;
    QVariantMap chats;
    QVariantMap unreadMessageInformation;
    QVariantMap unreadChatInformation;

    void setInitialParameters();
    void setEncryptionKey();
    void setLogVerbosityLevel();

};

#endif // TDLIBWRAPPER_H

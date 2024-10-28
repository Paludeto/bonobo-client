#include "client.h"

/**
 * @brief Construct a new Client object
 * 
 * @param serverAddress 
 * @param serverPort 
 */
Client::Client(QString serverAddress, quint16 serverPort) {
    _serverAddress = serverAddress;
    _serverPort = serverPort;
    _isConnected = false;
} 

/**
 * @brief Connects the client using class-specific virtual methods
 * 
 */
void Client::run() {
    if (!_isConnected) {
        connectToNetwork();
        _isConnected = true;
    }

    runClient();
}

/**
 * @brief Closes the client using class-specific virtual methods
 * 
 */
void Client::close() {
    if (_isConnected) {
        disconnectFromNetwork();
        _isConnected = false;
    }
}

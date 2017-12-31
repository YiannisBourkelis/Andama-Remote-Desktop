#ifndef SHARED_ENUMS_H
#define SHARED_ENUMS_H

//prosdiorizei ean o client syndeetai ston host mesw tou proxy
//h apeftheias (peer-to-peer)
enum connectMessageType {
    proxy = 0,
    proxy_or_reverse_p2p = 1,
    proxy_or_p2p = 2
};

enum msgType{ MSG_ID = 0,
              MSG_CONNECTION_ACCEPTED = 1, //to remote pc me enimerwse oti to aitima syndesis egine apodekto
              MSG_REMOTE_CLIENT_ACCEPTED = 2, //apodextika to aitima syndesis pou ekane to remote pc.ksekinw na stelnw screenshots
              MSG_SCREENSHOT = 3,
              MSG_SCREENSHOT_DIFF = 4,
              MSG_SCREENSHOT_REQUEST = 5,
              MSG_SCREENSHOT_DIFF_REQUEST = 6,
              MSG_MOUSE = 7,
              MSG_MOUSE_CURSOR = 8,
              MSG_KEYBOARD = 9,
              MSG_CONNECT_ID_NOT_FOUND =10, // epistrefetai apo ton server otan to Remote ID pou zitithike apo to connect den vrethike
              MSG_NO_INTERNET_CONNECTION = 11,
              MSG_NO_PROXY_CONNECTION = 12,
              MSG_REMOTE_COMPUTER_DISCONNECTED = 13,
              MSG_LOCAL_PASSWORD_GENERATED = 14,
              MSG_BAN_IP_WRONG_ID = 15, // synolika 5 bytes
              MSG_WARNING_BAN_IP_WRONG_ID = 16, // synolika 2 bytes
              MSG_CONNECT_PASSWORD_NOT_CORRECT = 17,
              MSG_BAN_IP_WRONG_PWD = 18, // synolika 1 byte
              MSG_WARNING_BAN_IP_WRONG_PWD = 19, // synolika 2 byte
              MSG_ERROR_CANNOT_CONNECT_SAME_ID = 20, // synolika 2 byte
              MSG_ERROR_APP_VERSION_NOT_ACCEPTED = 21, // (mono command)
              MSG_ERROR_PROTOCOL_VERSION_NOT_ACCEPTED = 22 // (mono command)
             };

#endif // SHARED_ENUMS_H

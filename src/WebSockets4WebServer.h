

#ifndef __WEBSOCKETS4WEBSERVER_H
#define __WEBSOCKETS4WEBSERVER_H

#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>

#if ((WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_RP2040)) && WEBSERVER_HAS_HOOK

class WebSockets4WebServer : public WebSocketsServerCore {
#if defined(ESP8266)
    using WebServerClass = ESP8266WebServer;
#else
    using WebServerClass = WebServer;
#endif

  public:
    WebSockets4WebServer(const String & origin = "", const String & protocol = "arduino")
        : WebSocketsServerCore(origin, protocol) {
        begin();
    }

    WebServerClass::HookFunction hookForWebserver(const String & wsRootDir, WebSocketServerEvent event) {
        onEvent(event);

        return [&, wsRootDir](const String & method, const String & url, WiFiClient * tcpClient, WebServerClass::ContentTypeFunction contentType) {
            (void)contentType;

            if(!(method == "GET" && url.indexOf(wsRootDir) == 0)) {
                return WebServerClass::CLIENT_REQUEST_CAN_CONTINUE;
            }

            // allocate a WiFiClient copy (like in WebSocketsServer::handleNewClients())
            WEBSOCKETS_NETWORK_CLASS * newTcpClient = new WEBSOCKETS_NETWORK_CLASS(*tcpClient);

            // Then initialize a new WSclient_t (like in WebSocketsServer::handleNewClient())
            WSclient_t * client = handleNewClient(newTcpClient);

            if(client) {
                // give "GET <url>"
                String headerLine;
                headerLine.reserve(url.length() + 5);
                headerLine = "GET ";
                headerLine += url;
                handleHeader(client, &headerLine);
            }

            // tell webserver to not close but forget about this client
            return WebServerClass::CLIENT_IS_GIVEN;
        };
    }
};
#else    // WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266 && WEBSERVER_HAS_HOOK

#ifndef WEBSERVER_HAS_HOOK
#error Your current Framework / Arduino core version does not support Webserver Hook Functions
#else
#error Your Hardware Platform does not support Webserver Hook Functions
#endif

#endif    // WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266 && WEBSERVER_HAS_HOOK

#endif    // __WEBSOCKETS4WEBSERVER_H

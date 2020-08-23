#pragma once

#include "BaseRouteHandler.h"
#include "HttpParser.h"
#include "CommonConnection.h"

namespace Protocol
{

class WebsocketPath : public BaseRouteHandler
{
public:
    WebsocketPath();
    ~WebsocketPath();
    bool DoHandshake(char * path, char * host);

private:
    bool _websocketConnected = false;
    // TimeLimit pingTimeout = {};
    bool analyzeRequest(char * path, char * host);

    void disconnectStream();

    void ReceivedData(const uint8_t *data, uint16_t length) override;

    void ConnectionStateChanged(ConnectionState state, ConnectionChangeReason reason) override;

    bool SendFrame(const uint8_t *data, uint16_t length) override;

	void setConnection() override;

	bool isTerminated() const override;

	bool start() override;

	void terminate() override;

	void process() override;

private:
    /// @brief	Hide Copy constructor.
    WebsocketPath(const WebsocketPath &) = delete;

    /// @brief	Hide Assignment operator.
    WebsocketPath &operator=(const WebsocketPath &) = delete;

    /// @brief	Hide Move constructor.
    WebsocketPath(WebsocketPath &&) = delete;

    /// @brief	Hide Move assignment operator.
    WebsocketPath &operator=(WebsocketPath &&) = delete;
};
} // namespace Protocol
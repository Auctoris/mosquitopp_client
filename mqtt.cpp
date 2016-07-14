#include "mqtt.h"
#define PUBLISH_TOPIC "EXAMPLE_TOPIC"

#ifdef DEBUG
#include <iostream>
#endif

mqtt_client::mqtt_client(const char *id, const char *host, int port) : mosquittopp(id)
{
    int keepalive = DEFAULT_KEEP_ALIVE;
    connect(host, port, keepalive);
}

mqtt_client::~mqtt_client()
{
}

void mqtt_client::on_connect(int rc)
{
    if (!rc)
    {
        #ifdef DEBUG
            std::cout << "Connected - code " << rc << std::endl;
        #endif
    }
}

void mqtt_client::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
    #ifdef DEBUG
        std::cout << "Subscription succeeded." << std::endl;
    #endif
}

void mqtt_client::on_message(const struct mosquitto_message *message)
{
    int payload_size = MAX_PAYLOAD + 1;
    char buf[payload_size];

    if(!strcmp(message->topic, PUBLISH_TOPIC))
    {
        memset(buf, 0, payload_size * sizeof(char));

        /* Copy N-1 bytes to ensure always 0 terminated. */
        memcpy(buf, message->payload, MAX_PAYLOAD * sizeof(char));

        #ifdef DEBUG
            std::cout << buf << std::endl;
        #endif

        // Examples of messages for M2M communications...
        if (!strcmp(buf, "STATUS"))
        {
            snprintf(buf, payload_size, "This is a Status Message...");
            publish(NULL, PUBLISH_TOPIC, strlen(buf), buf);
            #ifdef DEBUG
                std::cout << "Status Request Recieved." << std::endl;
            #endif
        }

        if (!strcmp(buf, "ON"))
        {
            snprintf(buf, payload_size, "Turning on...");
            publish(NULL, PUBLISH_TOPIC, strlen(buf), buf);
            #ifdef DEBUG
                std::cout << "Request to turn on." << std::endl;
            #endif
        }

        if (!strcmp(buf, "OFF"))
        {
            snprintf(buf, payload_size, "Turning off...");
            publish(NULL, PUBLISH_TOPIC, strlen(buf), buf);
            #ifdef DEBUG
                std::cout << "Request to turn off." << std:: endl;
            #endif
        }
    }
}

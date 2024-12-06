/* main.c */

// imports
#include <stdio.h>

#include <concord/discord.h>
#include <concord/log.h>

#include "commands/command.h"


void on_ready(struct discord *client, const struct discord_ready *event)
{
    log_info("Successfully logged in as %s!", event->user->username);
}


// c macros are cool
#define bot_init(config_file_path) \
    discord_config_init(config_file_path); \
    \
    discord_add_intents(client, DISCORD_GATEWAY_MESSAGE_CONTENT); \
    \
    discord_set_on_ready(client, &on_ready); \
    discord_set_on_message_create(client, &on_message);
    //discord_set_on_command(client, "sync-rizz", &sync_rizz);


void bot_cleanup(struct discord *client)
{
    discord_cleanup(client);
}


int main(int argc, char *argv[])
{
    if ( argc < 2 )
    {
        puts("Please provide the path to your config file");
        return 1;
    }
    const char *config_file_path = argv[1];

    log_info("Attempting to initalize...");
    struct discord *client = bot_init(config_file_path);

    discord_run(client);

    bot_cleanup(client);
    return 0;
}
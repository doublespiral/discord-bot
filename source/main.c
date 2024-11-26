/* main.c */

// imports
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <concord/discord.h>
#include <concord/discord-internal.h>
#include <concord/channel.h>
#include <concord/log.h>

// types
typedef struct discord client_t;
typedef struct discord_create_message new_message_t;
typedef struct discord_message message_t;
typedef struct discord_embeds embeds_t ;

// defines
#define CHANNEL_RULES      1300928626392301750
#define CHANNEL_TRASH_OUT  1302776301161287752
#define BOT_PANCAKE         239631525350604801

// funcs
char *format_content(char *base_content)
{
    char *formatted = malloc( strlen(base_content) );
    if (formatted == NULL)
    {
        log_info("malloc skill issue for `formatted`, exiting safely");
        exit(-1);
    }

    for (int i = 0; base_content[i] != '\0'; i++)
    {
        formatted[i] = tolower( (unsigned char)base_content[i] );
    }

    return formatted;
}


void copy_message(struct discord *client,
                  const struct discord_message *base_message,
                  u64snowflake to_channel,
                  char *edits)
{
    // copy message
    struct discord_create_message copied_message = {};

    copied_message.content = malloc( strlen(edits) + strlen(base_message->content) );

    if (copied_message.content == NULL)
    {
        log_info("malloc skill issue for `copied_message.content`, returning early");
        return;
    }

    sprintf(copied_message.content, "%s%s", edits, base_message->content);

    copied_message.embeds = base_message->embeds;

    // send copied message
    discord_create_message(client, to_channel, &copied_message, NULL);

    free(copied_message.content);
}


void move_message(client_t *client, const message_t *base_message, u64snowflake to_channel)
{
    copy_message(client, base_message, to_channel, "Moved message: \n");

    discord_delete_message(client,
        base_message->channel_id,
        base_message->id,
        NULL, NULL
    );
}



// command type shi
void on_ready(client_t *client, const struct discord_ready *event)
{
    log_info("Successfully logged in as %s!", event->user->username);

}


void on_message(client_t *client, const message_t *message)
{
    if (message->author->id == client->self.id) return;

    // handle specific cases
    if (message->author->id == BOT_PANCAKE)
    {
        move_message(client, message, CHANNEL_TRASH_OUT);
    }


    // handle general cases
    char *formatted = format_content(message->content);

    if ( strcmp(formatted, "enjoy your lobster") == 0 )
    {
        new_message_t response = { .content = "((He actually means something else))" };
        discord_create_message(client, message->channel_id, &response, NULL);
    }

    free( formatted );
}


client_t *bot_init(const char *config_file)
{
    client_t *client = discord_config_init(config_file);

    discord_set_on_ready(client, &on_ready);
    discord_add_intents(client, DISCORD_GATEWAY_MESSAGE_CONTENT);
    discord_set_on_message_create(client, &on_message);

    return client;
}


void bot_cleanup(client_t *client)
{
    discord_cleanup(client);
}


// main
int main(int argc, char*argv[])
{
    if ( argc < 2 )
    {
        puts("Please provide the path to your config file");
        return 1;
    }
    const char *config_file = argv[1];

    log_info("Attempting to initalize...");
    client_t *client = bot_init(config_file);

    discord_run(client);

    bot_cleanup(client);
    return 0;
}